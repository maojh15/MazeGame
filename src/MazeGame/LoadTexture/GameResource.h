//
// Created by 冒家豪 on 2023/8/27.
//

#ifndef DEMO_IMGUI_GAMERESOURCE_H
#define DEMO_IMGUI_GAMERESOURCE_H

#include "LoadTexture.h"
#include "imgui.h"
#include "SDL_filesystem.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class GameResource {
public:
    GameResource() {
        auto base_path = SDL_GetBasePath();
        root_dir = std::string(base_path) + root_dir;

        LoadImageResource();
    }

    std::shared_ptr<LoadTextureTool> GetBackgroundTextureObj() {
        return background_texture;
    }

    std::shared_ptr<LoadTextureTool> GetWinTextureObj() {
        return win_texture;
    }

private:
    std::string root_dir = "MazeGameSolveAssets/resources/";

    const std::string img_background = "background.jpg";
    const std::string img_win = "Win.png";

    std::shared_ptr<LoadTextureTool> background_texture, win_texture;

    void LoadImageResource() {
//        background_texture = std::make_shared<LoadTextureTool>((root_dir + img_background).c_str());
        win_texture = std::make_shared<LoadTextureTool>((root_dir + img_win).c_str());
    }
};


#endif //DEMO_IMGUI_GAMERESOURCE_H
