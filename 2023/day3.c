#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 100
#define INITIAL_LINE_LENGTH 1000

typedef struct {
    int start_i;
    int start_j;
    int end_j;
    int value;
    int is_part_number;
} Number;

int main() {
    char** grid = NULL;
    int capacity = INITIAL_CAPACITY;
    int nrows = 0;
    int i, j;

    FILE* fp = fopen("day3_input.txt", "r");
    if (!fp) {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    // Allocate memory for grid
    grid = (char**)malloc(capacity * sizeof(char *));
    if (!grid) {
        fprintf(stderr, "Memory allocation error.\n");
        fclose(fp);
        return 1;
    }

    // Read lines from the file
    char line[INITIAL_LINE_LENGTH];
    int max_cols = 0;
    while (fgets(line, sizeof(line), fp)) {
        // Remove newline character if any
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[len - 1] = '\0';
            len--;
        }

        // Reallocate grid if need be
        if (nrows >= capacity) {
            capacity *= 2;
            char **new_grid = (char **)realloc(grid, capacity * sizeof(char*));
            if (!new_grid) {
                fprintf(stderr, "Memory alloc error.\n");
                fclose(fp);
                return 1;
            }
            grid = new_grid;
        }

        // Allocate memory for the line and store it
        grid[nrows] = strdup(line);
        if (!grid[nrows]) {
            fprintf(stderr, "Memory alloc error.\n");
            fclose(fp);
            return 1;
        }
        if (len > max_cols)
            max_cols = len;
        nrows++;
    }

    fclose(fp);

    // Arrays to store numbers
    int numbers_capacity = INITIAL_CAPACITY;
    Number* numbers = (Number*)malloc(numbers_capacity * sizeof(Number));
    int numbers_count = 0;

    // First pass: Parse the grid and store numbers
    for (i = 0; i < nrows; i++) {
        int row_len = strlen(grid[i]);
        j = 0;
        while (j < row_len) {
            char c = grid[i][j];
            if (c >= '0' && c <= '9') {
                // Start parsing number
                int value = 0;
                int start_j = j;
                while (j < row_len && grid[i][j] >= '0' && grid[i][j] <= '9') {
                    value = value * 10 + (grid[i][j] - '0');
                    j++;
                }
                int end_j = j - 1;
                // Store number
                if (numbers_count >= numbers_capacity) {
                    numbers_capacity *= 2;
                    numbers = (Number*)realloc(numbers, numbers_capacity * sizeof(Number));
                    if (!numbers) {
                        fprintf(stderr, "Memory allocation error.\n");
                        return 1;
                    }
                }
                numbers[numbers_count].start_i = i;
                numbers[numbers_count].start_j = start_j;
                numbers[numbers_count].end_j = end_j;
                numbers[numbers_count].value = value;
                numbers[numbers_count].is_part_number = 0; // Initially false
                numbers_count++;
            } 
            else j++;
        }
    }

    // Part 1: Determine part numbers and sum them
    int sum_part_numbers = 0;
    for (int num_idx = 0; num_idx < numbers_count; num_idx++) {
        Number* num = &numbers[num_idx];
        int found_symbol = 0;
        for (int pos_j = num->start_j; pos_j <= num->end_j && !found_symbol; pos_j++) {
            int pos_i = num->start_i;
            for (int di = -1; di <= 1 && !found_symbol; di++) {
                for (int dj = -1; dj <= 1 && !found_symbol; dj++) {
                    if (di == 0 && dj == 0) continue;
                    int ni = pos_i + di;
                    int nj = pos_j + dj;
                    if (ni >= 0 && ni < nrows && nj >= 0 && nj < strlen(grid[ni])) {
                        char c = grid[ni][nj];
                        if (!(c >= '0' && c <= '9') && c != '.' && c != '\0') {
                            num->is_part_number = 1;
                            sum_part_numbers += num->value;
                            found_symbol = 1;
                        }
                    }
                }
            }
        }
    }

    // Part 2: Find gears and compute gear ratios
    long long sum_gear_ratios = 0;
    for (i = 0; i < nrows; i++) {
        int row_len = strlen(grid[i]);
        for (j = 0; j < row_len; j++) {
            char c = grid[i][j];
            if (c == '*') {
                // Find adjacent part numbers
                int adjacent_numbers[8];
                int adjacent_count = 0;
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        if (di == 0 && dj == 0) continue;
                        int ni = i + di;
                        int nj = j + dj;
                        if (ni >= 0 && ni < nrows && nj >= 0 && nj < strlen(grid[ni])) {
                            char adj_c = grid[ni][nj];
                            if (adj_c >= '0' && adj_c <= '9') {
                                // Parse the number at this position
                                int num_j = nj;
                                while (num_j > 0 && grid[ni][num_j - 1] >= '0' && grid[ni][num_j - 1] <= '9') {
                                    num_j--;
                                }
                                // Find the number in numbers array
                                for (int k = 0; k < numbers_count; k++) {
                                    Number* num = &numbers[k];
                                    if (num->start_i == ni && num->start_j == num_j && num->is_part_number) {
                                        // Check if already added
                                        int already_added = 0;
                                        for (int l = 0; l < adjacent_count; l++) {
                                            if (adjacent_numbers[l] == k) {
                                                already_added = 1;
                                                break;
                                            }
                                        }
                                        if (!already_added) {
                                            adjacent_numbers[adjacent_count++] = k;
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                if (adjacent_count == 2) {
                    // (Metal) Gear?!
                    long long gear_ratio = (long long)numbers[adjacent_numbers[0]].value * (long long)numbers[adjacent_numbers[1]].value;
                    sum_gear_ratios += gear_ratio;
                }
            }
        }
    }

    // Free allocated memory
    for (i = 0; i < nrows; i++) {
        free(grid[i]);
    }
    free(grid);
    free(numbers);

    // Output the sums
    printf("Part 1 - Sum of all part numbers: %d\n", sum_part_numbers);
    printf("Part 2 - Sum of all gear ratios: %lld\n", sum_gear_ratios);

    return 0;
}