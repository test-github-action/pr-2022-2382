#include <stdio.h> 
#include <string.h> 
#include <dirent.h> 
#define MAX 1024 
 
char *result[MAX]; 
int index_ = 0; 
 
int saveFilepath(char *dir_name, char *filename, char *path); 
int findingMinotaur(char* root_dir, char* filename); 
 
 
int main() { 
    char* firstFilename = "file.txt"; 
    char* firstDirname = "./labyrinth"; 
    FILE *file; 
    file = fopen("./result.txt", "w"); 
    if (findingMinotaur(firstDirname, firstFilename)) { 
        for (int i = 0; i < index_; i++) { 
            fputs(result[i], file); 
            fputs("\n", file); 
        } 
    } 
    fclose(file); 
    return 0; 
} 
 
 
int saveFilepath(char *dir_name, char *filename, char *path) { 
    DIR *dir = opendir(dir_name); 
    struct dirent *ent = readdir(dir); 
    int flag = 0; 
     
    if (!dir) { 
        return flag; 
    } 
     
    while (ent) { 
        if (ent->d_type == DT_DIR) { 
            if (strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".") == 0){ 
                ent = readdir(dir); 
                continue; 
            } 
            char next_dir_name[MAX]; 
            snprintf(next_dir_name, sizeof(next_dir_name), "%s/%s", dir_name, ent->d_name); 
            if (saveFilepath(next_dir_name, filename, path)) { 
                flag = 1; 
                closedir(dir); 
                return flag; 
            } 
        } else { 
            if (!strcmp(ent->d_name, filename)) { 
                char file_path[MAX]; 
                snprintf(file_path, sizeof(file_path), "%s/%s", dir_name, ent->d_name); 
                strcpy(path, file_path); 
                flag = 1; 
                closedir(dir); 
                return flag; 
            } 
        } 
        ent = readdir(dir); 
    } 
    closedir(dir); 
    return flag; 
} 
 
 
int findingMinotaur(char* root_dir, char* filename) { 
     
    char file_path[MAX]; 
    int flag = 1; 
    char* serv_string = "@include "; 
    char string[MAX]; 
    char* deadend = "Deadlock"; 
    char* findstr = "Minotaur"; 
     
    if (!saveFilepath(root_dir, filename, file_path)){ 
        return 0; 
    } 
    FILE *file = fopen(file_path, "r"); 
    if (!file){ 
        return 0; 
    } 
    for (int i = 0; i < index_; i++) { 
        if (!strcmp(result[i], file_path)) { 
            flag = 0; 
            break; 
        } 
    } 
    if (flag){ 
        result[(index_)++] = file_path; 
    } 
     
    while (fgets(string, sizeof(string), file)) { 
         
        if (!strcmp(string, deadend)) { 
            fclose(file); 
            result[index_] = NULL; 
            return 0; 
        } else if (!strcmp(string, findstr)) { 
            fclose(file); 
            return 1; 
        } else if (strncmp(string, serv_string, strlen(serv_string)) == 0 && string[strlen(string) - 1] == '\n') { 
            int i = strlen(serv_string); 
            while (string[i] != '\n'){ 
                i++; 
            } 
            string[i] = '\0'; 
            if (findingMinotaur(root_dir, &string[strlen(serv_string)])) { 
                fclose(file); 
                return 1; 
            } 
        } 
    } 
    fclose(file); 
    return 0;
}
