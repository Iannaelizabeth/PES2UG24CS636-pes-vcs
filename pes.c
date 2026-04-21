#include "index.h"
#include "tree.h"
#include "commit.h"
#include "pes.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

void cmd_init() {
    mkdir(".pes", 0755);
    mkdir(".pes/objects", 0755);
    mkdir(".pes/refs", 0755);
    mkdir(".pes/refs/heads", 0755);

    FILE *h = fopen(".pes/HEAD", "w");
    fprintf(h, "refs/heads/main\n");
    fclose(h);

    FILE *b = fopen(".pes/refs/heads/main", "w");
    fclose(b);

    FILE *i = fopen(".pes/index", "w");
    fclose(i);

    printf("Initialized empty PES repository\n");
}

void cmd_add(int argc, char *argv[]) {
    Index index;
    index_load(&index);

    for (int i = 2; i < argc; i++) {
        index_add(&index, argv[i]);
    }

    index_save(&index);
}

void cmd_status() {
    Index index;
    index_load(&index);

    printf("Staged files:\n");
    for (int i = 0; i < index.count; i++) {
        printf("%s\n", index.entries[i].path);
    }
}

void cmd_commit(int argc, char *argv[]) {
    if (argc < 4 || strcmp(argv[2], "-m") != 0) {
        printf("Usage: ./pes commit -m \"message\"\n");
        return;
    }

    const char *message = argv[3];

    ObjectID commit_id;

    if (create_commit(message, &commit_id) != 0) {
        printf("Commit failed\n");
    }
}

void cmd_log() {
    char ref[256];

    FILE *h = fopen(".pes/HEAD", "r");
    if (!h) return;

    fgets(ref, sizeof(ref), h);
    fclose(h);

    ref[strcspn(ref, "\n")] = 0;

    char path[300];
    snprintf(path, sizeof(path), ".pes/%s", ref);

    FILE *f = fopen(path, "r");
    if (!f) return;

    char commit_hash[65];
    fgets(commit_hash, sizeof(commit_hash), f);
    fclose(f);

    commit_hash[strcspn(commit_hash, "\n")] = 0;

    while (strlen(commit_hash) > 0) {
        printf("commit %s\n", commit_hash);

        ObjectID id;
        hex_to_hash(commit_hash, &id);

        char obj_path[512];
        object_path(&id, obj_path, sizeof(obj_path));

        FILE *cf = fopen(obj_path, "rb");
        if (!cf) break;

        fseek(cf, 0, SEEK_END);
        size_t size = ftell(cf);
        rewind(cf);

        char *buffer = malloc(size);
        fread(buffer, 1, size, cf);
        fclose(cf);

        char *data = memchr(buffer, '\0', size);
        data++;

        printf("%s\n", data);

        // find parent
        char *parent_line = strstr(data, "parent ");
        if (parent_line) {
            parent_line += 7;
            strncpy(commit_hash, parent_line, 64);
            commit_hash[64] = '\0';
            commit_hash[strcspn(commit_hash, "\n")] = 0;
        } else {
            break;
        }

        free(buffer);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./pes <command>\n");
        return 1;
    }

    if (strcmp(argv[1], "init") == 0) {
        cmd_init();
    }
    else if (strcmp(argv[1], "add") == 0) {
        cmd_add(argc, argv);
    }
    else if (strcmp(argv[1], "status") == 0) {
        cmd_status();
    }
    else if (strcmp(argv[1], "commit") == 0) {
        cmd_commit(argc, argv);
    }
    else if (strcmp(argv[1], "log") == 0) {
        cmd_log();
    }
    else {
        printf("Unknown command\n");
    }

    return 0;
}
