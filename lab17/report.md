# Operating Systems - Laboratory Work No. 17
## Threads Share Memory, and Race
**Track A:** The real OS, from the outside (Linux)

**Student:** Ermicev Sofia, FAF-241  
**Environment:** Ubuntu on WSL2  
**Compiler command:** `gcc -Wall -Wextra -pthread -o fire fire.c`

## Part 1 - Shared and racing (2 threads)

The program starts two POSIX threads. Each thread increments the same global `counter` variable `N = 1,000,000` times without a lock. The expected final value is `2,000,000`.

### Observed runs

| Run | Thread 1 initially saw | Thread 2 initially saw | Expected | Actual | Lost increments |
| --- | ---: | ---: | ---: | ---: | ---: |
| 1 | 0 | 170479 | 2000000 | 1667671 | 332329 |
| 2 | 0 | 0 | 2000000 | 1161983 | 838017 |
| 3 | 0 | 17756 | 2000000 | 1128104 | 871896 |

### Observe 1 - Thread 2 prints a non-zero counter at the start. What does that prove about the memory of the two threads?

It shows that threads of the same process access a shared address space. Thread 2 can observe changes made to the global `counter` by Thread 1, rather than having an independent copy of the variable. The exact value read depends on the thread interleaving.

### Observe 2 - Why is the final counter below 2,000,000 and different on each run? Explain the three steps of `counter++`.

`counter++` is a read-modify-write operation: (1) read the old value, (2) add one, (3) write the result back. Both threads can read the same old value before either writes its new value. Their writes then overwrite one another, so an increment is lost. The scheduling and overlap of the threads differ across runs, producing different final counts. Technically, an unsynchronized conflicting access to a non-atomic variable is a C data race, so the program has undefined behavior.

## Part 2 - Make it worse: four threads

I changed `main()` to start four threads, each incrementing the same shared counter `1,000,000` times. The expected result is now `4,000,000`.

### Observed runs

| Run | T1 initially saw | T2 initially saw | T3 initially saw | T4 initially saw | Expected | Actual | Lost increments |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| 1 | 0 | 8442 | 60430 | 80136 | 4000000 | 1547086 | 2452914 |
| 2 | 0 | 280820 | 344838 | 407074 | 4000000 | 1718549 | 2281451 |
| 3 | 0 | 31221 | 111505 | 142414 | 4000000 | 1857731 | 2142269 |

With two threads, the average observed fraction of lost increments was approximately **34.0%**; with four threads, it was approximately **57.3%**. In these measurements, increasing the number of threads increased the proportion of lost updates because more threads competed to modify the same variable without synchronization.

### Observe 3 - In one sentence, what single idea would stop lost updates?

Protect the shared `counter++` operation with a **mutex**, so only one thread can execute that critical section at a time.

For example, `pthread_mutex_lock(&lock); counter++; pthread_mutex_unlock(&lock);` (with a properly initialized mutex). This is a proposed solution, not implemented in `fire.c`, as required for this lab.

## Conclusion

The experiments demonstrate that POSIX threads within one process share memory and can interfere with one another. Without synchronization, concurrent updates to the same variable cause a data race and unreliable results. Four threads produced more lost updates proportionally in the observed runs. A mutex or an atomic increment can make the counter update safe.
