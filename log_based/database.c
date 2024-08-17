#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int create_table(char filename[]) {
    if(access(filename, F_OK) != -1) {
        printf("Table already exists\n");
        return 1;
    }
    FILE *fptr;
    fptr = fopen(filename , "w");
    return 0;
}

int set(char filename[], int key, char value[]) {
    FILE *fptr;
    fptr = fopen(filename , "a");
    
    char key_str[8];
    sprintf(key_str, "%d", key);

    fprintf(fptr, "%s", key_str);
    fprintf(fptr, " ");
    fprintf(fptr, "%s", value);
    fprintf(fptr, "\n");

    fclose(fptr);
    return 0;
}

char* get(char filename[], int key) {
    FILE *fptr;
    static const long max_len = 64;
    char buffer[max_len+1];
    
    fptr = fopen(filename , "r");
    if(fptr == NULL) { return NULL; }
    
    fseek(fptr, -max_len, SEEK_END);
    fread(buffer, max_len-1, 1, fptr);
    fclose(fptr);

    buffer[max_len] = '\0';

    char *last_newline = strchr(buffer, '\n');

    if(last_newline == NULL) {
        printf("Error reading table\n");
    }

    char *last_line = last_newline + 1;
    // printf("%s", last_line);
    return last_line;
}

int main();
int main(int argc, char **argv) {    
    // create <table name>
    // set <table name> <key> <value>
    // get <table name> <key>
    
    if (argc == 1) {
        printf("No arguments provided\n");
        return 1;
    } 

    if (argc < 3) {
        printf("Table name not provided\n");
        return 1;
    }

    if (strlen(argv[2]) > 32) {
        printf("Filename too long\n");
        return 1;
    }
   
    char command[10];
    strcpy(command, argv[1]);

    char filename[40];
    strcpy(filename, argv[2]);
    strcat(filename, ".table");

    if (strcmp(command, "create") == 0) {
        printf("Created table: %s\n", filename);
        create_table(filename); 
        return 0;
    }

    if (strcmp(command, "set") == 0) {
        if (argc < 5) {
            printf("Invalid command, usage: create <table name> <key> <value>\n");
            return 1;
        }

        printf("Appending to table: %s\n", filename);
        int key = atoi(argv[3]);

        set(filename, key, argv[4]);
        return 0;
    }

    if (strcmp(command, "get") == 0) {
        if (argc < 4) {
            printf("Invalid command, usage: get <table name> <key>\n");
            return 1;
        }

        int key = atoi(argv[3]);
        char *value = get(filename, key);
        if (value == NULL) {
            printf("Error reading table\n");
            return 1;
        }

        printf("Value: %s\n", value);
        return 0;
    }

};