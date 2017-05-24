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


char webpage[]=
        "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                "<!DOCTYPE html>\r\n"
                "<html><head><title>first page</title></head>\r\n"
                "<body>error page</body></html>\r\n";

char error1[]=
        "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                "<!DOCTYPE html>\r\n"
                "<html><head><title>first page</title></head>\r\n"
                "<body>Page Not Found</body></html>\r\n";

char webpage2[]=
        "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html; charset=UTF-8\r\n\r\n";

int main(int argc, char const *argv[])
{

    size_t len = 0;
    char method[20] = {'\0'};
    char url[50] = {'\0'};

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


            int length = (int) strlen(url);
            const char *last_four = &url[length-4];

            printf("%s\n",last_four);

            char m_type[20];

            if(!strcmp(method,"GET")){
                int fp = 0;
                struct stat st;
                long size;
                stat("error.png", &st);
                size = st.st_size;

                if(!strcmp(last_four,".php")){
                    char cmd[50];

                    printf("%zu\n", strlen(url));

                    char cmd1[] = "php -f ";

                    int i = 7;

                    strcpy(cmd, cmd1);

                    printf("%d\n",i);

                    int j;
                    for(j =0; j<strlen(url);j++){
                        cmd[i] = url[j];
                        i++;
                    }
                    printf("%d\n",i);

                    char cmd2[] = " > temp.html";

                    int k;
                    for(k =0; k<12;k++){
                        cmd[i] = cmd2[k];
                        i++;
                    }

                    cmd[i] = '\0';

                    printf("%d\n",i);
                    printf("%s\n",cmd1);
                    printf("%s\n",cmd);

                    int err = system(cmd);

                    printf("%d\n",err);

                    if(err == 0){
                        fp = open("temp.html", O_RDONLY);
                        if (fp != -1){
                            char ok[] = "HTTP/1.1 200 OK\r\n";
                            write(fd_client, ok, sizeof(ok));
                            char headContentTypeOther[]=
                                    "Content-Type: text/html; charset=utf-8\r\n\r\n\0";
                            write(fd_client,headContentTypeOther, sizeof(headContentTypeOther));
                        }
                    }
                    else if(err == 256){
                        write(fd_client, error1, sizeof(webpage) - 1);
                        close(fd_client);
                    } else{
                        write(fd_client, webpage, sizeof(webpage) - 1);
                        close(fd_client);
                    }


                }
                else if(!strcmp(last_four, "jpeg")){
                    fp = open(url, O_RDONLY);
                    if (fp != -1){
                        char ok[] = "HTTP/1.1 200 OK\r\n";
                        write(fd_client, ok, sizeof(ok));
                        write(fd_client, "Content-Type: image/jpeg; charset=UTF-8\r\n\r\n", 43);
                    }
                }
                else if(!strcmp(last_four, ".png")){
                    fp = open(url, O_RDONLY);

                    if (fp != -1){
                        char ok[] = "HTTP/1.1 200 OK\r\n";
                        write(fd_client, ok, sizeof(ok));
                        write(fd_client, "Content-Type: image/png; charset=UTF-8\r\n\r\n", 42);
                    }
                }
                else if(!strcmp(last_four, ".pdf")){
                    fp = open(url, O_RDONLY);
                    if (fp != -1){
                        char ok[] = "HTTP/1.1 200 OK\r\n";
                        write(fd_client, ok, sizeof(ok));
                        char headContentTypePDF[]=
                                "Content-Type: application/pdf; charset=utf-8\r\n\r\n\0";
                        write(fd_client,headContentTypePDF, sizeof(headContentTypePDF));
                    }
                }
                else{
                    fp = open(url, O_RDONLY);
                    if (fp != -1){
                        char ok[] = "HTTP/1.1 200 OK\r\n";
                        write(fd_client, ok, sizeof(ok));
                        char headContentTypeOther[]=
                                "Content-Type: text/html; charset=utf-8\r\n\r\n\0";
                        write(fd_client,headContentTypeOther, sizeof(headContentTypeOther));
                    }

                }

                if(fp == -1){
                    write(fd_client, error1, sizeof(webpage) - 1);
                    close(fd_client);
                }
                else if(!strcmp(url,"/")){
                    write(fd_client, webpage, sizeof(webpage) - 1);
                    close(fd_client);
                }
                else{
                    stat(url, &st);
                    size = st.st_size;
                    sendfile(fd_client, fp, NULL, (size_t) size);
                    close(fd_client);
                }
            }
            else{
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
