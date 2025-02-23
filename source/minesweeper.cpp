// #include <bits/stdc++.h>
// #include <string>
// #include <fstream>
#include "raylib.h"
#include "field.h"

enum UI_STATE
{
    MAIN_MENU,
    IN_GAME,
    GAME_SETTING_GENERAL,
    GAME_SETTING_CUSTOM,
    HIGH_SCORE,
    QUIT
};

UI_STATE app_state = MAIN_MENU;

// SYSTEM CONSTANTS
const int screenWidth = 1920;
const int screenHeight = 1080;

// Scrolling
float scrollingBack = 0.0f;

// Origin
Vector2 origin = {0.0f, 0.0f};

// Declare Textures
Texture2D logo;
Texture2D playButton;
Texture2D loadgameButton;
Texture2D quitgameButton;
Texture2D clockTexture;
Texture2D flagTexture;
Texture2D numTexture[10];
Texture2D colonTexture;
Texture2D menuTexture;
Texture2D background;
Texture2D winTexture;
Texture2D loseTexture;
Texture2D incTexture;
Texture2D decTexture;
Texture2D widthTexture;
Texture2D startTexture;
Texture2D heightTexture;
Texture2D mineTexture;
Texture2D easyTexture;
Texture2D normalTexture;
Texture2D hardTexture;
Texture2D customTexture;
Texture2D scoreTexture;
Texture2D yourbestTexture;
Texture2D cupTexture;

// Declare Rectangles
Rectangle plbutton;
Rectangle continueRec;
Rectangle quitRec;
Rectangle dstClock;
Rectangle dstFlag;
Rectangle numRec[7];
Rectangle dstMenu;
Rectangle dstInc[3];
Rectangle dstDec[3];
Rectangle dstCustom[6];
Rectangle dstWidth;
Rectangle dstHeight;
Rectangle dstMine;
Rectangle dstCustom2;
Rectangle dstScore[6];
Rectangle dstScoreTex;

// Declare Scale Variables
float playbutton_scale;
float continue_scale;
float quit_scale;
float custom_scale;
float easy_scale;
float normal_scale;
float hard_scale;
float menu_scale;
float start_scale;
float inc_scale[3];
float dec_scale[3];
float cup_scale;

// Declare NPATCH_INFO
NPatchInfo npClock;
NPatchInfo npFlag;
NPatchInfo npNum[10];
NPatchInfo npColon;
NPatchInfo npMenu;
NPatchInfo npInc;
NPatchInfo npDec;
NPatchInfo npWidth;
NPatchInfo npHeight;
NPatchInfo npMine;
NPatchInfo npCustom;
NPatchInfo npScore;

void mousezoomButton(const Vector2 &mousePoint, const Rectangle &buttonRec, float &scale_obj)
{
    if (CheckCollisionPointRec(mousePoint, buttonRec))
    {
        scale_obj += 0.06f;
        scale_obj = std::min(scale_obj, 1.2f);
    }
    else
    {
        scale_obj = 1.0f;
    }
}

int getNum(int num, int ind)
{
    num = std::min(num, 60);
    ind = 2 - ind - 1;
    while (ind--)
    {
        num /= 10;
    }
    return num % 10;
}

void LoadAllTextures()
{
    // Background
    background = LoadTexture("resources/background.png");

    logo = LoadTexture("resources/minesweeper.png");
    playButton = LoadTexture("resources/playbutton.png");
    loadgameButton = LoadTexture("resources/loadgame.png");
    quitgameButton = LoadTexture("resources/quitgame.png");

    plbutton = {1.0f * screenWidth / 2 - playButton.width / 2,
                1.0f * screenHeight / 2 - playButton.height / 2,
                1.0f * playButton.width,
                1.0f * playButton.height};
    continueRec = {1.0f * screenWidth / 2 - loadgameButton.width / 2,
                   1.0f * screenHeight / 2 - playButton.height / 2 + playButton.height + 75,
                   1.0f * loadgameButton.width,
                   1.0f * loadgameButton.height};
    quitRec = {1.0f * screenWidth / 2 - quitgameButton.width / 2,
               1.0f * screenHeight / 2 - playButton.height / 2 + playButton.height + 80 - loadgameButton.height / 2 + loadgameButton.height + 80 - quitgameButton.height / 2,
               1.0f * quitgameButton.width,
               1.0f * quitgameButton.height};

    playbutton_scale = 1.0f;
    continue_scale = 1.0f;
    quit_scale = 1.0f;

    // Clock prepare
    clockTexture = LoadTexture("resources/clock.png");
    dstClock = {40.0f, 10.0f, 64.0f, 64.0f};
    npClock = {(Rectangle){0.0f, 0.0f, 128.0f, 128.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    // Flag prepare
    flagTexture = LoadTexture("resources/flag.png");
    dstFlag = {40.0f, 10.0f + 64.0f + 30.0f, 64.0f, 64.0f};
    npFlag = {(Rectangle){0.0f, 0.0f, 120.0f, 120.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    // Numbers prepare
    for (int i = 0; i < 10; i++)
    {
        numTexture[i] = LoadTexture(("resources/numbers/" + std::to_string(i) + ".png").c_str());
    }

    npNum[0] = {(Rectangle){0.0f, 0.0f, 80.0f, 85.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npNum[1] = {(Rectangle){0.0f, 0.0f, 66.0f, 83.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npNum[2] = {(Rectangle){0.0f, 0.0f, 79.0f, 84.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npNum[3] = {(Rectangle){0.0f, 0.0f, 78.0f, 85.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npNum[4] = {(Rectangle){0.0f, 0.0f, 87.0f, 83.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npNum[5] = {(Rectangle){0.0f, 0.0f, 77.0f, 85.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npNum[6] = {(Rectangle){0.0f, 0.0f, 80.0f, 85.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npNum[7] = {(Rectangle){0.0f, 0.0f, 78.0f, 83.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npNum[8] = {(Rectangle){0.0f, 0.0f, 75.0f, 85.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npNum[9] = {(Rectangle){0.0f, 0.0f, 80.0f, 85.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    numRec[0] = {40.0f + 64.0f + 15.0f, 10.0f + 16.0f, 32.0f, 32.0f};
    numRec[1] = {40.0f + 64.0f + 15.0f + 32.0f + 5.0f, 10.0f + 16.0f, 32.0f, 32.0f};
    numRec[2] = {40.0f + 64.0f + 15.0f + 32.0f + 5.0f + 32.0f + 15.0f, 10.0f + 16.0f, 16.0f, 32.0f};
    numRec[3] = {40.0f + 64.0f + 15.0f + 32.0f + 5.0f + 32.0f + 15.0f + 16.0f + 15.0f, 10.0f + 16.0f, 32.0f, 32.0f};
    numRec[4] = {40.0f + 64.0f + 15.0f + 32.0f + 5.0f + 32.0f + 15.0f + 16.0f + 15.0f + 32.0f + 5.0f, 10.0f + 16.0f, 32.0f, 32.0f};
    numRec[5] = {40.0f + 64.0f + 15.0f, 10.0f + 64.0f + 30.0f + 16.0f, 32.0f, 32.0f};
    numRec[6] = {40.0f + 64.0f + 15.0f + 32.0f + 5.0f, 10.0f + 64.0f + 30.0f + 16.0f, 32.0f, 32.0f};

    // Colon prepare
    colonTexture = LoadTexture("resources/numbers/colon.png");
    npColon = {(Rectangle){0.0f, 0.0f, 40.0f, 77.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    // Menu
    menuTexture = LoadTexture("resources/menu.png");
    dstMenu = {1920.0f - 30.0f - 100.0f, 30.0f, 100.0f, 100.0f};
    npMenu = {(Rectangle){0.0f, 0.0f, 230.0f, 242.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    menu_scale = 1.0f;

    // Win Text
    winTexture = LoadTexture("resources/win.png");

    // Lose Text
    loseTexture = LoadTexture("resources/lose.png");

    // Inc
    incTexture = LoadTexture("resources/inc.png");
    dstInc[0] = {483.0f, 420.0f, 76.0f, 90.0f};
    dstInc[1] = {1042.0f, 420.0f, 76.0f, 90.0f};
    dstInc[2] = {1595.0f, 420.0f, 76.0f, 90.0f};
    inc_scale[0] = inc_scale[1] = inc_scale[2] = 1.0f;
    npInc = {(Rectangle){0.0f, 0.0f, 124.0f, 144.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    // Dec
    decTexture = LoadTexture("resources/dec.png");
    dstDec[0] = {236.0f, 420.0f, 76.0f, 90.0f};
    dstDec[1] = {795.0f, 420.0f, 76.0f, 90.0f};
    dstDec[2] = {1349.0f, 420.0f, 76.0f, 90.0f};
    dec_scale[0] = dec_scale[1] = dec_scale[2] = 1.0f;
    npDec = {(Rectangle){0.0f, 0.0f, 124.0f, 144.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    // Custom number
    dstCustom[0] = {351.0f, 440.0f, 46.0f, 49.0f};
    dstCustom[1] = {405.0f, 440.0f, 46.0f, 49.0f};
    dstCustom[2] = {910.0f, 440.0f, 46.0f, 49.0f};
    dstCustom[3] = {964.0f, 440.0f, 46.0f, 49.0f};
    dstCustom[4] = {1464.0f, 440.0f, 46.0f, 49.0f};
    dstCustom[5] = {1518.0f, 440.0f, 46.0f, 49.0f};

    // Start button
    startTexture = LoadTexture("resources/continue.png");
    start_scale = 1.0f;

    // properties
    widthTexture = LoadTexture("resources/width.png");
    heightTexture = LoadTexture("resources/height.png");
    mineTexture = LoadTexture("resources/mines.png");

    dstWidth = {199.0f, 274.0f, 405.0f, 121.0f};
    dstHeight = {741.0f, 274.0f, 455.0f, 121.0f};
    dstMine = {1306.0f, 273.0f, 415.0f, 121.0f};

    npWidth = {(Rectangle){0.0f, 0.0f, 413.0f, 128.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npHeight = {(Rectangle){0.0f, 0.0f, 463.0f, 128.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    npMine = {(Rectangle){0.0f, 0.0f, 423.0f, 128.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    // easy
    easyTexture = LoadTexture("resources/easy.png");
    easy_scale = 1.0f;
    // normal
    normalTexture = LoadTexture("resources/normal.png");
    normal_scale = 1.0f;
    // hard
    hardTexture = LoadTexture("resources/hard.png");
    hard_scale = 1.0f;

    // custom
    customTexture = LoadTexture("resources/custom.png");
    dstCustom2 = {892.0f, 690.0f, 136.0f, 142.0f};
    npCustom = {(Rectangle){0.0f, 0.0f, 296.0f, 308.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    custom_scale = 1.0f;

    // dstScore
    scoreTexture = LoadTexture("resources/score.png");
    for (int i = 0; i < 6; i++)
    {
        dstScore[i] = {40.0f + 64.0f + 15.0f + 37.0f * i, 10.0f + 64.0f + 30.0f + 16.0f + 94.0f, 32.0f, 32.0f};
    }
    dstScoreTex = {40.0f, 10.0f + 64.0f + 30.0f + 94.0f, 64.0f, 64.0f};
    npScore = {(Rectangle){0.0f, 0.0f, 175.0f, 174.0f}, 0, 0, 0, 0, NPATCH_NINE_PATCH};

    // yourbest
    yourbestTexture = LoadTexture("resources/bestscore.png");

    // cup
    cupTexture = LoadTexture("resources/cup.png");
    cup_scale = 1.0f;

    // dstScore = {};
    // numRec[5] = {40.0f + 64.0f + 15.0f, 10.0f + 64.0f + 30.0f + 16.0f, 32.0f, 32.0f};
    // numRec[6] = {40.0f + 64.0f + 15.0f + 32.0f + 5.0f, 10.0f + 64.0f + 30.0f + 16.0f, 32.0f, 32.0f};
}

void UnloadAll()
{
    UnloadTexture(background);
    UnloadTexture(logo);
    UnloadTexture(playButton);
    UnloadTexture(loadgameButton);
    UnloadTexture(quitgameButton);
    UnloadTexture(clockTexture);
    UnloadTexture(flagTexture);
    UnloadTexture(winTexture);
    UnloadTexture(loseTexture);
    UnloadTexture(incTexture);
    UnloadTexture(decTexture);
    UnloadTexture(startTexture);
    UnloadTexture(widthTexture);
    UnloadTexture(heightTexture);
    UnloadTexture(mineTexture);
    UnloadTexture(easyTexture);
    UnloadTexture(normalTexture);
    UnloadTexture(hardTexture);
    UnloadTexture(customTexture);
    for (int i = 0; i < 10; i++)
    {
        UnloadTexture(numTexture[i]);
    }
}

void backgroundFunc()
{
    scrollingBack -= 0.5f;
    if (scrollingBack <= -background.width)
        scrollingBack = 0;

    DrawTextureEx(background, (Vector2){scrollingBack, 0}, 0.0f, 1.0f, WHITE);
    DrawTextureEx(background, (Vector2){background.width + scrollingBack, 0}, 0.0f, 1.0f, WHITE);
}

int getDigit(int x, int ind)
{
    while (ind--)
    {
        x /= 10;
    }
    return x % 10;
}

bool isActionAt(const Rectangle &block)
{
    return CheckCollisionPointRec(GetMousePosition(), block) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

Rectangle getRecCenter(const Rectangle &block, float scale_obj)
{
    return (Rectangle){block.x - (scale_obj - 1.0f) * block.width / 2, block.y - (scale_obj - 1.0f) * block.height / 2, block.width * scale_obj, block.height * scale_obj};
}

int main()
{
    InitWindow(screenWidth, screenHeight, "minesweeper");

    // Prepare
    SetTargetFPS(60);
    ToggleFullscreen();
    LoadAllTextures();

    Field game;
    std::vector<int> prop(3, 9);
    prop[2] = 10;
    std::vector<int> mnVal(3, 9);
    mnVal[2] = 2;
    std::vector<int> mxVal(3, 24);
    mxVal[2] = 99;

    // Timer
    // bool needTime = false;
    // int elapsedSeconds = 0;

    while (!WindowShouldClose())
    {

        if (app_state == MAIN_MENU)
        {
            Vector2 mousePoint = GetMousePosition();

            // Playbutton
            mousezoomButton(mousePoint, plbutton, playbutton_scale);
            if (CheckCollisionPointRec(mousePoint, plbutton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                // game.gameState = RUNNING;
                // game.init(9, 15, 15);
                // app_state = IN_GAME;
                app_state = GAME_SETTING_GENERAL;
                playbutton_scale = 1.0f;
            }

            // Continue
            mousezoomButton(mousePoint, continueRec, continue_scale);
            if (CheckCollisionPointRec(mousePoint, continueRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (game.LoadGame())
                {
                    app_state = IN_GAME;
                    continue_scale = 1.0f;
                }
            }

            // Quit
            mousezoomButton(mousePoint, quitRec, quit_scale);
            if (CheckCollisionPointRec(mousePoint, quitRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                quit_scale = 1.0f;
                break;
            }

            mousezoomButton(mousePoint, (Rectangle){1672.0f, 824.0f, 188.0f, 200.0f}, cup_scale);
            if (CheckCollisionPointRec(mousePoint, (Rectangle){1672.0f, 824.0f, 188.0f, 200.0f}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                app_state = HIGH_SCORE;
                cup_scale = 1.0f;
            }
        }
        else if (app_state == IN_GAME)
        {
            if (game.gameState == RUNNING)
                game.takeAction();

            Vector2 mousePoint = GetMousePosition();
            mousezoomButton(mousePoint, dstMenu, menu_scale);

            if (CheckCollisionPointRec(mousePoint, dstMenu) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                game.SaveGame();
                app_state = MAIN_MENU;
                menu_scale = 1.0f;
            }
        }
        else if (app_state == GAME_SETTING_CUSTOM)
        {
            for (int i = 0; i < 3; i++)
            {
                if (isActionAt(dstInc[i]))
                {
                    prop[i] = std::min(prop[i] + 1, mxVal[i]);
                    if (i == 2)
                    {
                        prop[i] = std::min(prop[i], prop[0] * prop[1] - 10);
                    }
                }
                else if (isActionAt(dstDec[i]))
                {
                    prop[i] = std::max(prop[i] - 1, mnVal[i]);
                }
            }
            if (isActionAt((Rectangle){812, 654, 296, 308}))
            {
                game.init(prop[1], prop[0], prop[2]);
                app_state = IN_GAME;
                start_scale = 1.0f;
            }

            Vector2 mousePoint = GetMousePosition();
            mousezoomButton(mousePoint, dstMenu, menu_scale);
            mousezoomButton(mousePoint, (Rectangle){812, 654, startTexture.width, startTexture.height}, start_scale);

            for (int i = 0; i < 3; i++)
            {
                inc_scale[i] = (CheckCollisionPointRec(mousePoint, dstInc[i]) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ? 0.8f : 1.0f;
                dec_scale[i] = (CheckCollisionPointRec(mousePoint, dstDec[i]) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ? 0.8f : 1.0f;
            }

            if (CheckCollisionPointRec(mousePoint, dstMenu) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                app_state = MAIN_MENU;
                menu_scale = 1.0f;
            }
        }
        else if (app_state == GAME_SETTING_GENERAL)
        {
            Vector2 mousePoint = GetMousePosition();

            mousezoomButton(mousePoint, (Rectangle){466.0f, 461.0f, 1.0f * easyTexture.width, 1.0f * easyTexture.height}, easy_scale);
            mousezoomButton(mousePoint, (Rectangle){874.0f, 329.0f, 1.0f * normalTexture.width, 1.0f * normalTexture.height}, normal_scale);
            mousezoomButton(mousePoint, (Rectangle){1317.0f, 461.0f, 1.0f * hardTexture.width, 1.0f * hardTexture.height}, hard_scale);
            mousezoomButton(mousePoint, dstCustom2, custom_scale);
            mousezoomButton(mousePoint, dstMenu, menu_scale);

            if (isActionAt((Rectangle){466.0f, 461.0f, 1.0f * easyTexture.width, 1.0f * easyTexture.height}))
            {
                game.init(9, 9, 10);
                app_state = IN_GAME;
                easy_scale = 1.0f;
            }
            if (isActionAt((Rectangle){874.0f, 329.0f, 1.0f * normalTexture.width, 1.0f * normalTexture.height}))
            {
                game.init(14, 18, 40);
                app_state = IN_GAME;
                normal_scale = 1.0f;
            }
            if (isActionAt((Rectangle){1317.0f, 461.0f, 1.0f * hardTexture.width, 1.0f * hardTexture.height}))
            {
                game.init(24, 24, 99);
                app_state = IN_GAME;
                hard_scale = 1.0f;
            }
            if (isActionAt(dstCustom2))
            {
                app_state = GAME_SETTING_CUSTOM;
                custom_scale = 1.0f;
            }

            if (CheckCollisionPointRec(mousePoint, dstMenu) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                app_state = MAIN_MENU;
                menu_scale = 1.0f;
            }
        }
        else if (app_state == HIGH_SCORE)
        {
            Vector2 mousePoint = GetMousePosition();
            mousezoomButton(mousePoint, dstMenu, menu_scale);
            if (CheckCollisionPointRec(mousePoint, dstMenu) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                app_state = MAIN_MENU;
                menu_scale = 1.0f;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        backgroundFunc();

        if (app_state == IN_GAME)
        {
            int elapsedSeconds = game.getElapsed();
            int gameScore = std::max(0, game.calcFinalScore());

            // # of flags
            DrawTextureNPatch(flagTexture, npFlag, dstFlag, origin, 0.0f, WHITE);
            DrawTextureNPatch(numTexture[std::max(0, (game.cntMine) / 10)], npNum[std::max(0, (game.cntMine) / 10)], numRec[5], origin, 0.0f, WHITE);
            DrawTextureNPatch(numTexture[std::max(0, (game.cntMine) % 10)], npNum[std::max(0, (game.cntMine) % 10)], numRec[6], origin, 0.0f, WHITE);

            // game
            game.printGame();
            if (game.gameState == WIN)
            {
                // Fade
                DrawRectangle(0, 0, 1920, 1080, Fade(BLACK, 0.7f));

                // Win
                DrawTexture(winTexture, 683, 490, WHITE);

                // Update Score
                std::ifstream inp("highscore.txt");
                int curBest = 0;
                inp >> curBest;
                curBest = std::max(curBest, gameScore);
                std::ofstream updScore("highscore.txt");
                updScore << curBest << "\n";
                // DrawText("CONGRATS!", 190, 200, 100, MAGENTA);
            }
            else if (game.gameState == LOSE)
            {
                // Fade
                DrawRectangle(0, 0, 1920, 1080, Fade(BLACK, 0.7f));

                // Win
                DrawTexture(loseTexture, 623, 490, WHITE);
            }

            // Timer
            DrawTextureNPatch(clockTexture, npClock, dstClock, origin, 0.0f, WHITE);
            DrawTextureNPatch(numTexture[getNum(elapsedSeconds / 60, 0)], npNum[getNum(elapsedSeconds / 60, 0)], numRec[0], origin, 0.0f, WHITE);
            DrawTextureNPatch(numTexture[getNum(elapsedSeconds / 60, 1)], npNum[getNum(elapsedSeconds / 60, 1)], numRec[1], origin, 0.0f, WHITE);
            DrawTextureNPatch(colonTexture, npColon, numRec[2], origin, 0.0f, WHITE);
            DrawTextureNPatch(numTexture[getNum(elapsedSeconds % 60, 0)], npNum[getNum(elapsedSeconds % 60, 0)], numRec[3], origin, 0.0f, WHITE);
            DrawTextureNPatch(numTexture[getNum(elapsedSeconds % 60, 1)], npNum[getNum(elapsedSeconds % 60, 1)], numRec[4], origin, 0.0f, WHITE);

            // Menu button
            DrawTextureNPatch(menuTexture, npMenu, getRecCenter(dstMenu, menu_scale), origin, 0.0f, WHITE);

            // score
            DrawTextureNPatch(scoreTexture, npScore, dstScoreTex, origin, 0.0f, WHITE);
            for (int i = 0; i < 5; i++)
            {
                DrawTextureNPatch(numTexture[getDigit(gameScore, 4 - i)], npNum[getDigit(gameScore, 4 - i)], dstScore[i], origin, 0.0f, WHITE);
            }
        }
        else if (app_state == MAIN_MENU)
        {
            // Game logo
            DrawTexture(logo, screenWidth / 2 - logo.width / 2, 40, WHITE);

            // Playbutton
            DrawTextureEx(playButton,
                          (Vector2){screenWidth / 2 - playButton.width * playbutton_scale / 2,
                                    screenHeight / 2 - playButton.height * playbutton_scale / 2},
                          0.0f,
                          playbutton_scale,
                          WHITE);

            // Continue
            DrawTextureEx(loadgameButton,
                          (Vector2){screenWidth / 2 - loadgameButton.width * continue_scale / 2,
                                    screenHeight / 2 - playButton.height / 2 + playButton.height + 80 - loadgameButton.height * continue_scale / 2},
                          0.0f,
                          continue_scale,
                          WHITE);

            // Quit
            DrawTextureEx(quitgameButton,
                          (Vector2){screenWidth / 2 - quitgameButton.width * quit_scale / 2,
                                    screenHeight / 2 - playButton.height / 2 + playButton.height + 80 - loadgameButton.height / 2 + loadgameButton.height + 80 - quitgameButton.height * quit_scale / 2},
                          0.0f,
                          quit_scale,
                          WHITE);

            // Cup
            DrawTextureEx(cupTexture,
                          (Vector2){1672.0f - cupTexture.width * (cup_scale - 1.0f) / 2,
                                    824.0f - cupTexture.height * (cup_scale - 1.0f) / 2},
                          0.0f,
                          cup_scale,
                          WHITE);
        }
        else if (app_state == GAME_SETTING_CUSTOM)
        {
            // print MENU
            DrawTextureNPatch(menuTexture, npMenu, getRecCenter(dstMenu, menu_scale), origin, 0.0f, WHITE);

            // draw Inc and Dec
            for (int i = 0; i < 3; i++)
            {
                DrawTextureNPatch(incTexture, npInc, getRecCenter(dstInc[i], inc_scale[i]), origin, 0.0f, WHITE);
                DrawTextureNPatch(decTexture, npDec, getRecCenter(dstDec[i], dec_scale[i]), origin, 0.0f, WHITE);
            }

            // draw numbers
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    DrawTextureNPatch(numTexture[getDigit(prop[i], 1 - j)], npNum[getDigit(prop[i], 1 - j)], dstCustom[i * 2 + j], origin, 0.0f, WHITE);
                }
            }

            // draw properties text
            DrawTextureNPatch(widthTexture, npWidth, dstWidth, origin, 0.0f, WHITE);
            DrawTextureNPatch(heightTexture, npHeight, dstHeight, origin, 0.0f, WHITE);
            DrawTextureNPatch(mineTexture, npMine, dstMine, origin, 0.0f, WHITE);

            // draw Start button
            // DrawTexture(startTexture, 812, 654, WHITE);
            DrawTextureEx(startTexture, (Vector2){812 - (start_scale - 1.0f) * startTexture.width / 2, 654 - (start_scale - 1.0f) * startTexture.height / 2}, 0.0f, start_scale, WHITE);
        }
        else if (app_state == GAME_SETTING_GENERAL)
        {
            DrawTextureEx(easyTexture, (Vector2){466 - (easy_scale - 1.0f) * easyTexture.width / 2, 461 - (easy_scale - 1.0f) * easyTexture.height / 2}, 0.0f, easy_scale, WHITE);
            DrawTextureEx(normalTexture, (Vector2){874 - (normal_scale - 1.0f) * normalTexture.width / 2, 329 - (normal_scale - 1.0f) * normalTexture.height / 2}, 0.0f, normal_scale, WHITE);
            DrawTextureEx(hardTexture, (Vector2){1317 - (hard_scale - 1.0f) * hardTexture.width / 2, 461 - (hard_scale - 1.0f) * hardTexture.height / 2}, 0.0f, hard_scale, WHITE);
            DrawTextureNPatch(customTexture, npCustom, getRecCenter(dstCustom2, custom_scale), origin, 0.0f, WHITE);
            DrawTextureNPatch(menuTexture, npMenu, getRecCenter(dstMenu, menu_scale), origin, 0.0f, WHITE);
        }
        else if (app_state == HIGH_SCORE)
        {
            DrawTexture(yourbestTexture, 380, 225, WHITE);
            DrawTexture(scoreTexture, 150, 186, WHITE);
            DrawTexture(scoreTexture, 1595, 186, WHITE);

            // print score
            std::ifstream inpScore("highscore.txt");
            std::string sc;
            inpScore >> sc;

            int lowerX = 1920 / 2 - 100 * ((int)sc.size()) / 2, lowerY = 1080 / 2 - 50 / 2;
            for (int i = 0; i < (int)sc.size(); i++)
            {
                DrawTextureNPatch(numTexture[sc[i] - '0'], npNum[sc[i] - '0'],
                                  (Rectangle){1.0f * lowerX + 100 * i, 1.0f * lowerY, 100.0f, 100.0f},
                                  origin,
                                  0.0f,
                                  WHITE);
            }

            // menu
            DrawTextureNPatch(menuTexture, npMenu, getRecCenter(dstMenu, menu_scale), origin, 0.0f, WHITE);
        }

        EndDrawing();
    }

    UnloadAll();
    CloseWindow();

    return 0;
}