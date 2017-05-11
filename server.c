//
// Created by vibodha on 4/17/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>

//char webpage[]= "HTTP/1.1 200 OK\\nServer: Apache-Coyote/1.1\\nPragma: no-cache";

char webpage[]=
        "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                "<!DOCTYPE html>\r\n"
                "<html><head><title>first page</title></head>\r\n"
                "<body>first page</body></html>\r\n";

char webpage2[]=
        "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html; charset=UTF-8\r\n\r\n";

int main(int argc, char const *argv[])
{

    //FILE *fp;
    //char * line = NULL;
    size_t len = 0;
    char method[20] = {'\0'};
    char url[50] = {'\0'};


//    fp = fopen("test.html", "r");
//
//    if (fp == NULL)
//        exit(EXIT_FAILURE);
//
//    char *result;
//
//    while (getline(&line, &len, fp) != -1) {
//        result = malloc(strlen(webpage2)+strlen(line)+1);
//        strcpy(result, webpage2);
//        strcat(result, line);
//        strcpy(webpage2, result);
//    }
//    result = malloc(strlen(webpage2)+strlen("\r\n")+1);
//    strcpy(result, webpage2);
//    strcat(result, "\r\n");
//    strcpy(webpage2, result);
//
//    fclose(fp);
//    if (line)
//        free(line);


    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_len = sizeof(client_addr);
    int fd_server , fd_client;
    char buf[2048];
    int on = 1;

    fd_server =socket(AF_INET, SOCK_STREAM, 0);
    if(fd_server < 0)
    {
        perror("socket");
        exit(1);
    }

    setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8090);

    if(bind(fd_server, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(fd_server);
        exit(1);
    }

    if(listen(fd_server, 10) == -1)
    {
        perror("listen");
        close(fd_server);
        exit(1);

    }

    while (1)
    {
        fd_client = accept(fd_server, (struct sockaddr *) &client_addr, &sin_len);

        if(fd_client == -1)
        {
            perror("Connection failed...\n");
            continue;
        }

        printf("Got client connection........\n");

        if(!fork())
        {
            close(fd_server);
            memset(buf, 0, 2048);
            read(fd_client, buf, 2047);

            //printf("%s\n", buf);

//            char url[17];
//            memcpy( url, &buf, 16 );
//            url[16] = '\0';

/////////////////////////////////////////////////////////
            char *p = &buf[0];
            int i1 = 0;
            while (*p != ' '){
                method[i1] = *p;
                //printf("%c", *p);
                i1++;
                p++;
            }
            p++;
            int i2 = 0;
            while (*p != ' '){
                url[i2] = *p;
                //printf("%c", *p);
                i2++;
                p++;
            }
/////////////////////////////////////////////////////////////

            printf("URL - %s \n", url);
            printf("Method - %s \n", method);



            if(!strcmp(method,"GET")){
                int fp;
                fp = open(url+1, O_RDONLY);
//              FILE * fp3 = fdopen(fp2, O_RDONLY);
//              fseek(fp3, 0L, SEEK_END);
//              double sz = ftell(fp3);
//              rewind(fp3);
//              printf("size - %lf\n", sz);

                if(fp == -1){
                    //printf(webpage);
                    write(fd_client, webpage, sizeof(webpage) - 1);
                    close(fd_client);
                }
                else if(!strcmp(url,"/")){
                    write(fd_client, webpage, sizeof(webpage) - 1);
                    close(fd_client);
                }
                else{
                    //printf(webpage2);
                    //write(fd_client, webpage2, sizeof(webpage2) - 1);
                    sendfile(fd_client, fp, NULL, 170);
                    close(fd_client);
                }
            }
            else{
                //printf(webpage);
                write(fd_client, webpage, sizeof(webpage) - 1);
                close(fd_client);
            }




            printf("closing..........\n");
            exit(0);
        }

        close(fd_client);
    }

    return 0;
}
