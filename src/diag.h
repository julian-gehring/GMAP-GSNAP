/* $Id: diag.h,v 1.14 2010/02/03 01:57:58 twu Exp $ */
#ifndef DIAG_INCLUDED
#define DIAG_INCLUDED
#include "bool.h"
#include "list.h"

#define T Diag_T
typedef struct T *T;

extern int
Diag_diagonal (T this);
extern int
Diag_querystart (T this);
extern int
Diag_queryend (T this);
extern int
Diag_nconsecutive (T this);
extern bool
Diag_dominatedp (T this);
extern void
Diag_set_dominatedp (T this);
extern int
Diag_compare_nconsecutive (const void *x, const void *y);
extern int
Diag_compare_diagonal (const void *x, const void *y);
extern double
Diag_update_coverage (bool *coveredp, int *ncovered, List_T diagonals, int querylength);
extern int
Diag_compare_querystart (const void *x, const void *y);
extern void
Diag_print_segments (List_T diagonals, char *queryseq_ptr, char *genomicseg_ptr);
extern void
Diag_range (int *start, int *end, List_T diagonals, int querylength);
extern int
Diag_compute_bounds (unsigned int *minactive, unsigned int *maxactive, List_T diagonals,
		     int genomiclength, int querylength, int indexsize, bool debug_graphic_p,
		     bool diagnosticp, char *queryuc_ptr, char *genomicuc_ptr);

#undef T
#endif

