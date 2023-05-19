#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct {
    int id;
    int type;
    int index;
} News;

typedef struct {
    int capacity;
    sem_t mutex;
    sem_t empty;
    sem_t full;
    News * NewsList;
} BoundedBuffer;

typedef struct {
    int Capacity;
    sem_t mutex;
    sem_t full;
    News * NewsList;
}UnBoundedBuffer;

typedef struct{
    BoundedBuffer * boundedBuffer;
    int id;
    int numberOfProducts;
}Producer;

typedef struct {
    BoundedBuffer ** BoundedBuffersList;
    UnBoundedBuffer * unBoundedBufferList[3];

}Dispatcher;

typedef struct {
    UnBoundedBuffer * unBoundedBuffer;
    BoundedBuffer * screenManagerBuffer;
}CoEditors;

typedef struct {
    BoundedBuffer boundedBuffer;
}ScreenManager;

///constructor of News (id,type,index)

///constructor of producer (id , numberOfProducts,size)

///construct of BoundedBuffer (size) return BoundedBuffer*

///construct of UnBoundedBuffer (size) return UnBoundedBuffer*

///constructor of the dispatcher , init his 3 queues with the co-editors.

///constructor of the Screen Manager;

/// function - add a bounded buffer to the Dispatcher Unbounded Buffer queue.

///function - add a news to a bounded buffer.

///function - remove a news from a bounded buffer.

///function - add a news to a unBounded buffer.

///function - remove a news from a unBounded buffer.

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


    return 0;
}
