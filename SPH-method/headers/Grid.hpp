#include <array>
#include <unordered_set>
#include <Particle.hpp>
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
    Grid(const float gap_, const float x_min_, const float x_max_, const float y_min_, const float y_max_) : gap(gap_), x_min(x_min_), y_min(y_min_), nb_columns(get_column(x_max_) + 1), nb_rows(get_row(y_max_) + 1)
    {
        grid = std::vector<T *>(nb_rows * nb_columns);
    }

    /* @brief get center, down-left, down, down-right and right surrounding elements
     *  @param center grid tile index
     *  @return list of element doubly linked list
     */
    const std::vector<T *> get_surrounding_elements(const int center)
    {
        std::vector<T *> surrounding_elements;
        int row = int(center / nb_columns);
        int column = center - (row * nb_columns);
        surrounding_elements.emplace_back(grid[center]);
        if (column < nb_columns - 1) // right
        {
            surrounding_elements.emplace_back(grid[center + 1]);
        }
        if (row < nb_rows - 1) // down
        {
            surrounding_elements.emplace_back(grid[center + nb_columns]);
        }
        if (column > 0 && row < nb_rows - 1) // down-left
        {
            surrounding_elements.emplace_back(grid[center + nb_columns - 1]);
        }
        if (column < nb_columns - 1 && row < nb_rows - 1) // down-right
        {
            surrounding_elements.emplace_back(grid[center + nb_columns + 1]);
        }
        return surrounding_elements;
    }

    /*
     * @brief move element in the right tile
     * new_x element new x position
     * new_y element new y position
     * element element not moved yet
     */
    void move_element(const float new_x, const float new_y, T *element)
    {
        int row = get_row(element->position.y());
        int column = get_column(element->position.x());
        int old_index = get_index(row, column);
        row = get_row(new_y);
        column = get_column(new_x);
        int new_index = get_index(row, column);
        if (old_index == new_index)
        {
            element->position[0] = new_x;
            element->position[1] = new_y;
        }
        else
        {
            remove(element, old_index);
            element->position[0] = new_x;
            element->position[1] = new_y;
            add(element, new_index);
        }
    }

    /*
     * @brief add new element in the container but not in the grid yet
     * @param x position in x
     * @param y position in y
     */
    void add_element(const float x, const float y)
    {
        elements.emplace_back(x, y);
    }

    /*
     * @brief add and sort all elements in the grid
     */
    void sort_grid()
    {
        for (auto &element : elements)
        {
            int row = get_row(element.position.y());
            int column = get_column(element.position.x());
            int index = get_index(row, column);
            add(&element,index);
        }
    }

    /*
     * @brief get all elements
     * @return all elements contained in the grid
     */
    std::vector<T> &get_all_elements()
    {
        return elements;
    }

    /*
     * @brief access to a grid tile
     * @param index tile index
     * @return grid tile
     */
    T * &operator[](int index)
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

private:
    /*
    * @brief remove element in the doubly linked list
    * @param element element to remove
    * @param index grid tile
    */
    void remove(T *element, int index)
    {
        if (element->previous == nullptr) //head
        {
            if (element->next == nullptr) //tail
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
            if (element->next == nullptr) //tail
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
    int get_row(const float y) const { return int((y - y_min) / gap); }

    /*
     * @brief get column index
     * @return column index
     */
    int get_column(const float x) const { return int((x - x_min) / gap); }

    /*
     * @brief get tile index
     * @return tile index
     */
    int get_index(const float row, const float column) const { return row * nb_columns + column; }

    /*grid x_min*/
    const float x_min = 0;
    /*grid y_min*/
    const float y_min = 0;
    /*tile gap*/
    const float gap = 0;
    /*number of columns*/
    const int nb_columns = 0;
    /*number of rows*/
    const int nb_rows = 0;
    /*grid tiles*/
    std::vector<T *> grid;
    /*list of elements*/
    std::vector<T> elements;
};