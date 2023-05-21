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
    newBoundedBuffer->doneFlag =0;
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
    newUnBoundedBuffer->doneFlag =0;
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

///constructor of the dispatcher.
void initDispatcher(Dispatcher *dispatcher,UnBoundedBuffer ** boundedBuffersList){
    dispatcher-> BoundedBuffersList = malloc(sizeof (BoundedBuffer *));
    dispatcher->numberOfProducers = 0;
    dispatcher->unBoundedBuffersList = boundedBuffersList;
}

///add a bounded buffer to the dispatcher list.
void addBoundedBufferToDispatcher(Dispatcher * dispatcher ,BoundedBuffer *boundedBuffer){
    dispatcher->numberOfProducers++;
    dispatcher->BoundedBuffersList = realloc(dispatcher->BoundedBuffersList, sizeof (BoundedBuffer *) + 1 * dispatcher->numberOfProducers);
    dispatcher->BoundedBuffersList[dispatcher->numberOfProducers -1] = boundedBuffer;
}

///constructor for the CoEditors (each create unboundedList)
void initCoEditor(CoEditor *coEditor,BoundedBuffer * screenManagerBuffer){
    coEditor->screenManagerBuffer = screenManagerBuffer;
    coEditor->unBoundedBuffer = createUnBoundedBuffer();
}

///constructor of the Screen Manager (create bounded list)
void initScreenManager(ScreenManager * screenManager, int queueSize){
    screenManager->boundedBuffer = createBoundedBuffer(queueSize);
}

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
void screenManagerOperation(ScreenManager *screenManager){
    int finishCounter = 0;
    char * msg;
    while( finishCounter != 3){
        msg = removeBounded(screenManager->boundedBuffer);
        if (!strcmp(msg,FINISH)){
            finishCounter++;
        }
        else{
            printf("%s",msg);
        }
    }
    //free screenManager allocations.
}

///find the appropriate co Editor based on a given string.
int findCoEditor(const char * msg){
    char str[20];
    int wordCounter = 0;
    strcpy(str,msg);
    char* token = strtok(str, " \n");
    while (token != NULL && wordCounter < 2){
        token = strtok(NULL, " \n");
        wordCounter++;
    }
    if(!strcmp(token,SPORTS)){
        return 0;
    }
    if(!strcmp(token,NEWS)){
        return 1;
    }
    if(!strcmp(token,WEATHER)){
        return 2;
    }
    perror("findCoEditor ");
    exit(-1);
}

///function Dispatcher take a news from bounded queue and insert it to an appropriate unBounded queue.
void dispatcherOperation(Dispatcher *dispatcher){
    int finishCounter = 0;
    char * msg;
    int currentBufferIndex = 0;
    BoundedBuffer * currentBuffer;
    while (finishCounter != dispatcher->numberOfProducers){
        currentBuffer = dispatcher->BoundedBuffersList[currentBufferIndex];
        if (!currentBuffer->doneFlag){
            /// in here the dispatcher takes exactly one msg per producer (check if need to change based on round robin).
            msg = removeBounded(currentBuffer);
            if (!strcmp(msg,FINISH)){
                currentBuffer->doneFlag = 1;
            }else{
                insertItemUnBounded(dispatcher->unBoundedBuffersList[findCoEditor(msg)],msg);
            }
        }
        currentBufferIndex = (currentBufferIndex + 1) % dispatcher->numberOfProducers;
    }
    insertUnBounded(dispatcher->unBoundedBuffersList[0],FINISH);
    insertUnBounded(dispatcher->unBoundedBuffersList[1],FINISH);
    insertUnBounded(dispatcher->unBoundedBuffersList[2],FINISH);
    //free dispatcher alocations.

}

///function Co-Editor remove a news from unbounded queue and insert it to the screen manager bounded queue.
void CoEditorOperation(CoEditor *coEditor){
    int finishFlag = 0;
    char * msg;
    while(!finishFlag){
        sleep(1);
        msg = removeUnBounded(coEditor->unBoundedBuffer);
        if(!strcmp(msg,FINISH)){
            insertBounded(coEditor->screenManagerBuffer,FINISH);
            finishFlag = 1;
        }
        else{
            insertBounded(coEditor->screenManagerBuffer,msg);
        }
    }
    // free coEditor aloocations

}

///function ScreenManager get news from his bounded queue and print them to the screen.
void producerOperation(Producer *producer){
    int numberOfProducts = producer->numberOfProducts;
    int typeNumber = 0;
    char *msg;
    int sportNum = 0;
    int newsNum = 0;
    int weatherNum = 0;
    for (int i=0 ;i<producer->numberOfProducts;i++){
        msg = malloc(STRING_LEN*sizeof (char));
        sleep(1);
        typeNumber = rand() % 3;
        if (typeNumber == 0){
            sprintf(msg,"Producer %d %s %d",producer->id,SPORTS,sportNum);
            sportNum++;
        }
        if (typeNumber == 1){
            sprintf(msg,"Producer %d %s %d",producer->id,NEWS,newsNum);
            newsNum++;
        }
        if (typeNumber == 2){
            sprintf(msg,"Producer %d %s %d",producer->id,WEATHER,weatherNum);
            weatherNum++;
        }
        else{
            printf("error");
            return;
        }
        insertBounded(producer->boundedBuffer,msg);
        msg = NULL;
    }
}


int readFile(char * fileName,Producer *** tempList,int  * const coEditorQueueSize,int * producersNum){
    FILE *file;
    int id, numberOfProducts, queueSize;
    int numberOfProducers = 0;

    file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Failed to open\n");
        return -1;
    }
    while (fscanf(file, "%d\n%d\n%d\n",&id,&numberOfProducts,&queueSize) == 3) {
        numberOfProducers++;
    }
    *producersNum = numberOfProducers;
    fseek(file,0,SEEK_SET);
    *tempList = malloc(sizeof (Producer *) * numberOfProducers);

    for (int i=0;i < numberOfProducers;i++) { //check for file here.
        fscanf(file, "%d\n", &id);
        fscanf(file, "%d\n", &numberOfProducts);
        fscanf(file, "%d\n", &queueSize);
        (*tempList)[i]->id = id;
        (*tempList)[i]->numberOfProducts =numberOfProducts;
        (*tempList)[i]->boundedBuffer = createBoundedBuffer(queueSize);
    }
    if (fscanf(file, "%d", coEditorQueueSize) != 1) {
        return -1;
    }
    // Close the file
    fclose(file);

    return 0;
}
int initBoundedBuffersList(BoundedBuffer *** boundedBuffersList,Producer **producersList,int *numberOfProducers){

}
int main(int argc,char * argv[]) {
    char * fileName = "config.txt";
    int coEditorQueueSize,numberOfProducers;
    Producer ** producersList;
    if(readFile(fileName,&producersList,&coEditorQueueSize,&numberOfProducers) == -1){
        printf("error in read file");
        return -1;
    }
    BoundedBuffer ** boundedBuffersList;
    initBoundedBuffersList(&boundedBuffersList,producersList,&numberOfProducers);
    CoEditor coEditor;
    ScreenManager screenManager;
    Dispatcher dispatcher;
    initScreenManager(&screenManager,coEditorQueueSize);
    initCoEditor(&coEditor,screenManager.boundedBuffer);
    initDispatcher(&dispatcher,producersList);


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
