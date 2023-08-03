set(IMGUI_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui_stdlib.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui_tables.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp"
	)
	
set(IMGUI_HDR
	"${CMAKE_CURRENT_SOURCE_DIR}/imconfig.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui_internal.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui_stdlib.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/imstb_rectpack.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/imstb_textedit.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/imstb_truetype.h"
	)
	
source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${IMGUI_SRC} ${IMGUI_HDR} )

add_library( imgui STATIC ${IMGUI_SRC} ${IMGUI_HDR} )
target_include_directories( imgui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    )
    
target_compile_features( imgui PUBLIC cxx_std_20 )
set_target_properties( imgui PROPERTIES CXX_EXTENSIONS OFF )
