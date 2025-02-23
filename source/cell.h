#pragma once

#include <vector>

enum CELL_STATE
{
    NOTHING,
    MINE,
    ADJ_MINE
};

struct Cell
{
    int num_adj;
    CELL_STATE state;
    bool hidden;
    bool flagged;
    int num_flag_adj;
    Cell();
};

Cell::Cell()
{
    hidden = true;
    flagged = false;
    state = NOTHING;
    num_adj = num_flag_adj = 0;
}

using GameGrid = std::vector<std::vector<Cell>>;