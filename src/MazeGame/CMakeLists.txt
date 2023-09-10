function(INITIALIZE_IMGUI)
    set(IMGUI_DIR "/Users/maojiahao/cppLibraries/package/imgui-master")

    find_library(SDL2Fwk SDL2 REQUIRED)
    message(STATUS "found SDL2Fwk=${SDL2Fwk}")
    set(SDL2_INCLUDE_DIR "${SDL2Fwk}/Headers")
    message(${SDL2_INCLUDE_DIR})

    add_library(imgui_lib
            "${IMGUI_DIR}/imgui.cpp"
            "${IMGUI_DIR}/imgui_demo.cpp"
            "${IMGUI_DIR}/imgui_draw.cpp"
            "${IMGUI_DIR}/imgui_tables.cpp"
            "${IMGUI_DIR}/imgui_widgets.cpp"
            "${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp"
            "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp")
    target_include_directories(imgui_lib PUBLIC
            ${SDL2_INCLUDE_DIR}
            "${IMGUI_DIR}"
            "${IMGUI_DIR}/backends")
    target_link_directories(imgui_lib PUBLIC ${BREW_LIBRARIES_DIR})
    target_link_libraries(imgui_lib PUBLIC "${SDL2Fwk}" "-framework OpenGL")
endfunction()

INITIALIZE_IMGUI()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

if (MACOSX)
    add_executable(maze_game MACOSX_BUNDLE maze_game.cpp)
else()
    add_executable(maze_game maze_game.cpp)
endif()

add_subdirectory(LoadTexture)
add_library(GameMachine GameMachine.cpp GameMachine.h)
target_link_libraries(GameMachine PUBLIC imgui_lib MazeGenerator LoadTexture MazeSolverAStar)

target_link_libraries(maze_game PUBLIC GameMachine)

# add_custom_command(TARGET maze_game POST_BUILD        # Adds a post-build event to maze_game
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
#         "${CMAKE_SOURCE_DIR}/src"      # <--this is in-file
#         $<TARGET_FILE_DIR:maze_game>)                 # <--this is out-file path

install(TARGETS maze_game
        DESTINATION "${CMAKE_SOURCE_DIR}/bin")

if (MAXCOS)
    set(CUR_TARGET maze_game)
    set_target_properties(${CUR_TARGET} PROPERTIES
        BUNDLE True
        MACOSX_BUNDLE_GUI_IDENTIFIER my.domain.style.identifier.${CUR_TARGET}
        MACOSX_BUNDLE_BUNDLE_NAME ${CUR_TARGET}
        MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/customtemplate.plist.in
    )
endif()