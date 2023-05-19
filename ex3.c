#include "ex3.h"

///constructor of News (id,type,index)

///construct of BoundedBuffer (size) return BoundedBuffer*
BoundedBuffer * createBoundedBuffer(int queueSize){
    BoundedBuffer * newBoundedBuffer = malloc(sizeof(BoundedBuffer));
    newBoundedBuffer->NewsList = malloc(sizeof (char *)*queueSize);
    sem_init(&newBoundedBuffer->empty,0,queueSize);
    sem_init(&newBoundedBuffer->full,0,0);
    sem_init(&newBoundedBuffer->mutex,0,1);
    newBoundedBuffer->firstIndex = 0;
    newBoundedBuffer->lastIndex = 0;
    newBoundedBuffer->size = queueSize;
    return newBoundedBuffer;
}

///construct of UnBoundedBuffer (size) return UnBoundedBuffer*
UnBoundedBuffer * createUnBoundedBuffer(){
    UnBoundedBuffer * newUnBoundedBuffer = malloc(sizeof(UnBoundedBuffer));
    newUnBoundedBuffer->currentSize = 20;
    newUnBoundedBuffer->NewsList = malloc(sizeof (char *) * newUnBoundedBuffer->currentSize);
    sem_init(&newUnBoundedBuffer->full,0,0);
    sem_init(&newUnBoundedBuffer->mutex,0,1);
    newUnBoundedBuffer->firstIndex = 0;
    newUnBoundedBuffer->lastIndex = 0;
    return newUnBoundedBuffer;
}

///constructor of producer (id , numberOfProducts,size) (create a bounded buffer)
Producer *  createProducer(int id, int numberOfProducts, int queueSize){
    Producer * newProducer = malloc(sizeof (Producer));
    newProducer->boundedBuffer = createBoundedBuffer(queueSize);
    newProducer->id = id;
    newProducer->numberOfProducts = numberOfProducts;
    return newProducer;
}

///constructor of the dispatcher. (probably will not needed cause i maje it on the stack.)

///constructor for the CoEditors (each create unboundedList)
void initCoEditor(CoEditor *coEditor,BoundedBuffer * screenManagerBuffer){
    coEditor->screenManagerBuffer = screenManagerBuffer;
    coEditor->unBoundedBuffer = createUnBoundedBuffer();
}

///constructor of the Screen Manager (create bounded list)
void initScreenManager(ScreenManager * screenManager, int queueSize){
    screenManager->boundedBuffer = createBoundedBuffer(queueSize);
}

/// function - add a bounded buffer to the Dispatcher Unbounded Buffer queue.(probably will not needed cause i add them on the main.)

///insertItem to a bounded buffer (critical section)
void insertItemBounded(BoundedBuffer * boundedBuffer,char *news){
    boundedBuffer->lastIndex =  (boundedBuffer->lastIndex + 1) % boundedBuffer->size;
    boundedBuffer->NewsList[boundedBuffer->lastIndex] = news;
}

///function - add a news to a bounded buffer.
void insertBounded(BoundedBuffer * boundedBuffer,char * news){
    sem_wait(&boundedBuffer->empty);
    sem_wait(&boundedBuffer->mutex);
    insertItemBounded(boundedBuffer,news);
    sem_post(&boundedBuffer->mutex);
    sem_post(&boundedBuffer->full);

}

/// remove an item from the buffer (critical section)
char * removeItemBounded(BoundedBuffer * boundedBuffer){
    char * itemPtr = boundedBuffer->NewsList[boundedBuffer->firstIndex];
    boundedBuffer->NewsList[boundedBuffer->firstIndex] =NULL;
    boundedBuffer->firstIndex  = (boundedBuffer->firstIndex + 1) % boundedBuffer->size;
    return itemPtr;

}
///function - remove a news from a bounded buffer.
char * removeBounded(BoundedBuffer * boundedBuffer){
    char * itemPtr = NULL;
    sem_wait(&boundedBuffer->full);
    sem_wait(&boundedBuffer->mutex);
    itemPtr = removeItemBounded(boundedBuffer);
    sem_post(&boundedBuffer->mutex);
    sem_post(&boundedBuffer->empty);
    return itemPtr;
}

///increase the newsList of unBounded buffer
void increaseUnBoundedSize(UnBoundedBuffer *unBoundedBuffer){
    unBoundedBuffer->currentSize *= 2;
    char ** newBuffer = malloc(sizeof (char *) * unBoundedBuffer->currentSize);
    int currentIndex = 0;
    while (unBoundedBuffer->firstIndex != unBoundedBuffer->lastIndex){
        newBuffer[currentIndex] = unBoundedBuffer->NewsList[unBoundedBuffer->firstIndex];
        currentIndex ++;
        unBoundedBuffer->firstIndex++;
    }
    unBoundedBuffer->lastIndex = currentIndex - 1;
    unBoundedBuffer->firstIndex = 0;
    free(unBoundedBuffer->NewsList);
    unBoundedBuffer->NewsList =newBuffer;
}

/// insert an item from the buffer (critical section)
void insertItemUnBounded(UnBoundedBuffer * unBoundedBuffer,char *news){
    if ((unBoundedBuffer->lastIndex + 1) % unBoundedBuffer->currentSize == unBoundedBuffer->firstIndex % unBoundedBuffer->currentSize){
        increaseUnBoundedSize(unBoundedBuffer);
    }
    unBoundedBuffer->lastIndex =  (unBoundedBuffer->lastIndex + 1) % unBoundedBuffer->currentSize;
    unBoundedBuffer->NewsList[unBoundedBuffer->lastIndex] = news;
}

///function - add a news to a unBounded buffer.
void insertUnBounded(UnBoundedBuffer * unBoundedBuffer,char * news){
    sem_wait(&unBoundedBuffer->mutex);
    insertItemUnBounded(unBoundedBuffer,news);
    sem_post(&unBoundedBuffer->mutex);
    sem_post(&unBoundedBuffer->full);

}

///remove an item from the buffer (critical section).
char * removeItemUnBounded(UnBoundedBuffer * unBoundedBuffer){
    char * itemPtr = unBoundedBuffer->NewsList[unBoundedBuffer->firstIndex];
    unBoundedBuffer->NewsList[unBoundedBuffer->firstIndex] =NULL;
    unBoundedBuffer->firstIndex  = (unBoundedBuffer->firstIndex + 1) % unBoundedBuffer->currentSize;
    return itemPtr;
}

///function - remove a news from a unBounded buffer.
char * removeUnBounded(UnBoundedBuffer * unBoundedBuffer){
    char * itemPtr = NULL;
    sem_wait(&unBoundedBuffer->full);
    sem_wait(&unBoundedBuffer->mutex);
    itemPtr = removeItemUnBounded(unBoundedBuffer);
    sem_post(&unBoundedBuffer->mutex);
    return itemPtr;
}

///function for the screen manager that takes a News from his queue and print it to the screen.

///function Dispatcher take a news from bounded queue and insert it to an appropriate unBounded queue.

///function Dispatcher functionality read from each bounded queue in his queue list using Round robin until all of them are empty.

///function Co-Editor remove a news from unbounded queue and insert it to the screen manager bounded queue (-1 when Done).

///function ScreenManager get news from his bounded queue and print them to the screen.


int main(int argc,char * argv[]) {
    ///open the conf file in argv[1](check open successfully)
    ///iterate over the file lines for each line create (except the last) create a producer.
    ///create three coEditors.
    ///create the dispatcher.
    ///create the screen manager.
    ///attached the producer queues and the co Editors queues to the dispatcher.
    ///attached the 3 coEditors to the screen managerUnboundedList.
    ///activate all the producers( each in separate thread)
    ///activate the dispatcher (in a thread)
    ///activate the three co Editors.( in a thread)
    ///activate the Screen Manager. (in a thread)
    ///wait for the Screen manger thread to finish.

    return 0;
}
