#########################################
#          Chess Game Interface         #
#---------------------------------------#
# @author Nikolaus Rauch                #
# @date 16.04.2022                      #
#---------------------------------------#
#########################################
project( chess_gui )

#########################################
#                Options                #
#########################################
option(SFML_BUILD_SOURCE  "build sfml from source" ON)

#########################################
#     Find/Build External-Libraries     #
#########################################
find_package( Threads REQUIRED )

set( OpenGL_GL_PREFERENCE GLVND )
find_package( OpenGL 3.0 REQUIRED )

if(SFML_BUILD_SOURCE)
    add_subdirectory(external/SFML-2.6.0)
    if(WIN32)
        file(COPY external/SFML-2.6.0/extlibs/bin/x64/openal32.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endif()
else()
    find_package( SFML 2.6 COMPONENTS system window graphics REQUIRED )
endif()

add_subdirectory(external/glad)
add_subdirectory(external/imgui)

#################################
#       Source Files Game       #
#################################
set( GAME_SRC 
    "${CMAKE_CURRENT_SOURCE_DIR}/chess_gui.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/chess_app.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/render_module.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/game_module.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/sound_module.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/gui_module.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/states/human_turn.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/states/human_move.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/states/ai_turn.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/states/ai_move.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/states/new_game.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/states/game_over.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/states/promotion.cpp"


    "${CMAKE_CURRENT_SOURCE_DIR}/uci/process.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/uci/windows_process.cpp"
)

set( GAME_HDR
    "${CMAKE_CURRENT_SOURCE_DIR}/chess_app.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/render_module.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/game_module.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/sound_module.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/gui_module.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/state_machine.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/states/state.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/states/human_turn.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/states/human_move.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/states/ai_turn.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/states/ai_move.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/states/new_game.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/states/game_over.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/states/promotion.h"


    "${CMAKE_CURRENT_SOURCE_DIR}/uci/process.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/uci/windows_process.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/uci/safe_queue.h"
)

source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${GAME_SRC} ${GAME_HDR} )

#################################
#     Build Engine Library      #
#################################
add_executable( chess_gui ${GAME_SRC} ${GAME_HDR} )

target_link_libraries( chess_gui
    chess_engine
    Threads::Threads OpenGL::GL
    sfml-system sfml-window sfml-graphics sfml-audio
    glad imgui
)


target_include_directories( chess_gui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    )

target_compile_features( chess_gui PUBLIC cxx_std_20 )
set_target_properties( chess_gui PROPERTIES CXX_EXTENSIONS OFF )



add_custom_target( chess_gui_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:chess_gui>/assets )
