/* Linux Advanced Utils Source Code
 * ----------------------------------------------------------------------------
 *  Title: lsblk analogue
 *  File: src/laugrep.cpp
 *  Author: OkulusDev aka DrArgentum
 *  License: GNU GPL v2
 * ----------------------------------------------------------------------------
 *  Description: A simple lsblk alternative for print info about disks */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <dirent.h>
#include <linux/kdev_t.h>

#define BUF_SIZE 1024

struct Disk {
    char name[BUF_SIZE];
    int maj_min;
    int rm;
    long long size;
    bool ro;
    char type[BUF_SIZE];
    char mountpoint[BUF_SIZE];
};

void print_disk(struct Disk disk) {
    printf("%-6s %-7d %-2d %-6lld %-2d %-6s %-12s\n", disk.name, disk.maj_min, disk.rm, disk.size, disk.ro, disk.type, disk.mountpoint);
}

int main() {
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char path[BUF_SIZE];
    struct Disk disk;
    FILE *mounts;

    printf("%-6s %-7s %-2s %-6s %-2s %-6s %-12s\n", "NAME", "MAJ:MIN", "RM", "SIZE", "RO", "TYPE", "MOUNTPOINTS");

    dir = opendir("/sys/block");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;

        snprintf(path, BUF_SIZE, "/sys/block/%s/removable", entry->d_name);
        if (stat(path, &st) == -1)
            continue;

        if (S_ISREG(st.st_mode) && st.st_size > 0) {
            disk.rm = 1;
        } else {
            disk.rm = 0;
        }

        snprintf(path, BUF_SIZE, "/sys/block/%s/ro", entry->d_name);
        if (stat(path, &st) == -1)
            continue;

        if (S_ISREG(st.st_mode) && st.st_size > 0) {
            disk.ro = true;
        } else {
            disk.ro = false;
        }

        snprintf(path, BUF_SIZE, "/sys/block/%s/size", entry->d_name);
        if (stat(path, &st) == -1)
            continue;

        disk.size = st.st_size * 512ULL;

        disk.maj_min = makedev(st.st_rdev >> 8, st.st_rdev & 0xFF);

        strcpy(disk.name, entry->d_name);

        snprintf(path, BUF_SIZE, "/dev/%s", entry->d_name);
        if ((disk.maj_min != 0 || disk.size != 0) && stat(path, &st) == 0 && S_ISBLK(st.st_mode)) {
            strcpy(disk.type, "disk");
            strcpy(disk.mountpoint, "-");
        } else {
            strcpy(disk.type, "part");
            sprintf(path, "/sys/block/%s/%s/mountpoint", entry->d_name, disk.name);
            mounts = fopen(path, "r");
            if (mounts == NULL) {
                perror("fopen");
                strcpy(disk.mountpoint, "-");
            } else {
                fgets(disk.mountpoint, BUF_SIZE, mounts);
                strtok(disk.mountpoint, "\n");
                fclose(mounts);
            }
        }

        print_disk(disk);
    }

    closedir(dir);
    exit(EXIT_SUCCESS);
}
