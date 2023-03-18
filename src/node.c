/* Note: Parts of this code are based on the sample implementation of a Flow Free solver by Matt Zucker: https://github.com/mzucker/flow_solver/blob/master/flow_solver.c */


#include "node.h"
#include "engine.h"
#include "extensions.h"

//////////////////////////////////////////////////////////////////////
// Compare total cost for nodes, used by heap functions below.

int node_compare(const tree_node_t* a,
                 const tree_node_t* b) {

	double af = a->cost_to_node;
	double bf = b->cost_to_node;

	if (af != bf) {
		return af < bf ? -1 : 1;
	} else {
		return a < b ? -1 : 1;
	}

}

//////////////////////////////////////////////////////////////////////
// Create a node and set the cost to the node

tree_node_t* node_create(node_memory_t* storage, tree_node_t* parent,
                         const game_state_t* state) {
  
	// Allocate memory space for the new node
	tree_node_t* new_n = node_mem_alloc(storage);
	
	if (!new_n) { return 0; }

	new_n->parent = parent;
	new_n->cost_to_node = 0;

	// update cost to node, unless is the root
	const size_t action_cost = 1;
	if (parent)
		new_n->cost_to_node = parent->cost_to_node + action_cost;
	
	memcpy(&(new_n->state), state, sizeof(game_state_t));
  
	return new_n;

}

//////////////////////////////////////////////////////////////////////
// Allocate sufficient memory space for a new node

tree_node_t* node_mem_alloc(node_memory_t* storage) {

	// 	Ensure that there is enough memory to allocate for the new node
  	if (storage->count >= storage->capacity) {
    	return NULL;
  	}

	// Allocate memory space to new_node 
	tree_node_t* new_n = storage->start + storage->count;
	++storage->count;

	return new_n;
}

//////////////////////////////////////////////////////////////////////
// Linearly allocate memory spcace for search nodes

node_memory_t create_node_mem(size_t max_nodes) {

    node_memory_t storage;

    // Allocate memory space to the first node
    storage.start = malloc(max_nodes*sizeof(tree_node_t));

    // If there is no memory available to allocate
    if (!storage.start) {
    fprintf(stderr, "unable to allocate memory for node storage!\n");
    exit(1);
    }

    //Define maximum number of nodes that can be allocated memory
    storage.capacity = max_nodes;
    //Initialise number of nodes already used
    storage.count = 0;

    return storage;
    
}

//////////////////////////////////////////////////////////////////////
// Perform diagnostics on the given node

void node_diagnostics(const game_info_t* info,
		      const tree_node_t* node) {

	printf("\n###################################"
	       "###################################\n\n");

	printf("node has cost to node %'g and cost to go %'d\n",
	       node->cost_to_node, node->state.num_free);

	if (node->state.last_color < info->num_colors) {
		printf("last move was for color %s\n",
		       color_name_str(info, node->state.last_color));

	} else {
		printf("no moves yet?\n");
	}

}

//////////////////////////////////////////////////////////////////////
// Animate the solution by printing out boards in reverse order,
// following parent pointers back from solution to root.

void animate_solution(const game_info_t* info,
                           const tree_node_t* node) {

	if (node->parent) {
		animate_solution(info, node->parent);
	}

	printf("%s", unprint_board(info));
	game_print(info, &node->state);
	fflush(stdout);

	delay_seconds(0.1);
  
}
