//
// Created by 冒家豪 on 2023/9/9.
//

#ifndef MAZE_GAMEMACHINE_H
#define MAZE_GAMEMACHINE_H

#include "MazeGenerator.h"

#include "imgui.h"
#include "SDL.h"

class GameMachine {
public:
    GameMachine() {
        SetRenderStyle();
    }

    void Render();

    void ProcessInput(SDL_Event &event);

private:
    MazeGenerator maze;
    int maze_witdh_ = 50, maze_height_ = 50;
    int rand_seed_ = 0;

    enum GameState {
        START, PLAYING, WIN
    };
    GameState game_state = START;

    std::vector<uint8_t> encoded_maze;

    void RenderStartState();

    void RenderPlayingState();

    void RenderWinState();

    void ProcessInputStartState(SDL_Event &event);

    void ProcessInputPlayingState(SDL_Event &event);

    void ProcessInputWinState(SDL_Event &event);

    void DrawMaze(ImDrawList *draw_list);

    void SetRenderStyle();

    void StartGame();

    void TryMovePlayer(int di, int dj);

    std::pair<int, int> player_coord;
};


#endif //MAZE_GAMEMACHINE_H
