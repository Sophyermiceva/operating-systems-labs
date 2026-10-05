# Operating Systems – Lab 2

## Meet the OS You Will Build

**Name:** Sofia Ermicev  
**Group:** FAF-241  
**Year:** 2026-2027

## Part 2 – Use xv6 as the Unix It Is

### Observations

1. Three programs included with xv6 are `ls`, `cat`, and `echo`.

2. For a pipe such as `ls | grep c` to work, the OS must support processes and inter-process communication. The shell runs the commands as separate processes and connects the output of one process to the input of another through a pipe.

3. The xv6 shell is much simpler than the Linux shell, but basic Unix commands and features such as `ls`, `cat`, `echo`, pipes, and standard input/output work in a similar way.

## Part 3 – Read the Source

### Observations

1. `user/cat.c` uses the following system calls:

- `read()` – asks the kernel to read data from a file descriptor into a buffer.
- `write()` – asks the kernel to write data from a buffer to a file descriptor.
- `open()` – asks the kernel to open a file.
- `close()` – asks the kernel to close a file descriptor.
- `exit()` – terminates the current process.

2. `sys_read` is implemented in `kernel/sysfile.c` at line **69**. `sys_write` starts at line **83**.

3. Code in `kernel/` is privileged operating-system code that manages system resources and handles system calls, while code in `user/` contains ordinary programs that run on top of the kernel and request its services through system calls.
