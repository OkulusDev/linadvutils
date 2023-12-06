/* Linux Advanced Utils Source Code
 * ----------------------------------------------------------------------------
 *  Title: lsblk analogue
 *  File: src/laups.cpp
 *  Author: OkulusDev aka DrArgentum
 *  License: GNU GPL v2
 * ----------------------------------------------------------------------------
 *  Description: A simple ps alternative for viewing processes */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#define RESET       "\033[0m"
#define YELLOW      "\033[33m"
#define GREEN       "\033[32m"
#define BLUE        "\033[34m"

typedef struct Process {
    char pid[10];
    char cmd[256];
    char time[10];
} Process;

void print_header() {
    printf(YELLOW "PID\t\t\tCMD\t\t\tTIME\n" RESET);
}

void print_process(Process *process, int use_colors) {
    if (use_colors) {
        printf(YELLOW "%s\t" GREEN "%s\t" BLUE "%s\n" RESET, process->pid, process->cmd, process->time);
    } else {
        printf("%s\t%s\t%s\n", process->pid, process->cmd, process->time);
    }
}

void ps(int show_all, int show_details, int show_owner, int show_threads, int show_memory, int use_colors) {
    DIR *dir;
    struct dirent *entry;

    print_header();

    dir = opendir("/proc");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[256];
            sprintf(path, "/proc/%s/status", entry->d_name);

            FILE *fp = fopen(path, "r");
            if (fp != NULL) {
                Process process;
                char line[256], key[256], value[256];

                while (fgets(line, sizeof(line), fp) != NULL) {
                    sscanf(line, "%s %s", key, value);

                    if (strcmp(key, "Pid:") == 0) {
                        strcpy(process.pid, value);
                    } else if (strcmp(key, "Name:") == 0) {
                        strcpy(process.cmd, value);
                    } else if (strcmp(key, "VmRSS:") == 0 && show_details) {
                        strcpy(process.time, value);
                    } else if (strcmp(key, "VmSize:") == 0 && show_memory) {
                        strcpy(process.time, value);
                    }
                }

                if (show_all || (show_owner && strcmp(process.pid, "1") != 0)) {
                    print_process(&process, use_colors);
                }

                fclose(fp);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    int show_all = 1, show_details = 1, show_owner = 1, show_threads = 1, show_memory = 1, use_colors = 1;

    ps(show_all, show_details, show_owner, show_threads, show_memory, use_colors);

    return 0;
}
