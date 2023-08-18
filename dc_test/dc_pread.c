#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdatomic.h>

#include "dc_config.h"


int main(int argc, char **argv) {
	int file = syscall(OPEN, fname, O_RDONLY, S_IRWXU);
	unsigned int x = syscall(PREAD, file, buf, LENGTH, OFFSET); 

	close(file);
	return 0;
}
