#ifndef OP_ASS3_EX3_H
#define OP_ASS3_EX3_H
///yarin sason 318229143

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define CO_EDITOR_NUM 3
#define STRING_LEN 20
#define FINISH "DONE"
#define SPORTS "SPORTS"
#define NEWS "NEWS"
#define WEATHER "WEATHER"

typedef struct {
    int doneFlag;
    int firstIndex;
    int lastIndex;
    int size;
    sem_t mutex;
    sem_t empty;
    sem_t full;
    char ** NewsList;
} BoundedBuffer;

typedef struct {
    int doneFlag;
    int firstIndex;
    int lastIndex;
    int currentSize;
    sem_t mutex;
    sem_t full;
    char ** NewsList;
}UnBoundedBuffer;

typedef struct{
    BoundedBuffer * boundedBuffer;
    int id;
    int numberOfProducts;
}Producer;

typedef struct {
    BoundedBuffer ** BoundedBuffersList;
    int numberOfProducers;
    UnBoundedBuffer ** unBoundedBuffersList;
}Dispatcher;

typedef struct {
    UnBoundedBuffer * unBoundedBuffer;
    BoundedBuffer * screenManagerBuffer;
}CoEditor;

typedef struct {
    BoundedBuffer *boundedBuffer;
}ScreenManager;

#endif //OP_ASS3_EX3_H
