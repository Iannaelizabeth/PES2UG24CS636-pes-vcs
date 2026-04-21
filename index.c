// index.c (Commit 1)

#include "index.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int index_load(Index *index) {
    index->count = 0;
    index->entries = NULL;
    return 0;
}

int index_save(Index *index) {
    (void)index;
    return 0;
}
