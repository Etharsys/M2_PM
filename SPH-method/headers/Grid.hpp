#include <array>
#include <unordered_set>
#include <Particle.hpp>
#include <memory>
#include <iostream>

template <class T>
class Grid
{
public:
    Grid(const float gap_, const float x_min_, const float x_max_, const float y_min_, const float y_max_) : gap(gap_), x_min(x_min_), x_max(x_max_), y_min(y_min_), y_max(y_max_), nb_columns(get_column(x_max) + 1), nb_rows(get_row(y_max) + 1)
    {
        grid = std::vector<std::unordered_set<T *>>(nb_rows * nb_columns);
    }

    const std::vector<std::unordered_set<T *>> get_surrounding_elements(const int center) const
    {
        std::vector<std::unordered_set<T *>> surrounding_elements;
        int row = int(center/nb_columns);
        int column = center - (row * nb_columns) ;
        surrounding_elements.emplace_back(grid[center]);
        if (column > 0) // left
        {
            surrounding_elements.emplace_back(grid[ center - 1]);
        }
        if (column < nb_columns - 1) // right
        {
            surrounding_elements.emplace_back(grid[ center + 1]);
        }
        if (row > 0) // up
        {
            surrounding_elements.emplace_back(grid[ center - nb_columns]);
        }
        if (row < nb_rows - 1) // down
        {
            surrounding_elements.emplace_back(grid[ center + nb_columns]);
        }
        if (column > 0 && row > 0) // up-left
        {
            surrounding_elements.emplace_back(grid[ center - nb_columns - 1]);
        }
        if (column < nb_columns - 1 && row > 0) // up-right
        {
            surrounding_elements.emplace_back(grid[ center - nb_columns + 1]);
        }
        if (column > 0 && row < nb_rows - 1) // down-left
        {
            surrounding_elements.emplace_back(grid[ center + nb_columns - 1]);
        }
        if (column < nb_columns - 1 && row < nb_rows - 1) // down-right
        {
            surrounding_elements.emplace_back(grid[ center + nb_columns + 1]);
        }
        return surrounding_elements;
    }

    void move_element(const float new_x, const float new_y, T &element)
    {
        int row = get_row(element.position.y());
        int column = get_column(element.position.x());
        int old_index = get_index(row, column);
        row = get_row(new_y);
        column = get_column(new_x);
        int new_index = get_index(row, column);
        if (old_index == new_index)
        {
            element.position[0] = new_x;
            element.position[1] = new_y;
        }
        else
        {
            grid[old_index].erase(&element);
            element.position[0] = new_x;
            element.position[1] = new_y;
            grid[new_index].insert(&element);
        }
    }

    void add_element(const float x, const float y)
    {
        elements.emplace_back(x, y);
    }

    void sort_grid()
    {
        for (auto &element : elements)
        {
            int row = get_row(element.position.y());
            int column = get_column(element.position.x());
            int index = get_index(row, column);
            grid[index].insert(&element);
        }
    }

    std::vector<T> &get_all_elements()
    {
        return elements;
    }

    std::unordered_set<T *>& operator[](int index)
    {
        return grid[index];
    }

    int size()
    {
        return grid.size();
    }

private:
    int get_row(const float y) const { return int((y - y_min) / gap); }

    int get_column(const float x) const { return int((x - x_min) / gap); }

    int get_index(const float row, const float column) const { return row * nb_columns + column; }

    const float x_min = 0;
    const float x_max = 0;
    const float y_min = 0;
    const float y_max = 0;
    const float gap = 0;
    const int nb_columns = 0;
    const int nb_rows = 0;
    std::vector<std::unordered_set<T *>> grid;
    std::vector<T> elements;
};