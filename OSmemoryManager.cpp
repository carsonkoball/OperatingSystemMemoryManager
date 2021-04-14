#include <iostream>
#include <fstream>

using namespace std;
//deconstructor
//check output

// HELPER CODE:

/*
    Author: Alex Wollman
    Class: CSC300 - Data Structures
    Description:
        Structure to store data lines read from the input file.
*/
struct ioStruct{
    int commandNumber;
    int memorySize;
    int processNumber;
    ioStruct *next;
};

/*
    Author: Alex Wollman
    Class: CSC300 - Data Structures
    Description:
        Deletes the ioStruct data 
*/
void deleteData(ioStruct *data){
    
    ioStruct *travel = data;

    while( NULL != data ){
        data = data->next;
        delete travel;
        travel = data;
    }
}


/*
    Author: Alex Wollman
    Class: CSC300 - Data Structures
    Description:
        Populates the variable 'myStruct' with contents from the file.
        This function does not detect malformed lines from the file.
    
*/
void populateStructure(ifstream &input, ioStruct *myStruct){
    
    char garbage;
    
    input >> myStruct->commandNumber;
    input >> garbage;
    input >> myStruct->memorySize;
    input >> garbage;
    input >> myStruct->processNumber;
    myStruct->next = NULL;

    return;
}

/*
    Author: Alex Wollman
    Class: CSC300 - Data Structures
    Description:
        Prints the contents of the linked list data read from the file to the screen.
*/
void printData( ioStruct *node ){
    
    while( node != NULL){
        cout<<"command: "<<node->commandNumber<<endl;
        cout<<"memory size: "<<node->memorySize<<endl;
        cout<<"process number: "<<node->processNumber<<endl;
        node = node->next;
    }

    return;
}


/*
    Author: Alex Wollman
    Class: CSC300 - Data Structures
    Description:
        Reads in the contents of the input file and stores them in a linked list
        for processing.
*/
ioStruct* readFromFile(const string filename){

    ifstream input;
    ioStruct *head = NULL;;
    ioStruct *travel = NULL;
    ioStruct *prev = NULL;

    input.open(filename);

    if( input.is_open() ){
        if( EOF != input.peek() ){
            try{
                travel = new ioStruct;
                head = travel;
                prev = travel;
                populateStructure(input, travel);
                while( !input.eof() ){
                    travel = travel->next;
                    travel = new ioStruct;
                    populateStructure(input, travel);
                    prev->next = travel;
                    prev = travel;
                }
            } catch( exception *e){
                cout<<"ERROR: "<<e->what()<<endl;
            }
        } else {
            cout<<"ERROR: File is empty."<<endl;
        }
    } else {
        cout<<"ERROR: Couldn't open file: "<<filename<<". Please make sure it is in your local directory."<<endl;
    }

    return head;

}

// ASSIGNMENT CODE:

struct node {
    // 0 = unallocated, 1 = allocated, 2 = deleted and allocated
    int state;
    int processID;
    int startingValue;
    int endingValue;
    node * next;    
};

class MemoryManager {
    public:
        MemoryManager();
        int allMemoryUsage(void);
        int cleanMemory(void);
        int contractNode(int, int);
        int createNode(int, int);
        int deleteNode(int);
        int expandNode(int, int);
        int processMemoryUsage(int);
        void printList(void);

    private:
    //variables
        node * head;
    //functions
        int p_allMemoryUsage(void);
        int p_cleanMemory(void);
        int p_contractNode(int, int);
        int p_createNode(int, int);
        int p_deleteNode(int);
        int p_expandNode(int, int);
        int p_processMemoryUsage(int);
        int p_memoryOfProcess(int);
};

/*----------------------------------------------PUBLIC: MemoryManager-----------------------------------------------*/
MemoryManager::MemoryManager(void) { // creates unallocated memory pool of 1000 blocks of memory (0-999)
    node * temp = new node;
    temp->state = 0;
    temp->startingValue = 0;
    temp->endingValue = 999;
    temp->next = NULL;
    this->head = temp;
}

/*----------------------------------------------PUBLIC: allMemoryUsage----------------------------------------------*/
int MemoryManager::allMemoryUsage(void) {
    int returnValue = this->p_allMemoryUsage();

    if (returnValue == 0) {
        //cout<<"Memory usage report successfully printed out..."<<endl;
    } else if (returnValue == 1) {
        cout<<"ERROR: Memory usage report unable to be printed out..."<<endl;
    }

    return returnValue;
}

/*----------------------------------------------PRIVATE: p_allMemoryUsage-------------------------------------------*/
int MemoryManager::p_allMemoryUsage(void) { // prints out memory information about all processes that exist in memory
    // 0 = report successfully made and printed, 1 = report not created
    int reportStatus = 1;
    int highestProcess = 0;

    node * travel = this->head;
    cout<<endl<<"MEMORY USAGE REPORT FOR ALL PROCESSES: "<<endl<<endl<<endl;

    while (travel != NULL) {
        if (travel->processID > highestProcess) {
            highestProcess = travel->processID;
        }
        travel = travel->next;
    }

    for (int i = 0; i <= highestProcess; i++) {
        p_processMemoryUsage(i);
        reportStatus = 0;
    }

    return reportStatus;
}

/*----------------------------------------------PUBLIC: cleanMemory-------------------------------------------------*/
int MemoryManager::cleanMemory(void) {
    int returnValue = this->p_cleanMemory();

    if (returnValue == 0) {
        //cout<<"Memory pool cleaned successfully..."<<endl;
    } else if (returnValue == 1) {
        cout<<"ERROR: No memory marked for deletion - nothing to clean..."<<endl;
    }

    return returnValue;
}

/*----------------------------------------------PRIVATE: p_cleanMemory----------------------------------------------*/
int MemoryManager::p_cleanMemory(void) { // moves all processes marked for deletion to allocated memory and combines like chunks of memory together that are the same process/state
    // 1 = no memory cleaned, 0 = any memory cleaned
    int cleanStatus = 1;

    node * travel = this->head;
    node * nodeToDelete;

    while (travel != NULL) { // moves all processes marked for deletion to allocated memory pool
        if (travel->state == 2) {
            travel->state = 0;
            travel->processID = 0;
            cleanStatus = 0;
        } 
        travel = travel->next;
    }

    travel = this->head;
    if (travel->state == travel->next->state && travel->processID == travel->next->processID) { // if node n and node n+1 share the same state and process id, combine them
        travel->next->startingValue = travel->startingValue;
        nodeToDelete = travel;
        head = travel->next;
        delete(nodeToDelete);
    }
    else {
        nodeToDelete = travel->next;
        while (nodeToDelete != NULL) {
            if (travel->state == nodeToDelete->state && travel->processID == nodeToDelete->processID) {
                travel->endingValue = nodeToDelete->endingValue;
                travel->next = nodeToDelete->next;
                delete(nodeToDelete);
                nodeToDelete = travel;
                nodeToDelete = nodeToDelete->next;
            } else {
                nodeToDelete = nodeToDelete->next;
                travel = travel->next;
            }
        }
    }

    return cleanStatus;
}

/*----------------------------------------------PUBLIC: contractNode------------------------------------------------*/
int MemoryManager::contractNode(int processIDInput, int size) {
    int returnValue = this->p_contractNode(processIDInput, size);

    if (returnValue == 0) {
        //cout<<"Successfully reduced size of process with ID of "<<processIDInput<<" by "<<size<<" blocks of memory..."<<endl;
    } else if (returnValue == 1) {
        cout<<"ERROR: Process with ID of "<<processIDInput<<" not found - nothing to contract..."<<endl;
    } else if (returnValue == 2) {
        cout<<"ERROR: Not enough memory in process "<<processIDInput<<" to contract it by "<<size<<" blocks..."<<endl;
    }

    return returnValue;
}

/*----------------------------------------------PRIVATE: p_contractNode---------------------------------------------*/
int MemoryManager::p_contractNode(int processIDInput, int size) { // shrinks existing process by given memory size
    // 0 = process successfully contracted, 1 = process not found, 2 = process not large enough to contract by x blocks
    int contractStatus = 2;
    // 0 = process with ID of processIDInput does not exist, 1 = process with ID of processIDInput exists
    int processExists = 0;
    int processMemory = this->p_memoryOfProcess(processIDInput);

    node * travel = this->head;

    while (travel != NULL) { // this checks to see if the process already exists in memory
        if (travel->processID == processIDInput && travel->state == 1) {
            processExists = 1;
        }
        travel = travel->next;
    }

    travel = this->head;
    node * nodeToContract = travel;

    if (processExists == 1 && processMemory >= size) { // if the process exist and has enough memory to be contracted
        while (size != 0) {
            while (travel != NULL) { // finds last allocated node of process
                if (travel->processID == processIDInput && travel->state == 1) {
                    nodeToContract = travel;
                }
                travel = travel->next;
            }
            if (size >= (nodeToContract->endingValue - nodeToContract->startingValue + 1)) { // if the node has less memory or just as much memory as needed
                nodeToContract->state = 2;
                size = size - (nodeToContract->endingValue - nodeToContract->startingValue + 1);
                processMemory = processMemory - size;
            } else { // if the node has more memory than what we want to shrink it by
                node * temp = new node;
                temp->state = 2;
                temp->processID = processIDInput;
                temp->startingValue = nodeToContract->endingValue - size + 1;
                temp->endingValue = nodeToContract->endingValue;
                nodeToContract->endingValue = nodeToContract->endingValue - size;
                temp->next = nodeToContract->next;
                nodeToContract->next = temp;                    
                size = 0;
                processMemory = processMemory - size;
            }
            travel = this->head;
            nodeToContract = travel;
        }
        contractStatus = 0;
    }

    return contractStatus;
}

/*----------------------------------------------PUBLIC: createNode--------------------------------------------------*/
int MemoryManager::createNode(int processIDInput, int size) { 
    int returnValue = this->p_createNode(processIDInput, size);

    if (returnValue == 0) {
        //cout<<"Process with ID of "<<processIDInput<<" and size of "<<size<<" blocks of created in memory successfully..."<<endl;
    } else if (returnValue == 1) {
        cout<<"ERROR: Insufficient available memory in memory pool to add process of size " <<size<<" blocks..."<<endl;;
    } else if (returnValue == 2) {
        cout<<"ERROR: A process with an ID of "<<processIDInput<<" already exists in memory..."<<endl;;
    }

    return returnValue;
}

/*----------------------------------------------PRIVATE: p_createNode-----------------------------------------------*/
int MemoryManager::p_createNode(int processIDInput, int size) { // creates a process in memory that doesn't already exist in memory
    // 0 = node successfully created, 1 = no room in memory to create node, 2 = processID already exists in memory
    int createStatus = 1;
    // 0 = processID is unique to memory, 1 = processID is already in memory
    int IDExists = 0;
    int availableMemory = this->p_memoryOfProcess(0);
    
    node * travel = this->head;
    while (travel != NULL) { // checks to see if the process exists in memory
        if (travel->processID == processIDInput) {
            IDExists = 1;
            createStatus = 2;
        }
        travel = travel->next;
    }
    
    if (availableMemory >= size && IDExists == 0) { // if there's enough unallocated memory and the process exists
        travel = this->head;
        node * oneBehind = travel;
        if (travel->endingValue - travel->startingValue + 1 > size && travel->state == 0) { // if the first node is available and has more unallocated memory than needed
            node * temp = new node;
            temp->state = 1;
            temp->processID = processIDInput;
            temp->startingValue = travel->startingValue;
            temp->endingValue = (travel->startingValue) + size - 1;
            travel->startingValue = temp->endingValue + 1;
            temp->next = travel;
            this->head = temp;
            createStatus = 0;
            size = 0;
        } else if (travel->endingValue - travel->startingValue  + 1 == size && travel->state == 0) { // if the first node is avilable and has as much memory as needed
            travel->state = 1;
            travel->processID = processIDInput;
            createStatus = 0;
            size = 0;
        } else if (travel->endingValue - travel->startingValue + 1 < size && travel->state == 0) { // if the first node is available and has less unallocated memory than needed
            travel->state = 1;
            travel->processID = processIDInput;
            createStatus = 0;
            createStatus = 0;
            size = size - (travel->endingValue - travel->startingValue);
        }
        
        if (size != 0) {
            travel = travel->next;
            while (size != 0) {
                if (travel->endingValue - travel->startingValue + 1 > size && travel->state == 0) { // if the nth node is available and has more memory than needed
                    node * temp = new node;
                    temp->state = 1;
                    temp->processID = processIDInput;
                    temp->startingValue = travel->startingValue;
                    temp->endingValue = (travel->startingValue) + size - 1;
                    travel->startingValue = temp->endingValue + 1;
                    temp->next = travel;
                    oneBehind->next = temp;
                    createStatus = 0;
                    size = 0;
                } else if (travel->endingValue - travel->startingValue  + 1 == size && travel->state == 0) { // if the nth node is available and has as much memory as needed
                    travel->state = 1;
                    travel->processID = processIDInput;
                    createStatus = 0;
                    size = 0;
                } else if (travel->endingValue - travel->startingValue + 1 < size && travel->state == 0) { // if the nth node is available and has less memory than needed
                    travel->state = 1;
                    travel->processID = processIDInput;
                    createStatus = 0;
                    size = size - (travel->endingValue - travel->startingValue + 1);
                }
                travel = travel->next;
                oneBehind = oneBehind->next;
            }
        }
    }
    
    return createStatus;
}

/*----------------------------------------------PUBLIC: deleteNode--------------------------------------------------*/
int MemoryManager::deleteNode(int processIDInput) {
    int returnValue = this->p_deleteNode(processIDInput);

    if (returnValue == 0) {
        //cout<<"Process with ID of "<<processIDInput<<" deleted from memory successfully..."<<endl;
    } else if (returnValue == 1) {
        cout<<"ERROR: Process with ID of "<<processIDInput<<" not found in memory - nothing to delete..."<<endl;
    }

    return returnValue;
}

/*----------------------------------------------PRIVATE: p_deleteNode-----------------------------------------------*/
int MemoryManager::p_deleteNode(int processIDInput) { // marks process as deleted
    // 0 = node marked for deletion successfully, 1 = processID not found in memory
    int deleteStatus = 1;

    node * travel = this->head;
    
    while (travel != NULL) {
        if (travel->processID == processIDInput) {
            travel->state = 2;
            deleteStatus = 0;
        }
        travel = travel->next;
    }

    return deleteStatus;
}

/*----------------------------------------------PUBLIC: expandNode--------------------------------------------------*/
int MemoryManager::expandNode(int processIDInput, int size) {
    int returnValue = this->p_expandNode(processIDInput, size);

    if (returnValue == 0) {
        //cout<<"Successfully increased size of process with ID of "<<processIDInput<<" by "<<size<<" blocks of memory..."<<endl;
    } else if (returnValue == 1) {
        cout<<"ERROR: Process with ID of "<<processIDInput<<" not found - nothing to expand..."<<endl;
    } else if (returnValue == 2) {
        cout<<"ERROR: Insufficient unallocated memory left - cannot expand process with ID of "<<processIDInput<<" by "<<size<<" blocks of memory..."<<endl;
    }

    return returnValue;
}

/*----------------------------------------------PRIVATE: p_expandNode-----------------------------------------------*/
int MemoryManager::p_expandNode(int processIDInput, int size) { // expands existing process by a given memory size
    // 0 = process successfully contracted, 1 = process not found,  2 = insufficient unallocated memory left
    int expandStatus = 1;
    // 0 = process with ID of processIDInput does not exist, 1 = process with ID of processIDInput exists
    int processExists = 0;
    node * travel = this->head;

    while (travel != NULL) { // this confirms if the process exists in memory
        if (travel->processID == processIDInput) {
            processExists = 1;
        }
        travel = travel->next;
    }

    travel = this->head;

    if (processExists == 1) {
        expandStatus = 2;
        if (travel->processID == 0 && (travel->endingValue - travel->startingValue - 1 > size)) { // if the first node is available and larger than needed
            node * temp = new node;
            temp->state = 1;
            temp->startingValue = travel->startingValue;
            temp->endingValue = travel->startingValue + size - 1;
            temp->processID = processIDInput;
            travel->startingValue = temp->endingValue + 1;
            temp->next = travel;
            this->head = temp;
            expandStatus = 0;
        } else if (travel->processID == 0 && (travel->endingValue - travel->startingValue - 1 == size)) { // if the first node is available and just as much as needed
            travel->processID = processIDInput;
            travel->state = 1;
            expandStatus = 0;
        } else {
            node * oneBehind = travel;
            travel = travel->next;
            while (travel != NULL) {
                if (travel->processID == 0 && (travel->endingValue - travel->startingValue - 1 > size)) { // if the nth node is available and larger than needed
                    node * temp = new node;
                    temp->state = 1;
                    temp->processID = processIDInput;
                    temp->startingValue = travel->startingValue;
                    temp->endingValue = travel->startingValue + size - 1;
                    travel->startingValue = temp->endingValue + 1;
                    temp->next = travel;
                    oneBehind->next = temp;
                    expandStatus = 0;
                    break;
                } else if (travel->processID == 0 && (travel->endingValue - travel->startingValue - 1 == size)) { // if the nth node is available and just as much as needed
                    travel->processID = processIDInput;
                    travel->state = 1;
                    expandStatus = 0;
                    break;
                }
                travel = travel->next;
                oneBehind = oneBehind->next;
            }
        }
    }

    return expandStatus;
}

/*----------------------------------------------PRIVATE: p_memoryOfProcess------------------------------------------*/
int MemoryManager::p_memoryOfProcess(int processIDInput) { // finds total memory of a process (private only)
    node * travel = this->head;
    int totalMemory = 0;

    if (processIDInput == 0) { // unallocated memory
        while (travel != NULL) {
            if (travel->processID == 0 && travel->state == 0) {
                totalMemory += travel->endingValue - travel->startingValue + 1;
            }
            travel = travel->next;
        }
    } else { // allocated memory 
        while (travel != NULL) {
            if (travel->processID == processIDInput && travel->state == 1) {
                totalMemory += travel->endingValue - travel->startingValue + 1;
            }
            travel = travel->next;
        }
    }    

    return totalMemory;
}

/*----------------------------------------------PUBLIC: processMemoryUsage------------------------------------------*/
int MemoryManager::processMemoryUsage(int processIDInput) {
    int returnValue = this->p_processMemoryUsage(processIDInput);

    if (returnValue == 0) {
        //cout<<"Memory usage report for process with an ID of "<<processIDInput<<" successfully printed out..."<<endl;
    } else if (returnValue == 1) {
        cout<<"ERROR: Process with ID of "<<processIDInput<<" not found - nothing to print..."<<endl;
    } 

    return returnValue;
}

/*----------------------------------------------PRIVATE: p_processMemoryUsage---------------------------------------*/
int MemoryManager::p_processMemoryUsage(int processIDInput) { // prints out addresses and total memory of a process
    
    // 1 = process information successfully printed out, 2 = process not found
    int reportStatus = 1;
    // 0 = process doesn't exist in memory, 1 = process exists in memory
    int processExists = 0;
    int memoryBlock = 0;
    int totalMemory = this->p_memoryOfProcess(processIDInput);

    node * travel = this->head;

    while (travel != NULL) {
        if (travel->processID == processIDInput) {
            processExists = 1;
        }
        travel = travel->next;
    }

    travel = this->head;

    if (processExists == 1) {
        if (processIDInput == 0) {
            cout<<"  MEMORY USAGE REPORT FOR UNALLOCATED MEMORY: "<<endl;
        } else {
            cout<<"  MEMORY USAGE REPORT FOR PROCESS "<<processIDInput<<": "<<endl;
        }

        while (travel != NULL) {
            if (travel->processID == processIDInput && travel->state == 1) {
                memoryBlock++;
                cout<<"    Memory Chunk "<<memoryBlock<<" : Starting Address Block: "<<travel->startingValue<<" Ending Address Block: "<<travel->endingValue<<endl;
                reportStatus = 0;
            }
            travel = travel->next;
       }
       cout<<"    Total Memory Used: "<<totalMemory<<" blocks"<<endl<<endl;
    }

    return reportStatus;
}

void MemoryManager::printList(void) {
    node * travel = this->head;
    cout<<"HEAD->";
    while (travel != NULL) {
        cout<<"(ID: "<<travel->processID<<"S: "<<travel->state<<"["<<travel->startingValue<<" : "<<travel->endingValue<<"] ->";
        travel = travel->next;
    }
    cout<<"TAIL"<<endl;
}

int main(int argc, char * argv[]) {
    MemoryManager newMemoryManager;
    int command, memory_size, process_number;

    ioStruct *ioNodeHead;
    ioStruct *travel;
    const string filename = "input.txt";

    ioNodeHead = readFromFile(filename);
    travel = ioNodeHead;
    
    while (travel != NULL) {
        command = travel->commandNumber;
        memory_size = travel->memorySize;
        process_number = travel->processNumber;
        if (command == 0) { // 0: create node
            newMemoryManager.createNode(process_number, memory_size);
        } else if (command == 1) { // 1: expand node
            newMemoryManager.expandNode(process_number, memory_size);
        } else if (command == 2) { // 2: contract node
            newMemoryManager.contractNode(process_number, memory_size);
        } else if (command == 3) { // 3: delete node
            newMemoryManager.deleteNode(process_number);
        } else if (command == 4) { // 4: clean memory
            newMemoryManager.cleanMemory();
        } else if (command == 5) { // 5: print process
            newMemoryManager.processMemoryUsage(process_number);
        } else if (command == 6) { // 6: print all processes
            newMemoryManager.allMemoryUsage();
        }
        travel = travel->next;
    } 
    
    //printData(travel);
    //deleteData(ioNodeHead);
}