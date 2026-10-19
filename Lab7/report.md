# Laboratory Work 7
## Three Small Programs: Processes, Redirection, a Pipe

Student: Ermicev Sofia
Group: FAF-241

## Part 1 — fork + exec + wait

### Observations

1. `perror("exec")` is printed only with `nosuchprog` because a successful `execlp()` replaces the current child process with the new program and never returns to the next line. If `execlp()` fails, execution continues and `perror()` is called.

2. The child prints the PID of its parent. In my run:
- Parent PID: 41031
- Child PID: 41032
- Child's PPID: 41031

3. Exit statuses:
- `ls`: 0
- `date`: 0
- `nosuchprog`: 127

Example:

    parent: my PID is 42078
    child: my PID is 42079, my parent is 42078
    exec: No such file or directory
    parent: child 42079 ended with status 127

## Part 2 — A zombie, on purpose

During the 20-second sleep, the process table showed:

    PID     PPID    STAT    CMD
    44268   40200   S       ./zombie
    44270   44268   Z       [ls] <defunct>

### Observations

1. `ls` had status `Z`. `Z` means zombie: the child has already finished, but the parent has not yet collected its exit status.

2. After 20 seconds, `[ls] <defunct>` disappeared.

The line that caused it to disappear was:

    waitpid(pid, &status, 0);

`waitpid()` collects the child's exit status and removes the zombie entry.

## Part 3 — Redirection

The program implements:

    ls -l > out.txt

Program output:

    open gave descriptor 1
    done: the listing is in out.txt

Beginning of `out.txt`:

    total 68
    -rw-r--r-- 1 sofia sofia     0 Oct 19 14:26 out.txt
    -rwxr-xr-x 1 sofia sofia 16344 Oct 19 14:26 redirect

### Observations

1. `open()` returned descriptor 1 because `close(1)` freed descriptor 1. `open()` uses the smallest available file descriptor.

2. The output of `ls` did not appear on the screen because stdout, descriptor 1, was redirected to `out.txt`.

3. To redirect errors, descriptor 2 (`stderr`) should be closed before `open()`.

## Part 4 — A pipe

The program implements:

    ls /etc | wc -l

My program produced:

    183

The shell command also produced:

    183

### Observations

1. Yes, both commands produced the same result: 183.

2. When the parent did not close `p[1]`, the `wc` process did not finish.

3. `wc` was waiting for EOF. EOF is received only when all write ends of the pipe are closed. Since the parent still had `p[1]` open, `wc` continued waiting.

## Conclusion

In this laboratory work I practiced process creation and communication in Linux. I used `fork()` to create child processes, `execlp()` to replace a child with another program, and `waitpid()` to wait for its termination. I also observed a zombie process.

I implemented output redirection using file descriptors and built a pipeline manually by connecting the output of `ls` to the input of `wc`. This demonstrated how Linux shells implement processes, redirection, and pipes.
