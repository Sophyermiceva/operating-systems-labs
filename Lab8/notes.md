# Laboratory Work 8
## The Same Calls, Inside Your Own OS

Student: Ermicev Sofia
Group: FAF-241

## Part 1 — run, inside xv6

### Observations

1. The three calls used in xv6 are:

- fork()
- exec()
- wait()

In Linux Lab 7, the corresponding calls were:

- fork()
- execlp()
- waitpid()

The general logic is the same in both systems:
create a child process, replace the child with another program, and wait for the child to finish.

2. The line:

    hello from the child

is printed by the `echo` program, not by the original `run` code.

After `exec("echo", args)` succeeds, the child process is replaced by the `echo` program.

3. When `exec()` fails in xv6, the child executes:

    exit(1);

so the parent receives status 1.

In Linux Lab 7, the program used:

    exit(127);

after a failed `execlp()`, so the parent received status 127.

### Example outputs

With echo:

    parent: my pid is 3
    child: my pid is 4
    hello from the child
    parent: child 4 ended with status 0

With ls:

    parent: my pid is 3
    child: my pid is 4
    ...
    parent: child 4 ended with status 0

With a non-existing program:

    parent: my pid is 3
    child: my pid is 4
    exec failed
    parent: child 4 ended with status 1

## Part 2 — pingpong

The program uses two pipes:

- p2c: parent to child
- c2p: child to parent

The result was:

    4: received ping
    3: received pong

### Observations

1. Two pipes are needed because one pipe is used for communication from parent to child, while the second pipe is used for communication from child back to parent.

2. The parent waits for the child's reply at:

    read(c2p[0], &b, 1);

The `read()` call blocks until the child writes a byte into the second pipe.

## Conclusion

In this laboratory work I used the same basic process operations inside xv6 that were used in Linux in Lab 7. I used `fork()` to create a child process, `exec()` to replace the child with another program, and `wait()` to wait for its completion.

I also implemented bidirectional communication between a parent and child process using two pipes. The parent sent one byte to the child, the child returned it, and the blocking behavior of `read()` synchronized the communication.
