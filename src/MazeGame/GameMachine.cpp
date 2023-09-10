//
// Created by 冒家豪 on 2023/9/9.
//

#include "GameMachine.h"
#include "MazeSolverAStar.h"

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
    ImGui::SliderInt("width", &maze_width_, 2, 200, "width: %d");
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

    ImGui::SetCursorPosX((avail.x - button_witdh) / 2);
    if (ImGui::Button("EXIT", ImVec2(button_witdh, 50))) {
        game_state = END;
    }
}

void GameMachine::RenderPlayingState() {
    RenderFunctionalButton();

    if (next_click_set_start_point_) {
        ImGui::Text("Click a point\nas Start point.\nRight click to cancel");
    }

    if (next_click_set_end_point_) {
        ImGui::Text("Click a point\nas End point.\nRight click to cancel");
    }

    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    DrawMaze(draw_list);

    if (have_found_path_) {
        RenderShortestPathFound();
    }

    if (player_coord_.first == maze_height_ - 1 && player_coord_.second == maze_width_ - 1 && game_state == PLAYING) {
        game_state = WIN;
    }
}

void GameMachine::RenderWinState() {
    RenderPlayingState();

    if (!has_win_waited_) {
        has_win_waited_ = true;
        SDL_Delay(500);
    }
    if (show_win_image_) {
        auto win_texture = game_resource_.GetWinTextureObj();
        auto io = ImGui::GetIO();
        int txt_width = 0.5 * io.DisplaySize.x;
        int txt_height = 0.5 * io.DisplaySize.y;
        ImGui::SetCursorPos(
                ImVec2((io.DisplaySize.x - txt_width) / 2, (io.DisplaySize.y - txt_height) / 2));
        ImGui::Image((void *) (intptr_t) (win_texture->textureID), ImVec2(txt_width, txt_height));
    }
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
    float scale_x = 0.85;
    float scale_y = 0.95;
    maze_pixel_size_ = std::min(scale_x * avail.x / maze_width_, scale_y * avail.y / maze_height_);

    draw_start_coord_.x = (avail.x - maze_width_ * maze_pixel_size_) / 2;
    draw_start_coord_.y = (avail.y - maze_height_ * maze_pixel_size_) / 2;
    ImColor wall_color(0, 0, 0, 255);
    ImU32 compress_wall_color = ImU32(wall_color);
    float boundary_wall_thick = 1.5f;

    // draw out rectangle
    ImVec2 tmp_coord1(draw_start_coord_.x + maze_pixel_size_, draw_start_coord_.y);
    ImVec2 tmp_coord2(draw_start_coord_.x + maze_pixel_size_ * maze_width_, draw_start_coord_.y);
    draw_list->AddLine(tmp_coord1, tmp_coord2, compress_wall_color, boundary_wall_thick);
    tmp_coord1.x = tmp_coord2.x, tmp_coord1.y = tmp_coord2.y + maze_pixel_size_ * (maze_height_ - 1);
    draw_list->AddLine(tmp_coord1, tmp_coord2, compress_wall_color, boundary_wall_thick);
    tmp_coord1.x = draw_start_coord_.x, tmp_coord1.y = draw_start_coord_.y + maze_pixel_size_;
    tmp_coord2.x = draw_start_coord_.x, tmp_coord2.y = draw_start_coord_.y + maze_pixel_size_ * maze_height_;
    draw_list->AddLine(tmp_coord1, tmp_coord2, compress_wall_color, boundary_wall_thick);
    tmp_coord1.x = draw_start_coord_.x + maze_pixel_size_ * (maze_width_ - 1);
    tmp_coord1.y = tmp_coord2.y;
    draw_list->AddLine(tmp_coord1, tmp_coord2, compress_wall_color, boundary_wall_thick);

    if (draw_visited_pixel_) {
        // draw visited pixel;
        ImU32 visited_pixel_col = ImU32(ImColor(186, 255, 21, 125));
        for (int pixel_index: visited_index_in_A_star_search_) {
            std::pair<int, int> coord = maze.PixelIndexToCoord(pixel_index);
            tmp_coord1.x = draw_start_coord_.x + (coord.second) * maze_pixel_size_;
            tmp_coord1.y = draw_start_coord_.y + (coord.first) * maze_pixel_size_;
            tmp_coord2.x = draw_start_coord_.x + (coord.second + 1) * maze_pixel_size_;
            tmp_coord2.y = draw_start_coord_.y + (coord.first + 1) * maze_pixel_size_;
            draw_list->AddRectFilled(tmp_coord1, tmp_coord2, visited_pixel_col);
        }
    }

    // draw maze
    float wall_thick = 1.0f;
    for (int i = 0; i < maze_height_; ++i) {
        ImVec2 left_top_coord(draw_start_coord_.x, draw_start_coord_.y + i * maze_pixel_size_);
        for (int j = 0; j < maze_width_; ++j) {
            int index = maze.CoordToPixelIndex(i, j);
            int val = encoded_maze[index];
            if (val & 1) {
                // has down wall
                tmp_coord1.x = left_top_coord.x;
                tmp_coord1.y = tmp_coord2.y = left_top_coord.y + maze_pixel_size_;
                tmp_coord2.x = left_top_coord.x + maze_pixel_size_;
                draw_list->AddLine(tmp_coord1, tmp_coord2, compress_wall_color, wall_thick);
            }
            if (val & 2) {
                // has right wall
                tmp_coord1.x = tmp_coord2.x = left_top_coord.x + maze_pixel_size_;
                tmp_coord1.y = left_top_coord.y;
                tmp_coord2.y = left_top_coord.y + maze_pixel_size_;
                draw_list->AddLine(tmp_coord1, tmp_coord2, compress_wall_color, wall_thick);
            }
            left_top_coord.x += maze_pixel_size_;
        }
    }

    // draw end point
//    tmp_coord1.x = draw_start_coord_.x + (maze_width_ + 0.5) * maze_pixel_size_;
    ImColor tmp_color(91, 240, 91, 255);
    tmp_coord1.x = draw_start_coord_.x + (end_coord_.second + 0.15) * maze_pixel_size_;
    tmp_coord1.y = draw_start_coord_.y + (end_coord_.first + 0.15) * maze_pixel_size_;
    tmp_coord2.x = draw_start_coord_.x + (end_coord_.second + 0.85) * maze_pixel_size_;
    tmp_coord2.y = draw_start_coord_.y + (end_coord_.first + 0.85) * maze_pixel_size_;
    draw_list->AddRectFilled(tmp_coord1, tmp_coord2, ImU32(tmp_color), 1.0f);

    // draw player
    tmp_color = ImColor(249, 91, 91, 255);
    tmp_coord1.x = draw_start_coord_.x + (player_coord_.second + 0.5) * maze_pixel_size_;
    tmp_coord1.y = draw_start_coord_.y + (player_coord_.first + 0.5) * maze_pixel_size_;
    draw_list->AddCircleFilled(tmp_coord1, maze_pixel_size_ * 0.45, ImU32(tmp_color));
}

/**
 *
 * @param event
 * @return true: if event can be handled by other functions; false: if event should be eaten.
 */
bool GameMachine::ProcessInput(SDL_Event &event) {
    switch (game_state) {
        case START:
            return ProcessInputStartState(event);
            break;
        case PLAYING:
            return ProcessInputPlayingState(event);
            break;
        case WIN:
            return ProcessInputWinState(event);
            break;
    }
    return true;
}

bool GameMachine::ProcessInputStartState(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_RETURN) {
            StartGame();
        }
    }
    return true;
}

bool GameMachine::ProcessInputPlayingState(SDL_Event &event) {
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

    if (next_click_set_start_point_ && event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == 1) {
            SetStartPoint(event.button.x, event.button.y);
            next_click_set_start_point_ = false;
            return false;
        } else if (event.button.button == 3) {
            next_click_set_start_point_ = false;
            return false;
        }

    }

    if (next_click_set_end_point_ && event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == 1) {
            SetEndPoint(event.button.x, event.button.y);
            next_click_set_end_point_ = false;
            return false;
        } else if (event.button.button == 3) {
            next_click_set_end_point_ = false;
            return false;
        }
        return false;
    }
    return true;
}

bool GameMachine::ProcessInputWinState(SDL_Event &event) {
    return true;
}

void GameMachine::StartGame() {
    maze.GenerateMaze(maze_width_, maze_height_, rand_seed_);
    encoded_maze = maze.EncodeMaze();
    player_coord_.first = player_coord_.second = 0;
    start_coord_ = player_coord_;
    end_coord_.first = maze_height_ - 1;
    end_coord_.second = maze_width_ - 1;
    has_win_waited_ = false;
    show_win_image_ = true;
    ResetDrawFlags();
    game_state = PLAYING;
}

void GameMachine::TryMovePlayer(int di, int dj) {
    int index = maze.CoordToPixelIndex(player_coord_.first, player_coord_.second);
    if (maze.GetMazeStruct().list_nodes[index].HasNeighbour(
            maze.CoordToPixelIndex(player_coord_.first + di, player_coord_.second + dj))) {
        player_coord_.first += di;
        player_coord_.second += dj;
    }
}

void GameMachine::RenderFunctionalButton() {
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(8 / 9.0f, 0.6f, 0.95f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(8 / 9.0f, 0.7f, 0.9f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(8 / 9.0f, 0.8f, 0.9f, 0.7f));

    auto io = ImGui::GetIO();
    int button_width = 200;
    int button_height = 40;

//    ImGui::SetCursorPosY(io.DisplaySize.y - 3 * button_height);

    if (ImGui::Button("Find Shortest Path", ImVec2(button_width, button_height))) {
        if (!have_found_path_) {
            FindShortestPath();
            have_found_path_ = true;
        }
    }

    button_width = 135;
    if (ImGui::Button(" Return Title ", ImVec2(button_width, button_height))) {
        game_state = START;
    }

    if (game_state == WIN && has_win_waited_) {
        // Add toggle win button.
//        ImGui::SetCursorPos(ImVec2(io.DisplaySize.x - 5 - button_width,
//                                   io.DisplaySize.y - 10 - button_height));
        std::string hide_win = " Hide 'win' ";
        std::string show_win = " Show 'win' ";
        if (ImGui::Button((show_win_image_ ? hide_win : show_win).c_str(), ImVec2(button_width, button_height))) {
            show_win_image_ = !show_win_image_;
        }
    }

    if (have_found_path_) {
        const std::string hint_show_shortest_path = "show path";
        const std::string hint_hide_shortest_path = "hide path";
        ImGui::SetCursorPosX(io.DisplaySize.x - 5 - button_width);
        ImGui::SetCursorPosY(10);
        if (ImGui::Button((show_shortest_path_ ? hint_hide_shortest_path : hint_show_shortest_path).c_str(),
                          ImVec2(button_width, button_height))) {
            show_shortest_path_ = !show_shortest_path_;
        }

        const std::string hint_show_visited_path = "show visited pos";
        const std::string hint_hide_visited_path = "hide visited pos";
        button_width = 180;
        ImGui::SetCursorPosX(io.DisplaySize.x - 5 - button_width);
        if (ImGui::Button((draw_visited_pixel_ ? hint_hide_visited_path : hint_show_visited_path).c_str(),
                          ImVec2(button_width, button_height))) {
            draw_visited_pixel_ = !draw_visited_pixel_;
        }
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.95, 0.95, 0.1, 0.95));
        ImGui::SetItemTooltip("Show/Hide visited position\nin A*-algorithm");
        ImGui::PopStyleColor();
    }

    ImGui::PopStyleColor(3);

    button_width = 180;
    ImGui::SetCursorPosY(4 * button_height);
    if (ImGui::Button("Set Start Point", ImVec2(button_width, button_height))) {
        next_click_set_start_point_ = true;
    }
    if (ImGui::Button("Set End Point", ImVec2(button_width, button_height))) {
        next_click_set_end_point_ = true;
    }
}

void GameMachine::FindShortestPath() {
    MazeSolverAStar maze_solver;
    shortest_path = maze_solver.FindShortestPath(maze,
                                                 maze.CoordToPixelIndex(start_coord_.first, start_coord_.second),
                                                 maze.CoordToPixelIndex(end_coord_.first, end_coord_.second));
    visited_index_in_A_star_search_ = std::move(maze_solver.visited_pixel_index);
//    for (auto &x: shortest_path) {
//        std::cout << "[" << x.first << "," << x.second << "]->";
//    }
//    std::cout << std::endl;
}

void GameMachine::RenderShortestPathFound() {
    if (show_shortest_path_) {
        ImColor draw_path_color(255, 0, 0, 255);
        ImU32 compressed_path_color = ImU32(draw_path_color);
        float draw_path_thickness = std::max(maze_pixel_size_ * 0.2, 1.0);
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        ImVec2 to_point(draw_start_coord_.x + (shortest_path[0].second + 0.5) * maze_pixel_size_,
                        draw_start_coord_.y + (shortest_path[0].first + 0.5) * maze_pixel_size_);
        ImVec2 from_point;
        for (int i = 1; i < shortest_path.size(); ++i) {
            from_point = std::move(to_point);
            const auto &elem = shortest_path[i];
            to_point.x = draw_start_coord_.x + (elem.second + 0.5) * maze_pixel_size_;
            to_point.y = draw_start_coord_.y + (elem.first + 0.5) * maze_pixel_size_;
            drawList->AddLine(from_point, to_point, compressed_path_color, draw_path_thickness);
        }
    }
}

std::pair<int, int> GameMachine::MousePositionToMazeCoord(int mouse_x, int mouse_y) {
    float coord_x = mouse_x, coord_y = mouse_y;
    if (coord_x < draw_start_coord_.x) {
        coord_x = draw_start_coord_.x;
    } else if (coord_x > draw_start_coord_.x + (maze_width_ - 1) * maze_pixel_size_) {
        coord_x = draw_start_coord_.x + (maze_width_ - 1) * maze_pixel_size_;
    }
    if (coord_y < draw_start_coord_.y) {
        coord_y = draw_start_coord_.y;
    } else if (coord_y > draw_start_coord_.y + (maze_height_ - 1) * maze_pixel_size_) {
        coord_y = draw_start_coord_.y + (maze_height_ - 1) * maze_pixel_size_;
    }
    int pos_i = (coord_y - draw_start_coord_.y) / maze_pixel_size_;
    int pos_j = (coord_x - draw_start_coord_.x) / maze_pixel_size_;
    return std::make_pair(pos_i, pos_j);
}

void GameMachine::SetStartPoint(int mouse_x, int mouse_y) {
    player_coord_ = MousePositionToMazeCoord(mouse_x, mouse_y);
    start_coord_ = player_coord_;
    ResetDrawFlags();
}

void GameMachine::SetEndPoint(int mouse_x, int mouse_y) {
    end_coord_ = MousePositionToMazeCoord(mouse_x, mouse_y);
    ResetDrawFlags();
}

void GameMachine::ResetDrawFlags() {
    have_found_path_ = false;
    show_shortest_path_ = true;
    draw_visited_pixel_ = false;
}