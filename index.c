#include "index.h"
#include "pes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INDEX_FILE ".pes/index"

int index_load(Index *index) {
    FILE *f = fopen(INDEX_FILE, "r");

    index->count = 0;

    if (!f) return 0;

    char hex[65];
    char path[256];

    while (fscanf(f, "%s %s", hex, path) == 2) {
        if (index->count >= MAX_INDEX_ENTRIES) break;

        hex_to_hash(hex, &index->entries[index->count].hash);

        strncpy(index->entries[index->count].path, path,
                sizeof(index->entries[index->count].path) - 1);
        index->entries[index->count].path[
            sizeof(index->entries[index->count].path) - 1
        ] = '\0';

        index->entries[index->count].mode = 100644;

        index->count++;
    }

    fclose(f);
    return 0;
}

int index_save(const Index *index) {
    FILE *f = fopen(INDEX_FILE, "w");
    if (!f) return -1;

    for (int i = 0; i < index->count; i++) {
        char hex[65];
        hash_to_hex(&index->entries[i].hash, hex);

        fprintf(f, "%s %s\n", hex, index->entries[i].path);
    }

    fclose(f);
    return 0;
}

int index_add(Index *index, const char *path) {
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

    if (fread(buffer, 1, size, f) != size) {
        fclose(f);
        free(buffer);
        return -1;
    }

    fclose(f);

    ObjectID id;
    object_write(OBJ_BLOB, buffer, size, &id);

    free(buffer);

    // update if exists
    for (int i = 0; i < index->count; i++) {
        if (strcmp(index->entries[i].path, path) == 0) {
            index->entries[i].hash = id;
            return 0;
        }
    }

    if (index->count >= MAX_INDEX_ENTRIES) return -1;

    IndexEntry *e = &index->entries[index->count];

    strncpy(e->path, path, sizeof(e->path) - 1);
    e->path[sizeof(e->path) - 1] = '\0';

    e->hash = id;
    e->mode = 100644;

    index->count++;

    return 0;
}
