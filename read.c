#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUFFER_SIZE 4096 // 읽어올 데이터 크기를 정의합니다.

int main() {
    char buffer[BUFFER_SIZE]; // 4096바이트를 저장할 수 있는 버퍼를 선언합니다.
    int file = open("tmp.txt", O_RDONLY); // 파일을 열고, 파일 디스크립터를 가져옵니다.
    
    if(file < 0){
        perror("open"); // 파일을 열지 못한 경우, 에러 메시지를 출력합니다.
        return 1;
    }
	 int i = 0;
    ssize_t bytesRead; // 실제로 읽은 바이트 수를 저장합니다.
    while((bytesRead = read(file, buffer, BUFFER_SIZE)) > 0) { // 파일의 끝에 도달할 때까지 읽습니다.
        //write(STDOUT_FILENO, buffer, bytesRead); // 읽어온 데이터를 출력합니다.
				//printf("%d\n", i++);
			i++;
    }

    if(bytesRead < 0){
        perror("read"); // 파일을 읽지 못한 경우, 에러 메시지를 출력합니다.
        return 1;
    }

    close(file); // 파일을 닫습니다.
		printf("read %d times.\n", i);
    return 0;
}

