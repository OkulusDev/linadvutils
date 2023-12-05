/* Linux Advanced Utils Source Code
 * ----------------------------------------------------------------------------
 *	Title: ls alternative command
 *	File: src/lauls.c
 *	Author: OkulusDev aka DrArgentum
 *	License: GNU GPL v2
 * ----------------------------------------------------------------------------
 *	Description: A simple script that shows a list of files and directories in
 *	 a specific directory, with color support */
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
#define RED				"\033[1;31m"
#define PURPLE			"\033[1;35m"
#define BLUE			"\033[1;34m"
#define CYAN			"\033[1;36m"
#define BOLD			"\033[1m"

// CONSTANTS
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
	/* Function for checking and outputting permissions, file or directory
	 * group. If st_mode fileStat is equal to d, then d is displayed in yellow,
	 * otherwise just -. And so on, we list write read and run permissions and
	 * output them in different colors:
	 * (user - green, group - blue, other - cyan)
	 *
	 * Input:
	 *	struct stat fileStat
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
	printf(PURPLE);
	printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
	printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
	printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
	printf(RESET);
}

void print_grid(char** items, int count, char** colors) {
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
				printf("%s%-*s	", colors[index], max_name_length, items[index]);
				printf("%s", RESET);
			}
		}
		printf("\n");
	}
}

void list_files(const char* dir_path, bool show_permissions, bool show_time,
				bool show_hidden, bool list_show) {
	/* Function for listing files and directories in a specific directory */
	DIR* dir;
	struct dirent* entry;
	struct stat file_stat;
	char** items = (char**)malloc(sizeof(char*) * MAX_FILE_NAME_LENGTH);
	char** colors = (char**)malloc(sizeof(char*) * MAX_FILE_NAME_LENGTH);
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
		char formattedString[MAX_FILE_NAME_LENGTH];
		char color[MAX_FILE_NAME_LENGTH];
		stat(full_path, &file_stat);
		snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

		// Skip hidden files and current/parent directories
		if (entry->d_name[0] == '.' && !show_hidden) {
			continue;
		}

		// Get the file stats
		if (lstat(full_path, &file_stat) < 0) {
			perror("lstat");
			continue;
		}

		if (show_permissions) {
			list_show = true;
			check_permissions(file_stat);
			printf("%s(%lo)%s ", YELLOW,
					(unsigned long)file_stat.st_mode & 0777, RESET);
		}

		if (show_time) {
			list_show = true;
			char time_str[100];
			strftime(time_str, sizeof(time_str), "%d.%m.%Y %H:%M:%S",
					localtime(&file_stat.st_ctime));
			printf("%s[%s]%s ", CYAN, time_str, RESET);
		}

		if (S_ISDIR(file_stat.st_mode)) {
				if (!list_show) {
					sprintf(formattedString, "%s", entry->d_name);
					sprintf(color, "%s", YELLOW);
				} else {
					print_styled(YELLOW, entry->d_name, '/');
				}
			} else if (S_ISLNK(file_stat.st_mode)) {
				if (!list_show) {
					sprintf(formattedString, "%s", entry->d_name);
					sprintf(color, "%s", CYAN);
				} else {
					print_styled(CYAN, entry->d_name, ' ');
				}
			} else if ((file_stat.st_mode & S_IXUSR) ||
						(file_stat.st_mode & S_IXGRP)
						|| (file_stat.st_mode & S_IXOTH)) {
				if (!list_show) {
					sprintf(formattedString, "%s", entry->d_name);
					sprintf(color, "%s", GREEN);
				} else {
					print_styled(GREEN, entry->d_name, '*');
				}
			} else if (access(full_path, R_OK) == 0 &&
						(strstr(entry->d_name, ".png") != NULL
						|| strstr(entry->d_name, ".jpg") != NULL
						|| strstr(entry->d_name, ".jpeg") != NULL
						|| strstr(entry->d_name, ".svg") != NULL
						|| strstr(entry->d_name, ".bmp") != NULL)) {
				if (!list_show) {
					sprintf(color, "%s", BLUE);
					sprintf(formattedString, "%s", entry->d_name);
				} else {
					print_styled(BLUE, entry->d_name, ' ');
				}
			} else {
				if (!list_show) {
					sprintf(color, "%s", BOLD);
					sprintf(formattedString, "%s", entry->d_name);
				} else {
					print_styled(BOLD, entry->d_name, ' ');
				}
			}

		if (!list_show) {
			// Get the file/directory name
			items[count] = strdup(formattedString);
			colors[count] = strdup(color);
		} else {
			printf("\n");
		}
		count++;
	}

	if (!list_show)
		print_grid(items, count, colors);

	// Clean up resources
	closedir(dir);
	for (int i = 0; i < count; i++) {
		free(items[i]);
	}
	free(items);
}

int main(int argc, char* argv[]) {
	char *dir_path = ".";
	bool show_permissions = false;
	bool show_time = false;
	bool show_hidden = false;
	bool show_list = false;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (argv[i][1] == 'p') {
				show_permissions = true;
			} else if (argv[i][1] == 't') {
				show_time = true;
			} else if (argv[i][1] == 'a') {
				show_hidden = true;
			} else if (argv[i][1] == 'l') {
				show_list = true;
			}
		} else {
			dir_path = argv[i];
		}
	}

	//display_files(dir_path, show_permissions, show_time,
	//				show_hidden, list_show);
	list_files(dir_path, show_permissions, show_time,
						show_hidden, show_list);

	return EXIT_SUCCESS;
}

