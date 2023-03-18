#ifndef __EXTENSIONS__
#define __EXTENSIONS__


#include "utils.h"
#include "node.h"

//////////////////////////////////////////////////////////////////////
// Place the game colors into a set order
void game_order_colors(game_info_t* info, game_state_t* state);

/////////////////////////////////////////////////////////////////////
// For sorting colors
int color_features_compare(const void* vptr_a, const void* vptr_b);

//////////////////////////////////////////////////////////////////////
// Check for dead-end regions of freespace where there is no way to
// put an active path into and out of it. Any freespace node which
// has only one free neighbor represents such a dead end. For the
// purposes of this check, cur and goal positions count as "free".

int game_check_deadends(const game_info_t* info, const game_state_t* state);



#endif
