set(trie_tree_FOUND TRUE)
set(trie_tree_ROOT_DIR ${CMAKE_SOURCE_DIR}/third/trie_tree)

find_path(trie_tree_INCLUDE_DIR NAMES trie_tree.hpp PATHS ${trie_tree_ROOT_DIR}/include)
find_library(trie_tree_LIBRARY NAMES libtrie_tree.a PATHS ${trie_tree_ROOT_DIR}/lib)

set(trie_tree_INCLUDE_DIR ${trie_tree_INCLUDE_DIR})
set(trie_tree_LIBRARIES ${trie_tree_LIBRARY})