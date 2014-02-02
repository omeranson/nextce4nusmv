
#include "NextCEPkg.h"
#include "NextCECmd.h"

#include "cmd/cmd.h"

#define NEXT_CE_CMD_NAME "next_ce"
#define RESET_CE_CMD_NAME "reset_ce"
#define COMPUTE_ALL_CE_CMD_NAME "compute_all"

void NextCE_init_cmd(void) {
	Cmd_CommandAdd(NEXT_CE_CMD_NAME,  CommandCENextCE, 0, true);
	Cmd_CommandAdd(RESET_CE_CMD_NAME,   CommandCEResetCE, 0, true);
	Cmd_CommandAdd(COMPUTE_ALL_CE_CMD_NAME, CommandCEComputeAll, 0, true);
}

void NextCE_quit_cmd(void) {
	Cmd_CommandRemove(NEXT_CE_CMD_NAME);
	Cmd_CommandRemove(RESET_CE_CMD_NAME);
	Cmd_CommandRemove(COMPUTE_ALL_CE_CMD_NAME);	
}

void NextCE_init(void) {
	/* TODO Structure initialisation will go here */
}

void NextCE_quit(void) {
	/* TODO Structure destruction will go here */
}

