#include <array>
#include <unordered_set>
#include <Particle.hpp>
#include <memory>
#include <iostream>

template <class T, class H>
class Grid
{
public:
    Grid(const float gap_, const float x_min_, const float x_max_, const float y_min_, const float y_max_) : gap(gap_), x_min(x_min_), x_max(x_max_), y_min(y_min_), y_max(y_max_), nb_columns(get_column(x_max) + 1), nb_rows(get_row(y_max) + 1)
    {
        grid = std::vector<std::unordered_set<T *, H>>(nb_rows * nb_columns);
    }

    const std::vector<const T *> get_surrounding_elements(const float x, const float y) const
    {
        std::vector<const T *> surrounding_elements;
        if (x < x_min or x > x_max or y < y_min or y > y_max)
        {
            return surrounding_elements;
        }
        int row = get_row(y);
        int column = get_column(x);
        int center = get_index(row, column);
        add_surrounding_elements(surrounding_elements, center);
        if (column > 0) // left
        {
            add_surrounding_elements(surrounding_elements, center - 1);
        }
        if (column < nb_columns - 1) // right
        {
            add_surrounding_elements(surrounding_elements, center + 1);
        }
        if (row > 0) // up
        {
            add_surrounding_elements(surrounding_elements, center - nb_columns);
        }
        if (row < nb_rows - 1) // down
        {
            add_surrounding_elements(surrounding_elements, center + nb_columns);
        }
        if (column > 0 && row > 0) // up-left
        {
            add_surrounding_elements(surrounding_elements, center - nb_columns - 1);
        }
        if (column < nb_columns - 1 && row > 0) // up-right
        {
            add_surrounding_elements(surrounding_elements, center - nb_columns + 1);
        }
        if (column > 0 && row < nb_rows - 1) // down-left
        {
            add_surrounding_elements(surrounding_elements, center + nb_columns - 1);
        }
        if (column < nb_columns - 1 && row < nb_rows - 1) // down-right
        {
            add_surrounding_elements(surrounding_elements, center + nb_columns + 1);
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

private:
    int get_row(const float y) const { return int((y - y_min) / gap); }

    int get_column(const float x) const { return int((x - x_min) / gap); }

    int get_index(const float row, const float column) const { return row * nb_columns + column; }

    void add_surrounding_elements(std::vector<const T *>& surrounding_elements, const int index) const
    {
        for (auto &element_set : grid[index])
        {
            surrounding_elements.emplace_back(element_set);
        }
    }

    const float x_min = 0;
    const float x_max = 0;
    const float y_min = 0;
    const float y_max = 0;
    const float gap = 0;
    const int nb_columns = 0;
    const int nb_rows = 0;
    std::vector<std::unordered_set<T *, H>> grid;
    std::vector<T> elements;
};