#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define MYOPEN 548
#define MYCALL 549
#define PAGE_SIZE 4096
#define NPAGE 2
#define LOOPS 10 

#define NTHREAD 10


const char *fname = "tmp.txt";
char prefetch[PAGE_SIZE * NPAGE];
char buf[NTHREAD][PAGE_SIZE * NPAGE];

typedef struct {
	int tid;
	off_t offset;
} ThreadArgs;

int bufEqual() {
	for (int i=0; i<NTHREAD; i++) {
		for(int j=0; j<PAGE_SIZE * NPAGE; j++) {
			if(buf[i][j] != prefetch[j])
				return 0;
		}
	}
  return 1;
}

void *thread_fn(void *args) {
	int iter = 0;
  ThreadArgs* threadArg = (ThreadArgs*)args;
	// this system call -paygo_open- tells the system 
	// that this file has been applied paygo reference counting.
	int file = syscall(MYOPEN, fname, O_RDONLY, S_IRWXU);
	for(int i=0; i<LOOPS; i++) {
		// this system call -paygo_pread- using paygo reference counting method
		// instead of the linux's reference count(folio_try_get_rcu)
		unsigned int x = syscall(MYCALL, file, buf[threadArg->tid], PAGE_SIZE * NPAGE, threadArg->offset);
		iter++;
		if(x != PAGE_SIZE * NPAGE) {
       perror("myread");
		}
	}	
	close(file);	
	return NULL;
}

int main() {
	//pre-wrok
	// 1. open the file.
	// 2. prefetching the pages that we'll test for paygo refernce counting.
	int file = open(fname, O_RDONLY, S_IRWXU);
	pread(file, prefetch, PAGE_SIZE * NPAGE, 0); 
	printf("prefetch step complete!\n");

	pthread_t threads[NTHREAD];
	ThreadArgs threadArgs[NTHREAD];
	for(int i=0; i<NTHREAD ; i++) {
		threadArgs[i].tid = i;
		threadArgs[i].offset = 0;		
		if(pthread_create(&threads[i], NULL, thread_fn, &threadArgs[i]) != 0) {
			perror("pthread create!\n");
			return 1;
		}			
	}
	for(int i=0; i<NTHREAD; i++) {
		pthread_join(threads[i], NULL);
	}
	
	// check to make sure we've read the right data through paygo reference counting
	if(bufEqual()) {
		printf("test clear!\n");
	} else {
		printf("test failed..\n");
	}
	printf("%s\n", prefetch);

	// close the file opened at prefeteching step.
	close(file);
	return 0;
}
