#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

void print_usage(char *prog) {
    printf("Usage: %s <file> <keyword1> [keyword2] ...\n", prog);
    printf("Example: %s data.txt ERROR WARNING\n", prog);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("FATAL ERROR: Could not open file");
        return 1;
    }

    struct stat st;
    fstat(fd, &st);
    size_t size = st.st_size;

    // MMAP: Mapping the monster file
    char *data = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return 1;
    }

    FILE *out = fopen("multi_filtered_results.txt", "w");
  // --- THE TIMER STARTS HERE ---
    clock_t start = clock(); 
    long long total_matches = 0;

    // --- STEP 1: PRE-COMPUTATION ---
    int num_targets = argc - 2;
    size_t target_lengths[num_targets];
    for (int i = 0; i < num_targets; i++) {
        target_lengths[i] = strlen(argv[i + 2]);
    }

    char *current = data;
    char *end = data + size;

    // --- STEP 2: THE JUMP ENGINE ---
    while (current < end) {
        // Hardware-optimized jump to the next bracket
        char *bracket = memchr(current, '[', end - current);

        if (bracket == NULL) break;

        for (int k = 0; k < num_targets; k++) {
            // Boundary check: Keyword length + bracket must be within file size
            if (bracket + 1 + target_lengths[k] <= end) {
                if (memcmp(bracket + 1, argv[k + 2], target_lengths[k]) == 0) {
                    total_matches++;
                }
            }
        }
        current = bracket + 1; 
    }

    // --- STEP 3: THE RESULTS ---
    clock_t end_time = clock();
    double time_spent = (double)(end_time - start) / CLOCKS_PER_SEC;

    printf("\n--- CN2 HIGH-SPEED LOG SCANNER V1.1 ---\n");
    printf("Scan Complete.\n");
    printf("Total Matches: %lld\n", total_matches);
    printf("Time: %.4f seconds\n", time_spent);
    printf("Throughput: %.2f GB/s\n", (size / (1024.0 * 1024.0 * 1024.0)) / time_spent);
    printf("---------------------------------------\n");

    munmap(data, size);
    fclose(out);
    close(fd);
    return 0;
}