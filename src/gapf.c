/* Linux Advanced Utils Source Code
 * ----------------------------------------------------------------------------
 *  Title: Echo command
 *  File: src/gapf.c
 *  Author: OkulusDev aka DrArgentum
 *  License: GNU GPL v2
 * ----------------------------------------------------------------------------
 *  Description: A simple script that outputs file content with info about it */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define BLUE_COLOR "\033[34m"
#define RESET_COLOR "\033[0m"

void printFileDetails(const char* filename) {
    struct stat fileStat;
    if (stat(filename, &fileStat) < 0) {
        perror("stat");
        return;
    }

    printf("\nFile Details:\n");
	printf("File Permissions: ");
    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("(%o)\n", fileStat.st_mode & 0777);
    printf("Last modified: %s", ctime(&fileStat.st_mtime));
}

void printFileType(const char* filename) {
    const char* extension = strrchr(filename, '.');
    if (extension != NULL) {
        if (strcmp(extension, ".md") == 0) {
            printf("File Type: Markdown\n");
        } else if (strcmp(extension, ".c") == 0) {
            printf("File Type: C Code\n");
		} else if (strcmp(extension, ".cpp") == 0) {
			printf("File Type: C++ Code\n");
		} else if (strcmp(extension, ".py") == 0) {
            printf("File Type: Python Code\n");
        } else {
            printf("File Type: Unknown\n");
        }
    } else {
        printf("File Type: Unknown\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    char line[256];
    int lineNumber = 1;

    while (fgets(line, sizeof(line), file)) {
        printf("%s%3d: %s%s", BLUE_COLOR, lineNumber, line, RESET_COLOR);
        lineNumber++;
    }

    fclose(file);

    printFileDetails(argv[1]);
    printFileType(argv[1]);

    return 0;
}
