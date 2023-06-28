#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define MAX_LEN 512

char *result[MAX_LEN];
int ind = 0;

int get_file_path(char *curr_dir, char *filename, char *path) {
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(curr_dir)) == NULL)
        return 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            char next_dir[MAX_LEN];
            snprintf(next_dir, sizeof(next_dir), "%s/%s", curr_dir, entry->d_name);
            if (get_file_path(next_dir, filename, path)) {
                closedir(dir);
                return 1;
            }
        } else {
            if (strcmp(entry->d_name, filename) == 0) {
                char file_path[MAX_LEN];
                snprintf(file_path, sizeof(file_path), "%s/%s", curr_dir, entry->d_name);
                strcpy(path, file_path);
                return 1;
            }
        }
    }
    closedir(dir);
    return 0;
}

int find_minotaur(char *root_dir, char *filename) {
    char file_path[MAX_LEN];
    if (get_file_path(root_dir, filename, file_path) == 0)
        return 0;
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
        return 0;
    int fl = 1;
    for (int i = 0; i < ind; i++) {
        if (strcmp(result[i], file_path) == 0) {
            fl = 0;
            break;
        }
    }
    if (fl)
        result[ind++] = file_path;
    char str[MAX_LEN];
    while (fgets(str, sizeof(str), file)) {
        if (strstr(str, "Deadlock") != NULL) {
            result[ind] = NULL;
            fclose(file);
            return 0;
        } else if (strstr(str, "Minotaur") != NULL) {
            fclose(file);
            return 1;
        } else if (strncmp(str, "@include ", 9) == 0 && str[strlen(str) - 1] == '\n') {
            int i = 9;
            while (str[i] != '\n')
                i++;
            str[i] = '\0';
            if (find_minotaur(root_dir, &str[9]) == 1) {
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);
    return 0;
}

int main() {
    FILE *file;
    file = fopen("./result.txt", "w");
    if (find_minotaur("./labyrinth", "file.txt")) {
        for (int i = 0; i < ind; i++) {
            fputs(result[i], file);
            fputs("\n", file);
        }
    }
    fclose(file);
    return 0;
}
