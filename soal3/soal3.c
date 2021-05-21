#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <dirent.h>
#include <errno.h> 
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/stat.h>
#include <netinet/in.h> 
#include <sys/time.h>
#include <ctype.h>

//char buffer[10240] ;
pthread_t* tid ;
char* filePaths[10240] ;

void parseSlash(char msg[], char a[]) {
    int i = 0 ;
    char* tok ;
    while (tok = strtok_r(msg, "/", &msg)) {
        strcpy(a, tok) ;
        i++ ;
    }
}
int counter = 0 ;

void* commandF(void* arg) {
    char* pathToFile = (char*) arg ;

    DIR* testDir = opendir(pathToFile) ;
    if (testDir) {
        //printf("File %d : Sad, gagal :(\n", ++counter) ;
        closedir(testDir) ;
        return (void *) 0 ;
    }
    closedir(testDir) ;
    
    if(access(pathToFile, F_OK) == -1) {
        //printf("File %d : Sad, gagal :(\n", ++counter) ;
        return (void *) 0 ;
    }

    char fileName[64] ;
    char buffer[1024];

    bzero(buffer, 1024) ;
    strcpy(buffer, pathToFile) ;
    parseSlash(buffer, fileName) ;
    strcpy(buffer, fileName) ;

    char* ext ;
    if (strchr(buffer, '.')) {
        char* test ;
        if (test = strchr(buffer, '.') != buffer) {
            ext = strchr(buffer, '.') + 1 ;
            int i ;
            for (i = 0 ; i < strlen(ext) ; i++) {
                ext[i] = tolower(ext[i]) ;
            }
        }
        else 
            strcpy(ext, "Hidden") ;
    }
    else {
        ext = (char*) malloc (sizeof(char) * 100) ;
        strcpy(ext, "Unknown") ;
    }
    // printf("%s\n", exts) ;
    mkdir(ext, 0777);

    char currentPath[128] ;
    bzero(currentPath, 128) ;
    getcwd(currentPath, 128) ;
    strcat(currentPath, "/") ; strcat(currentPath, ext) ; strcat(currentPath, "/") ; strcat(currentPath, fileName) ;
    // printf("--\n%s\n%s\n", pathToFile, currentPath) ;
    rename(pathToFile, currentPath) ;
    //printf("File %d : Berhasil Dikategorikan\n", ++counter) ;
    return (void *) 1;
}

void* commandD(void* arg) {
    char* pathToFile = (char*) arg ;
    //printf("FILE:%s\n", pathToFile) ;
    char fileName[10240] ;
    char buffer[10240];

    bzero(buffer, 10240) ;
    strcpy(buffer, pathToFile) ;
    parseSlash(buffer, fileName) ;
    strcpy(buffer, fileName) ;

    //printf("%s", fileName);

    char* ext ;
    if (strchr(buffer, '.')) {
        char* test ;
        if (test = strchr(buffer, '.') != buffer) {
            ext = strchr(buffer, '.') + 1 ;
            //strcpy(ext, test+1);
            int i ;
            for (i = 0 ; i < strlen(ext) ; i++) {
                ext[i] = tolower(ext[i]) ;
            }
        }
        else
        {
            ext = (char*) malloc (sizeof(char) * 100) ;
            strcpy(ext, "Hidden") ;

        } 
            
    }
    else {
        ext = (char*) malloc (sizeof(char) * 100) ;
        strcpy(ext, "Unknown") ;
    }
    // printf("%s\n", exts) ;
    mkdir(ext, 0777);

    char currentPath[12800] ;
    bzero(currentPath, 12800) ;
    getcwd(currentPath, 12800) ;
    strcat(currentPath, "/") ; strcat(currentPath, ext) ; strcat(currentPath, "/") ; strcat(currentPath, fileName) ;
    //printf("--\n%s\n", currentPath) ;
    rename(pathToFile, currentPath) ;
    
}

void listFilesRecursively(char *basePath)
{
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    char filePath[10240] ;
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            strcpy(filePath, basePath);
            strcat(filePath, "/");
            strcat(filePath, dp->d_name);
            if(dp->d_type & DT_DIR) {
                listFilesRecursively(filePath);
            }
            else {
                strcpy(filePaths[counter++], filePath) ;
            }


        }
    }

    closedir(dir);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage :\n") ;
        printf("./soal3 [Option] [Path to File1] [Path to File2] ... [Path to FileX]\n") ;
        return 0 ;
    }
    if (!strcmp(argv[1], "-f")) {
        tid = (pthread_t*) malloc (sizeof(pthread_t) * (argc)) ; 
        int i ;
        for (i = 2 ; i < argc ; i++) {
            //counter++ ;
            pthread_create(&(tid[i - 2]),NULL,&commandF,argv[i]) ;
        }

        for (i = 2 ; i < argc ; i++) {
            int returnValue;
            void *ptr;
            pthread_join(tid[i - 2], &ptr) ;
            returnValue = (int) ptr;
            if (returnValue) printf("File %d: Berhasil Dikategorikan\n", i-1);
            else printf("File %d: Sad, gagal :(\n", i-1);
        }

        
    }
    else if (!strcmp(argv[1], "-d")) {
        
        tid = (pthread_t*) malloc (sizeof(pthread_t) * 1024) ;
        int i ;
        for (i = 0 ; i < 1024 ; i++) {
            filePaths[i] = (char*) malloc (sizeof(char) * 1024) ;
        }

        struct dirent *dp;
        DIR *dir = opendir(argv[2]);
        //printf("yyayaya\n");
        if (dir)
        {
            closedir(dir);
            listFilesRecursively(argv[2]) ;

            for (i = 0 ; i < counter ; i++) {
            //printf("%s\n", filePaths[i]) ;
            pthread_create(&(tid[i]),NULL,&commandD,filePaths[i]) ;
            }

            for (i = 0 ; i < counter ; i++) {
            pthread_join(tid[i], NULL) ;
            }
            printf("Direktori sukses disimpan!\n");
        }
            
        else printf("Yah, gagal disimpan :(\n");
        
        

    }
        else
        {
            tid = (pthread_t*) malloc (sizeof(pthread_t) * 1024) ;
        int i ;
        for (i = 0 ; i < 1024 ; i++) {
            filePaths[i] = (char*) malloc (sizeof(char) * 1024) ;
        }

        char ambilwd[1000];
        getcwd(ambilwd, sizeof(ambilwd));
        listFilesRecursively(ambilwd) ;
        
        for (i = 0 ; i < counter ; i++) {
            // printf("%s\n", filePaths[i]) ;
            pthread_create(&(tid[i]),NULL,&commandD,filePaths[i]) ;
            //pthread_join(tid[i], NULL) ;

        }

        for (i = 0 ; i < counter ; i++) {
            pthread_join(tid[i], NULL) ;
            }
    }
}
