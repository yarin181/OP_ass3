#ifndef OP_ASS3_EX3_H
#define OP_ASS3_EX3_H

#define CO_EDITOR_NUM 3
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
//typedef struct {
//    int id;
//    int type;
//    int index;
//} News;
typedef struct {
    int firstIndex;
    int lastIndex;
    int size;
    sem_t mutex;
    sem_t empty;
    sem_t full;
    char ** NewsList;
} BoundedBuffer;

typedef struct {
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
    UnBoundedBuffer * unBoundedBufferList[CO_EDITOR_NUM];

}Dispatcher;

typedef struct {
    UnBoundedBuffer * unBoundedBuffer;
    BoundedBuffer * screenManagerBuffer;
}CoEditor;

typedef struct {
    BoundedBuffer *boundedBuffer;
}ScreenManager;











#endif //OP_ASS3_EX3_H
