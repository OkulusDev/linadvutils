#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

// COLORS
#define RESET			"\033[0m"
#define YELLOW			"\033[1;33m"
#define GREEN			"\033[1;32m"
#define RED 			"\033[1;31m"
#define PURPLE			"\033[1;35m"
#define BLUE			"\033[1;34m"
#define CYAN			"\033[1;36m"
#define BOLD			"\033[1m"


#define MAX_FILE_NAME_LENGTH 256
#define MAX_GRID_COLUMNS 4

void print_styled(const char* color, const char* text, char spch) {
    /* Print styled text, with colors and (if exists) special char */
    if (spch != ' ') { // if special char is empty
        printf("%c%s%s%s\t", spch, color, text, RESET);
    } else {
        printf("%s%s%s\t", color, text, RESET);
    }
}

void check_permissions(struct stat fileStat) {
    /* Function for checking and printing file permissions */
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
}

void print_grid(char** items, int count) {
    /* Function for printing items in a grid */
    int max_name_length = 0;
    int num_cols = 0;
    int num_rows = 0;

    // Find the maximum length of file/directory names
    for (int i = 0; i < count; i++) {
        int curr_length = strlen(items[i]);
        if (curr_length > max_name_length) {
            max_name_length = curr_length;
        }
    }

    // Calculate the number of columns and rows in the grid
    num_cols = MAX_GRID_COLUMNS;
    num_rows = (count + num_cols - 1) / num_cols;

    // Print the items in a grid
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            int index = i + j * num_rows;
            if (index < count) {
                printf("%-*s  ", max_name_length, items[index]);
            }
        }
        printf("\n");
    }
}

void list_files(const char* dir_path) {
    /* Function for listing files and directories in a specific directory */
    DIR* dir;
    struct dirent* entry;
    struct stat fileStat;
    char** items = (char**)malloc(sizeof(char*) * MAX_FILE_NAME_LENGTH);
    int count = 0;

    // Open the directory
    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    // Read the directory entries
    while ((entry = readdir(dir)) != NULL) {
        char full_path[MAX_FILE_NAME_LENGTH];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        // Get the file stats
        if (lstat(full_path, &fileStat) < 0) {
            perror("lstat");
            continue;
        }

        // Skip hidden files and current/parent directories
        if (entry->d_name[0] == '.') {
            continue;
        }

        // Get the file/directory name
        items[count] = strdup(entry->d_name);
        count++;
    }

    // Print the items in a grid
    print_grid(items, count);

    // Clean up resources
    closedir(dir);
    for (int i = 0; i < count; i++) {
        free(items[i]);
    }
    free(items);
}

int main() {
    // Get the current working directory
    char cwd[MAX_FILE_NAME_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return 1;
    }

    // List the files and directories in the current directory
    list_files(cwd);

    return 0;
}
