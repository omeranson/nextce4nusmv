#include <stdio.h>

#include "NextCECmd.h"
#include "NextCEDbg.h"
#include "prop/Prop.h"
#include "prop/PropDb.h"
#include "prop/propPkg.h"
#include "fsm/sexp/Expr.h"
#include "trace/Trace.h"
#include "trace/TraceManager.h"
#include "trace/pkg_trace.h"
#include "parser/symbols.h"
#include "node/node.h"

extern FILE * nusmv_stderr;
typedef struct {
	int prop_num;
} options_t;

void debug_show_options(const char * fname, const options_t *options) {
	nextce_debug(5, "%s: Showing options:", fname);
	nextce_debug(5, "%s: \tprop_num: %d", fname, options->prop_num);
}

static Trace_ptr get_ce(Prop_ptr prop) {
	static const char * fname = __func__;
	int index = Prop_get_trace(prop);
	nextce_debug(5, "%s: Enter. Index: %d", fname, index);
	TraceManager_ptr traceManager = TracePkg_get_global_trace_manager();
	Trace_ptr trace = TraceManager_get_trace_at_index(traceManager, index-1);
	nextce_debug(5, "%s: Exit", fname);
	return trace;
}

static void fipath_trunc(Trace_ptr trace) {
	/* TODO Stub */
	// NOP;
}

static void fipath_progress(Trace_ptr trace) {
	/* TODO Stub */
	/* For each step in trace,
		if next step is same step, then
			remove step */
}

static void fipath_reduce_config_loops(Trace_ptr trace) {
	/* TODO Stub */
}

static void fipath_reduce_init_config(Trace_ptr trace) {
	/* TODO Stub */
}

static void fipath_reduce_vals(Trace_ptr trace) {
	/* TODO Stub */
}

/* Create a FIPATH from a counter example, as defined in the paper. */
static Trace_ptr create_fipath(Trace_ptr trace) {
	static const char * fname = __func__;
	Trace_ptr result = Trace_copy(trace, TRACE_END_ITER, FALSE);
	fipath_trunc(result);
	fipath_progress(result);
	fipath_reduce_config_loops(result);
	fipath_reduce_init_config(result);
	fipath_reduce_vals(result);
	return result;
}

static void debug_print_expr(Expr_ptr expr) {
	Prop_ptr prop;
	if (!is_nextce_debug(5)) {
		return;
	}
	prop = Prop_create_partial(expr, Prop_Ltl);
	Prop_print(prop, stdout, PROP_PRINT_FMT_FORMULA);
	fprintf(stdout, "\n");
	Prop_destroy(prop);
}

/* Create \phi U \psi */
static Expr_ptr nextce_expr_until(Expr_ptr phi, Expr_ptr psi) {
	Expr_ptr result = new_node(UNTIL, phi, psi);
	return result;
}

static Expr_ptr get_inv(Prop_ptr prop) {
	Expr_ptr expr = Prop_get_expr(prop);
	int type = node_get_type(expr);
	switch (type) {
		case CONTEXT:
			return cadr(expr);
		case OP_GLOBAL:
			return car(expr);
		default:
			fprintf(stderr, "Unkown node type %d: ", type);
			print_node(nusmv_stderr, expr);
			fprintf(stderr, "\n");
	}
	return expr; /* Unhealthy default */
}

static Expr_ptr generate_disjunc(Prop_ptr prop, Trace_ptr fipath) {
	static const char * fname = __func__;
	TraceIter last_state;
	TraceSymbolsIter symbols_iter;
	Expr_ptr result = NULL;
	Expr_ptr inv = NULL;
	node_ptr symbol = NULL;
	SymbTable_ptr symb_table;
	/* At the moment only treat case 4: Last state */
	nextce_debug(5, "%s: Enter", fname);
	inv = get_inv(prop);
	result = Expr_not(inv);
	symb_table = Trace_get_symb_table(fipath);
	last_state = Trace_last_iter(fipath);
	last_state = TraceIter_get_prev(last_state);
	TRACE_SYMBOLS_FOREACH(fipath, TRACE_ITER_ALL_VARS, symbols_iter, symbol) {
		node_ptr value = Trace_step_get_value(fipath, last_state, symbol);
		Expr_ptr symb_val_pair;
		if (!value) {
			nextce_debug(5, "%s: Did not find symbol:", fname);
			if (is_nextce_debug(5)) {
				print_node(nusmv_stderr, symbol);
			}
			nextce_debug(5, "");
			continue;
		}
		symb_val_pair = Expr_equal(symbol, value, symb_table);
		result = Expr_and(result, symb_val_pair);
	}
	result = nextce_expr_until(inv, result);
	nextce_debug(5, "%s: Exit", fname);
	debug_print_expr(result);
	return result;
}

/* Update the property - if property hasn't been checked, do nothing. Otherwise,
add L_{q_i} to the property.*/ 
Prop_ptr create_updated_prop(Prop_ptr prop, const options_t * options) {
	static const char * fname = __func__;
	Prop_Status status;
	Trace_ptr trace;
	Trace_ptr fipath;
	Expr_ptr Lqi;
	Expr_ptr newProp;
	nextce_debug(5, "%s: Enter", fname); 
	status = Prop_get_status(prop);
	if ((status == Prop_NoStatus) || (status == Prop_Unchecked)) {
		nextce_debug(5, "%s: Exit: Propery is not checked", fname); 
		return prop;
	}
	trace = get_ce(prop);
	fipath = create_fipath(trace);
	Lqi = generate_disjunc(prop, fipath);
	if (!Lqi) {
		nextce_debug(5, "%s: Failed to generate Lqi", fname); 
		return prop;
	}
//	debug_print_expr(Lqi);
	newProp = Prop_get_expr(prop);
	newProp = Expr_or(newProp, Lqi);
	debug_print_expr(newProp);
//	Prop_set_expr(newProp);
/*
	trace <- get_ce(prop)
	fipath <- create_fipath(trace)
	L_q_i <- generate_disjunc(prop, fipath)
	debug print L_q_i
	append_disjunct(prop, L_q_i)	
	debug print prop
	free_disjunc(L_q_i)
	free_fipath(fipath)
*/
	nextce_debug(5, "%s: Exit", fname); 
	return Prop_create_partial(newProp, Prop_get_type(prop));
}

/**
	Returns 0 if prop has no more counter examples - i.e. prop is now true
*/
int displayNextCE(Prop_ptr prop, const options_t * options) {
	static const char * fname = __func__;
	/* TODO We may need to filter properties */
	nextce_debug(5, "%s: Enter", fname); 
	prop = create_updated_prop(prop, options);
	if (is_nextce_debug(5)) {
		printf("%s: About to verify property: ", fname); 
		Prop_print(prop, stdout, PROP_PRINT_FMT_FORMULA);
		printf("\n");
	}
	Prop_verify(prop); // Also prints
	if (Prop_get_status(prop) == Prop_True) {
		nextce_debug(5, "%s: No more counterexamples", fname); 
		return 0;
	}
	nextce_debug(5, "%s: There may be more counterexamples", fname); 
	return 1;
}

int nextceDo(const options_t * options) {
	static const char * fname = __func__;
	int cnt;
	PropDb_ptr propdb = PropPkg_get_prop_database();
	if (options->prop_num != -1) {
		Prop_ptr prop = PropDb_get_prop_at_index(propdb, options->prop_num);
		displayNextCE(prop, options);
		return 0;
	}
	/* Iterate all properties */
	for (cnt = 0; cnt < PropDb_get_size(propdb); cnt++) {
		Prop_ptr prop = PropDb_get_prop_at_index(propdb, cnt);
		displayNextCE(prop, options);
	}
	return 0;
}

int NextCEUsage() {
	fprintf(nusmv_stderr, "Usage: next_ce [-h] [ [ -n index ] | [ -P name ] ]\n");
	fprintf(nusmv_stderr, "  -h \t\tPrints this message\n");
	fprintf(nusmv_stderr, "  -n \t\tDisplay next counter example for property numbered 'index'\n");
	fprintf(nusmv_stderr, "  -P \t\tDisplay next counter example for property named 'name'\n");
	return 1;
}

int CommandCENextCE(int argc, char ** argv) {
	static const char * fname = "CommandCENextCE";
	int rc;
	int c;
	options_t options = {-1};
	nextce_debug(5, "%s: Enter", fname); 
	util_getopt_reset();
	while ((c = util_getopt(argc, argv, "hn:P:")) != EOF) {
		switch (c) {
		case 'h':
			return NextCEUsage();
		case 'n':
			if (options.prop_num != -1) {
				return NextCEUsage();
			}
			options.prop_num = PropDb_get_prop_index_from_string(
					PropPkg_get_prop_database(),
					util_optarg);
			if (options.prop_num == -1) {
				return(1);
			}
			break;
		case 'P': {
			char * formula_name;
			if (options.prop_num != -1) {
				return NextCEUsage();
			}
			formula_name = util_strsav(util_optarg);
			options.prop_num = PropDb_get_prop_index_from_string(
					PropPkg_get_prop_database(),
					util_optarg);
			if (options.prop_num == -1) {
				fprintf(nusmv_stderr, "No property named '%s'\n",
						formula_name);
				FREE(formula_name);
				return(1);
			}
			FREE(formula_name);
			break;
		}
		}
	}
	if (argc != util_optind) {
		return NextCEUsage();
	}
	debug_show_options(fname, &options);
	rc = nextceDo(&options);
	nextce_debug(5, "%s: Exit", fname); 
	return rc;
}

int CommandCEResetCE(int argc, char ** argv) {
	static const char * fname = "CommandCEResetCE";
	nextce_debug(5, "%s: Enter", fname); 
	nextce_debug(5, "%s: Exit", fname); 
	return 0;
}

int CommandCEComputeAll(int argc, char ** argv) {
	static const char * fname = "CommandCEComputeAll";
	nextce_debug(5, "%s: Enter", fname); 
	nextce_debug(5, "%s: Exit", fname); 
/* Algo:
	For each prop in properties
		prop <- clone(property to check )
		while (true) {
			Prop_verify(prop); // Also prints
			if (Prop_get_status(prop) == Prop_True) {
				break; // We're done with this property
			}
		}
	endfor
*/
	return 0;
}

