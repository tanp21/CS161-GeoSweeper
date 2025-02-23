#pragma once

#include <chrono>
#include <random>
#include <utility>
#include <queue>
#include <tuple>
#include <fstream>
#include <string>
#include "raylib.h"
#include "cell.h"

const int CHORD_BONUS = 2;

enum GAMESTATE
{
    LOSE,
    WIN,
    RUNNING,
    IDLE
};

struct Field
{
    GAMESTATE gameState;
    GameGrid grid;
    bool first_hit;
    int numRow, numCol, numBomb;
    int currentSeed;
    int numHidden;
    Vector2 origin; //
    int cntMine;

    int CELL_SIZE;
    int lowerX, lowerY;

    int score;

    Texture2D cellTexture[3]; //
    NPatchInfo npcellinfo[3]; //

    Texture2D numTexture[8]; //
    NPatchInfo npnuminfo[8]; //

    Texture2D flagTexture; //
    NPatchInfo npflaginfo; //

    Texture2D mineTexture; //
    NPatchInfo npmineinfo; //

    int last_elapsed;
    std::chrono::time_point<std::chrono::high_resolution_clock> beginTime, currentTime;

    // Field(int num_row, int num_col, int num_bomb);
    bool isInside(int row, int col);
    bool canOpen(int row, int col);
    void startSweep(int row, int col, int pointBonus);
    void toggleFlag(int row, int col);
    void generateField(int row, int col);
    void openSurroundings(int row, int col);

    void showAllMines();

    void takeAction();

    void printCell(int row, int col);
    void printGame();
    void SaveGame();
    bool LoadGame();
    void init(int num_row, int num_col, int num_bomb);
    int calcFinalScore();

    int getElapsed();
    ~Field();
};

void Field::init(int num_row, int num_col, int num_bomb)
{
    score = std::min(99999, int(10000.0f * num_bomb / (num_row * num_col)) + num_row * num_col * 20);
    gameState = RUNNING;
    last_elapsed = 0;

    // int lo = 1, hi = 200;
    // while(lo < hi) {
    //     int mid = lo + (hi-lo+1)/2;
    //     if(1920 - mid*num_col >= 400 && 1080 - mid*num_row >= 20) lo = mid;
    //     else hi = mid - 1;
    // }
    // std::cerr << lo << "\n";

    CELL_SIZE = std::min((1920 - 700) / num_col, (1080 - 80) / num_row);
    int GRID_H = CELL_SIZE * num_row;
    int GRID_W = CELL_SIZE * num_col;
    lowerX = (1920 - GRID_W) / 2;
    lowerY = (1080 - GRID_H) / 2;

    numHidden = num_row * num_col;
    first_hit = true;
    numRow = num_row, numCol = num_col;
    numBomb = cntMine = num_bomb;
    grid.assign(num_row, std::vector<Cell>(num_col));

    cellTexture[0] = LoadTexture("resources/block_even.png");
    cellTexture[1] = LoadTexture("resources/block_odd.png");
    cellTexture[2] = LoadTexture("resources/block2.png");

    npcellinfo[0] = {(Rectangle){0.0f, 0.0f, 81.0f, 81.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npcellinfo[1] = {(Rectangle){0.0f, 0.0f, 81.0f, 81.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npcellinfo[2] = {(Rectangle){0.0f, 0.0f, 81.0f, 81.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    origin = {0.0f, 0.0f};

    numTexture[0] = LoadTexture("resources/1.png");
    numTexture[1] = LoadTexture("resources/2.png");
    numTexture[2] = LoadTexture("resources/3.png");
    numTexture[3] = LoadTexture("resources/4.png");
    numTexture[4] = LoadTexture("resources/5.png");
    numTexture[5] = LoadTexture("resources/6.png");
    numTexture[6] = LoadTexture("resources/7.png");
    numTexture[7] = LoadTexture("resources/8.png");

    npnuminfo[0] = {(Rectangle){0.0f, 0.0f, 76.0f, 101.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npnuminfo[1] = {(Rectangle){0.0f, 0.0f, 101.0f, 101.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npnuminfo[2] = {(Rectangle){0.0f, 0.0f, 101.0f, 101.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npnuminfo[3] = {(Rectangle){0.0f, 0.0f, 101.0f, 101.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npnuminfo[4] = {(Rectangle){0.0f, 0.0f, 101.0f, 101.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npnuminfo[5] = {(Rectangle){0.0f, 0.0f, 101.0f, 101.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npnuminfo[6] = {(Rectangle){0.0f, 0.0f, 101.0f, 101.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npnuminfo[7] = {(Rectangle){0.0f, 0.0f, 101.0f, 101.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    flagTexture = LoadTexture("resources/flag.png");
    npflaginfo = {(Rectangle){0.0f, 0.0f, 120.0f, 120.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    mineTexture = LoadTexture("resources/mine.png");
    npmineinfo = {(Rectangle){0.0f, 0.0f, 81.0f, 81.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
}

bool Field::isInside(int row, int col)
{
    return std::min(row, col) >= 0 && row < numRow && col < numCol;
}

bool Field::canOpen(int row, int col)
{
    return grid[row][col].state != MINE && !grid[row][col].flagged && grid[row][col].hidden;
}

void Field::generateField(int row, int col)
{
    beginTime = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<bool>> marked(numRow, std::vector<bool>(numCol, false));
    std::random_device rd;
    currentSeed = rd();
    std::mt19937 gen(currentSeed);
    std::uniform_int_distribution<int> RowGen(0, numRow - 1);
    std::uniform_int_distribution<int> ColGen(0, numCol - 1);

    std::vector<std::pair<int, int>> lst_mines;

    for (int t = 0; t < numBomb; t++)
    {
        while (true)
        {
            int i = RowGen(gen);
            int j = ColGen(gen);
            if ((i >= row - 1 && i <= row + 1 && j >= col - 1 && j <= col + 1) || marked[i][j])
            {
                continue;
            }
            marked[i][j] = true;
            grid[i][j].state = MINE;
            lst_mines.push_back({i, j});

            break;
        }
    }

    for (const auto &[i, j] : lst_mines)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                if (dx != 0 || dy != 0)
                {
                    if (isInside(i + dx, j + dy))
                    {
                        grid[i + dx][j + dy].num_adj++;
                        if (grid[i + dx][j + dy].state != MINE)
                        {
                            grid[i + dx][j + dy].state = ADJ_MINE;
                        }
                    }
                }
            }
        }
    }
}

void Field::startSweep(int row, int col, int pointBonus)
{
    if (!grid[row][col].hidden || grid[row][col].flagged)
        return;
    if (first_hit)
    {
        generateField(row, col);
        first_hit = false;
    }
    if (grid[row][col].state == MINE)
    {
        showAllMines();
        gameState = LOSE;
        return;
    }
    std::queue<std::pair<int, int>> q;
    q.push({row, col});
    grid[row][col].hidden = false;
    numHidden--;
    score += pointBonus;
    while (!q.empty())
    {
        std::tie(row, col) = q.front();
        q.pop();
        if (grid[row][col].state == ADJ_MINE)
        {
            score += 2 * grid[row][col].num_adj;
            continue;
        }

        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                if (dx != 0 || dy != 0)
                {
                    if (isInside(row + dx, col + dy) && canOpen(row + dx, col + dy))
                    {
                        numHidden--;
                        score += pointBonus;
                        grid[row + dx][col + dy].hidden = false;
                        q.push({row + dx, col + dy});
                    }
                }
            }
        }
    }
}

void Field::toggleFlag(int row, int col)
{
    if (!grid[row][col].hidden)
        return;
    int to_add = int((grid[row][col].flagged ^= 1) << 1) - 1;
    cntMine -= to_add;
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            if (dx != 0 || dy != 0)
            {
                if (isInside(row + dx, col + dy))
                {
                    grid[row + dx][col + dy].num_flag_adj += to_add;
                }
            }
        }
    }
}

void Field::showAllMines()
{
    for (int i = 0; i < numRow; i++)
    {
        for (int j = 0; j < numCol; j++)
        {
            if (grid[i][j].state == MINE)
            {
                grid[i][j].hidden = false;
            }
        }
    }
}

void Field::openSurroundings(int row, int col)
{
    if (grid[row][col].flagged)
        return;
    if (grid[row][col].hidden)
    {
        startSweep(row, col, 1);
        return;
    }
    if (grid[row][col].state == NOTHING)
        return;
    bool ok = true;
    if (grid[row][col].state == ADJ_MINE)
    {
        if (grid[row][col].num_adj == grid[row][col].num_flag_adj)
        {
            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    if (dx != 0 || dy != 0)
                    {
                        if (!isInside(row + dx, col + dy))
                            continue;
                        if (!grid[row + dx][col + dy].flagged && grid[row + dx][col + dy].state == MINE)
                        {
                            ok = false;
                        }
                    }
                }
            }
        }
    }
    if (!ok)
    {
        showAllMines();
        gameState = LOSE;
        return;
    }
    if (grid[row][col].state == ADJ_MINE)
    {
        if (grid[row][col].num_adj == grid[row][col].num_flag_adj)
        {
            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    if (dx != 0 || dy != 0)
                    {
                        if (!isInside(row + dx, col + dy))
                            continue;
                        if (!grid[row + dx][col + dy].flagged)
                        {
                            startSweep(row + dx, col + dy, 2);
                        }
                    }
                }
            }
        }
    }
}

void Field::takeAction()
{
    Vector2 mousePoint = GetMousePosition();
    GAMESTATE pre = gameState;
    // IsMouse
    for (int i = 0; i < numRow; i++)
    {
        for (int j = 0; j < numCol; j++)
        {
            Rectangle displayCell = {lowerX + CELL_SIZE * j, lowerY + CELL_SIZE * i, 1.0f * CELL_SIZE, 1.0f * CELL_SIZE};
            if (CheckCollisionPointRec(mousePoint, displayCell))
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && grid[i][j].hidden == false && grid[i][j].state == ADJ_MINE)
                    openSurroundings(i, j);
                else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    startSweep(i, j, 1);
                else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
                    toggleFlag(i, j);
                if (numHidden == numBomb)
                {
                    gameState = WIN;
                }
                if (gameState != pre)
                {
                    if (first_hit)
                    {
                        last_elapsed = 0;
                    }
                    else
                    {
                        currentTime = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::nano> elapsed = currentTime - beginTime;
                        last_elapsed += int(std::chrono::duration_cast<std::chrono::seconds>(elapsed).count());
                    }
                }
                return;
            }
        }
    }
}

void Field::printCell(int row, int col)
{
    Vector2 mousePoint = GetMousePosition();
    Rectangle displayCell = {lowerX + CELL_SIZE * col, lowerY + CELL_SIZE * row, 1.0f * CELL_SIZE, 1.0f * CELL_SIZE};
    Rectangle displayInfo = {lowerX + CELL_SIZE * col + 1.0f * CELL_SIZE / 4, lowerY + CELL_SIZE * row + 1.0f * CELL_SIZE / 4, 1.0f * CELL_SIZE / 2, 1.0f * CELL_SIZE / 2};
    if (grid[row][col].hidden)
    {
        DrawTextureNPatch(cellTexture[2], npcellinfo[2], displayCell, origin, 0.0f, WHITE);
        if (grid[row][col].flagged)
        {
            DrawTextureNPatch(flagTexture, npflaginfo, displayInfo, origin, 0.0f, WHITE);
        }
        if (gameState == RUNNING && CheckCollisionPointRec(mousePoint, displayCell))
        {
            DrawRectangleRec(displayCell, Fade(WHITE, 0.2));
        }
    }
    else
    {
        if (grid[row][col].state == ADJ_MINE)
        {
            DrawTextureNPatch(cellTexture[(row + col) & 1], npcellinfo[(row + col) & 1], displayCell, origin, 0.0f, WHITE);
            int num = grid[row][col].num_adj;
            DrawTextureNPatch(numTexture[num - 1], npnuminfo[num - 1], displayInfo, origin, 0.0f, WHITE);
            if (gameState == RUNNING && CheckCollisionPointRec(mousePoint, displayCell))
            {
                DrawRectangleRec(displayCell, Fade(WHITE, 0.2));
            }
        }
        else if (grid[row][col].state == MINE)
        {
            DrawTextureNPatch(mineTexture, npmineinfo, displayCell, origin, 0.0f, WHITE);
        }
        else
        {
            DrawTextureNPatch(cellTexture[(row + col) & 1], npcellinfo[(row + col) & 1], displayCell, origin, 0.0f, WHITE);
        }
    }
}

void Field::printGame()
{
    for (int i = 0; i < numRow; i++)
    {
        for (int j = 0; j < numCol; j++)
        {
            printCell(i, j);
        }
    }
}

void Field::SaveGame()
{
    last_elapsed = getElapsed();

    std::ofstream myFile("save.txt");
    myFile << score << "\n";
    myFile << gameState << "\n";
    myFile << (int)first_hit << "\n";
    myFile << numRow << "\n";
    myFile << numCol << "\n";
    myFile << numBomb << "\n";
    myFile << currentSeed << "\n";
    myFile << numHidden << "\n";
    myFile << cntMine << "\n";
    myFile << CELL_SIZE << "\n";
    myFile << lowerX << "\n";
    myFile << lowerY << "\n";
    myFile << last_elapsed << "\n";
    for (int i = 0; i < numRow; i++)
    {
        for (int j = 0; j < numCol; j++)
        {
            myFile << grid[i][j].num_adj << "\n";
            myFile << grid[i][j].state << "\n";
            myFile << grid[i][j].hidden << "\n";
            myFile << grid[i][j].flagged << "\n";
            myFile << grid[i][j].num_flag_adj << "\n";
        }
    }
}

bool Field::LoadGame()
{
    std::ifstream inp("save.txt");
    cellTexture[0] = LoadTexture("resources/block_even.png");
    cellTexture[1] = LoadTexture("resources/block_odd.png");
    cellTexture[2] = LoadTexture("resources/block2.png");

    npcellinfo[0] = {(Rectangle){0.0f, 0.0f, 81.0f, 81.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npcellinfo[1] = {(Rectangle){0.0f, 0.0f, 81.0f, 81.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npcellinfo[2] = {(Rectangle){0.0f, 0.0f, 81.0f, 81.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    origin = {0.0f, 0.0f};

    numTexture[0] = LoadTexture("resources/1.png");
    numTexture[1] = LoadTexture("resources/2.png");
    numTexture[2] = LoadTexture("resources/3.png");
    numTexture[3] = LoadTexture("resources/4.png");
    numTexture[4] = LoadTexture("resources/5.png");
    numTexture[5] = LoadTexture("resources/6.png");
    numTexture[6] = LoadTexture("resources/7.png");
    numTexture[7] = LoadTexture("resources/8.png");

    npnuminfo[0] = {(Rectangle){0.0f, 0.0f, 60.0f, 93.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npnuminfo[1] = {(Rectangle){0.0f, 0.0f, 109.0f, 93.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npnuminfo[2] = {(Rectangle){0.0f, 0.0f, 109.0f, 93.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npnuminfo[3] = {(Rectangle){0.0f, 0.0f, 108.0f, 93.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npnuminfo[4] = {(Rectangle){0.0f, 0.0f, 104.0f, 93.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npnuminfo[5] = {(Rectangle){0.0f, 0.0f, 104.0f, 93.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npnuminfo[6] = {(Rectangle){0.0f, 0.0f, 115.0f, 93.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npnuminfo[7] = {(Rectangle){0.0f, 0.0f, 113.0f, 93.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    flagTexture = LoadTexture("resources/flag.png");
    npflaginfo = {(Rectangle){0.0f, 0.0f, 120.0f, 120.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    mineTexture = LoadTexture("resources/mine.png");
    npmineinfo = {(Rectangle){0.0f, 0.0f, 81.0f, 81.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    if (inp.is_open())
    {
        inp >> score;
        int x;
        inp >> x;
        gameState = (GAMESTATE)x;
        inp >> x;
        first_hit = x;
        inp >> numRow;
        inp >> numCol;
        inp >> numBomb;
        inp >> currentSeed;
        inp >> numHidden;
        inp >> cntMine;
        inp >> CELL_SIZE;
        inp >> lowerX;
        inp >> lowerY;
        inp >> last_elapsed;
        grid.assign(numRow, std::vector<Cell>(numCol));
        for (int i = 0; i < numRow; i++)
        {
            for (int j = 0; j < numCol; j++)
            {
                inp >> grid[i][j].num_adj;
                inp >> x;
                grid[i][j].state = (CELL_STATE)x;
                inp >> x;
                grid[i][j].hidden = x;
                inp >> x;
                grid[i][j].flagged = x;
                inp >> grid[i][j].num_flag_adj;
            }
        }
        if (!first_hit)
        {
            beginTime = std::chrono::high_resolution_clock::now();
        }
        return true;
    }
    return false;
}

int Field::getElapsed()
{
    if (gameState != RUNNING)
        return last_elapsed;
    if (first_hit)
        return 0;
    currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> elapsed = currentTime - beginTime;
    return int(std::chrono::duration_cast<std::chrono::seconds>(elapsed).count()) + last_elapsed;
}

int Field::calcFinalScore()
{
    const int COEF = 5;
    if (gameState != RUNNING)
    {
        return score - COEF * getElapsed();
    }

    int res = score - COEF * getElapsed();

    for (int i = 0; i < numRow; i++)
    {
        for (int j = 0; j < numCol; j++)
        {
            if (grid[i][j].state == MINE && !grid[i][j].flagged)
            {
                res += 2;
            }
        }
    }
    return res;
}

Field::~Field()
{
    for (int i = 0; i < 3; i++)
    {
        UnloadTexture(cellTexture[i]);
    }
    for (int i = 0; i < 8; i++)
    {
        UnloadTexture(numTexture[i]);
    }
    UnloadTexture(flagTexture);
    UnloadTexture(mineTexture);
}