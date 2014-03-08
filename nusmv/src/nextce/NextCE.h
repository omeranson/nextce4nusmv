#ifndef NEXTCE_H
#define NEXTCE_H

#include "node/node.h"
#include "fsm/sexp/Expr.h"

typedef struct nextce_t * NextCE_ptr;
#define NEXTCE(o)	((NextCE_ptr)o)

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

#define ITERATE_DISJUNCTS(self, list, disjunct) \
	for (list = NextCE_get_disjuncts(self); !is_list_empty(list); disjunct = car(list), list = cdr(list))
#endif /* NEXTCE_H */

