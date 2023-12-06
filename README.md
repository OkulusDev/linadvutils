# linadvutils
Advanced Linux Itils source code

## About LinAdvUtils
Linux Advanced Utils, also known as LAU, is a comprehensive package of small C programs designed to optimize the user experience and boost productivity on Linux machines. This article delves into the features and benefits of LAU, showcasing how it enables users to work more efficiently and conveniently with their Linux systems.

Linux Advanced Utils (LAU) offers a comprehensive set of C programs that enhance the user experience of Linux systems. With a range of utilities dedicated to seamless file management, efficient text processing, simplified system monitoring, intelligent archiving, and customization options, LAU empowers users to work faster, smarter, and with enhanced convenience on their Linux machines. LAU serves as a valuable toolkit for novices and experienced users alike, amplifying productivity while maintaining the flexibility and simplicity that Linux users appreciate. Embrace LAU today and unlock the true potential of your Linux environment.

## Compilation from source code
You must have the FASM assembler compiler, the gcc C compiler, and the make package assembly system installed.

After installing these programs from repositories, follow the following instructions:

### Cloning repository

```bash
git clone https://github.com/OkulusDev/linadvutils.git
cd linadvutils
```

### Compilation & Run

```bash
make clean build install
```

## Usage
Below you can see examples of how the utility works from the LAU repository:

### lau_echo
lau_echo is a simple alternative to the echo command from Linux. It is designed to output text, as if it creates an "echo". Below you can see examples of how lau_echo works:

```bash
# Echo a "Hello World"
$ lau_echo Hello World

Hello World

# Echo a ampersand (&)
$ lau_echo "&"

&
```

### lauls
lauls is an ls-like program that outputs all files and directories in a given directory. It has the ability to show the file creation date, permissions and access. By default, it outputs directories and files in the form of a grid, but with the -l flag, the program outputs everything in a list.

```bash
# Display files and directories in the folder
$ lauls <folder>
# or
$ lauls

# Display all (including hidden) files and directories in the current folder
$ lauls -a

# Display files and directories as a list
$ lauls -l

# Display files and directories together with rights information
$ lauls -p

# Output files and directories along with information about the time of change
$ lauls -t

# Output all files and directories (including hidden ones), along with permissions and modification time information
$ lauls -a -p -t
```

### gapf
gapf is a simple program to output file content and information about a file, and gapf can also interact with directories by outputting directory information.

```bash
# Printing info and content example.txt
$ gapf example.txt

# Printing info about example_dir folder
$ gapf example_dir
```

### calc
calc is a simple terminal calculator. It supports operation priority, parentheses, and Pi.

```bash
$ calc

Enter an expression: 2 + 2 * 2
Result: 6
```

### laulsblk
laulsblk is a simple utility for displaying information about connected disks

```bash
$ laulsblk

NAME   MAJ:MIN RM SIZE   RO TYPE   MOUNTPOINTS
sdb    0       1  2097152 1  disk   -
sda    0       1  2097152 1  disk   -
```

### laups
laups allows you to view all running processes on your system

```bash
$ laups
```

### laugrep
laugrep allows you to search and output the words you are looking for in the output.

```bash
$ lau_echo "hello" | laugrep "h"

<RED COLOR>hello</RED COLOR> world
```

## License

The creator and developer of this repository is Okulus Dev (also known as DrArgentum or Alekseev Bronislav)

	Copyright © 2023  Alekseev Bronislav

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
