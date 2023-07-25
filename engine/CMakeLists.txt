#########################################
#            chess_engine               #
#---------------------------------------#
# @author Nikolaus Rauch                #
# @date 16.04.2022                      #
#########################################
project( chess_engine )

#########################################
#     Find/Build External-Libraries     #
#########################################
find_package( Threads REQUIRED )


#################################
#      Source Files Engine      #
#################################
set( CHESS_ENGINE_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/iface/iface_string.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/iface/uci.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/iface/cli.cpp"
)

set( CHESS_ENGINE_HDR
    "${CMAKE_CURRENT_SOURCE_DIR}/eval/eval_table.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/eval/base.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/eval/table_michniewski.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/eval/table_codemonkeyking.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/search/negamax.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/iface/iface_string.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/iface/uci.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/iface/cli.h"
)

source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${CHESS_ENGINE_SRC} ${CHESS_ENGINE_HDR} )


set( CHESS_ENGINE_EXE_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/checkily.cpp"
)

set( CHESS_ENGINE_EXE_HDR
    ""
)

source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${CHESS_ENGINE_EXE_SRC} ${CHESS_ENGINE_EXE_HDR} )

#################################
#     Build Engine Library      #
#################################
add_library( chess_engine SHARED ${CHESS_ENGINE_SRC} ${CHESS_ENGINE_HDR} )
target_link_libraries( chess_engine Threads::Threads chess_lib )

#################################
#     Build Engine Executable   #
#################################
add_executable( checkily ${CHESS_ENGINE_EXE_SRC} ${CHESS_ENGINE_EXE_HDR} )
target_link_libraries( checkily Threads::Threads chess_engine )

target_include_directories( chess_engine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    )

target_compile_features( chess_engine PUBLIC cxx_std_20 )
set_target_properties( chess_engine PROPERTIES CXX_EXTENSIONS OFF )
