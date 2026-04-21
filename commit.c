#include "commit.h"
#include "tree.h"
#include "index.h"
#include "pes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int create_commit(const char *message, ObjectID *commit_id) {
    ObjectID tree_id;

    if (tree_from_index(&tree_id) != 0) {
        return -1;
    }

    char tree_hex[65];
    hash_to_hex(&tree_id, tree_hex);

    // read parent commit (if exists)
    char parent[65] = {0};

    FILE *head = fopen(".pes/refs/heads/main", "r");
    if (head) {
        fgets(parent, sizeof(parent), head);
        parent[strcspn(parent, "\n")] = 0;
        fclose(head);
    }

    char buffer[2048];

    int len;
    if (strlen(parent) > 0) {
        len = snprintf(buffer, sizeof(buffer),
            "tree %s\nparent %s\n\n%s\n",
            tree_hex,
            parent,
            message
        );
    } else {
        len = snprintf(buffer, sizeof(buffer),
            "tree %s\n\n%s\n",
            tree_hex,
            message
        );
    }

    if (object_write(OBJ_COMMIT, buffer, len, commit_id) != 0) {
        return -1;
    }

    char commit_hex[65];
    hash_to_hex(commit_id, commit_hex);

    // ensure refs exist
    mkdir(".pes/refs", 0755);
    mkdir(".pes/refs/heads", 0755);

    // write branch
    FILE *f = fopen(".pes/refs/heads/main", "w");
    if (f) {
        fprintf(f, "%s\n", commit_hex);
        fclose(f);
    }

    // HEAD points to branch
    FILE *h = fopen(".pes/HEAD", "w");
    if (h) {
        fprintf(h, "refs/heads/main\n");
        fclose(h);
    }

    printf("Committed: %s\n", commit_hex);

    return 0;
}
