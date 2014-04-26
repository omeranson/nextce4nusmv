#ifndef NEXTCE_H
#define NEXTCE_H

/**
 * This file contains data structures used by the next-ce plugin.
 */
#include "node/node.h"
#include "fsm/sexp/Expr.h"

typedef struct nextce_t * NextCE_ptr;
#define NEXTCE(o)	((NextCE_ptr)o)

/**
 * The status of the nextce structure. Unknown when the status is unknown - the
 * structure was just created. True/False in case the last contructed formula is
 * true or false, respectively. Reset is after the structure has been reset
 */
typedef enum {
	NextCE_Unknown,
	NextCE_Reset,
	NextCE_False,
	NextCE_True
} NextCE_Status;

NextCE_ptr NextCE_create();
void NextCE_destroy(NextCE_ptr self);

node_ptr NextCE_get_disjuncts(NextCE_ptr self);
void NextCE_add_disjunct(NextCE_ptr self, Expr_ptr disjunct);
void NextCE_clear_disjuncts(NextCE_ptr self);

NextCE_Status NextCE_get_status(NextCE_ptr self);
void NextCE_set_status(NextCE_ptr self, NextCE_Status status);

/**
 * Iterate the list of disjuncts in the structure in order. The disjuncts are
 * the parts of the formula that are added (in disjunction) to 'accept' the
 * current counter example, and generate a new one
 * @param self the nextce structure
 * @param list A node_ptr used to hold the rest of the disjuncts.
 * @param disjunct A node_ptr holding the current disjunct 
 */
#define ITERATE_DISJUNCTS(self, list, disjunct) \
	for (list = NextCE_get_disjuncts(self); !is_list_empty(list); disjunct = car(list), list = cdr(list))

#endif /* NEXTCE_H */

