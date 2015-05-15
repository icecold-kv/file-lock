#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Not enough arguments.\n");
		return -1;
	}
	char path[70];
	struct stat st;
	strcpy(path,argv[1]);
	strcat(path, ".lck");
	int r = stat(path, &st);
	if (r == 0) {
		printf("File already locked.\nWaiting...\n");
		while (r == 0) {
			sleep(2);
			r = stat(path, &st);
		}
	}
	FILE *lck = fopen(path, "w");
	FILE *of = fopen(argv[1], "w");
	fprintf(lck, "%d\n",getpid());
	fclose(lck);
	flock(of, LOCK_EX);
	printf("Writing to file...\n");
	sleep(20);
	flock(of, LOCK_UN);
	close(of);
	remove(path);
	return 0;
}
