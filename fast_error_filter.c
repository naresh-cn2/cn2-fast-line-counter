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
    clock_t start = clock();
    long long total_matches = 0;

    // THE MULTI-KEYWORD SCANNER
    for (size_t i = 0; i < size; i++) {
        // Optimized: Only check keywords if we hit a potential starting character (like '[')
        if (data[i] == '[') {
            for (int k = 2; k < argc; k++) {
                char *key = argv[k];
                size_t k_len = strlen(key);

                // Check if the current window matches the keyword
                if (i + k_len < size && strncmp(&data[i + 1], key, k_len) == 0) {
                    
                    // Found it! Extract the whole line
                    size_t start_line = i;
                    while (start_line > 0 && data[start_line-1] != '\n') start_line--;
                    size_t end_line = i;
                    while (end_line < size && data[end_line] != '\n') end_line++;

                    fwrite(&data[start_line], 1, end_line - start_line + 1, out);
                    total_matches++;
                    i = end_line; // Skip to end of line
                    break; 
                }
            }
        }
    }

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    fclose(out);
    munmap(data, size);
    close(fd);

    printf("\n--- AXIOM MULTI-FILTER COMPLETE ---\n");
    printf("Keywords Searched : %d\n", argc - 2);
    printf("Total Matches     : %lld\n", total_matches);
    printf("Processing Time   : %.4f seconds\n", time_spent);
    printf("Speed             : %.2f GB/s\n", (size / 1024.0 / 1024.0 / 1024.0) / time_spent);
    printf("Results saved to  : multi_filtered_results.txt\n");

    return 0;
}
