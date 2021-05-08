#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#define PORT 8080

char pathToServer[] = "/home/han/praktikum sisop/Prak3/server/" ;
char pathToClient[] = "/home/han/praktikum sisop/Prak3/client/" ;
struct sockaddr_in address;
int sock = 0, valread;
struct sockaddr_in serv_addr;
char buffer[1024] = {0};
char cmd[100] = {0} ; int loggedIn = 0 ;
pthread_t tid ;
int parseMode ;

int parse(char msg[], char a[], char b[]) {
    int i = 0 ;
    char* tok ;
    while (tok = strtok_r(msg, " ", &msg)) {
        if (i == 0)
            strcpy(a, tok) ;
        else if (i == 1)
            strcpy(b, tok) ;
        else {
            if (parseMode == 1)
                return 1 ;
            if (parseMode == 2) {
                strcat(b, " ") ;
                strcat(b, tok) ;
            }
        }

        i++ ;
    }
    return 0 ;
}

// Register Login----------------------------------------------
void* reglog(void* arg) {
    bzero(buffer, 1024) ;
    read( sock , buffer, 1024) ;
    printf("%s\n", buffer) ;
    memset(buffer, 0, 1024) ;

    scanf(" %[^\n]s", buffer) ;
    send(sock, buffer, strlen(buffer), 0) ;
    bzero(buffer, 1024) ;
    
    read(sock, buffer, 1024) ;
    printf("%s\n", buffer) ;
    if (!strcmp(cmd, "login")) {
        loggedIn = 1 ;
    }
}
// Register Login----------------------------------------------

// Add---------------------------------------------------------
void* add(void* arg) {
    int i ;
    for (i = 0 ; i < 3 ; i++) {
        bzero(buffer, 1024) ;
        read(sock, buffer, 1024) ;
        printf("%s", buffer) ;

        scanf(" %[^\n]s", buffer) ;
        send(sock, buffer, strlen(buffer), 0) ;
    }

    FILE* file = fopen(buffer, "rb") ;
    char data[1024] = {0};

    while(fgets(data, 1024, file) != NULL) {
        send(sock, data, strlen(data), 0);
        bzero(data, 1024) ;
    }
    fclose(file) ;

    bzero(buffer, 1024) ;
    read(sock, buffer, 1024) ;
    printf("%s\n", buffer) ;
    bzero(buffer, 1024) ;
}
// Add---------------------------------------------------------

// Download----------------------------------------------------
void* download(void* arg) {

    char a[30] ;
    parseMode = 2 ;
    parse(cmd, a, buffer) ;
    
    char local[100] ;
    strcpy(local, pathToClient) ; strcat(local, buffer) ;
    
    bzero(buffer, 1024) ;
    read(sock, buffer, 1024);
    if (strcmp(buffer, "Buku tidak ditemukan") && strcmp(buffer, "Masukkan nama buku")) {
        FILE* file = fopen(local, "w") ;
        fprintf(file, "%s", buffer) ;
        fclose(file) ;

        printf("Buku berhasil diunduh\n") ;
    }
    else {
        printf("%s\n", buffer) ;
    }

}
// Download----------------------------------------------------

// Delete------------------------------------------------------
void* delete(void* arg) {
    bzero(buffer, 1024) ;
    read(sock, buffer, 1024) ;
    printf("%s\n", buffer) ;
    bzero(buffer, 1024) ;
}
// Delete------------------------------------------------------

// See---------------------------------------------------------
void* see(void* arg) {
    bzero(buffer, 1024) ;
    read(sock, buffer, 1024) ;
    printf("%s\n", buffer) ;
    bzero(buffer, 1024) ;
}
// See---------------------------------------------------------

// Find--------------------------------------------------------
void* find(void* arg) {
    bzero(buffer, 1024) ;
    read(sock, buffer, 1024) ;
    printf("%s\n", buffer) ;
    bzero(buffer, 1024) ;
}
// Find--------------------------------------------------------

// Login First-------------------------------------------------
void* loginFirst(void* arg) {
    bzero(buffer, 1024) ;
    read(sock, buffer, 1024) ;
    printf("%s\n", buffer) ;
    bzero(buffer, 1024) ;
}
// Login First-------------------------------------------------

int main(void) {
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    read(sock, buffer, 1024) ;
    printf("%s\n", buffer) ;
    bzero(buffer, 1024) ;
    
    while (1) {
        printf(">> ") ;
        scanf(" %[^\n]", cmd) ;
        send(sock , cmd , strlen(cmd) , 0 ) ;
        if (!strcmp(cmd, "register") || !strcmp(cmd, "login")) {
            pthread_create(&(tid),NULL,&reglog,NULL) ;
        }
        else if (!strcmp(cmd, "add") && loggedIn) {
            pthread_create(&(tid),NULL,&add,NULL) ;
        }
        else if (!strncmp(cmd, "download", 8) && loggedIn) {
            pthread_create(&(tid),NULL,&download,NULL) ;
        }
        else if (!strncmp(cmd, "delete", 6) && loggedIn) {
            pthread_create(&(tid),NULL,&delete,NULL) ;
        }
        else if (!strcmp(cmd, "see") && loggedIn) {
            pthread_create(&(tid),NULL,&see,NULL) ;
        }
        else if (!strncmp(cmd, "find", 4) && loggedIn) {
            pthread_create(&(tid),NULL,&find,NULL) ;
        }
        else {
            pthread_create(&(tid),NULL,&loginFirst,NULL) ;
        }
        pthread_join(tid, NULL) ;
    }
}