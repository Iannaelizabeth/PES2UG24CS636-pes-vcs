// index.c (Commit 5 FINAL)

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

    if (fread(&index->count, sizeof(int), 1, f) != 1) {
        fclose(f);
        return -1;
    }

    index->entries = malloc(sizeof(IndexEntry) * index->count);

    if (fread(index->entries, sizeof(IndexEntry), index->count, f) != (size_t)index->count) {
        fclose(f);
        free(index->entries);
        return -1;
    }

    fclose(f);
    return 0;
}

int index_save(Index *index) {
    FILE *f = fopen(INDEX_FILE, "wb");
    if (!f) return -1;

    if (fwrite(&index->count, sizeof(int), 1, f) != 1) {
        fclose(f);
        return -1;
    }

    if (fwrite(index->entries, sizeof(IndexEntry), index->count, f) != (size_t)index->count) {
        fclose(f);
        return -1;
    }

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

    strncpy(e->path, path, sizeof(e->path) - 1);
    e->path[sizeof(e->path) - 1] = '\0';

    e->hash = *id;
    e->mode = mode;

    index->count++;

    return 0;
}
