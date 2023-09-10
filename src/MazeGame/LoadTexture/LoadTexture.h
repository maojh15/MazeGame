#ifndef __LOADTEXTURETOOL_H_
#define __LOADTEXTURETOOL_H_


#include "stb_image.h"

#include "SDL_opengl.h"

#include <iostream>

class LoadTextureTool {
public:
    LoadTextureTool(const char *filename) {
        LoadTextureFromFile(filename, &textureID, &width, &height);
        std::cout << filename << ", image size: " << width << "x" << height << std::endl;
    }

    GLuint textureID;
    int width, height;

private:
    // Simple helper function to load an image into a OpenGL texture with common settings
    bool LoadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height);
};

#endif