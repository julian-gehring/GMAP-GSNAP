static char rcsid[] = "$Id: resulthr.c,v 1.13 2010/03/08 20:25:36 twu Exp $";
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "resulthr.h"
#include <stdlib.h>
#include "mem.h"
#include "stage3hr.h"


#define T Result_T
struct T {
  Resulttype_T resulttype;
  int id;
  int worker_id;
  void **array;
  int npaths;
  void **array2;
  int npaths2;
};


Resulttype_T
Result_resulttype (T this) {
  return this->resulttype;
}

int
Result_id (T this) {
  return this->id;
}


int
Result_worker_id (T this) {
  return this->worker_id;
}


void **
Result_array (int *npaths, T this) {
  *npaths = this->npaths;
  return this->array;
}


void **
Result_array2 (int *npaths, T this) {
  *npaths = this->npaths2;
  return this->array2;
}


T
Result_single_read_new (int id, int worker_id, void **resultarray, int npaths) {
  T new = (T) MALLOC(sizeof(*new));

  new->resulttype = SINGLEEND_READ;
  new->id = id;
  new->worker_id = worker_id;
  new->array = resultarray;
  new->npaths = npaths;

  return new;
}

T
Result_paired_read_new (int id, int worker_id, void **resultarray, int npaths) {
  T new = (T) MALLOC(sizeof(*new));

  if (npaths == 0) {
    abort();
  } else {
#if 0
    if (Stage3pair_pairtype((Stage3pair_T) resultarray[0]) == CONCORDANT) {
      new->resulttype = PAIREDEND_CONCORDANT;
    } else {
      new->resulttype = PAIREDEND_SAMECHR;
    }
#else
    new->resulttype = PAIREDEND_CONCORDANT;
#endif
  }
  new->id = id;
  new->worker_id = worker_id;
  new->array = resultarray;
  new->npaths = npaths;

  return new;
}

T
Result_paired_as_singles_new (int id, int worker_id, void **hits5, int npaths5, void **hits3, int npaths3) {
  T new = (T) MALLOC(sizeof(*new));

  if (npaths5 == 1 && npaths3 == 1) {
    new->resulttype = PAIREDEND_AS_SINGLES_UNIQUE;
  } else {
    new->resulttype = PAIREDEND_AS_SINGLES;
  }
  new->id = id;
  new->worker_id = worker_id;
  new->array = hits5;
  new->npaths = npaths5;
  new->array2 = hits3;
  new->npaths2 = npaths3;

  return new;
}

void
Result_free (T *old) {
  int i;
  Stage3_T stage3;
  Stage3pair_T stage3pair;

  if ((*old)->resulttype == SINGLEEND_READ) {
    for (i = 0; i < (*old)->npaths; i++) {
      stage3 = (*old)->array[i];
      Stage3_free(&stage3);
    }
    FREE((*old)->array);

  } else if ((*old)->resulttype == PAIREDEND_CONCORDANT || (*old)->resulttype == PAIREDEND_SAMECHR) {
    for (i = 0; i < (*old)->npaths; i++) {
      stage3pair = (*old)->array[i];
      Stage3pair_free(&stage3pair);
    }
    FREE((*old)->array);

  } else if ((*old)->resulttype == PAIREDEND_AS_SINGLES || (*old)->resulttype == PAIREDEND_AS_SINGLES_UNIQUE) {
    for (i = 0; i < (*old)->npaths2; i++) {
      stage3 = (*old)->array2[i];
      Stage3_free(&stage3);
    }
    FREE((*old)->array2);

    for (i = 0; i < (*old)->npaths; i++) {
      stage3 = (*old)->array[i];
      Stage3_free(&stage3);
    }
    FREE((*old)->array);

  } else {
    fprintf(stderr,"Don't recognize resulttype %d\n",(*old)->resulttype);
    abort();
  }

  FREE(*old);
  return;
}


