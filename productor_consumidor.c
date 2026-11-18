#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10

int buffer;
sem_t vacio, lleno;

void *productor(void *arg) {
    int id = *(int *)arg;

    for (int i = 1; i <= N; i++) {
        int dato = id * 100 + i;
        usleep(rand() % 50000);

        sem_wait(&vacio);
        buffer = dato;
        printf("Productor %d puso %d\n", id, dato);
        sem_post(&lleno);
    }
    return NULL;
}

void *consumidor(void *arg) {
    int id = *(int *)arg;

    for (int i = 1; i <= N; i++) {
        sem_wait(&lleno);
        int dato = buffer;
        printf("    Consumidor %d tomo %d\n", id, dato);
        sem_post(&vacio);

        usleep(rand() % 50000);
    }
    return NULL;
}

int main() {
    pthread_t p[2], c[2];
    int ids[2] = {1, 2};

    sem_init(&vacio, 0, 1);
    sem_init(&lleno, 0, 0);

    for (int i = 0; i < 2; i++) {
        pthread_create(&p[i], NULL, productor, &ids[i]);
        pthread_create(&c[i], NULL, consumidor, &ids[i]);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(p[i], NULL);
        pthread_join(c[i], NULL);
    }

    sem_destroy(&vacio);
    sem_destroy(&lleno);
    printf("\nTermino el programa\n");
    return 0;
}
