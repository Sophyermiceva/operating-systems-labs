# OS Lab 12 — Producer, Consumer and the Other Tools

**Student:** Ermicev Sofia, FAF-241  
**Environment:** Ubuntu (WSL2), GCC 13.3.0; compiled with `gcc -Wall -pthread`  
**Note:** All outputs below are the actual observations from the terminal.

## Part 1 — Producer and consumer

The correct `prodcons.c` kept occupancy between 0 and 4, produced items 1–8, and consumed all eight in order. It ended with `take 8 (in buffer: 0)`.

**Observe 1:** `sem_wait(&empty)` blocks the producer when all four slots are occupied.  
**Observe 2:** The consumer initially waits at `sem_wait(&full)` because `full` starts at 0.  
**Observe 3:** With the mutex locked before `sem_wait(&empty)` in `prodcons_wrong.c`, the producer can hold the mutex while waiting for an empty slot. The consumer needs that same mutex to remove an item; therefore both threads wait indefinitely. Actual result: after `put 5 (in buffer: 4)`, `timeout 5` returned exit code **124**.

## Part 2 — Condition variable

Both `condvar.c` (with `while`) and `condvar_if.c` (with `if`) printed:

```text
B: not ready, I sleep
A: message written
B: I read "hello"
```

**Observe 1:** `ready` remembers whether the message has been written.  
**Observe 2:** Yes, `if` worked in this particular run.  
**Observe 3:** A condition wait may return spuriously, or another thread may invalidate the predicate before the waiting thread reacquires the mutex. `while` rechecks `ready` and is therefore necessary for general correctness.

## Part 3 — Readers and writers

Actual output:

```text
writer 1 writes 10 (alone)
reader 1 reads 10     (readers inside: 1)
reader 2 reads 10     (readers inside: 2)
reader 3 reads 10     (readers inside: 3)
writer 2 writes 20 (alone)
```

**Observe 1:** `if (++readers == 1) sem_wait(&room);` excludes writers when the first reader enters, and `if (--readers == 0) sem_post(&room);` admits writers when the last reader leaves.  
**Observe 2:** `rlock` protects updates of the shared reader count; without it, increments or decrements might be lost, making access rules unreliable.  
**Observe 3:** Continually arriving readers can postpone a writer indefinitely (writer starvation).

## Part 4 — Deadlock and its correction

Original program with opposite mutex order and `sleep(1)` in both threads:

```text
one: I have A, I want B
two: I have B, I want A
exit code: 124
```

Removing both sleeps from the **original** program and running ten trials produced these exit codes:

| Run | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
|---|---|---|---|---|---|---|---|---|---|---|
| Exit code | 124 | 0 | 0 | 0 | 0 | 0 | 124 | 0 | 0 | 0 |

Thus **2/10 runs deadlocked** and **8/10 completed**.

After fixing `deadlock.c` to acquire locks in the same A → B order, the actual output was:

```text
one: I have A, I want B
one: I have both
two: I have A, I want B
two: I have both
done
exit code: 0
```

**Observe 1:** Thread one holds A while waiting for B; thread two holds B while waiting for A. The cycle is `T1 → B → T2 → A → T1`.  
**Observe 2:** Without the sleeps, the program did not always hang (2/10 runs did). This makes the bug more difficult to reproduce and debug, not less serious.  
**Observe 3:** A consistent lock order A → B rules out circular waiting between these mutexes; a thread never holds B while attempting to acquire A.

## Conclusion

Semaphore counts enforce bounded-buffer capacity; mutexes protect shared state; condition variables wait for predicates to change. Incorrect ordering causes deadlock or starvation. A consistent mutex acquisition order eliminated the demonstrated deadlock.
