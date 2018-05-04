#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

void error(const char*msg){
    perror(msg);
    exit(1);
}

int main(int argc, char** argv){
    if(argc < 2){
        error("No port provided, compile like './main [portnumber] [ip]' where portnumber > 2000");
        exit(1);
    }
    // initialize variables
    system("clear");
    int sockfd = 0;
    int bytesRes = 0, portno;
    char rec_buff[1024];
    struct sockaddr_in hint;
    char serv_addr[20];
    memset(serv_addr, '0', sizeof(serv_addr));
    memset(rec_buff, '0', sizeof(rec_buff));

    portno = atoi(argv[1]);
    // get server's ip address
    if( argc < 3){
        printf("Enter IP address: ");
        fgets(serv_addr, 20, stdin);
    }else{
        strcpy(serv_addr, argv[2]);
    }

    // create the socket for listening
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("Error could not create socket");
    }
    // bind protocol, ip and port number
    hint.sin_family = AF_INET;
    hint.sin_addr.s_addr = inet_addr(serv_addr);
    hint.sin_port = htons(portno);
    
    if(connect(sockfd, (struct sockaddr *)&hint, sizeof(hint))<0){
        error("Could not connect to server");
    }
    FILE *fp;
    char file_name[100];
    read(sockfd, file_name, 256);
    printf("File Name: %s\n", file_name);
    fp = fopen(file_name, "wb");
    if(fp == NULL){
        error("Could not open file");
    }
    while((bytesRes = read(sockfd, rec_buff, 1024)) > 0){
        if(bytesRes < 0){
            printf("Could not read");
        }
        fflush(stdout);
        fwrite(rec_buff, 1, bytesRes, fp);
    }
    puts("Completed.");
    return 0;
}