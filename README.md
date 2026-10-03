*This project has been created as part of the 42 curriculum by salegari.*

# get_next_line

## Description

get_next_line is a C function that reads a file descriptor one line at a time. Each call returns the next line as a dynamically allocated, null-terminated string, including the newline character when present. The final line is returned even if it does not end with a newline.

The project develops an understanding of file descriptors, the `read()` system call, static variables, and dynamic memory management. It implements line-based reading using only `read()`, `malloc()`, `free()`, and custom string helpers.

```c
char *get_next_line(int fd);
```

The function returns `NULL` when no data remains or an error prevents it from returning a line. The caller must free each returned line.

The mandatory version stores the unread content for one input stream. The bonus version maintains independent buffers for multiple file descriptors, allowing calls to alternate between them.

## Instructions

### Requirements

- A C compiler such as GCC or Clang.
- A POSIX environment providing `read()`, such as Linux or macOS.
- Git to clone the repository.

### Download

```sh
git clone https://github.com/sanlega/gnl_2.git
cd gnl_2
```

### Compilation

The repository contains function implementations rather than a standalone program. It does not include a Makefile or an active `main()`.

Compile the mandatory sources into object files:

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 -c get_next_line.c get_next_line_utils.c
```

For the bonus version:

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 -c get_next_line_bonus.c get_next_line_utils_bonus.c
```

`BUFFER_SIZE` sets the maximum number of bytes requested by each `read()` call; it does not limit line length. It must be positive. If it is not defined at compilation, the headers use a default value of `1000000`.

Compile either the mandatory or the bonus implementation into a program. Both define the same function names and should not be linked together.

### Execution example

Create a `main.c` file in the repository root:

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int     fd;
    char    *line;

    if (argc > 2)
        return (1);
    fd = STDIN_FILENO;
    if (argc == 2)
    {
        fd = open(argv[1], O_RDONLY);
        if (fd == -1)
        {
            perror("open");
            return (1);
        }
    }
    line = get_next_line(fd);
    while (line != NULL)
    {
        printf("%s", line);
        free(line);
        line = get_next_line(fd);
    }
    if (argc == 2)
        close(fd);
    return (0);
}
```

Build and read a text file:

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c -o gnl
./gnl example.txt
```

Read from standard input:

```sh
printf 'First line\nSecond line\n' | ./gnl
```

To use the bonus implementation, change the include in `main.c` to `"get_next_line_bonus.h"` and compile:

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line_bonus.c get_next_line_utils_bonus.c -o gnl_bonus
./gnl_bonus example.txt
```

In the current bonus implementation, descriptors must satisfy `0 <= fd < MAX_FD`, where `MAX_FD` is `1024`. The function does not check the upper bound before indexing its static array.

The example stops on `NULL`; the function's interface does not distinguish end-of-file from an error.

## Algorithm and justification

### Buffered reading with persistent leftovers

The implementation combines block-based reading with a static pointer that preserves unread characters between calls.

1. **Validate the input.** Reject a negative file descriptor or a non-positive `BUFFER_SIZE`.
2. **Prepare the buffer.** `ft_concat()` allocates a temporary buffer of `BUFFER_SIZE + 1` bytes. If there is no saved content, it creates an empty string.
3. **Read until a line is available.** `ft_read_concat()` first checks the saved content for a newline. While none is present, it reads another block, adds a null terminator, and joins the block to the accumulated string. Reading stops when the accumulated string contains a newline, `read()` returns zero, or an error occurs.
4. **Extract the result.** `ft_extract()` with flag `0` copies the first line into a separate allocation, including its newline if present.
5. **Preserve the remainder.** `ft_extract()` with flag `1` copies the characters after that newline into the static state and frees the previous accumulated string. If there is no newline, the final content has been returned and the state is cleared.
6. **Handle exhaustion and failures.** An empty accumulation or a read/allocation failure during accumulation causes the function to release its saved content and return `NULL`. Failure to allocate the returned line also clears the state.

For example, if a read produces `"Hello\nWorld\n"`, the first call returns `"Hello\n"` and saves `"World\n"`. The next call returns the saved line without performing another `read()`.

### Why this algorithm?

Reading blocks reduces the number of system calls compared with reading one byte at a time when `BUFFER_SIZE > 1`. A static variable is necessary because a block may contain several lines: characters after the first newline must remain available for the next call.

Dynamic concatenation supports lines longer than `BUFFER_SIZE` without imposing a fixed line-length limit. Separating the returned line from the saved remainder also gives the caller ownership of the result while the function retains its internal state.

The implementation is straightforward and fits the project's restricted function set. Its tradeoff is repeated allocation, scanning, and copying: `ft_strjoin()` copies all accumulated content whenever another block is appended. For a long line of length `L` read in blocks of size `B`, this can take roughly `O(L² / B + L)` time, becoming quadratic when `B` is fixed. Peak temporary storage is proportional to the accumulated content plus the read buffer.

### Bonus: multiple file descriptors

The bonus replaces the single static pointer with:

```c
static char *line[MAX_FD];
```

Each descriptor indexes its own saved remainder. Alternating calls between two open files therefore preserves their independent reading positions and buffered content.

An array provides direct access to each descriptor's state and keeps the reading algorithm unchanged. Its tradeoffs are a fixed descriptor range and the need to respect that range before accessing the array.

### Usage considerations

- The function is intended for text input; its string helpers do not support embedded null bytes.
- Free every returned line.
- Read a stream to completion to release its saved state. Closing a descriptor early does not automatically free its buffered content.
- The mandatory version should not be used to alternate between input streams.
- The bonus version is not thread-safe and does not automatically reset saved content when a descriptor is closed and reused.
- If allocation of the leftover substring fails, the current code still returns the extracted line but loses the unread remainder.

## Resources

- [42](https://42.fr/en/homepage/) — the school and curriculum behind the project. The project subject supplied by the campus is the reference for evaluation requirements.
- [POSIX read() specification](https://pubs.opengroup.org/onlinepubs/9699919799/functions/read.html) — reading bytes from a file descriptor, return values, and error conditions.
- [POSIX malloc() specification](https://pubs.opengroup.org/onlinepubs/9699919799/functions/malloc.html) — dynamic allocation.
- [POSIX free() specification](https://pubs.opengroup.org/onlinepubs/9699919799/functions/free.html) — releasing allocated memory.
- *The C Programming Language*, Brian W. Kernighan and Dennis M. Ritchie — pointers, arrays, storage duration, and memory management.
- [42 Norminette](https://github.com/42School/norminette) — the tool used to check the 42 coding standard.

### Use of AI

AI was used to inspect the repository and prepare this README, including the compilation commands, usage example, and explanation and justification of the implemented algorithm. This documentation task did not modify the project's C source files.

This statement covers the assistance used to prepare this README. Any AI assistance used elsewhere during development should also be documented here before submission.

