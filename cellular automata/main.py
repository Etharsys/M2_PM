from __future__ import annotations

from typing import Callable

import cv2
import numpy as np

WINDOW_NAME = 'cellular automaton'


def lerp(a: any, b: any, t: float) -> any:
    """
    Linear interpolation between a and b by t (clamped).

    :param a: value when t = 0.
    :param b: value when t = 1.
    :param t: interpolation coefficient.
    :return: interpolated value [a, b] by t.

    >>> lerp(0.0, 10.0, 1.0), lerp(0.0, 10.0, 0.0), lerp(0.0, 10.0, 0.5), lerp(0.0, 10.0, 3.5), lerp(0.0, 10.0, -10.0)
    (10.0, 0.0, 5.0, 10.0, 0.0)
    """
    x, y = min(a, b), max(a, b)
    return max(x, min(y, (1 - t) * a + t * b))


def ilerp(a: any, b: any, v: any) -> float:
    """
    Inverse linear interpolation between a and b with value v (unclamped).

    :param a: value mapped to 0.
    :param b: value mapped to 1.
    :param v: interpolated value.
    :return: a value in [0, 1] if v is in [a, b].

    >>> ilerp(0.0, 10.0, 10.0), ilerp(0.0, 10.0, 0.0), ilerp(0.0, 10.0, 5.0),\
        ilerp(0.0, 10.0, 11.0), ilerp(0.0, 10.0, -1.0)
    (1.0, 0.0, 0.5, 1.1, -0.1)
    """
    return (v - a) / (b - a)


def clamp(v: any, low: any, high: any) -> any:
    """
    :param v: the value to clamp.
    :param low: lowest bound.
    :param high: highest bound.
    :return: clamped value v between low and high.

    >>> clamp(2, 1, 3), clamp(0, 1, 3), clamp(4, 1, 3)
    (2, 1, 3)
    """
    return max(min(v, high), low)


class FluidConfiguration:
    """
    Configuration of the fluid.
    """

    def __init__(self, **kwargs) -> None:
        self.max_compression = kwargs.get("max_compression", 0.02)
        self.max_mass = kwargs.get("max_mass", 1)
        self.min_mass = kwargs.get("min_mass", 0.0001)
        self.min_draw = kwargs.get("min_draw", 0.01)
        self.max_draw = kwargs.get("max_draw", 1.1)
        self.max_speed = kwargs.get("max_speed", 1)
        self.min_flow = kwargs.get("min_flow", 0.01)


class World:
    """
    Main class.
    Hold the cell grid and fluid configuration.
    The world can update and get_world_buffer it self.

    When a mass is <0, it represent a wall;
    otherwise it's water or air if mass is null.
    """

    def __init__(self, width: int, height: int, res: tuple[int, int], configuration: FluidConfiguration) -> None:
        """
        :param width: number of cell per line (without border).
        :param height: number of cell per column (without border).
        :param res: resolution of the window.
        :param configuration: configuration of the fluid.
        """
        self.width = width
        self.height = height
        self.mass = np.zeros((height + 2, width + 2))
        self.buf = np.zeros((height, width, 3))
        self.res = res
        self.configuration = configuration
        self.mouse = None

    def compute_color(self, mass: float) -> list[float]:
        """
        :param mass: the mass of a cell.
        :return: the color corresponding to the given cell.
        An empty cell is white, a full cell is blue and an overflowing cell is black.
        """
        mm = self.configuration.max_mass
        rg = lerp(205, 0, min(mm, mass)) / 255
        b = lerp(200, 0, (max(mm, mass)) - mm) / 255
        return [b, rg, rg]

    def add_box(self, offset: int = 0) -> World:
        """
        :param offset: distance between box bounds and border.
        :return: self.
        """
        w, h = self.width + 2, self.height + 2
        # Todo: use numpy syntaxe to improve speed
        for i in range(offset, w - offset):
            self[i, 0 + offset] = -1
            self[i, h - 1 - offset] = -1

        for i in range(offset, h - offset):
            self[0 + offset, i] = -1
            self[w - 1 - offset, i] = -1
        return self

    def get_world_buffer(self) -> np.array:
        """
        :return: the world as a color buffer with the correct dimensions.
        """
        md = self.configuration.min_draw
        for i in range(self.height):
            for j in range(self.width):
                m = self.mass[i+1, j+1]
                if 0 > m:
                    self.buf[i, j] = [0.15, 0.13, 0.16]
                elif m >= md:
                    self.buf[i, j] = self.compute_color(m)
                else:
                    self.buf[i, j] = [0.8, 0.8, 0.8]
        if self.mouse is not None:
            mx, my = self.mouse
            b, g, r = self.buf[mx, my]
            self.buf[mx, my] = [b, min(1, g + .2), min(1, r + .2)]
        return cv2.resize(self.buf, self.res, interpolation=cv2.INTER_NEAREST)

    def simulate_all(self) -> None:
        """
        Update all cells and clean border.
        """
        new_mass = self.mass.copy()
        for i in range(1, self.height + 1):
            for j in range(1, self.width + 1):
                self.update_cell(new_mass, i, j)

        self.mass = new_mass

        # Remove water from borders
        self.mass[:1, :] = 0  # Top
        self.mass[:, :1] = 0  # Left
        self.mass[-1:, :] = 0  # Bottom
        self.mass[:, -1:] = 0  # Right

    def update_cell(self, mass: np.array, i: int, j: int) -> None:
        """
        Update cell based on it's neighbourhood.
        :param mass: array of all mass.
        :param i: index of the target row.
        :param j: index of the target column.
        """
        if self.mass[i, j] < 0: return

        remaining = self.mass[i, j]
        if remaining <= 0: return
        ms = self.configuration.max_speed

        flow = self.get_stable(remaining + self.mass[i + 1, j]) - self.mass[i + 1, j]
        remaining = self.update_neighbour(mass, (i, j), (+1, 0), flow, min(ms, remaining), remaining)
        if remaining <= 0: return

        flow = (self.mass[i][j] - self.mass[i][j - 1]) / 4
        remaining = self.update_neighbour(mass, (i, j), (0, -1), flow, remaining, remaining)
        if remaining <= 0: return

        flow = (self.mass[i][j] - self.mass[i][j + 1]) / 4
        remaining = self.update_neighbour(mass, (i, j), (0, 1), flow, remaining, remaining)
        if remaining <= 0: return

        flow = remaining - self.get_stable(remaining + self.mass[i - 1, j])
        self.update_neighbour(mass, (i, j), (-1, 0), flow, min(ms, remaining), remaining)

    def update_neighbour(self, mass: np.array, pos: tuple[int, int], neighbour: tuple[int, int],
                         flow: float, high: float, remaining: float) -> float:
        """
        Update a cell using a neighbour.
        :param mass: array of all mass.
        :param pos: position of the current cell.
        :param neighbour: the offset of the neighbour.
        :param flow: quantity that should flow out of the cell to the neighbour.
        :param high: limit of flow quantity.
        :param remaining: quantity of water remaining in the cell.
        :return the remaining quantity of water in the cekk
        """
        i, j = pos
        i2, j2 = i + neighbour[0], j + neighbour[1]

        if self.mass[i2, j2] < 0:
            return remaining
        if flow > self.configuration.min_flow:
            flow *= .5
        flow = clamp(flow, 0, high)
        mass[i, j] -= flow
        mass[i2, j2] += flow

        return remaining - flow

    def get_stable(self, quantity: float) -> float:
        """
        :param quantity: aa quantity of water.
        :return: the pondered quantity of water a cell can handle.
        """
        mm, mc = self.configuration.max_mass, self.configuration.max_compression
        if quantity <= 1: return 1
        if quantity < 2 * mm + mc:
            return (mm ** 2 + quantity * mc) / (mc + mm)
        return (quantity + mc) / 2

    def coord_to_position(self, x: float, y: float) -> tuple[int, int]:
        """
        :param x: x-coordinate.
        :param y: y-coordinate.
        :return: the conversion between window coordinate and grid position.
        """
        return int(lerp(0, self.height - 1, ilerp(0, self.res[1], y)) + .5), \
            int(lerp(0, self.width - 1, ilerp(0, self.res[0], x)) + .5)

    def __setitem__(self, key: tuple[int, int], mass: float) -> None:
        """
        Allow numpy syntaxe to update cell mass.
        :param key: index of the targeted cell.
        :param mass: new mass of the cell.
        """
        i, j = key
        self.mass[i, j] = mass


class MouseController:
    """
    Controller to handle mouse motion and click.
    """
    def __init__(self, world: World) -> None:
        """
        :param world: Where to do modifications
        """
        self.world = world
        self.r_pressed = self.l_pressed = False
        self.coord = self.flag = None

    def mouse_callback(self) -> Callable:
        """
        :return: a callback function for mouse events.
        """

        # noinspection PyUnusedLocal
        def callback(event, x: int, y: int, flag: int, param) -> None:
            """
            Update mouse position and state.
            :param event: cv2 event.
            :param x: mouse x coordinate.
            :param y: mouse y coordinate.
            :param flag: keyboard modifiers.
            :param param: unused. (but mandatory)
            """
            if event == cv2.EVENT_LBUTTONUP: self.l_pressed = False
            if event == cv2.EVENT_LBUTTONDOWN: self.l_pressed = True
            if event == cv2.EVENT_RBUTTONUP: self.r_pressed = False
            if event == cv2.EVENT_RBUTTONDOWN: self.r_pressed = True

            i, j = self.world.coord_to_position(x, y)
            self.world.mouse = i, j
            self.flag = flag
            self.coord = i + 1, j + 1

        return callback

    def update(self) -> None:
        """
        Update world based on user input.
        """
        if self.coord is None: return
        i, j = self.coord
        if self.l_pressed:
            if self.flag & cv2.EVENT_FLAG_SHIFTKEY != 0:
                if self.world.mass[i][j] < 0:  # Only replace wall with l click and shift
                    self.world[i, j] = 0
            else:
                self.world[i, j] = -1
        elif self.r_pressed:
            if self.flag & cv2.EVENT_FLAG_SHIFTKEY != 0:
                if self.world.mass[i][j] > 0:  # Only replace water with r click and shift
                    self.world[i, j] = 0
            else:
                fac = 0.2 if self.flag & cv2.EVENT_FLAG_CTRLKEY == 0 else 1
                self.world[i, j] = self.world.mass[i, j] + fac


def main() -> None:
    world = World(50, 50, (700, 700), FluidConfiguration()).add_box(1)
    mouse_controller = MouseController(world)

    # Create canvas
    cv2.namedWindow(WINDOW_NAME)
    cv2.setMouseCallback(WINDOW_NAME, mouse_controller.mouse_callback(), world)

    # Main loop (stop if user click on window's close button)
    while cv2.getWindowProperty(WINDOW_NAME, cv2.WND_PROP_VISIBLE) > 0:
        # Update world
        mouse_controller.update()
        world.simulate_all()

        # Draw on screen
        cv2.imshow(WINDOW_NAME, world.get_world_buffer())

        # Handle keys
        key = cv2.waitKey(1)
        if key & 0xFF == ord("q"):
            break


if __name__ == '__main__':
    main()
