
# https://google.github.io/googletest/quickstart-cmake.html

#################################
#        Get google tests       #
#################################
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/8aa657ee397a3273f9d2bb14a10b39fd1d556a77.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

#################################
#            Build Tests        #
#################################
add_executable( tests
    test_board.cpp
    test_format.cpp
    test_attack.cpp
    test_movegen.cpp
)

target_link_libraries( tests PRIVATE chess_lib gtest_main gmock_main)
target_compile_features( tests PUBLIC cxx_std_20 )
set_target_properties( tests PROPERTIES CXX_EXTENSIONS OFF )

include(GoogleTest)
gtest_discover_tests(tests)
