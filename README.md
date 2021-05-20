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
![filestsv](https://i.imgur.com/xj55n53.png)

Isi folder FILES :
![FILES](https://i.imgur.com/YYhsokY.png)

Kendala :
1. Proses read file pada server gagal
2. Proses request iteratif secara bergantian gagal

### d. 
