/* Note: Parts of this code are based on the sample implementation of a Flow Free solver by Matt Zucker: https://github.com/mzucker/flow_solver/blob/master/flow_solver.c */


#include "extensions.h"
#include "options.h"

//////////////////////////////////////////////////////////////////////
// For sorting colors

int color_features_compare(const void* vptr_a, const void* vptr_b) {

	const color_features_t* a = (const color_features_t*)vptr_a;
	const color_features_t* b = (const color_features_t*)vptr_b;

	int u = cmp(a->user_index, b->user_index);
	if (u) { return u; }

	int w = cmp(a->wall_dist[0], b->wall_dist[0]);
	if (w) { return w; }

	int g = -cmp(a->wall_dist[1], b->wall_dist[1]);
	if (g) { return g; }

	return -cmp(a->min_dist, b->min_dist);

}

//////////////////////////////////////////////////////////////////////
// Place the game colors into a set order

void game_order_colors(game_info_t* info,
                       game_state_t* state) {

	if (g_options.order_random) {
    
		srand(now() * 1e6);
    
		for (size_t i=info->num_colors-1; i>0; --i) {
			size_t j = rand() % (i+1);
			int tmp = info->color_order[i];
			info->color_order[i] = info->color_order[j];
			info->color_order[j] = tmp;
		}

	} else { // not random

		color_features_t cf[MAX_COLORS];
		memset(cf, 0, sizeof(cf));

		for (size_t color=0; color<info->num_colors; ++color) {
			cf[color].index = color;
			cf[color].user_index = MAX_COLORS;
		}
    

		for (size_t color=0; color<info->num_colors; ++color) {
			
			int x[2], y[2];
			
			for (int i=0; i<2; ++i) {
				pos_get_coords(state->pos[color], x+i, y+i);
				cf[color].wall_dist[i] = get_wall_dist(info, x[i], y[i]);
			}

			int dx = abs(x[1]-x[0]);
			int dy = abs(y[1]-y[0]);
			
			cf[color].min_dist = dx + dy;
			
		

		}


		qsort(cf, info->num_colors, sizeof(color_features_t),
		      color_features_compare);

		for (size_t i=0; i<info->num_colors; ++i) {
			info->color_order[i] = cf[i].index;
		}
    
	}

	if (!g_options.display_quiet) {

		printf("\n************************************************"
		       "\n*               Branching Order                *\n");
		if (g_options.order_most_constrained) {
			printf("* Will choose color by most constrained\n");
		} else {
			printf("* Will choose colors in order: ");
			for (size_t i=0; i<info->num_colors; ++i) {
				int color = info->color_order[i];
				printf("%s", color_name_str(info, color));
			}
			printf("\n");
		}
		printf ("*************************************************\n\n");

	}

}
////////////////////////////////////////////////////////////////////// 
// Check if a specific cell is a deadend 

static int cell_is_deadend(const game_info_t* info, const game_state_t* 
state, pos_t pos);

static int cell_is_deadend(const game_info_t* info,
                    const game_state_t* state,
                    pos_t pos) {

  // Ensure that the position of the cell is valid and is not an initial state
  assert( !state->cells[pos]&& pos != INVALID_POS );

  // Get the coordinates of the cell
  int x, y;
  pos_get_coords(pos, &x, &y);
  
  int free_cells = 0;

  for (int dir=0; dir<4; ++dir) {

	// Obtain the coordinates of its neighbouring cells in all four directions
    pos_t neighbor_pos = offset_pos(info, x, y, dir);
    
	// Check if neighbouring cells are free cells
	// Verify that neighbouring cell is not in an invalid position
	if (neighbor_pos != INVALID_POS) {

      if (!state->cells[neighbor_pos]) {
        free_cells++;

      } else {

		// Check if cell is not an initial or goal state
        for (size_t color=0; color<info->num_colors; color++) {
          if ( (1 << color)& state->completed ) {
            continue;
          }

		  // Check if cell is painted with path color
          if (neighbor_pos == info->goal_pos[color] ||
              neighbor_pos == state->pos[color]) {
            free_cells++;
          }
        }
                                                                 
      }
    }
  }

  return (free_cells <= 1);

}

//////////////////////////////////////////////////////////////////////
// Check for dead-end regions of freespace where there is no way to
// put an active path into and out of it. Any freespace node which
// has only one free neighbor represents such a dead end. For the
// purposes of this check, cur and goal positions count as "free".

int game_check_deadends(const game_info_t* info,
                        const game_state_t* state) {


	/**
	 * FILL CODE TO DETECT DEAD-ENDS
	 */

	// Assign the last color of the path endpoint to current cell
	size_t color = state->last_color;

	// Stop checking if color is invalid
	if (color >= info->num_colors) {
		return 0;
	}

	// Get the position of current cell including coordinates
	pos_t cur_pos = state->pos[color];
	int x,y;
	pos_get_coords(cur_pos,&x,&y);
	
	for (int dir=0; dir<4; ++dir) {
		
		// Find position neighbouring cells of the current cell in each direction
		pos_t neighbor_pos = offset_pos(info,x,y,dir);

		// Check if the neighbouring cell is a deadend
		if (neighbor_pos != INVALID_POS && !state->cells[neighbor_pos] && 
        cell_is_deadend(info,state,neighbor_pos)) {
			return 1;
		} 

		// Find position of other cells surrounding each neighbouring cell in all directions
		int a,b;
		pos_get_coords(neighbor_pos,&a,&b);

		for (int dir=0; dir<4; ++dir) {
			pos_t neighbor2_pos = offset_pos(info,a,b,dir);

			// Check if any of those cells is a dead end 
			if (neighbor2_pos != INVALID_POS && !state->cells[neighbor2_pos] && 
            cell_is_deadend(info,state,neighbor2_pos)) {
				return 1;
			}
		}	

	}

	return 0;

}
