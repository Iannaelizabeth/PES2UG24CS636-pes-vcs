// tree.c (Commit 1)

#include "tree.h"
#include "index.h"
#include "pes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tree_from_index(Index *index, ObjectID *tree_id) {
    if (!index || index->count == 0) {
        return -1;
    }

    printf("Building tree from index...\n");

    return 0;
}
