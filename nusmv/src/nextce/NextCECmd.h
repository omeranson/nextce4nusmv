#ifndef __NEXTCE_CMD_H__
#define __NEXTCE_CMD_H__

int CommandCENextCE(int argc, char ** argv);
int CommandCEResetCE(int argc, char ** argv);
int CommandCEComputeAll(int argc, char ** argv);

int NextCE_get_equivalence_class();
void NextCE_set_equivalence_class(int class);

#endif /* __NEXTCE_CMD_H__ */
