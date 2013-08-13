#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#ifdef	__cplusplus
extern "C" {
#endif

#include "utils.h"

void read_dcd_header(FILE *dcdf, DCD *dcd);
void read_dcd_traj(FILE *dcdf, DCD *dcd, float **crd, int index_c[], int index_r[], int ntot);
void read_charmm_cor(FILE *corfile, DCD *dcd, float **ref,
                     char **sele_cent, int elem_cent, PARSING_MODE *pmode_cent, int index_cent[], int *nsel_cent,
                     char **sele_rmsd, int elem_rmsd, PARSING_MODE *pmode_rmsd, int index_rmsd[], int *nsel_rmsd);

#ifdef	__cplusplus
}
#endif

#endif // IO_H_INCLUDED