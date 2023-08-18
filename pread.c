#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdatomic.h>

#include "config.h"

atomic_int start_read;
atomic_int stop_read;


char prefetch[LENGTH];
char buf[NTHREAD][LENGTH];
int count[NTHREAD];


typedef struct {
	int tid;
	off_t offset;
	size_t length;	
} ThreadArgs;

int bufEqual() {
	for (int i=0; i<NTHREAD; i++) {
		for(int j=0; j<LENGTH; j++) {
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
	int file = syscall(OPEN, fname, O_RDONLY, S_IRWXU);

	while(!atomic_load(&start_read));	

	while(!atomic_load(&stop_read)) {
		// this system call -paygo_pread- using paygo reference counting method
		// instead of the linux's reference count(folio_try_get_rcu)
		unsigned int x = syscall(PREAD, file, buf[threadArg->tid], threadArg->length, threadArg->offset);
		iter++;
		if(x != PAGE_SIZE * NPAGE) {
       perror("myread");
		}
	}	
	count[threadArg->tid] = iter;
	close(file);	
	return NULL;
}

int main(int argc, char **argv) {
	//pre-wrok
	// 0. memset the bufs.
	// 1. set start_read & stop_read flags.
	memset(prefetch, 0, LENGTH);
	for(int i=0; i<NTHREAD; i++) {
		memset(buf[i], 0, LENGTH);
	}	
	int cfile = open(cname, O_RDONLY, S_IRWXU);
	pread(cfile, prefetch, LENGTH, OFFSET); 
	start_read = 0;
	stop_read = 0;
	printf("pre-work step complete!\n");

	pthread_t threads[NTHREAD];
	ThreadArgs threadArgs[NTHREAD];
	for(int i=0; i<NTHREAD ; i++) {
		threadArgs[i].tid = i;
		threadArgs[i].offset = OFFSET;		
		threadArgs[i].length = LENGTH;
		if(pthread_create(&threads[i], NULL, thread_fn, &threadArgs[i]) != 0) {
			perror("pthread create!\n");
			return 1;
		}			
	}

	// start!
	__sync_fetch_and_add(&start_read, 1);
	// sleep for thread working
	sleep(TIME);
	// stop!
	__sync_fetch_and_add(&stop_read, 1);

	for(int i=0; i<NTHREAD; i++) {
		pthread_join(threads[i], NULL);
	}
	
	// check to make sure we've read the right data through paygo reference counting
	if(bufEqual()) {
		printf("test clear!\n");
	} else {
		printf("test failed..\n");
	}	


	for(int i=0; i<NTHREAD; i++) {
		printf("thread[%d] reads %d times\n", i, count[i]);
	}

	// tear down
	// close the file opened at prefeteching step.
	close(cfile);
	return 0;
}
