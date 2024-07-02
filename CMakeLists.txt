cmake_minimum_required(VERSION 3.26)
project(argument_parser)  # replace your third lib name

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE "Release")
# set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_SOURCE_FILE_DIRECTORY ${PROJECT_SOURCE_DIR}/src)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SOURCE_FILE ${CMAKE_SOURCE_FILE_DIRECTORY}/*.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

message(STATUS "try to find trie_tree")
find_package(trie_tree REQUIRED MODULE)
if (trie_tree_FOUND)
    message(STATUS "trie_tree found")
    include_directories(${trie_tree_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PUBLIC
        ${trie_tree_LIBRARIES}
    )
endif (trie_tree_FOUND)