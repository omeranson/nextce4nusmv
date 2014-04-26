#ifndef __NEXTCE_PKG_H__
#define __NEXTCE_PKG_H__

 /** 
 * This file (and the matching .c file) provide methods used by NuSMV start-up
 * and shut-down calls to set up and clean up the NextCE library / plugin.
 */


void NextCE_init_cmd(void);
void NextCE_quit_cmd(void);

void NextCE_init(void);
void NextCE_quit(void);

void NextCE_set_ce_equivalence(int eq_class);

#endif /* __NEXTCE_PKG_H__ */

