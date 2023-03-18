#ifndef __SEARCH__
#define __SEARCH__


#include "node.h"
#include "engine.h"

//////////////////////////////////////////////////////////////////////
// Peforms Dijkstra  search

int game_dijkstra_search(const game_info_t* info, const game_state_t* init_state, 
                        double* elapsed_out, size_t* nodes_out, 
                        game_state_t* final_state);

// Adjust storage space for search nodes if deadends are found
tree_node_t* deadend_mem_adjust(const game_info_t* info, tree_node_t* node, 
                                node_memory_t* storage);

#endif
