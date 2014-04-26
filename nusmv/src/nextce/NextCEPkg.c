
#include "NextCEPkg.h"
#include "NextCECmd.h"

#include "cmd/cmd.h"

#define NEXT_CE_CMD_NAME "next_ce"
#define RESET_CE_CMD_NAME "reset_ce"
#define COMPUTE_ALL_CE_CMD_NAME "compute_all"

/**
 * Initialise the nextce library. set up the commands defined in the macros at
 * the beginning of this file
 */
void NextCE_init_cmd(void) {
	Cmd_CommandAdd(NEXT_CE_CMD_NAME,  CommandCENextCE, 0, true);
	Cmd_CommandAdd(RESET_CE_CMD_NAME,   CommandCEResetCE, 0, true);
	Cmd_CommandAdd(COMPUTE_ALL_CE_CMD_NAME, CommandCEComputeAll, 0, true);
}

/**
 * De-initialise the nextce library. Clear up the registered commands
 */
void NextCE_quit_cmd(void) {
	Cmd_CommandRemove(NEXT_CE_CMD_NAME);
	Cmd_CommandRemove(RESET_CE_CMD_NAME);
	Cmd_CommandRemove(COMPUTE_ALL_CE_CMD_NAME);	
}

/**
 * Initialise any global structures the nextce library uses.
 * Note: None are used at the moment, so this command does nothing
 */
void NextCE_init(void) {
	/* TODO Structure initialisation will go here */
	/* Nothing to do here at this time */
}

/**
 * De-initialise any global structures the nextce library uses.
 * Note: None are used at the moment, so this command does nothing
 */
void NextCE_quit(void) {
	/* TODO Structure destruction will go here */
	/* Nothing to do here at this time */
}

/**
 * Forward this call to the internal implementation in NextCECmd.c.
 * Setter function for the global equivalnce_class registry value.
 * This method is invoked when the user calls 'set ce_equivalence'
 * @param eq_class The new equivalence class
 */
void NextCE_set_ce_equivalence(int eq_class) {
	NextCE_set_equivalence_class(eq_class);
}

