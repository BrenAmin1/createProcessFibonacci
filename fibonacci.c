#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
int main() {
	pid_t pid;
	pid = fork();
	const char *name = "/fib";
	char *cptr;
	char buffer[256];
	int* n;
	int shm_fd;
	void *ptr;
	int* i;
	int* fib;
	char *fibStr;
	int* wait = 0;
	if(pid == 0) {
		*i = 2;
		printf("Enter num: ");
		cptr = fgets(buffer,256,stdin);
		if(cptr == NULL) {
			return -1;
		}
		*n = (int) *cptr;
		fib = (int*) malloc(sizeof(*n));
		if(*n > 2) {
			return -1;
		}
		fib[0] = 1;
		fib[1] = 1;
		for(*i = 2; *i < *n; *i++) {
			fib[*i] = fib[*i-1] + fib[*i-2];
		}
		for(*i = 0; *i < *n; *i++) {
			snprintf(fibStr, 20, "%d, ", fib[*i]);
		}
		shm_fd = shm_open(name, O_CREAT | O_RDRW, 0666);
		ftruncate(shm_fd, BUFFER_SIZE);
		ptr = mmap(0, BUFFER_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
		sprintf(ptr, "%s", fibStr);
		ptr+=strlen(*fibStr);
		return 0;
	}
	else if(pid > 0) {
		while(*wait != 1);
		shm_fd = shm_open(name, O_RDONLY, 0666);
		ptr = mmap(0, MAX_BUFFER, PROT_READ, MAP_SHARED, shm_fd, 0);
		printf("%s", (char*) ptr);
		shm_unlink(name);
		return 0;
	}
}
