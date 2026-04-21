// index.c (Commit 4)

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
    FILE *f = fopen(INDEX_FILE, "wb");
    if (!f) return -1;

    fwrite(&index->count, sizeof(int), 1, f);
    fwrite(index->entries, sizeof(IndexEntry), index->count, f);

    fclose(f);
    return 0;
}

int index_add(Index *index, const char *path, ObjectID *id, int mode) {
    for (int i = 0; i < index->count; i++) {
        if (strcmp(index->entries[i].path, path) == 0) {
            index->entries[i].hash = *id;
            index->entries[i].mode = mode;
            return 0;
        }
    }

    index->entries = realloc(index->entries, sizeof(IndexEntry) * (index->count + 1));

    IndexEntry *e = &index->entries[index->count];

    strcpy(e->path, path);
    e->hash = *id;
    e->mode = mode;

    index->count++;

    return 0;
}
