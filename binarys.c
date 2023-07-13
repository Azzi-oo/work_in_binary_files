#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_PATH_LENGTH 256

void print_file_contents(const char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("n/a\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

int write_in_file(const char* path, const char* text) {
    FILE* file = fopen(path, "a");
    if (file == NULL) {
        printf("Unable to open file.\n");
        return 0;
    }

    fprintf(file, "%s\n", text);
    fclose(file);

    return 1;
}

void caesar_cipher(char* text, int shift) {
    int i = 0;
    while (text[i] != '\0') {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = ((text[i] - 'A' + shift) % 26) + 'A';
        } else if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = ((text[i] - 'a' + shift) % 26) + 'a';
        }
        i++;
    }
}

void process_directory(const char* directory, int shift) {
    DIR* dir = opendir(directory);
    if (dir == NULL) {
        printf("Unable to open directory.\n");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {  // Check if it is a regular file
            char file_path[MAX_PATH_LENGTH];
            snprintf(file_path, sizeof(file_path), "%s/%s", directory, entry->d_name);
            if (strstr(entry->d_name, ".c") != NULL) {
                FILE* file = fopen(file_path, "r+");
                if (file != NULL) {
                    char line[256];
                    while (fgets(line, sizeof(line), file)) {
                        caesar_cipher(line, shift);
                        fseek(file, -strlen(line), SEEK_CUR);
                        fputs(line, file);
                    }
                    fclose(file);
                } else {
                    printf("Unable to open file: %s\n", file_path);
                }
            } else if (strstr(entry->d_name, ".h") != NULL) {
                FILE* file = fopen(file_path, "w");
                if (file != NULL) {
                    fclose(file);
                } else {
                    printf("Unable to open file: %s\n", file_path);
                }
            }
        }
    }

    closedir(dir);
}

int main() {
    char path[MAX_PATH_LENGTH];
    const char* text;
    int choice = 0;

    while (choice != -1) {
        printf("Menu:\n");
        printf("1. Enter file path\n");
        printf("2. Enter text to append to file\n");
        printf("3. Encrypt .c files and clear .h files in directory\n");
        printf("-1. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter file path: ");
            scanf("%s", path);

            if (strcmp(path, "-1") == 0) {
                choice = -1;
                continue;
            }

            printf("File contents:\n");
            print_file_contents(path);
        } else if (choice == 2) {
            printf("Enter file path: ");
            scanf("%s", path);

            if (strcmp(path, "-1") == 0) {
                choice = -1;
                continue;
            }

            FILE* file = fopen(path, "r");
            if (file == NULL) {
                printf("Unable to open file.\n");
                continue;
            }

            printf("Enter text to append to file (max 255 characters): ");
            char inputText[256];
            scanf(" %[^\n]", inputText);

            if (strcmp(inputText, "-1") == 0) {
                choice = -1;
                continue;
            }

            text = inputText;

            if (write_in_file(path, text)) {
                printf("File contents after appending:\n");
                print_file_contents(path);
            }
        } else if (choice == 3) {
            int shift;
            printf("Enter shift for Caesar cipher: ");
            scanf("%d", &shift);
            process_directory("src/ai_modules", shift);
            printf("Files processed.\n");
        }

        printf("\n");
    }

    return 0;
}
