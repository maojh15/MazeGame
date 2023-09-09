//
// Created by 冒家豪 on 2023/9/9.
//

#include "GameMachine.h"

#include <iostream>

void GameMachine::Render() {
    switch (game_state) {
        case START:
            RenderStartState();
            break;
        case PLAYING:
            RenderPlayingState();
            break;
        case WIN:
            RenderWinState();
            break;
        default:
            break;
    }
}

void GameMachine::RenderStartState() {
    std::string hint_text{"Please set parameters of Maze"};
    const auto &avail = ImGui::GetIO().DisplaySize;
    ImGui::SetCursorPosX((avail.x - ImGui::CalcTextSize(hint_text.c_str()).x) / 2);
    ImGui::SetCursorPosY(0.25f * avail.y);
    ImGui::Text("%s", hint_text.c_str());

    float slider_witdh = std::min(450.0, avail.x * 0.9);
    ImGui::PushItemWidth(slider_witdh);
    ImGui::SetCursorPosX((avail.x - slider_witdh) / 2);
    ImGui::SliderInt("width", &maze_witdh_, 2, 200, "width: %d");
    ImGui::SetCursorPosX((avail.x - slider_witdh) / 2);
    ImGui::SliderInt("height", &maze_height_, 2, 200, "height: %d");
    ImGui::PopItemWidth();

    float input_width = 300;
    ImGui::PushItemWidth(input_width);
    ImGui::SetCursorPosX((avail.x - input_width) / 2);
    ImGui::InputInt("random seed", &rand_seed_);
    ImGui::PopItemWidth();

    float button_witdh = 150;
    ImGui::SetCursorPosX((avail.x - button_witdh) / 2);
    if (ImGui::Button("START", ImVec2(button_witdh, 50))) {
        StartGame();
    }
}

void GameMachine::RenderPlayingState() {
    ImGui::Text("Playing maze ... ");
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    DrawMaze(draw_list);
}

void GameMachine::RenderWinState() {

}

void GameMachine::SetRenderStyle() {
    auto &style = ImGui::GetStyle();
    style.FrameRounding = 6;
    style.Colors[ImGuiCol_Text].x = style.Colors[ImGuiCol_Text].y = style.Colors[ImGuiCol_Text].z = 0;
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.956, 0.622, 0.190, 0.540);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.757, 0.439, 0.079, 0.400);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.729, 0.623, 0.282, 0.670);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.573, 0.449, 0.812, 1.0);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.643, 0.459, 0.762, 1.0);
    // style.Colors[ImGuiCol_Button] = (ImVec4)ImColor::HSV(6 / 7.0f, 0.6f, 0.6f);
    // style.Colors[ImGuiCol_ButtonHovered] =(ImVec4)ImColor::HSV(7 / 7.0f, 0.7f, 0.7f);
    // style.Colors[ImGuiCol_ButtonActive] = (ImVec4)ImColor::HSV(7 / 7.0f, 0.7f, 0.7f);
}

void GameMachine::DrawMaze(ImDrawList *draw_list) {
    auto io = ImGui::GetIO();
    const auto &avail = io.DisplaySize;
    float scale = 0.95;
    float pixel_size = std::min(scale * avail.x / maze_witdh_, scale * avail.y / maze_height_);

    ImVec2 start_coord((avail.x - maze_witdh_ * pixel_size) / 2, (avail.y - maze_height_ * pixel_size) / 2);
    ImColor wall_color(0, 0, 0, 255);
    ImU32 compress_wall_color = ImU32(wall_color);
    float boundary_wall_thick = 1.5f;

    // draw out rectangle
    ImVec2 tmp_coord1(start_coord.x + pixel_size, start_coord.y);
    ImVec2 tmp_coord2(start_coord.x + pixel_size * maze_witdh_, start_coord.y);
    draw_list->AddLine(tmp_coord1, tmp_coord2, compress_wall_color, boundary_wall_thick);
    tmp_coord1.x = tmp_coord2.x, tmp_coord1.y = tmp_coord2.y + pixel_size * (maze_height_ - 1);
    draw_list->AddLine(tmp_coord1, tmp_coord2, compress_wall_color, boundary_wall_thick);
    tmp_coord1.x = start_coord.x, tmp_coord1.y = start_coord.y + pixel_size;
    tmp_coord2.x = start_coord.x, tmp_coord2.y = start_coord.y + pixel_size * maze_height_;
    draw_list->AddLine(tmp_coord1, tmp_coord2, compress_wall_color, boundary_wall_thick);
    tmp_coord1.x = start_coord.x + pixel_size * (maze_witdh_ - 1);
    tmp_coord1.y = tmp_coord2.y;
    draw_list->AddLine(tmp_coord1, tmp_coord2, compress_wall_color, boundary_wall_thick);

    // draw maze
    float wall_thick = 1.0f;
    for (int i = 0; i < maze_height_; ++i) {
        ImVec2 left_top_coord(start_coord.x, start_coord.y + i * pixel_size);
        for (int j = 0; j < maze_witdh_; ++j) {
            int index = maze.CoordToPixelIndex(i, j);
            int val = encoded_maze[index];
            if (val & 1) {
                // has down wall
                tmp_coord1.x = left_top_coord.x;
                tmp_coord1.y = tmp_coord2.y = left_top_coord.y + pixel_size;
                tmp_coord2.x = left_top_coord.x + pixel_size;
                draw_list->AddLine(tmp_coord1, tmp_coord2, compress_wall_color, wall_thick);
            }
            if (val & 2) {
                // has right wall
                tmp_coord1.x = tmp_coord2.x = left_top_coord.x + pixel_size;
                tmp_coord1.y = left_top_coord.y;
                tmp_coord2.y = left_top_coord.y + pixel_size;
                draw_list->AddLine(tmp_coord1, tmp_coord2, compress_wall_color, wall_thick);
            }
            left_top_coord.x += pixel_size;
        }
    }

    // draw player
    ImColor player_color(249, 91, 91, 255);
    tmp_coord1.x = start_coord.x + (player_coord.second + 0.5) * pixel_size;
    tmp_coord1.y = start_coord.y + (player_coord.first + 0.5) * pixel_size;
    draw_list->AddCircleFilled(tmp_coord1, pixel_size * 0.45, ImU32(player_color));
}

void GameMachine::ProcessInput(SDL_Event &event) {
    switch (game_state) {
        case START:
            ProcessInputStartState(event);
            break;
        case PLAYING:
            ProcessInputPlayingState(event);
            break;
        case WIN:
            ProcessInputWinState(event);
            break;
    }
}

void GameMachine::ProcessInputStartState(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_RETURN) {
            StartGame();
        }
    }
}

void GameMachine::ProcessInputPlayingState(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_w:
            case SDLK_UP:
                TryMovePlayer(-1, 0);
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                TryMovePlayer(0, 1);
                break;
            case SDLK_s:
            case SDLK_DOWN:
                TryMovePlayer(1, 0);
                break;
            case SDLK_a:
            case SDLK_LEFT:
                TryMovePlayer(0, -1);
                break;
            default:
                break;
        }
    }
}

void GameMachine::ProcessInputWinState(SDL_Event &event) {

}

void GameMachine::StartGame() {
    maze.GenerateMaze(maze_witdh_, maze_height_, rand_seed_);
    encoded_maze = maze.EncodeMaze();
    player_coord.first = player_coord.second = 0;
    game_state = PLAYING;
}

void GameMachine::TryMovePlayer(int di, int dj) {
    int index = maze.CoordToPixelIndex(player_coord.first, player_coord.second);
    if (maze.GetMazeStruct().list_nodes[index].HasNeighbour(
            maze.CoordToPixelIndex(player_coord.first + di, player_coord.second + dj))) {
        player_coord.first += di;
        player_coord.second += dj;
    }
}