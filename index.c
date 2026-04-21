// index.c (Commit 2)

#include "index.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INDEX_FILE ".pes/index"

int index_load(Index *index) {
    FILE *f = fopen(INDEX_FILE, "rb");

    if (!f) {
        index->count = 0;
        index->entries = NULL;
        return 0;
    }

    fread(&index->count, sizeof(int), 1, f);

    index->entries = malloc(sizeof(IndexEntry) * index->count);

    fread(index->entries, sizeof(IndexEntry), index->count, f);

    fclose(f);
    return 0;
}

int index_save(Index *index) {
    (void)index;
    return 0;
}
