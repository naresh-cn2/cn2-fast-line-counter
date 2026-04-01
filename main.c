#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
    // 1. HARDEN: Check if user provided a file
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // 2. Open the file descriptor
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // 3. Get the file size (needed for mmap)
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Error getting file size");
        close(fd);
        return 1;
    }
    size_t size = st.st_size;

    // 4. MAP THE FILE (The Magic Step)
    char *data = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return 1;
    }

    long long line_count = 0;
    clock_t start = clock();

    // 5. THE KILL LOOP: Scan memory directly
    for (size_t i = 0; i < size; i++) {
        if (data[i] == '\n') {
            line_count++;
        }
    }

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    // 6. Cleanup
    munmap(data, size);
    close(fd);

    printf("--- MMAP PERFORMANCE ---\n");
    printf("Total Lines: %lld\n", line_count);
    printf("Time taken : %.4f seconds\n", time_spent);

    return 0;
}
