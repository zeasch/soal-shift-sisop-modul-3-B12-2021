# soal-shift-sisop-modul-3-B12-2021

## Soal 1
```
Untuk source code tidak ditampilkan disini karena terlalu panjang, 
silahkan merujuk ke file dalam repositori. 
Sudah disediakan segmen-segmen kode program terkait.
```
### a. Register dan Login
Register dilakukan dengan :
1. Client mengirim message "register" ke server. 
2. Setelah diterima, server akan mengirim message permintaan memasukkan Username dan Password. 
3. Lalu, Client akan mengirim username dan password. 
4. Lalu, Server akan menyimpan username dan password tersebut dalam file "akun.txt". Dan mengirimkan pesan register sukses.

Login dilakukan dengan :
1. Client mengirim message "login" ke server.
2. Setelah diterima, server akan mengirim message permintaan memasukkan Username dan Password. 
3. Lalu, Client akan mengirim username dan password. 
4. Server akan mengecek ketersediaan username dan password.
5. Jika ditemukan, Server menyimpan siapa yang sedang login dan mengirim pesan sukses login
6. Jika tidak, server akan mengirim pesan login gagal

![akun](https://i.imgur.com/3Rd2RAE.png)

Kendala :
1. Passing message dari server ke client (dan sebaliknya) terkadang gagal


### b. Membuat files.tsv dan folder FILES
Akan dibahas lebih lanjut pada beberapa sub soal berikutnya

![file](https://i.imgur.com/dCT2txM.png)

### c. Menambahkan Buku (Add)
Dapat dilakukan dengan cara :
1. Client mengirim message "add" ke server.
2. Server akan mengecek apakah sudah login atau belum
3. Jika belum akan mengirim pesan perintah login
4. Jika sudah akan meminta secara bergantian Publisher, Tahun publish, dan Path
5. Client menerima dan mengirim kembali secara bergantian dengan server.
6. Setelah selesai, client mengirim file ke server.
7. Server menerima file dan menyimpan nya dalam folder FILES dengan nama serta ekstensi yang sama
8. Server juga menulis identitas buku dalam file files.tsv
9. Server mengirim pesan "Penambahan Berhasil" kepada client.

Isi files.tsv :

![filestsv](https://i.imgur.com/SeWLuqx.png)

Isi folder FILES :

![FILES](https://i.imgur.com/YYhsokY.png)

Kendala :
1. Proses read file pada server gagal
2. Proses request iteratif secara bergantian gagal

### d. Download
Setelah file di tambahkan, tentunya kita dapat men-downloadnya.
Berikut langkah - langkahnya :
1. Client mengirim message "download namaFile.ekstensi" ke server.
2. Server akan melakukan pengecekan apakah file yang dimaksud ada didalam files.tsv
3. Jika tidak, Server akan mengirim message file tidak ditemukan.
4. Jika ada, file akan dikirim ke client.
5. Client menerima file dengan nama serta ekstensi yang sama
6. Server akan mengirimkan message file berhasil diunduh kepada client.

Kendala :
Alhamdulillah tidak ada karena proses unggah dan unduh persis dengan fungsi "add"

### e. Delete
Delete dapat dilakukan dengan cara :
1. Client mengirimkan message "delete namaFile.ekstensi" kepada seerver.
2. Sever akan mengecek apakah file tersedia.
3. Jika tidak, server akan mengirim emssage bahwa file tidak ditemukan.
4. Jika ada, server akan membuat file baru katakan "temp.tsv".
5. Semua record yang ada dipindahkan dari files.tsv ke temp.tsv kecuali file yg ingin di delete.
6. Server menghapus files.tsv.
7. Server me-rename temp.tsv menjadi files.tsv.
8. Server me-rename namaFile.ekstensi menjadi old-namaFile.ekstensi.
9. Server mengirim message bahwa file telah dihapus kepada client.

Untuk gambar hasil bisa dilihat pada sub soal c (add). Disana terdapat file dengan awalan old- dan tidak tercantum dalam files.tsv

Kendala :
1. Program sempat gagal mengecualikan pemindahan menuju temp.tsv
2. Program sempat gagal melakukan rename

### f. See (Melihat Daftar Buku)
Disini, untuk melakukan fungsi "see", dibutuhkan fungsi yang dapat memisahkan kata per kata yang dipisahkan oleh suatu karakter.
Fungsi "see" dapat dilakukan dengan cara :
1. Client mengirimkan message "see" kepada server.
2. Server akan langsung membuka files.tsv
3. Server menggunakan suatu variabel untuk menyimpan sekaligus smua yang ingin dikirim kepada client
4. Pada tiap baris, ambil data - data yang diperlukan dengan memisahkan kata per kata dengan fungsi yang telah dibuat
5. Sambungkan tiap data tiap baris dalam variabel yang sama
6. Server mengirimkan variabel tersebut kepada client

Masih dalam sistem yang sama, berikut penampakan fungsi "see" :

![see](https://i.imgur.com/7dxCErr.png)

Kendala :
1. Proses menyambungkan pesan terjadi kesalahan

### g. Find (Mencari Buku)
Client mengirimkan message "find namaFile".
Persis seperti fungsi "see", yang membedakan adalah dilakukan pengecekan terhadap nama file, apakah terdapat substring file yang dicari didalamnya.
Jika ada, akan disambungkan dengan variabel yang akan dikirim ke client. Selebihnya adalah sama dengan fungsi "see".

Tampak pula fungsi find yang hanya menampilkan buku yang mengandung substring yang diminta

![find](https://i.imgur.com/89OOzVo.png)

Kendala :
Alhamdulillah tidak ada karena cara yang digunakan persis seperti fungsi "see"

### h. Membuat log
Pada tiap fungsi "add" yang sukses, akan dilakukan penulisan running.log sebagai berikut :
```c
FILE* log = fopen("running.log", "a") ;
fprintf(log, "Tambah : %s %s\n", fileName, loggedIn) ;
fclose(log) ;
```
fileName adalah nama file dan loggedIn adalah id dan password akun yang sedang login dalam format (id:password)

![log](https://i.imgur.com/D394g5N.png)

Kendala :
Alhamdulillah tidak ada, karena cukup mudah
## Soal 2
### a.
Membuat program perkalian matriks 4x3 dan 3x6 dan menampilkan hasilnya lalu matriks hasil dikirim ke program kedua.<br><br>
```c
int y1=4,x1=3,y2=3,x2=6;
int mat1[6][6];//input mat1
int mat2[6][6];//input mat2
int *matrix;
int segid;
key_t key=1234;
segid=shmget(key,sizeof(int)*x2*y1,IPC_CREAT|0666);
matrix = (int *)shmat(segid,NULL,0);
```
deklarasi variabel-variabel dan juga array matrix yang akan digunakan untuk menyimpan hasil perkalian matriks dan mengirimnya ke program kedua.<br><br>

```c
for(int i=0;i<y1;i++)
   	for(int j=0;j<x1;j++)
       	scanf("%d",&mat1[i][j]);

for(int i=0;i<y2;i++)
   	for(int j=0;j<x2;j++)
       	scanf("%d",&mat2[i][j]);
```
menginput matriks pertama dan kedua.<br><br>
```c
for(int i=0;i<y1;i++)
   	for(int j=0;j<x2;j++){
       	matrix[i*x2+j]=0;
       	for(int k=0;k<x1;k++)
           	matrix[i*x2+j]+=mat1[i][k]*mat2[k][j];
       	}
printf("matriks hasil perkalian\n");
for(int i=0;i<y1;i++){
	for(int j=0;j<x2;j++){
       	printf("%d\t",matrix[i*x2+j]);
       	}
   	printf("\n");
    	}
```
melakukan perkalian dan menunjukkan hasilnya. Setelah itu program di sleep selama 30 detik untuk memberi waktu untuk inputan program kedua.<br><br>
Kendala :
Mencari cara mengirim sebuah matriks melalui shared memory
### b.
membuat program yang menerima matriks hasil program pertama dan selanjutnya melakukan perhitungan dengan matriks inputan baru.<br><br>
```c
key_t key=1234;
	int *shmatrix;
	int segid=shmget(key,sizeof(int)*4*6,IPC_CREAT|0666);
	shmatrix=(int *)shmat(segid,NULL,0);
	int inmatrix[30];
```
deklarasi array inputan,array dari shared memory dan key yang sama dengan program pertama.<br><br>
```c

	for(int i=0;i<4;i++){
    	for(int j=0;j<6;j++){
            	printf("%d\t",shmatrix[i*6+j]);
    	}
    	printf("\n");
    	}

	for(int i=0;i<4;i++)
       	for(int j=0;j<6;j++)
           	scanf("%d",&inmatrix[i*6+j]);
```
menunjukkan matriks yang diterima dari program pertama dan juga melakukan input matriks untuk perhitungan.<br><br>
```c
int count=0;
	while(count<25)
	{
	a=shmatrix[ii*6+jj];
	b=inmatrix[ii*6+jj];
	pthread_create(&(tid[count]),NULL,&fact,NULL);
	usleep(500);
	if(jj==5)
    	{
    	ii++;
    	jj=0;
    	}
	else
    	jj++;

	count++;
	}
```
membuat thread untuk perhitungan setiap cell matriks, terdapat 24 cell sehingga membutuhkan 24 thread. argumen untuk fungsi fact menggunakan variabel global. variabel ii dan jj berawal dengan nilai 0 dan digunakan untuk menunjuk anggota array yang akan digunakan.<br><br>
```c
void* fact(void *arg){
int loca=a,locb=b,locii=ii,locjj=jj;
	long long int hasil=loca;
	if(loca!=0&&locb!=0){
	if(loca>=locb)
    	{
    	int j=1;
    	for(int i=loca-1;j<locb;j++,i--)
        	{
        	hasil=hasil*i;
        	}
    	matrixnew[locii][locjj]=hasil;
    	}
	else if(locb>loca)
    	{
    	for(int i=loca-1;i>0;i--)
        	{
        	hasil=hasil*i;
        	}
    	matrixnew[locii][locjj]=hasil;
    	}
	}
	else matrixnew[locii][locjj]=0;

}
```
nilai-nilai variabel global disimpan ke variabel local dan selanjutnya dilakukan perhitungannya.<br><br>
```c
for(int i=0;i<25;i++)
    	pthread_join(tid[i],NULL);

	printf("hasil\n");
	for(int i=0;i<4;i++){
    	for(int j=0;j<6;j++){
        	printf("%lld\t\t",matrixnew[i][j]);
    	}
    	printf("\n");
	}
	shmdt(shmatrix);
	shmctl(segid,IPC_RMID,NULL);
}
```
dilakukan pthread_join agar memastikan semua thread telah selesai lalu matriks hasil perhitungan ditunjukkan.<br>
Kendala :<br>
1. Mencari cara mengirim argumen ke fungsi thread tanpa harus menggunakan struct.<br>
2. Segmentation fault

### c.
Membuat program yang melakukan perintah ```ps aux | sort -nrk 3,3 | head -5``` menggunakan IPC pipes
```c
int main(){
  pipe(pipe1);
  pipe(pipe2);
 if (fork()==0)
	one();
 if (fork()==0)
	two();
  close(pipe1[0]);
  close(pipe1[1]);
  if (fork()==0)
	three();
}
```
membuat tiga fungsi yang menjalankan exec yang berisi perintah-perintah yang dibutuhkan. 
```c
void one(){
  dup2(pipe1[1], 1);
  close(pipe1[0]);
  close(pipe1[1]);
  execlp("ps", "ps", "aux",NULL);
}
void two(){
  dup2(pipe1[0], 0);
  dup2(pipe2[1], 1);
  close(pipe1[0]);
  close(pipe1[1]);
  close(pipe2[0]);
  close(pipe2[1]);
  execlp("sort","sort","-nrk","3,3", NULL);
}
void three(){
  dup2(pipe2[0], 0);
  close(pipe2[0]);
  close(pipe2[1]);
  execlp("head", "head","-5", NULL);
}
```
fungsi ```dup2()``` dapat mengarahkan input dan output. ```dup2(pipe,1)``` berarti mengarahkan output ke pipe dan ```dup2(pipe,0)``` berarti mendapatkan input dari pipe,<br> 
Kendala :
Tidak ada, karena mirip dengan latihan soal ke-2
## Soal 3

### a. Argumen -f
Penjelasan:\
Jika file tidak bisa dibuka atau file tidak ada maka akan menampilkan
output gagal
```c
    DIR* testDir = opendir(pathToFile) ;
    if (testDir) {
        closedir(testDir) ;
        return (void *) 0 ;
    }
    closedir(testDir) ;
    
    if(access(pathToFile, F_OK) == -1) {
        return (void *) 0 ;
    }
```

Mengambil extension file dari titik '.' terdepan. Jika titik terletak
di depan maka file tersebut termasuk 'Hidden', selain itu jika
file tidak memiliki extension maka file tersebut termasuk 'Unknown'
```c
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
        {
            ext = (char*) malloc (sizeof(char) * 100) ;
            strcpy(ext, "Hidden") ;
        } 
    }
    else {
        ext = (char*) malloc (sizeof(char) * 100) ;
        strcpy(ext, "Unknown") ;
    }
```

Membuat folder dengan nama extension dari file dan memindahkan file
tersebut ke folder yang sesuai dengan extension nya
```c
    mkdir(ext, 0777);
    
    char currentPath[12800] ;
    bzero(currentPath, 12800) ;
    getcwd(currentPath, 12800) ;
    strcat(currentPath, "/") ; strcat(currentPath, ext) ; strcat(currentPath, "/") ; strcat(currentPath, fileName) ;
    rename(pathToFile, currentPath) ;
    return (void *) 1;
```

Membuat thread
```c
    if (!strcmp(argv[1], "-f")) {
        tid = (pthread_t*) malloc (sizeof(pthread_t) * (argc)) ; 
        int i ;
        for (i = 2 ; i < argc ; i++) {
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
```

Hasil:\
Sebelum
![3a1](https://imgur.com/jJgw1pg.png)\
Sesudah\
![3a2](https://imgur.com/jxuUGdD.png)
![3a3](https://imgur.com/ip3ioKz.png)

Kendala:
1. Output tidak urut sesuai urutan argumen
2. Output 'File %d' kurang benar
3. Kesalahan pada conditional if-else di fungsi main
4. Segmentation fault

### b. Argumen -d
Penjelasan:\
Mengambil semua file di dalam sebuah direktori secara rekursif
```c
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
```

Mengambil extension file dari titik '.' terdepan. Jika titik terletak
di depan maka file tersebut termasuk 'Hidden', selain itu jika
file tidak memiliki extension maka file tersebut termasuk 'Unknown'.
```c
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
        {
            ext = (char*) malloc (sizeof(char) * 100) ;
            strcpy(ext, "Hidden") ;
        } 
    }
    else {
        ext = (char*) malloc (sizeof(char) * 100) ;
        strcpy(ext, "Unknown") ;
    }
```

Membuat folder dengan nama extension dari file dan memindahkan file
tersebut ke folder yang sesuai dengan extension nya.
```c
    mkdir(ext, 0777);

    char currentPath[12800] ;
    bzero(currentPath, 12800) ;
    getcwd(currentPath, 12800) ;
    strcat(currentPath, "/") ; strcat(currentPath, ext) ; strcat(currentPath, "/") ; strcat(currentPath, fileName) ;
    rename(pathToFile, currentPath) ;
```

Membuat thread
```c
    else if (!strcmp(argv[1], "-d")) {
        
        tid = (pthread_t*) malloc (sizeof(pthread_t) * 1024) ;
        int i ;
        for (i = 0 ; i < 1024 ; i++) {
            filePaths[i] = (char*) malloc (sizeof(char) * 1024) ;
        }

        struct dirent *dp;
        DIR *dir = opendir(argv[2]);
        if (dir)
        {
            closedir(dir);
            listFilesRecursively(argv[2]) ;

            for (i = 0 ; i < counter ; i++) {
            pthread_create(&(tid[i]),NULL,&commandD,filePaths[i]) ;
            }

            for (i = 0 ; i < counter ; i++) {
            pthread_join(tid[i], NULL) ;
            }
            printf("Direktori sukses disimpan!\n");
        }
            
        else printf("Yah, gagal disimpan :(\n");
    }
```
Hasil:\
Sebelum
![3b1](https://imgur.com/K8ujVHk.png)\
Sesudah\
![3b2](https://imgur.com/P8aq72A.png)
![3b3](https://imgur.com/0L5M6nT.png)

Kendala:
1. Beberapa file tidak terkategorikan
2. Program tidak dapat berjalan secara paralel
3. Segmentation fault

### c. Argumen \\*
Penjelasan:\
Mengambil semua file di dalam working directory secara rekursif
```c
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
```

Mengambil extension file dari titik '.' terdepan. Jika titik terletak
di depan maka file tersebut termasuk 'Hidden', selain itu jika
file tidak memiliki extension maka file tersebut termasuk 'Unknown'.
```c
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
        {
            ext = (char*) malloc (sizeof(char) * 100) ;
            strcpy(ext, "Hidden") ;
        } 
    }
    else {
        ext = (char*) malloc (sizeof(char) * 100) ;
        strcpy(ext, "Unknown") ;
    }
```

Membuat folder dengan nama extension dari file dan memindahkan file
tersebut ke folder yang sesuai dengan extension nya.
```c
    mkdir(ext, 0777);

    char currentPath[12800] ;
    bzero(currentPath, 12800) ;
    getcwd(currentPath, 12800) ;
    strcat(currentPath, "/") ; strcat(currentPath, ext) ; strcat(currentPath, "/") ; strcat(currentPath, fileName) ;
    rename(pathToFile, currentPath) ;
```

Membuat thread
```c
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
            pthread_create(&(tid[i]),NULL,&commandD,filePaths[i]) ;
        }

        for (i = 0 ; i < counter ; i++) {
            pthread_join(tid[i], NULL) ;
        }
    }
```

Hasil:\
Sebelum
![3c1](https://imgur.com/yI0xTcJ.png)\
Sesudah\
![3c2](https://imgur.com/ZEPogKU.png)
![3c3](https://imgur.com/es9ngC6.png)

Kendala:
1. Beberapa file tidak terkategorikan
2. Program tidak dapat berjalan secara paralel
3. Segmentation fault
