
file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.hpp")

add_library(spa ${srcs} ${headers} "src/ExprTokensValidator.cpp" "src/ExprTokensValidator.h")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/pkb" "${CMAKE_CURRENT_SOURCE_DIR}/pql" "${CMAKE_CURRENT_SOURCE_DIR}/frontend")
# this makes the headers accessible for other projects which uses spa lib

target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/pkb ${CMAKE_CURRENT_SOURCE_DIR}/src/pql
        ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend)