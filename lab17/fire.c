/* fire.c - four threads share the same counter WITHOUT a lock.
 * Build: gcc -Wall -Wextra -pthread -o fire fire.c
 * Run:   ./fire
 */
#include <stdio.h>
#include <pthread.h>

#define N 1000000
long counter = 0;

void *work(void *arg) {
    long id = (long)arg;
    printf("thread %ld sees counter = %ld\n", id, counter);
    for (int i = 0; i < N; i++)
        counter++;
    return NULL;
}

int main(void) {
    pthread_t threads[4];

    for (long i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, work, (void *)(i + 1));
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("expected: %d\n", 4 * N);
    printf("counter: %ld    (lost updates: the race)\n", counter);
    return 0;
}
