#########################################
#            chess_engine               #
#---------------------------------------#
# @author Nikolaus Rauch                #
# @date 16.04.2022                      #
#########################################
project( chess_lib )

#########################################
#     Find/Build External-Libraries     #
#########################################
find_package( Threads REQUIRED )


#################################
#      Source Files Engine      #
#################################
set( CHESS_LIB_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/chess_string.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/chess_format.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/board.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/move/attack.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/move/movegen.cpp"
)

set( CHESS_LIB_HDR
    "${CMAKE_CURRENT_SOURCE_DIR}/chess.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/chess_string.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/chess_format.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/board.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/bitboard.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/move.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/move/attack.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/move/magic.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/move/movegen.h"
)

source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${CHESS_LIB_SRC} ${CHESS_LIB_HDR} )



#################################
#     Build Engine Library      #
#################################
add_library( chess_lib SHARED ${CHESS_LIB_SRC} ${CHESS_LIB_HDR} )

target_link_libraries( chess_lib Threads::Threads )

target_include_directories( chess_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    )

target_compile_features( chess_lib PUBLIC cxx_std_20 )
set_target_properties( chess_lib PROPERTIES CXX_EXTENSIONS OFF )
target_compile_options( chess_lib PUBLIC
    $<$<CXX_COMPILER_ID:GNU>:-fconstexpr-ops-limit=100000000>
    $<$<CXX_COMPILER_ID:MSVC>:>)
