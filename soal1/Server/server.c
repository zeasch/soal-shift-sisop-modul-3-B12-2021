#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/stat.h>
#include <netinet/in.h> 
#include <sys/time.h>
#define PORT 8080

char pathToServer[] = "/home/han/praktikum sisop/Prak3/server/" ;
char pathToClient[] = "/home/han/praktikum sisop/Prak3/client/" ;
int opt = 1;  
int master_socket , addrlen , new_socket , client_socket[30] , 
        max_clients = 30 , activity, i , valread , sd;  
int max_sd;
struct sockaddr_in address;  
char buffer[1025]; 
fd_set readfds;
int isLoggedIn ;
int isConnected ;
char loggedIn[100] ;
int parseMode ;

void fixQueue() {
    int i ;
    for (i = 1 ; i < max_clients ; i++) {
        client_socket[i - 1] = client_socket[i] + client_socket[i - 1] - (client_socket[i] = client_socket[i - 1]) ;
    }
}

int parse(char msg[], char a[], char b[]) {
    int i = 0 ;
    char* tok ;
    char token[5] = " " ;
    if (parseMode == 3) strcpy(token, "\t") ;
    while (tok = strtok_r(msg, token, &msg)) {
        if (i == 0)
            strcpy(a, tok) ;
        else if (i == 1)
            strcpy(b, tok) ;
        else {
            if (parseMode == 1)
                return 1 ;
            // if (parseMode == 2) {
            //     strcat(b, " ") ;
            //     strcat(b, tok) ;
            // }
            if (parseMode == 3) {
                strcpy(b, tok) ;
            }
        }

        i++ ;
    }
    return 0 ;
}

int bacaAman() {
    if ((valread = read( sd , buffer, 1024)) == 0){  
        close( sd );  
        client_socket[i] = 0;
        isLoggedIn = 0 ; isConnected = 0 ;
        fixQueue() ;  
        return 0 ;
    }
    return 1 ;
}

// Register-------------------------------------------------------------
void updateAcc(char up[]) {
    FILE* file = fopen("akun.txt", "a") ;
    fputs(up, file) ;
    fclose(file) ;
}

void* reg(void* arg) {
    strcpy(buffer, "[Register Form]\nUsage : [ID] [Password]") ;
    send(sd, buffer, strlen(buffer), 0) ;
    bzero(buffer, 1024) ;

    if(!bacaAman())  return 0 ;

    char id[30], password[30] ;
    parseMode = 1 ;
    if(parse(buffer, id, password)){
        strcpy(buffer, "Mohon masukkan form register yang valid") ;
        send(sd, buffer, strlen(buffer), 0) ;
    }
    else {
        char up[70] = {0} ;
        sprintf(up, "%s:%s\n", id, password) ;
        updateAcc(up) ;
        strcpy(buffer, "Registrasi akun sukses, mohon login terlebih dahulu") ;
        send(sd, buffer, strlen(buffer), 0) ;
    }
    bzero(buffer, 1024) ;
}
// Register-------------------------------------------------------------

// Login----------------------------------------------------------------
int checkAcc(char up[]) {
    FILE* file = fopen("akun.txt", "r") ;
    char line[80] ;
    while(fgets(line, 80, file)) {
        if (!strcmp(line, up)) {
            fclose(file) ;
            return 1 ;
        }
    }
    fclose(file) ;
    return 0 ;
}

void* login(void* arg) {
    strcpy(buffer, "[Login Form]\nUsage : [ID] [Password]") ;
    send(sd, buffer, strlen(buffer), 0) ;
    bzero(buffer, 1024) ;

    if(!bacaAman())  return 0 ;

    char id[30], password[30] ;
    parseMode = 1 ;
    if(parse(buffer, id, password)){
        strcpy(buffer, "Mohon masukkan form login yang valid, silahkan login kembali") ;
        send(sd, buffer, strlen(buffer), 0) ;
    }
    else {
        char up[70] = {0} ;
        sprintf(up, "%s:%s\n", id, password) ;
        if(checkAcc(up)) {
            strcpy(buffer, "Login sukses, Selamat datang!") ;
            isLoggedIn = 1 ;
            bzero(loggedIn, 100) ;
            sprintf(loggedIn, "(%s:%s)", id, password) ;
        }
        else {
            strcpy(buffer, "Id/Password salah, silahkan login kembali") ;
        }
        send(sd, buffer, strlen(buffer), 0) ;
    }
    bzero(buffer, 1024) ;
}
// Login----------------------------------------------------------------

// Add------------------------------------------------------------------
void* add(void* arg) {
    char up[1024] = {0} ;

    strcpy(buffer, "Publisher: ") ;
    send(sd, buffer, strlen(buffer), 0) ;
    bzero(buffer, 1024) ;

    if(!bacaAman())  return 0 ;

    strcpy(up, buffer) ;
    strcat(up, "\t") ;

    strcpy(buffer, "Tahun Publish: ") ;
    send(sd, buffer, strlen(buffer), 0) ;
    bzero(buffer, 1024) ;

    if(!bacaAman())  return 0 ;

    strcat(up, buffer) ;
    strcat(up, "\t") ;

    strcpy(buffer, "Filepath: ") ;
    send(sd, buffer, strlen(buffer), 0) ;
    bzero(buffer, 1024) ;

    if(!bacaAman())  return 0 ;

    char fileName[511] ;
    strcpy(fileName, buffer) ;
    
    strcat(up, pathToServer) ;
    strcat(up, "FILES/") ;
    strcat(up, buffer) ;
    strcat(up, "\n") ;

    FILE* file = fopen("files.tsv", "a") ;
    fputs(up, file) ;
    fclose(file) ;
    mkdir("FILES", 0777);

    char local[100] ;
    strcpy(local, pathToServer) ; strcat(local, "FILES/") ; strcat(local, buffer) ;

    FILE* file2 = fopen(local, "w") ;

    bzero(buffer, 1024) ;
    read(sd, buffer, 1024);
    fprintf(file2, "%s", buffer) ;
    fclose(file2) ;

    strcpy(buffer, "Buku berhasil ditambahkan") ;
    send(sd, buffer, strlen(buffer), 0) ;
    bzero(buffer, 1024) ;

    FILE* log = fopen("running.log", "a") ;
    fprintf(log, "Tambah : %s %s\n", fileName, loggedIn) ;
    fclose(log) ;
}
// Add------------------------------------------------------------------

// Download-------------------------------------------------------------
void* download(void* arg) {
    char* fileName = (char*) arg ;
    if(!strcmp(fileName, "")) {
        strcpy(buffer, "Masukkan nama buku") ;
        send(sd, buffer, strlen(buffer), 0) ;
        bzero(buffer, 1024) ;
    }
    else {
        bzero(buffer, 1024) ;
        strcpy(buffer, pathToServer) ;
        strcat(buffer, "FILES/") ;
        strcat(buffer, fileName) ;

        FILE* file = fopen("files.tsv", "r") ;
        char line[1024] ;
        int found = 0 ;
        while(fgets(line, 1024, file)) {
            char a[30], b[100] ;
            bzero(b, 100) ;
            parseMode = 3 ;
            char temp[100] ; strcpy(temp, line) ;
            parse(line, a, b) ;

            char path[100] ;
            strcpy(path, buffer) ;
            strcat(path, "\n") ;
            if(!strcmp(path, b)) {
                found = 1 ;
                break ;
            }
        }
        fclose(file) ;

        if (found) {
            file = fopen(buffer, "rb") ;
            char data[1024] = {0};
            while(fgets(data, 1024, file) != NULL) {
                send(sd, data, strlen(data), 0);
                bzero(data, 1024) ;
            }
            fclose(file) ;
            bzero(buffer, 1024) ;
        }
        else {
            strcpy(buffer, "Buku tidak ditemukan") ;
            send(sd, buffer, strlen(buffer), 0) ;
            bzero(buffer, 1024) ;
        }
    }
}
// Download-------------------------------------------------------------

// Delete---------------------------------------------------------------
void* delete(void* arg) {
    char* fileName = (char*) arg ;
    if(!strcmp(fileName, "")) {
        strcpy(buffer, "Masukkan nama buku") ;
    }
    else {
        char path[100] ;
        strcpy(path, pathToServer) ;
        strcat(path, "FILES/") ;
        strcat(path, fileName) ;
        FILE* test = fopen(path, "r") ;
        if (!test) {
            strcpy(buffer, "Buku tidak ditemukan") ;
        }
        else {

            FILE* file = fopen("files.tsv", "r") ;
            FILE* file2 = fopen("temp.tsv", "w") ;
            char line[1024] ;
            while(fgets(line, 1024, file)) {
                char a[30], b[100] ;
                bzero(b, 100) ;
                parseMode = 3 ;
                char temp[100] ; strcpy(temp, line) ;
                parse(line, a, b) ;

                strcpy(path, pathToServer) ;
                strcat(path, "FILES/") ;
                strcat(path, fileName) ;
                strcat(path, "\n") ;
                if(strcmp(path, b)) {
                    fprintf(file2, "%s", temp) ;
                }
            }
            fclose(file) ;
            fclose(file2) ;
            remove("files.tsv") ;
            rename("temp.tsv", "files.tsv") ;

            char old[100] ;
            strcpy(path, pathToServer) ; strcpy(old, pathToServer) ;
            strcat(path, "FILES/") ; strcat(old, "FILES/old-") ;
            strcat(path, fileName) ; strcat(old, fileName) ;

            rename(path, old) ;

            strcpy(buffer, "Buku sukses dihapus") ;
        }
        fclose(test) ;
    }
    send(sd, buffer, strlen(buffer), 0) ;
    bzero(buffer, 1024) ;

    FILE* log = fopen("running.log", "a") ;
    fprintf(log, "Hapus : %s %s\n", fileName, loggedIn) ;
    fclose(log) ;
}
// Delete---------------------------------------------------------------

// See------------------------------------------------------------------
void parseTab(char msg[], char a[], char b[], char c[]) {
    int i = 0 ;
    char* tok ;
    while (tok = strtok_r(msg, "\t", &msg)) {
        if (i == 0) strcpy(a, tok) ;
        else if (i == 1) strcpy(b, tok) ;
        else if (i == 2) strncpy(c, tok, strlen(tok) - 1) ;
        i++ ;
    }
}

void parseSlash(char msg[], char a[]) {
    int i = 0 ;
    char* tok ;
    while (tok = strtok_r(msg, "/", &msg)) {
        strcpy(a, tok) ;
        i++ ;
    }
}

void parseDot(char msg[], char a[], char b[]) {
    int i = 0 ;
    char* tok ;
    if (strchr(msg, '.')) {
        char* ext = strchr(msg, '.') + 1 ;
        strcpy(b, ext) ;
    }
    while (tok = strtok_r(msg, ".", &msg)) {
        if (i == 0) strcpy(a, tok) ;
        
        i++ ;
    }
}

void* see(void* arg) {
    FILE* file = fopen("files.tsv", "r") ;
    char line[1024] ;
    char publisher[255] = {0} ;
    char year[10] = {0} ;
    char path[511] = {0} ;
    char fileName[63] = {0} ;
    char name[50] = {0} ;
    char extension[10] = {0} ;
    char savePath[511] = {0} ;
    bzero(buffer, 1024) ;
    int awal = 0 ;
    while(fgets(line, 1024, file)) {
        if (awal) 
            strcat(buffer, "\n\n") ;
            
        parseTab(line, publisher, year, path) ;
        strcpy(savePath, path) ;
        parseSlash(path, fileName) ;
        parseDot(fileName, name, extension) ;
        strcat(buffer, "Nama: ") ; strcat(buffer, name) ;
        strcat(buffer, "\nPublisher: ") ; strcat(buffer, publisher) ;
        strcat(buffer, "\nTahun publishing: ") ; strcat(buffer, year) ;
        strcat(buffer, "\nEkstensi file: ") ; strcat(buffer, extension) ;
        strcat(buffer, "\nFile path: ") ; strcat(buffer, savePath) ;
        // printf("%s\n", buffer) ;
        

        awal++ ;
    }

    // strcpy(buffer, "\e[s\n\e[u") ;
    send(sd, buffer, strlen(buffer), 0) ;
    bzero(buffer, 1024) ;
}
// See------------------------------------------------------------------

// Find-----------------------------------------------------------------
void* find(void* arg) {
    char* fileSearchName = (char*) arg ;
    if(!strcmp(fileSearchName, "")) {
        strcpy(buffer, "Masukkan nama buku") ;
    }
    else {
        FILE* file = fopen("files.tsv", "r") ;
        char line[1024] ;
        char publisher[255] = {0} ;
        char year[10] = {0} ;
        char path[511] = {0} ;
        char fileName[63] = {0} ;
        char name[50] = {0} ;
        char extension[10] = {0} ;
        char savePath[511] = {0} ;
        bzero(buffer, 1024) ;
        
        int awal = 0 ;
        while(fgets(line, 1024, file)) {
                
            parseTab(line, publisher, year, path) ;
            strcpy(savePath, path) ;
            parseSlash(path, fileName) ;
            parseDot(fileName, name, extension) ;

            char searchName[63], searchExt[10] ;
            parseDot(fileSearchName, searchName, searchExt) ;
            if (!strstr(name, searchName)) continue ;

            if (awal) 
                strcat(buffer, "\n\n") ;

            strcat(buffer, "Nama: ") ; strcat(buffer, name) ;
            strcat(buffer, "\nPublisher: ") ; strcat(buffer, publisher) ;
            strcat(buffer, "\nTahun publishing: ") ; strcat(buffer, year) ;
            strcat(buffer, "\nEkstensi file: ") ; strcat(buffer, extension) ;
            strcat(buffer, "\nFile path: ") ; strcat(buffer, savePath) ;
            

            awal++ ;
        }

        if (!awal) {
            strcpy(buffer, "Buku tidak ditemukan") ;
        }
    }
    send(sd, buffer, strlen(buffer), 0) ;
    bzero(buffer, 1024) ;
}
// Find-----------------------------------------------------------------

// Login First----------------------------------------------------------
void* loginFirst(void* arg) {
    strcpy(buffer, "Aksi tidak dapat dilakukan, mohon login terlebih dahulu") ;
    send(sd, buffer, strlen(buffer), 0) ;
    bzero(buffer, 1024) ;
}
// Login First-----------------------------------------------------------

int main(void) {
    // Init
    for (i = 0; i < max_clients; i++){  
        client_socket[i] = 0;  
    }  
        
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0){  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  
    
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  
     
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons( PORT );  
         
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0){  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }

    if (listen(master_socket, 3) < 0) {  
        perror("listen");  
        exit(EXIT_FAILURE);  
    }  
         
    addrlen = sizeof(address) ;

    FILE* init = fopen("akun.txt", "a") ;
    fclose(init) ;

    while (1) {
        FD_ZERO(&readfds);

        FD_SET(master_socket, &readfds);  
        max_sd = master_socket;
        isLoggedIn = 0 ;

        for ( i = 0 ; i < max_clients ; i++){  
            sd = client_socket[i];  
                 
            if(sd > 0)  
                FD_SET( sd , &readfds); 

            if(sd > max_sd)  
                max_sd = sd;  
        }

        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
       
        if ((activity < 0) && (errno!=EINTR))    
            printf("select error");  
        
        if (FD_ISSET(master_socket, &readfds)){  
            new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen) ;  
            
            char message[] = "Sukses menyambungkan ke server" ;
            send(new_socket, message, strlen(message), 0) ;

            for (i = 0; i < max_clients; i++){  
                if( client_socket[i] == 0 ){  
                    client_socket[i] = new_socket;
                    break;  
                }  
            }  
        }

        sd = client_socket[0] ;
        isConnected = 1 ;
        while (isConnected) {
            bacaAman() ;

            pthread_t tid ;

            if (!strcmp(buffer, "register")) {
                pthread_create(&(tid),NULL,&reg,NULL) ;
            }
            else if (!strcmp(buffer, "login")) {
                pthread_create(&(tid),NULL,&login,NULL) ;
            }
            else if(isLoggedIn) {
                if (!strcmp(buffer, "add")) {
                    pthread_create(&(tid),NULL,&add,NULL) ;
                }
                else if (!strncmp(buffer, "download", 8)) {
                    parseMode = 2 ;
                    char a[30], fileName[100] ;
                    bzero(fileName, 100) ;
                    parse(buffer, a, fileName) ;
                    pthread_create(&(tid),NULL,&download,(void*)fileName) ;
                }
                else if (!strncmp(buffer, "delete", 6)) {
                    parseMode = 2 ;
                    char a[30], fileName[100] ;
                    bzero(fileName, 100) ;
                    parse(buffer, a, fileName) ;
                    pthread_create(&(tid),NULL,&delete,(void*)fileName) ;
                }
                else if (!strcmp(buffer, "see")) {
                    pthread_create(&(tid),NULL,&see,NULL) ;
                }
                else if (!strncmp(buffer, "find", 4)) {
                    parseMode = 2 ;
                    char a[30], fileName[100] ;
                    bzero(fileName, 100) ;
                    parse(buffer, a, fileName) ;
                    pthread_create(&(tid),NULL,&find,(void*)fileName) ;
                }
            }
            else {
                pthread_create(&(tid),NULL,&loginFirst,NULL) ;
            }

            pthread_join(tid, NULL) ;
        }
    }
}