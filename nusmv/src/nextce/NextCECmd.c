#include <stdio.h>

#include "NextCECmd.h"
#include "NextCEDbg.h"
#include "prop/Prop.h"
#include "prop/PropDb.h"
#include "prop/propPkg.h"

extern FILE * nusmv_stderr;
typedef struct {
	int prop_num;
} options_t;

void debug_show_options(const char * fname, const options_t *options) {
	nextce_debug(5, "%s: Showing options:", fname);
	nextce_debug(5, "%s: \tprop_num: %d", fname, options->prop_num);
}

/* Update the property - if property hasn't been checked, do nothing. Otherwise,
add L_{q_i} to the property.*/ 
void update_prop(Prop_ptr prop, const options_t * options) {
	static const char * fname = __func__;
	nextce_debug(5, "%s: Enter", fname); 
	nextce_debug(5, "%s: Exit", fname); 
}

/**
	Returns 0 if prop has no more counter examples - i.e. prop is now true
*/
int displayNextCE(Prop_ptr prop, const options_t * options) {
	static const char * fname = __func__;
	/* TODO We may need to filter properties */
	nextce_debug(5, "%s: Enter", fname); 
	update_prop(prop, options);
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

