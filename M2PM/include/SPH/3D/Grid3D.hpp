#pragma once

#include <array>
#include <unordered_set>
#include <memory>
#include <iostream>

/* @brief Grid system dividing space in multiple tile of same size, each tile is a doubly linked list
 * @tparam T elements type
 */
template <class T>
class Grid
{
public:
    /* @brief Initialize grid container
     *  @param gap_ tile size
     *  @param x_min_ grid x_min
     *  @param x_max_ grid x_max
     *  @param y_min_ grid y_min
     *  @param y_max_ grid y_max
     */
    Grid(const float gap_, const float x_min_, const float x_max_, const float y_min_, const float y_max_, const float z_min_, const float z_max_) : gap(gap_), x_min(x_min_), y_max(y_max_), z_max(z_max_), nb_columns(get_column(x_max_) + 1), nb_rows(get_row(y_min_) + 1), nb_cubes(get_cube(z_max) + 1)
    {
        grid = std::vector<T *>(nb_rows * nb_columns);
    }

    /* @brief get center, down-left, down, down-right and right surrounding elements
     *  @param center grid tile index
     *  @return list of element doubly linked list
     */
    const std::array<T *, 27>& get_surrounding_elements(const int center)
    {
        int row = int(center / nb_columns);
        int column = int(center/ nb_rows);
        int cube = center - (row * nb_columns + column * nb_cubes); 
        surrounding_elements[0] = grid[center];
        if (column < nb_columns - 1) // right
        {
            surrounding_elements[1] = grid[center + 1];
        }
        if (row < nb_rows - 1) // down
        {
            surrounding_elements[2] = grid[center + nb_columns];
        }
        if (column > 0 && row < nb_rows - 1) // down-left
        {
            surrounding_elements[3] = grid[center + nb_columns - 1];
        }
        if (column < nb_columns - 1 && row < nb_rows - 1) // down-right
        {
            surrounding_elements[4] = grid[center + nb_columns + 1];
        }
        if (row > 0) // up
        {
            surrounding_elements[5] = grid[center - nb_columns];
        }
        if (column > 0 && row > 0) // up-left
        {
            surrounding_elements[6] = grid[center - nb_columns - 1];
        }
        if (column < nb_columns - 1 && row > 0) // up-right
        {
            surrounding_elements[7] = grid[center - nb_columns + 1];
        }
        if (column > 0) // left
        {
            surrounding_elements[8] = grid[center - 1];
        }
        return surrounding_elements;
    }
    /*
     * @brief move element in the right tile
     * new_x element new x position
     * new_y element new y position
     * element element not moved yet
     */
    void move_element(const float new_x, const float new_y, const float new_z, T *element)
    {
        int row = get_row(element->position.y());
        int column = get_column(element->position.x());
        int cube = get_cube(element->position.z());
        int old_index = get_index(row, column, cube);
        row = get_row(new_y);
        column = get_column(new_x);
        cube = get_cube(new_z);
         int new_index = get_index(row, column, cube);
        if (old_index == new_index)
        {
            element->position[0] = new_x;
            element->position[1] = new_y;
            element->position[2] = new_z;
        }
        else
        {
            remove(element, old_index);
            element->position[0] = new_x;
            element->position[1] = new_y;
            element->position[2] = new_z;
            add(element, new_index);
        }
    }

    /*
     * @brief add and sort all elements in the grid
     */
    void sort_grid(std::vector<T> &elements)
    {
        for (auto &element : elements)
        {
            int row = get_row(element.position.y());
            int column = get_column(element.position.x());
            int cube = get_cube(element.position.z());
            int index = get_index(row, column, cube);
            add(&element, index);
        }
    }

    /*
     * @brief access to a grid tile
     * @param index tile index
     * @return grid tile
     */
    T *&operator[](int index)
    {
        return grid[index];
    }

    /*
     * @brief get grid number of tile
     * @return number of tile
     */
    int size()
    {
        return grid.size();
    }

    /*
     * @brief get grid container
     * @return grid vector
     */
    const std::vector<T *> &get_grid() const
    {
        return grid;
    }

    /*
     * @brief get the number of row in the grid
     * @return the number of row
     */
    int get_nb_row() const
    {
        return nb_rows;
    }

    /*
     * @brief get the number of column in the grid
     * @return the number of column
     */
    int get_nb_columns() const
    {
        return nb_columns;
    }

    int get_nb_cubes() const
    {
        return nb_cubes;
    }

private:
    /*
     * @brief remove element in the doubly linked list
     * @param element element to remove
     * @param index grid tile
     */
    void remove(T *element, int index)
    {
        if (element->previous == nullptr) // head
        {
            if (element->next == nullptr) // tail
            {
                grid[index] = nullptr;
            }
            else
            {
                element->next->previous = nullptr;
                grid[index] = element->next;
                element->next = nullptr;
            }
        }
        else
        {
            if (element->next == nullptr) // tail
            {
                element->previous->next = nullptr;
                element->previous = nullptr;
            }
            else
            {
                element->previous->next = element->next;
                element->next->previous = element->previous;
                element->previous = nullptr;
                element->next = nullptr;
            }
        }
    }
    /*
     * @brief add element in the doubly linked list
     * @param element element to add
     * @param index grid tile
     */
    void add(T *element, int index)
    {
        if (grid[index] != nullptr)
        {
            element->next = grid[index];
            element->next->previous = element;
        }
        grid[index] = element;
    }
    /*
     * @brief get row index
     * @return row index
     */
    int get_row(const float y) const { return int((y_max - y) / gap); }

    /*
     * @brief get column index
     * @return column index
     */
    int get_column(const float x) const { return int((x - x_min) / gap); }

    int get_cube(const float z) const {return int((z_max - z) / gap);};

    /*
     * @brief get tile index
     * @return tile index
     */
    int get_index(const float row, const float column, const float cube) const { return row * nb_columns + column * nb_cubes + cube; }

    /*grid x_min*/
    const float x_min = 0;
    /*grid y_min*/
    const float y_max = 0;
    /*grid y_min*/
    const float z_max = 0;
    /*tile gap*/
    const float gap = 0;
    /*number of columns*/
    const int nb_columns = 0;
    /*number of rows*/
    const int nb_rows = 0;

    const int nb_cubes = 0;
    /*grid tiles*/
    std::vector<T *> grid;
    /*static array for surrounding elements*/
    std::array<T *, 27> surrounding_elements{};
};