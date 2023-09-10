file(GLOB LoadTextureSources *.h *.cpp)
add_library(LoadTexture ${LoadTextureSources})
target_include_directories(LoadTexture INTERFACE ./)

target_link_libraries(LoadTexture PRIVATE imgui_lib)