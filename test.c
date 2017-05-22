//
// Created by vibodha on 4/17/17.
//
//
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int main(int argc, char const *argv[]) {
//    FILE *fp;
//    char * line = NULL;
//    size_t len = 0;
//
//    fp = fopen("test.html", "r");
//
//    if (fp == NULL)
//        exit(EXIT_FAILURE);
//
//    while (getline(&line, &len, fp) != -1) {
//        printf("%s", line);
//    }
//    fclose(fp);
//    if (line)
//        free(line);
    char url[50];
    strcpy(url,"index.php");
    char cmd3[50];
    char cmd[50];
    char cmd1[] = "php -f ";
    char cmd2[] = " > index.html";
    strcpy(cmd3, strcat(cmd1, url+1));
    strcpy(cmd,strcat(cmd3 , cmd2));

    system(cmd);
    printf("%s\n",cmd3);
    printf("%s\n",cmd2);
    printf("%s\n",cmd);
}