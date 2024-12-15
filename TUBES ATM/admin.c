#include "admin.h"
#include <stdio.h>
#include "atm.h"
#include <string.h>
#include "struct.h"


// Login Admin
void loginAdmin() {
    char adminUsername[50];
    char inputUsername[50];
    int adminPassword;
    int inputPassword;
    int isAdminValid = 0;

    // Data admin (hardcoded atau gunakan file admin.txt)
    strcpy(adminUsername, "admin");
    adminPassword=1234;
	system("cls");
    printTopCenter("LOGIN ADMIN");
    printf("\t\t\t\t\t<=====================================>\n");
    printf("\n\nMasukkan Username Admin: ");
    scanf("%s", inputUsername);
    inputPin(&inputPassword);

    if (strcmp(inputUsername, adminUsername) == 0 && inputPassword==adminPassword) {
        isAdminValid = 1;
    }

    if (isAdminValid) {
    	printf("\n\n\nLogin Admin berhasil. Selamat datang, Admin.\n");
        Sleep(2000);
        menuAdmin();
    } else {
        printf("\nLogin Admin gagal! Username atau Password salah.\n");
    }
}
void menuAdmin(){
	
        // Fitur tambahan untuk admin
        system("cls");
        printTopCenter("ADMIN");
		printf("\t\t\t\t\t<=====================================>\n");
		printf("\n\n\n 1. <-- Lihat Semua Data Nasabah\t\t\t\t\t\t\t             Buka Blokir --> 4.\n\n\n");
    	printf("\n 2. <-- Hapus Semua Data Nasabah\t\t\t\t\t\t\t      Hapus Akun Nasabah --> 5.\n\n\n");
    	printf("\n 3. <-- Isi Saldo ATM\t\t\t\t\t\t\t\t                   Cek Saldo ATM --> 6.\n\n\n");
        printf("\n\nMasukkan pilihan Anda: ");
        int adminChoice;
        scanf("%d", &adminChoice);

        switch (adminChoice) {
            case 1:
                lihatSemuaNasabah();
                break;
            case 2:
                hapusSemuaNasabah();
                break;
            case 3:
            	isiSaldoATM();
            	break;
            case 4:
            	bukaBlokir();
            	break;
            case 5:
            	hapusAkunNasabah();
            	break;
            case 6:
            	cekSaldoATM();
            	break;
            default:
                printf("Pilihan tidak valid.\n");
        }
}
// Fungsi untuk menampilkan semua data nasabah
void lihatSemuaNasabah() {
	char back;
    FILE *file = fopen("datanasabah.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file database.\n");
        return;
    }
	system("cls");
    printTopCenter("DATA NASABAH");
		printf("\t\t\t\t\t<=====================================>\n\n");
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
    printf("kembali ke menu admin? (y/t): ");scanf(" %c",&back);
    if (back=='y'){
    	menuAdmin();
	}
}
// Fungsi untuk membuka blokir nasabah


//HapusSemuaNasabah
void hapusSemuaNasabah() {
	char back;
    // Buka file dengan mode "w" untuk mengosongkan isinya
    FILE *file = fopen("datanasabah.txt", "w");
    if (file == NULL) {
        printf("Gagal membuka file untuk menghapus data.\n");
        return;
    }

    // Menutup file secara langsung akan menghapus semua isinya
    fclose(file);
    printf("Semua data nasabah telah berhasil dihapus.\n");
    printf("kembali ke menu admin? (y/t): ");scanf(" %c",&back);
    if (back=='y'){
    	menuAdmin();
	}
}

void hapusAkunNasabah() {
	char back;
    char inputUsername[50];
    int found = 0;
    Nasabah bufferNasabah[100];
    int count = 0;

    // Buka file untuk membaca data
    FILE *file = fopen("datanasabah.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file untuk membaca data nasabah.\n");
        return;
    }

    // Baca semua data nasabah ke buffer
    while (bacaNasabah(file, &bufferNasabah[count])) {
        count++;
    }
    fclose(file);

    // Tampilkan prompt untuk username nasabah yang akan dihapus
    system("cls");
    printTopCenter("HAPUS AKUN NASABAH");
	printf("\t\t\t\t\t<=====================================>\n");
    printf("\n\nMasukkan username akun nasabah yang akan dihapus: ");
    scanf("%s", inputUsername);

    // Cari nasabah berdasarkan username
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(inputUsername, getUsername(&bufferNasabah[i])) == 0) {
            found = 1;
            // Geser elemen-elemen setelah akun yang akan dihapus
            int j;
            for (j = i; j < count - 1; j++) {
                bufferNasabah[j] = bufferNasabah[j + 1];
            }
            count--; // Kurangi jumlah nasabah
            break;
        }
    }

    if (found) {
        // Tulis ulang data nasabah ke file tanpa akun yang dihapus
        file = fopen("datanasabah.txt", "w");
        if (file == NULL) {
            printf("Gagal membuka file untuk menyimpan data nasabah.\n");
            return;
        }
        for (i = 0; i < count; i++) {
            simpanNasabah(file, &bufferNasabah[i]);
        }
        fclose(file);
        printf("Akun nasabah dengan username '%s' berhasil dihapus.\n", inputUsername);
    } else {
        printf("Akun dengan username '%s' tidak ditemukan.\n", inputUsername);
    }
    printf("kembali ke menu admin? (y/t): ");scanf(" %c",&back);
    if (back=='y'){
    	menuAdmin();
	}
}
	
void bukaBlokir() {
    char inputUsername[50];
    int found = 0;
    char back;
    Nasabah nasabah;
    FILE *file = fopen("datanasabah.txt", "r+"); // Buka file untuk mode baca dan tulis
    if (file == NULL) {
        printf("Gagal membuka file database.\n");
        return;
    }
	system("cls");
    printTopCenter("BUKA BLOKIR AKUN NASABAH\n");
    printf("\t\t\t\t\t<=====================================>\n");
    printf("\n\nMasukkan username nasabah yang akan dibuka blokirnya: ");
    scanf("%s", inputUsername);

    // Cari nasabah yang diblokir
    while (bacaNasabah(file, &nasabah)) {
        if (strcmp(inputUsername, getUsername(&nasabah)) == 0){
        	blokir(&nasabah,0);
        	printf("Akun %s telah dibuka blokirnya.\n", inputUsername);
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Akun tidak ditemukan atau akun tidak diblokir.\n");
    }
    printf("kembali ke menu admin? (y/t): ");scanf(" %c",&back);
    if (back=='y'){
    	menuAdmin();
	}
}      
        
        
void blokir(Nasabah *nasabah,int status) {

        setIsBlocked(&nasabah,status);

        // Baca semua data nasabah dari file
        FILE *file = fopen("datanasabah.txt", "r");
        if (file == NULL) {
            printf("Gagal membuka file untuk membaca data nasabah.\n");
            return;
        }

        // Simpan data ke buffer
        Nasabah bufferNasabah[100];
        int count = 0;
        while (bacaNasabah(file, &bufferNasabah[count])) {
            count++;
        }
        fclose(file);

        // Perbarui data nasabah yang sesuai
        int i;
        for (i = 0; i < count; i++) {
            if (strcmp(nasabah->username, bufferNasabah[i].username) == 0 &&
                nasabah->noRek == bufferNasabah[i].noRek) {
                bufferNasabah[i].isBlocked = status;
                break;
            }
        }

        // Tulis kembali semua data nasabah ke file
        file = fopen("datanasabah.txt", "w");
        if (file == NULL) {
            printf("Gagal membuka file untuk menyimpan data nasabah.\n");
            return;
        }
        for (i = 0; i < count; i++) {
            simpanNasabah(file, &bufferNasabah[i]);
        }
        fclose(file);

}
void isiSaldoATM() {
    int saldoATM, tambahanSaldo;
    char back;
    
    saldoATM = bacaSaldoATM(); // Baca saldo ATM saat ini
    system("cls");
    printTopCenter("ISI SALDO ATM\n");
    printf("\t\t\t\t\t<=====================================>\n");
    printf("\n\nSaldo ATM saat ini: %d\n", saldoATM);
    printf("Masukkan jumlah saldo yang ingin ditambahkan: ");
    scanf("%d", &tambahanSaldo);

    saldoATM += tambahanSaldo;
    simpanSaldoATM(saldoATM); // Simpan saldo ATM baru ke file
    printf("Saldo ATM berhasil ditambahkan. Saldo ATM baru: %d\n", saldoATM);
    printf("kembali ke menu admin? (y/t): ");scanf(" %c",&back);
    if (back=='y'){
    	menuAdmin();
	}
}

void cekSaldoATM() {
	char back;
    FILE *file = fopen("saldoATM.txt", "r"); // Buka file saldoATM.txt untuk membaca
    if (file == NULL) {
        printf("Gagal membuka file saldoATM.txt!\n");
        return;
    }

    int saldoATM = 0;
    fscanf(file, "%d", &saldoATM); // Membaca saldo ATM dari file
    fclose(file); // Tutup file setelah membaca

    // Tampilkan saldo ATM
    system("cls"); // Bersihkan layar (opsional)
    printTopCenter("CEK SALDO ATM\n");
    printf("\t\t\t\t\t<=====================================>\n");
    printf("\n\nSaldo ATM saat ini: Rp.%d\n", saldoATM);
    printf("kembali ke menu admin? (y/t): ");scanf(" %c",&back);
    if (back=='y'){
    	menuAdmin();
	}
}




