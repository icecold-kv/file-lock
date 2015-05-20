#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int write_to_f (FILE *of, char *argv[]) {
	char buf[1024];
	strcpy(buf, "");
	char tmp[50];
	int strN = 1;
	char *str = fgets(tmp, 50, of);
	while (str != NULL) {
		if (strN == atoi(argv[2])) {
			strcat(buf, argv[3]);
			strcat(buf, "\n");
		} else
			strcat(buf, tmp);
		str = fgets(tmp, 50, of);
		strN++;
	}
	rewind(of);
	fwrite(buf, 1, strlen(buf), of);
	return 0;
}

int main(int argc, char *argv[]) {
	if (argc < 4) {
		printf("Usage: ./lock file number newstring\n");
		return -1;
	}
	char path[70];
	struct stat st;
	strcpy(path, argv[1]);
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
	FILE *of = fopen(argv[1], "r+");
	fprintf(lck, "%d\n",getpid());
	fclose(lck);
	flock(of, LOCK_EX);
	printf("Writing to file...\n");
	write_to_f(of, argv);
	sleep(10);
	flock(of, LOCK_UN);
	close(of);
	remove(path);
	return 0;
}
