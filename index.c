#include "index.h"
#include "pes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INDEX_FILE ".pes/index"

int index_load(Index *index) {
    FILE *f = fopen(INDEX_FILE, "rb");

    if (!f) {
        index->count = 0;
        return 0;
    }

    if (fread(&index->count, sizeof(int), 1, f) != 1) {
        fclose(f);
        return -1;
    }

    if (fread(index->entries, sizeof(IndexEntry), index->count, f) != (size_t)index->count) {
        fclose(f);
        return -1;
    }

    fclose(f);
    return 0;
}

int index_save(const Index *index) {
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

int index_add(Index *index, const char *path) {
    // read file
    FILE *f = fopen(path, "rb");
    if (!f) return -1;

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    rewind(f);

    char *buffer = malloc(size);
    if (!buffer) {
        fclose(f);
        return -1;
    }

    fread(buffer, 1, size, f);
    fclose(f);

    ObjectID id;
    object_write(OBJ_BLOB, buffer, size, &id);

    free(buffer);

    // check if already exists
    for (int i = 0; i < index->count; i++) {
        if (strcmp(index->entries[i].path, path) == 0) {
            index->entries[i].hash = id;
            return 0;
        }
    }

    if (index->count >= MAX_INDEX_ENTRIES) {
        return -1;
    }

    IndexEntry *e = &index->entries[index->count];

    strncpy(e->path, path, sizeof(e->path) - 1);
    e->path[sizeof(e->path) - 1] = '\0';

    e->hash = id;
    e->mode = 100644;

    index->count++;

    return 0;
}
