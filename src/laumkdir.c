/* Linux Advanced Utils Source Code
 * ----------------------------------------------------------------------------
 *  Title: mkdir analogue
 *  File: src/laumkdir.c
 *  Author: OkulusDev aka DrArgentum
 *  License: GNU GPL v2
 * ----------------------------------------------------------------------------
 *  Description: A simple utilite for creating directories */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

void make_directory(const char *path) {
    int result = mkdir(path, 0777);
    if (result == 0) {
        printf("\033[0;32mSuccess:\033[0m Directory '%s' created.\n", path);
    } else {
        printf("\033[0;31mError:\033[0m Failed to create directory '%s'.\n", path);
        exit(1);
    }
}

void create_parent_directories(const char *path) {
    char *path_copy = strdup(path);
    char *token = strtok(path_copy, "/");
    char *directory_path = NULL;

    while (token != NULL) {
        if (directory_path == NULL) {
            directory_path = strdup(token);
        } else {
            size_t length = strlen(directory_path) + strlen(token) + 2;
            char *temp = (char *) malloc(length);
            snprintf(temp, length, "%s/%s", directory_path, token);
            free(directory_path);
            directory_path = temp;
        }

        make_directory(directory_path);
        token = strtok(NULL, "/");
    }

    free(directory_path);
    free(path_copy);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("\033[0;31mError:\033[0m No directory name specified.\n");
        exit(1);
    }

    if (strcmp(argv[1], "-p") == 0 || strcmp(argv[1], "--parents") == 0) {
        if (argc < 3) {
            printf("\033[0;31mError:\033[0m No directory path specified.\n");
            exit(1);
        }

        create_parent_directories(argv[2]);
    } else {
        make_directory(argv[1]);
    }

    return 0;
}
