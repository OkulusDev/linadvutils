/* Linux Advanced Utils Source Code
 * ----------------------------------------------------------------------------
 *  Title: ls alternative command
 *  File: src/lauls.c
 *  Author: OkulusDev aka DrArgentum
 *  License: GNU GPL v2
 * ----------------------------------------------------------------------------
 *  Description: A simple script that shows a list of files and directories in
 *   a specific directory, with color support */
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
#define BLUE			"\033[1;34m"
#define CYAN			"\033[1;36m"
#define BOLD			"\033[1m"

// CONSTANTS
#define MAX_FILE_NAME_LENGTH 256
#define MAX_GRID_COLUMNS 5

void print_styled(const char* color, const char* text, char spch) {
	/* Print styled text, with colors and (if exists) special char */
	if (spch != ' ') {
		printf("%c%s%-10s%s", spch, color, text, RESET);
	} else {
		printf("%s%-10s%s", color, text, RESET);
	}
}

void check_permissions(struct stat fileStat) {
	/* Function for checking and outputting permissions, file or directory
	 * group. If st_mode fileStat is equal to d, then d is displayed in yellow,
	 * otherwise just -. And so on, we list write read and run permissions and
	 * output them in different colors:
	 * (user - green, group - blue, other - cyan)
	 *
	 * Input:
	 *  struct stat fileStat
	 *
	 * Output: void*/
	printf(YELLOW);
	printf((S_ISDIR(fileStat.st_mode)) ? "d"
			: ((S_ISLNK(fileStat.st_mode)) ? : "-"));
	printf(GREEN);
 	printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
	printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
	printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
	printf(BLUE);
	printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
	printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
	printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
	printf(CYAN);
	printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
	printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
	printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
	printf(RESET);
}

void display_files(char *dir_path, bool show_permissions, bool show_time,
					bool show_hidden, bool list_show) {
	DIR* dir = opendir(dir_path);
	if (dir == NULL) {
		printf("Error when opening directory");
		exit(1);
	}

	struct dirent *entry;
	struct stat file_stat;
	char file_path[MAX_FILE_NAME_LENGTH];
	int file_count = 0;

	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] != '.' && !show_hidden || show_hidden) {
			sprintf(file_path, "%s/%s", dir_path, entry->d_name);
			stat(file_path, &file_stat);

			if (S_ISDIR(file_stat.st_mode)) {
				print_styled(YELLOW, entry->d_name, '/');
			} else if (S_ISLNK(file_stat.st_mode)) {
				print_styled(CYAN, entry->d_name, ' ');
			} else if ((file_stat.st_mode & S_IXUSR) ||
						(file_stat.st_mode & S_IXGRP)
						|| (file_stat.st_mode & S_IXOTH)) {
				print_styled(GREEN, entry->d_name, '*');
			} else if (access(file_path, R_OK) == 0 &&
						(strstr(entry->d_name, ".png") != NULL
						|| strstr(entry->d_name, ".jpg") != NULL
						|| strstr(entry->d_name, ".jpeg") != NULL
						|| strstr(entry->d_name, ".svg") != NULL
						|| strstr(entry->d_name, ".bmp") != NULL)) {
				print_styled(BLUE, entry->d_name, ' ');
			} else {
				print_styled(BOLD, entry->d_name, ' ');
			}

			if (show_permissions) {
				check_permissions(file_stat);
				printf("(%lo) ", (unsigned long)file_stat.st_mode & 0777);
			}

			if (show_time) {
				char time_str[100];
				strftime(time_str, sizeof(time_str), "%d.%m.%Y %H:%M:%S",
						localtime(&file_stat.st_ctime));
				printf("[%s] ", time_str);
			}

			file_count++;

			if (file_count % MAX_GRID_COLUMNS == 0 && !show_time) {
				printf("\n");
			} else if (show_time || list_show) {
				printf("\n");
			}
		}
	}

	closedir(dir);
}

int main(int argc, char* argv[]) {
	char *dir_path = ".";
	bool show_permissions = false;
	bool show_time = false;
	bool show_hidden = false;
	bool list_show = false;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (argv[i][1] == 'p') {
				show_permissions = true;
			} else if (argv[i][1] == 't') {
				show_time = true;
			} else if (argv[i][1] == 'a') {
				show_hidden = true;
			} else if (argv[i][1] == 'l') {
				list_show = true;
			}
		} else {
			dir_path = argv[i];
		}
	}

	display_files(dir_path, show_permissions, show_time,
					show_hidden, list_show);
	printf("\n");

	return EXIT_SUCCESS;
}

