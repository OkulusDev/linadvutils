/* Linux Advanced Utils Source Code
 * ----------------------------------------------------------------------------
 *  Title: touch analogue
 *  File: src/lautch.cpp
 *  Author: OkulusDev aka DrArgentum
 *  License: GNU GPL v2
 * ----------------------------------------------------------------------------
 *  Description: A simple utilite for creating files */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <utime.h>
#include <time.h>
#include <string.h>

// ANSI escape codes for ASCII colors
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Function to get ASCII color code based on file extension
const char* get_color_code(const char* filename) {
    const char* ext = strrchr(filename, '.');
    
    if (ext != NULL) {
        if (strcmp(ext, ".c") == 0)
            return ANSI_COLOR_GREEN;
        else if (strcmp(ext, ".h") == 0)
            return ANSI_COLOR_CYAN;
        else if (strcmp(ext, ".txt") == 0)
            return ANSI_COLOR_YELLOW;
        else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".png") == 0 || strcmp(ext, ".gif") == 0)
            return ANSI_COLOR_MAGENTA;
    }
    
    return ANSI_COLOR_RESET;
}

int main(int argc, char* argv[]) {
    // Handle command-line arguments
    bool verbose = false;
    bool create = true;
    bool modify = false;
    const char* filename = NULL;
    
    int option;
    while ((option = getopt(argc, argv, "cv")) != -1) {
        switch(option) {
            case 'c':
                create = true;
                break;
            case 'v':
                verbose = true;
                break;
            default:
                fprintf(stderr, "Usage: %s [-c] [-v] filename\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    
    if (optind < argc) {
        filename = argv[optind];
    } else {
        fprintf(stderr, "Usage: %s [-c] [-v] filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    // Check if file exists
    struct stat st;
    bool file_exists = (stat(filename, &st) == 0);
    
    // Create file if it doesn't exist
    if (create && !file_exists) {
        int fd = open(filename, O_CREAT, 0644);
        if (fd == -1) {
            perror("Failed to create file");
            exit(EXIT_FAILURE);
        }
        
        close(fd);
    }
    
    // Modify file access and modification times
    if (modify && file_exists) {
        struct utimbuf times;
        times.actime = st.st_atime;
        times.modtime = time(NULL);
        if (utime(filename, &times) == -1) {
            perror("Failed to modify timestamps");
            exit(EXIT_FAILURE);
        }
    }
    
    // Print file information if verbose flag is set
    if (verbose && file_exists) {
        printf("File: %s\n", filename);
        printf("Size: %ld bytes\n", st.st_size);
        printf("Permissions: ");
        printf((st.st_mode & S_IRUSR) ? "r" : "-");
        printf((st.st_mode & S_IWUSR) ? "w" : "-");
        printf((st.st_mode & S_IXUSR) ? "x" : "-");
        printf((st.st_mode & S_IRGRP) ? "r" : "-");
        printf((st.st_mode & S_IWGRP) ? "w" : "-");
        printf((st.st_mode & S_IXGRP) ? "x" : "-");
        printf((st.st_mode & S_IROTH) ? "r" : "-");
        printf((st.st_mode & S_IWOTH) ? "w" : "-");
        printf((st.st_mode & S_IXOTH) ? "x" : "-");
        printf("\n");
    }
    
    // Print success message and colorize filename
    if (file_exists) {
        printf("Updated ");
    } else {
        printf("Created ");
    }
    
    printf("%s%s%s\n", get_color_code(filename), filename, ANSI_COLOR_RESET);
    
    return 0;
}
