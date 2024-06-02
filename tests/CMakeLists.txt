
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  parfume_bazaar_tests
  "parfume_bazaar_tests.cpp" "makler_tests.cpp" "admin_tests.cpp" "date_tests.cpp" "item_tests.cpp" "bargain_tests.cpp")





target_link_libraries(
  parfume_bazaar_tests
  PRIVATE 
   parfume_bazaar_lib
   GTest::gtest_main
   -fprofile-arcs -ftest-coverage
)


set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})


add_custom_command(
    TARGET parfume_bazaar_tests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SOURCE_DIR}/test.db
        ${BINARY_DIR}/test.db
)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

set(CMAKE_BUILD_TYPE Debug)

target_compile_options(parfume_bazaar_tests PRIVATE -fprofile-arcs -ftest-coverage -O0 -g)


include(GoogleTest)
gtest_discover_tests( parfume_bazaar_tests)
