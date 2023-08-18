#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define MYTRAVERSE 550


int main() {
	syscall(MYTRAVERSE);	
	return 0;
}
