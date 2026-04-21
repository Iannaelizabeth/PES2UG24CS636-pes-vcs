void init_repo() {
    printf("Init command detected\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./pes <command>\n");
        return 1;
    }

    if (strcmp(argv[1], "init") == 0) {
        init_repo();
    } else {
        printf("Unknown command\n");
    }

    return 0;
}
