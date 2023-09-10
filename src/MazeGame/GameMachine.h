//
// Created by 冒家豪 on 2023/9/9.
//

#ifndef MAZE_GAMEMACHINE_H
#define MAZE_GAMEMACHINE_H

#include "GameResource.h"
#include "MazeGenerator.h"

#include "imgui.h"
#include "SDL.h"

class GameMachine {
public:
    GameMachine() {
        SetRenderStyle();
    }

    void Render();

    bool ProcessInput(SDL_Event &event);

    bool IsEnd() const { return game_state == END; }

private:
    GameResource game_resource_;
    MazeGenerator maze;
    int maze_width_ = 50, maze_height_ = 50;
    int rand_seed_ = 0;
    bool has_win_waited_ = false; // when player arrive end point, wait some time before show win image.
    bool show_win_image_ = true;
    bool have_found_path_ = false;

    enum GameState {
        START, PLAYING, WIN, END
    };
    GameState game_state = START;

    std::vector<uint8_t> encoded_maze;

    void RenderStartState();

    void RenderPlayingState();

    void RenderWinState();

    bool ProcessInputStartState(SDL_Event &event);

    bool ProcessInputPlayingState(SDL_Event &event);

    bool ProcessInputWinState(SDL_Event &event);

    void DrawMaze(ImDrawList *draw_list);

    void SetRenderStyle();

    void StartGame();

    void TryMovePlayer(int di, int dj);

    void RenderFunctionalButton();

    void FindShortestPath();

    void RenderShortestPathFound();

    void SetStartPoint(int mouse_x, int mouse_y);

    void SetEndPoint(int mouse_x, int mouse_y);

    std::pair<int, int> MousePositionToMazeCoord(int mouse_x, int mouse_y);

    bool next_click_set_start_point_ = false;
    bool next_click_set_end_point_ = false;

    std::pair<int, int> player_coord_, end_coord_, start_coord_;
    std::vector<std::pair<int, int>> shortest_path;
    bool show_shortest_path_ = true;
    ImVec2 draw_start_coord_;
    float maze_pixel_size_;
};


#endif //MAZE_GAMEMACHINE_H
