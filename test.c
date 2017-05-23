//
// Created by vibodha on 4/17/17.
//
//
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <fcntl.h>
#include <sys/stat.h>

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
    //              FILE * fp3 = fdopen(fp, O_RDONLY);

    //int fp = open("error.png", O_RDONLY);
    struct stat st;
    stat("error.png", &st);
    long izesize = st.st_size;
    printf("size - %li\n",izesize);
}