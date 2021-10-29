#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>

#define MAX_BYTES (100ULL<<20) /*100 MB */

int main(void) {
	int fd;
	FILE* f;
	uint64_t count = 0;
	uint64_t file_value;

	fd  = open(".", __O_TMPFILE | O_RDWR, S_IRUSR | S_IWUSR);

	if (fd == -1) {
		printf("O_TMPFILE error: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	printf("O_TMPFILE success\n");

	// write
	f = fdopen(fd, "w+");
	if (!f) {
		printf("fdopen() failed: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	for (count = 0; count < MAX_BYTES / sizeof(count); count++) {
		if (fwrite(&count, sizeof(count), 1, f) != 1) {
			printf("fwrite() failed: %s\n", strerror(errno));
			return EXIT_FAILURE;
		}
	}

	fflush(f);
	rewind(f);

	for (count = 0; count < MAX_BYTES / sizeof(count); count++) {
		if (fread(&file_value, sizeof(file_value), 1, f) != 1) {
			printf("fwrite() failed: %s\n", strerror(errno));
			return EXIT_FAILURE;
		}
		if (count != file_value) {
			printf("value mismatch: expected %llu, got %llu\n", count, file_value);
			return EXIT_FAILURE;
		}
	}

	if (fread(&file_value, sizeof(file_value), 1, f) == 1) {
		printf("unexcpected read success: read %llu, expected eof", file_value);
	}

	fclose(f);

	return EXIT_SUCCESS;
}
