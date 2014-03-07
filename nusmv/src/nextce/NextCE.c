#include "NextCE.h"

#include <stdlib.h>

struct nextce_t {
	node_ptr disjuncts;
	NextCE_Status status;
};

NextCE_ptr NextCE_create() {
	NextCE_ptr result = (NextCE_ptr)calloc(sizeof(struct nextce_t), 1);
	result->disjuncts = new_list();
	result->status = NextCE_Unknown;
	return result;
}

void NextCE_destroy(NextCE_ptr self) {
	free_list(self->disjuncts);
	self->disjuncts = NULL;
	free(self);
}

node_ptr NextCE_get_disjuncts(NextCE_ptr self) {
	return reverse_ns(self->disjuncts);
}

void NextCE_add_disjunct(NextCE_ptr self, Expr_ptr disjunct) {
	self->disjuncts = cons(disjunct, self->disjuncts);
}

void NextCE_clear_disjuncts(NextCE_ptr self, Expr_ptr disjunct) {
/* TODO Possibly free list members (but I don't undestand node memory management):
	node_ptr list;
	Expr_ptr disjunct;
	ITERATE_DISJUNCTS(self, list, disjunct) {
		
	}
*/
	free_list(self->disjuncts);
	self->disjuncts = new_list();
}

NextCE_Status NextCE_get_status(NextCE_ptr self) {
	return self->status;
}

void NextCE_set_status(NextCE_ptr self, NextCE_Status status) {
	self->status = status;
}

