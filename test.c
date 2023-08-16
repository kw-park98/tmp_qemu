#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define MYOPEN 548
#define MYCALL 549
#define PAGE_SIZE 4096
#define NPAGE 2

#define LOOPS 10 
const char *fname = "tmp.txt";

int main() {
	int file = syscall( MYOPEN, fname, O_RDONLY, S_IRWXU);
	char buf[PAGE_SIZE * NPAGE];	
	for(int i=0; i<LOOPS; i++) {
		unsigned int x = syscall(MYCALL, file, buf, PAGE_SIZE * NPAGE, 0); 
		//printf("x= %u\n", x);
		if(x != PAGE_SIZE * NPAGE) {
			printf("failed to read!\n");
			return 1;
		}
	}
	printf("%s\n", buf);
	close(file);
	return 0;
}
