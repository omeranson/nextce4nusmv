#include "NextCE.h"

#include <stdlib.h>

/* The nextce internal structure. It holds the status as seen from the nextce
 * library, and the list of disjuncts that have been already generated
 */
struct nextce_t {
	node_ptr disjuncts;
	NextCE_Status status;
};

/**
 * Constructs a new nextce internal structure. It has to be freed with
 * #NextCE_destroy
 * @return a new nextce internal structure
 */
NextCE_ptr NextCE_create() {
	NextCE_ptr result = (NextCE_ptr)calloc(sizeof(struct nextce_t), 1);
	result->disjuncts = new_list();
	result->status = NextCE_Unknown;
	return result;
}

/**
 * Frees the given nextce internal structure, including the list of disjuncts.
 * @param self The structure to free.
 */
void NextCE_destroy(NextCE_ptr self) {
	free_list(self->disjuncts);
	self->disjuncts = NULL;
	free(self);
}

/**
 * Return the list of disjuncts that were added to this internal structure.
 * @param self The nextce internal structure
 * @return list of disjuncts.
 */
node_ptr NextCE_get_disjuncts(NextCE_ptr self) {
	return reverse_ns(self->disjuncts);
}

/**
 * Adds a disjunct to the given nextce interal datastructure
 * @param self The nextce internal structure
 * @param disjunct The disjunct (LTL expression) to add.
 */
void NextCE_add_disjunct(NextCE_ptr self, Expr_ptr disjunct) {
	self->disjuncts = cons(disjunct, self->disjuncts);
}

/**
 * Clear the list of disjuncts in the given nextce internal structure. e.g. After
 * this operation, #NextCE_get_disjuncts will return an empty list.
 * @param self The nextce internal structure
 */
void NextCE_clear_disjuncts(NextCE_ptr self) {
/* TODO Possibly free list members (but node_ptr memory management is incoherent) */
	free_list(self->disjuncts);
	self->disjuncts = new_list();
}

/**
 * Getter function for the status of the given nextce internal structure
 * @param self The nextce internal structure
 * @return The status of the given structure
 */
NextCE_Status NextCE_get_status(NextCE_ptr self) {
	return self->status;
}

/**
 * Setter function for the status of the given nextce internal structure
 * @param self The nextce internal structure
 * @param status The (new) status of the given structure
 */
void NextCE_set_status(NextCE_ptr self, NextCE_Status status) {
	self->status = status;
}

