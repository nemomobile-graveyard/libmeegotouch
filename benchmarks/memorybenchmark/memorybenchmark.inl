#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

#include <QDebug>

long int usedMemorySize = 0;

void resetMemorySizeSnapshot()
{
#ifdef __APPLE__
	usedMemorySize = mstats().bytes_used;
#else
	usedMemorySize = mallinfo().uordblks;
#endif
}

long int usedMemorySizeSnapshot()
{
#ifdef __APPLE__
	return mstats().bytes_used - usedMemorySize;
#else
	return mallinfo().uordblks - usedMemorySize;
#endif
}

void printUsedMemorySizeSnapshot()
{
	qDebug() << "Memory used: " << usedMemorySizeSnapshot() << " bytes";
}

#include "memorybenchmark.h"
typedef struct Allocation_t
{
    void* address;
    size_t size;
    void* backtrace;
    Allocation_t* prev;
}Allocation;

static size_t        allocatedSize = 0;
static Allocation*   allocations = 0;
static bool          store_backtrace = false;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

//For backtrace
#include <execinfo.h>
#include <cxxabi.h>
#include <dlfcn.h>

void demangleBacktrace(void* trace)
{   
    if(trace == 0) {
        printf("no backtrace");
        return;
    }
    
    char** all_symbols = backtrace_symbols (&trace, 1);
    char* symbol = all_symbols[0];
    
    char* begin = 0, *end = 0;
    int length = strlen(symbol);
    
    for(int i=0; i<length; ++i) {
        if(symbol[i] == '(') {
            begin = &symbol[++i];
        }
        else if(symbol[i] == '+') {
            end = &symbol[i];
            break;
        }            
    }
    
    if(begin && end) {
        
        *end = 0;

        int status = 0;
        char* demangled = abi::__cxa_demangle(begin, NULL, 0, &status);
        if(status == 0 && demangled) {
            printf("%s", demangled);
        } else {
            printf("%s", begin);
        }
    } else {
        printf("%s", symbol);
    }
    
    free(all_symbols);
}

void beginBacktracing()
{
    store_backtrace = true;
}

void endBacktracing() 
{
    store_backtrace = false;
}

void outputAllocatedMemoryBacktrace()
{
    pthread_mutex_lock( &mutex );
    Allocation* allocation = allocations;
    while(allocation) {
        printf("memory allocation: %d bytes from: ", int(allocation->size));
        demangleBacktrace(allocation->backtrace);
        printf("\n");
        allocation = allocation->prev;
    }
    pthread_mutex_unlock( &mutex );
}

void* operator new (size_t size)
{
    pthread_mutex_lock( &mutex );
    void *p=malloc(size); 
    
    if (p==0) {
        pthread_mutex_unlock( &mutex );
        throw std::bad_alloc(); // ANSI/ISO compliant behavior
    }
    
    allocatedSize += size;
    
    Allocation* allocation = (Allocation*) malloc(sizeof(Allocation));
    allocation->address = p;
    allocation->size = size;
    allocation->prev = 0;
        
    if(allocations) {
        allocation->prev = allocations;
    }
    
    allocations = allocation;
    pthread_mutex_unlock( &mutex );

    if(store_backtrace) {
        void* trace[2];
        backtrace(trace, 2);
        allocation->backtrace = trace[1];
    } else {
        allocation->backtrace = 0;
    }

    return p;
}

#include <stdio.h>
void operator delete (void *p)
{
    pthread_mutex_lock( &mutex );

    Allocation* lastProcessed = 0;
    Allocation* allocation = allocations;
    
    while(allocation) {
        // check if this is the one we want
        if(allocation->address == p) {
        
            // reduce allocated size
            allocatedSize -= allocation->size;
            
            // keep the chain valid
            if(lastProcessed) {
                lastProcessed->prev = allocation->prev;
            }
            
            // chain pointer changing
            if(allocation == allocations) {
                allocations = allocation->prev;
            }
                       
            free(allocation);
            break;
        } else {
            // store this as last processed
            lastProcessed = allocation;
            // get next in list (we'll be traversing from back to start)
            allocation = allocation->prev;
        }
    }   
    free(p);
    pthread_mutex_unlock( &mutex );

}

void beginMemoryBenchmark()
{
    pthread_mutex_lock( &mutex );
    while(allocations) {
        Allocation* newhead = allocations->prev;
        free(allocations);
          allocations = newhead;
    }
    
    allocations = 0;
    allocatedSize = 0;
    pthread_mutex_unlock( &mutex );    
}

size_t allocatedMemorySize()
{
    return allocatedSize;
}

void outputAllocatedMemorySize()
{
	qDebug() << "Used memory:" << allocatedMemorySize() << "bytes.";
}

size_t endMemoryBenchmark(bool outputUnfreedMemory)
{
    if(outputUnfreedMemory) {
        qDebug() << "Unfreed memory: " << int(allocatedSize) << " bytes";
    }
    return allocatedSize;
}
