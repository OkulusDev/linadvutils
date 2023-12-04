/* Linux Advanced Utils Source Code
 * ----------------------------------------------------------------------------
 *  Title: Echo command
 *  File: src/lau_echo.c
 *  Author: OkulusDev aka DrArgentum
 *  License: GNU GPL v2
 * ----------------------------------------------------------------------------
 *  Description: A simple script that outputs what the user entered in the
 *   argument, that is, creates an "echo" */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void echo(const char* message) {
	/* Print message function (echo) */
	printf("%s\n", message);
}

int main(int argc, char* argv[]) {
	if (argc > 1) {
		char* message = (char*)malloc(sizeof(char));
		strcpy(message, argv[1]);

		for (int i=2; i < argc; i++) {
			message = (char*)realloc(message, strlen(message) + strlen(argv[i]) + 2);
			strcat(message, " ");
			strcat(message, argv[i]);
		}

		echo(message);
		free(message);
	}

	return 0;
}

