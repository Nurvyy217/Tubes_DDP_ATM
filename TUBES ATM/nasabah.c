#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "atm.h"
#include <windows.h>
#include "nasabah.h"
#include "admin.h"


// Fungsi untuk registrasi nasabah baru
void registrasi() {
    Nasabah nasabahBaru;
    char username[50];
    int pin, konfirmasiPin;
    long noRek;

	retry:
    printTopCenter("BUAT AKUN");
    printf("\t\t\t\t\t<=====================================>\n");
    printf("\n\nMasukkan username: ");
    scanf("%s", username);
    if (cekUsernameSudahAda(username)) {
        printf("Username sudah digunakan. Silakan pilih username lain.\n");
        Sleep(2000);
        system("cls");
        goto retry;
        return;
    }
    inputPin(&pin);
    printf("Konfirmasi PIN: ");
    inputPin(&konfirmasiPin);

    if (pin == konfirmasiPin) {
        // Membuat nomor rekening secara random
        srand(time(NULL));
        noRek = 1000000000 + rand() % 9000000000;

        // Set data nasabah
        setUsername(&nasabahBaru, username);
        setNoRek(&nasabahBaru, noRek);
        setPin(&nasabahBaru, pin);
        setSaldo(&nasabahBaru, 0);
        setIsBlocked(&nasabahBaru, 0);

        // Simpan data ke file
        FILE *file = fopen("datanasabah.txt", "a");
        if (file == NULL) {
            printf("Gagal membuka file untuk menyimpan data.\n");
            return;
        }
        simpanNasabah(file, &nasabahBaru);
        fclose(file);

        printf("Selamat! Anda berhasil membuat akun.\n");
        printf("Nomor rekening Anda adalah: %ld\n", nasabahBaru.noRek);
    } else {
        printf("PIN tidak cocok! Silakan coba lagi.\n");
    }
}

int cekUsernameSudahAda(const char *username) {
    FILE *file = fopen("datanasabah.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file untuk memeriksa username.\n");
        return 0; // Jika file tidak ada, dianggap username belum ada
    }

    Nasabah nasabah;
    while (bacaNasabah(file, &nasabah)) {
        if (strcmp(username, nasabah.username) == 0) {
            fclose(file);
            return 1; // Username sudah ada
        }
    }

    fclose(file);
    return 0; // Username belum ada
}

// Fungsi untuk login nasabah
void loginNasabah() {
    char inputUsername[50];
    int pinlogin, salahPinCount = 0;
    Nasabah nasabah;
    int found = 0;

    printTopCenter("LOGIN NASABAH");
    printf("\t\t\t\t\t<=====================================>\n");
    printf("\n\nMasukkan username: ");
    scanf("%s", inputUsername);

    // Buka file untuk membaca data nasabah
    FILE *file = fopen("datanasabah.txt", "r+"); // Gunakan mode "r+" untuk dapat menulis kembali
    if (file == NULL) {
        printf("Gagal membuka file! Pastikan file datanasabah.txt ada.\n");
        return;
    }

    // Cek data nasabah di file
    while (bacaNasabah(file, &nasabah)) {
        if (strcmp(inputUsername, getUsername(&nasabah)) == 0) {
            if (nasabah.isBlocked==1) {
                printf("Akun Anda diblokir! Silakan hubungi admin.\n");
                found = 1;
                break;
            }

            // Memulai proses verifikasi PIN
            while (salahPinCount < 3) {
                inputPin(&pinlogin);
                if (pinlogin == getPin(&nasabah)) {
                    printf("Login berhasil! Selamat datang, %s.\n", getUsername(&nasabah));
                    tarikSaldo(&nasabah);
                    found = 1;
                    break;
                } else {
                    salahPinCount++;
                    printf("PIN salah! Coba lagi (%d/3)\n", salahPinCount);
                }
            }

            // Jika tiga kali salah PIN, blokir akun
            if (salahPinCount == 3) {
            	found=1;
            	printf("\nAkun Anda Terblokir silahkan hubungi admin!");
                blokir(&nasabah,1);
            }

            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Login gagal! Username tidak ditemukan atau PIN salah.\n");
    }
}


void tarikSaldo(Nasabah *nasabah) {
	char back;
	int choice;
    int jumlahTarikSaldo,saldoATM;
    saldoATM = bacaSaldoATM();
    start:
    system("cls");
    printTopCenter("TARIK SALDO");
    printf("\t\t\t\t\t<=====================================>\n");
    printf("\n\n 1. <-- 100.000\t\t\t\t\t\t\t\t\t\t\t       1.000.000 --> 4.\n\n\n");
    printf("\n 2. <-- 200.000\t\t\t\t\t\t\t\t\t\t\t       1.500.000 --> 5.\n\n\n");
    printf("\n 3. <-- 300.000\t\t\t\t\t\t\t\t\t\t\t     JUMLAH LAIN --> 6.\n\n\n");
    printf("\n 7. <-- SETOR TUNAI\t\t\t\t\t\t\t\t\t\t    MENU LAINNYA --> 8.\n\n\n");
    printf("\nPILIHAN: ");scanf("%d",&choice);
    switch(choice){
    	case 1:
    		jumlahTarikSaldo=100000;
    		break;
    	case 2:
    		jumlahTarikSaldo=200000;
    		break;
    	case 3:
    		jumlahTarikSaldo=300000;
    		break;
    	case 4:
    		jumlahTarikSaldo=1000000;
    		break;
    	case 5:
    		jumlahTarikSaldo=1500000;
    		break;
    	case 6:
    		printf("Masukkan jumlah saldo yang Anda ingin tarik: ");scanf("%d",&jumlahTarikSaldo);
    		printf("Penarikan sebesar Rp.%d berhasil",jumlahTarikSaldo);
    		break;
    	case 7:
    		setorTunai(nasabah);
    		return;
    	case 8:
    		menuLainnya(nasabah);
    		return;
    	default:
    		printf("masukkan angka yang ada di pilihan");
    		return;
	}

    if (jumlahTarikSaldo > 0) {
        if (nasabah->saldo >= jumlahTarikSaldo) {
        	if (saldoATM >= jumlahTarikSaldo){
        		nasabah->saldo -= jumlahTarikSaldo;
            	saldoATM -= jumlahTarikSaldo;
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
	                    bufferNasabah[i].saldo = nasabah->saldo;
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
	            simpanSaldoATM(saldoATM);
	            system("cls");
	            printTopCenter("Sedang diproses, harap tunggu sebentar!");
	            Sleep(3000);
	            system("cls");
				printTopCenter("Penarikan sebesar Rp.%d berhasil!",jumlahTarikSaldo);
				printf("\n\ningin tarik saldo lagi? (y/t): ");scanf(" %c",&back);
			    if (back=='y'){
			    	goto start;
				}
				} else {
					system("cls");
	        		printTopCenter("Penarikan gagal! Saldo ATM tidak mencukupi.\n");
	        		printf("Saldo ATM saat ini: Rp.%d\n", saldoATM);
	        		printf("\nkembali ke menu tarik saldo? (y/t): ");scanf(" %c",&back);
				    if (back=='y'){
				    	goto start;
					}
    			} 
			} else {
				system("cls");
	            printTopCenter("Saldo tidak mencukupi untuk melakukan penarikan.\n");
	            printf("\n\nkembali ke menu tarik saldo? (y/t): ");scanf(" %c",&back);
			    if (back=='y'){
			    	goto start;
				}
        	}
	    } else {
	    	system("cls");
	        printTopCenter("Jumlah saldo yang ditarik harus lebih dari 0.\n");
	        printf("\n\nkembali ke menu tarik saldo? (y/t): ");scanf(" %c",&back);
			    if (back=='y'){
			    	goto start;
				}
    	}
}
void setorTunai(Nasabah *nasabah) {
	char back;
    int jumlahIsiSaldo;
    system("cls");
    printTopCenter("SETOR TUNAI");
    printf("\t\t\t\t\t<=====================================>\n");
    printf("\n\nMasukkan jumlah saldo yang ingin diisi: ");
    scanf("%d", &jumlahIsiSaldo);

    if (jumlahIsiSaldo > 0) {
        // Perbarui saldo nasabah
        nasabah->saldo += jumlahIsiSaldo;

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
                bufferNasabah[i].saldo = nasabah->saldo;
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
        printf("Saldo berhasil diisi. Saldo Anda: %d\n", nasabah->saldo);
    } else {
        printf("Jumlah saldo yang diisi harus lebih dari 0.\n");
    }
    printf("kembali ke menu tarik saldo? (y/t): ");scanf(" %c",&back);
    if (back=='y'){
    	tarikSaldo(nasabah);
	}
}
void menuLainnya(Nasabah *nasabah){
	int pilih;

	system("cls");
	printTopCenter("MENU LAINNYA");
	printf("\t\t\t\t\t<=====================================>\n");
    printf("\n\n\n 1. <-- UBAH PIN\t\t\t\t\t\t\t\t\t  PEMBELIAN / PEMBAYARAN --> 4.\n\n\n");
    printf("\n 2. <-- CEK SALDO\t\t\t\t\t\t\t\t\t\t\t KEMBALI --> 5.\n\n\n");
    printf("\n 3. <-- TRANSFER\n\n\n");
    printf("\n PILIHAN: ");scanf("%d",&pilih);
    switch(pilih){
    	case 1:
    		ubahPin(nasabah);
    		break;
    	case 2:
    		cekSaldo(nasabah);
    		break;
    	case 3:
    		transfer(nasabah);
    		break;
    	case 4:
    		pembelian(nasabah);
    		break;
    	case 5:
    		printf("Kembali ke menu utama");
    		tarikSaldo(nasabah);
    		break;
		default:
			printf("belum ada");
	}
}
void ubahPin(Nasabah *nasabah) {
	char back;
    int pinBaru, konfirmasiPin;
	system("cls");
    printTopCenter("UBAH PIN");
    printf("\t\t\t\t\t<=====================================>\n");
    printf("\n\nMASUKKAN PIN BARU\n");
    inputPin(&pinBaru);
    printf("KONFIRMASI PIN BARU\n");
    inputPin(&konfirmasiPin);

    if (pinBaru == konfirmasiPin) {
        // Perbarui PIN di struct nasabah
        nasabah->pin = pinBaru;

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
                bufferNasabah[i].pin = nasabah->pin;
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

        printf("PIN berhasil diperbarui.\n");
    } else {
        printf("PIN baru dan konfirmasi PIN tidak cocok. Silakan coba lagi.\n");
    }
    printf("kembali ke menu lainnya? (y/t): ");scanf(" %c",&back);
    if (back=='y'){
    	menuLainnya(nasabah);
	}
}
void cekSaldo(Nasabah *nasabah) {
	char back;
	system("cls");
	printTopCenter("INFORMASI SALDO");
    printf("\t\t\t\t\t<=====================================>\n");
    printf("\n\nUsername: %s\n", getUsername(nasabah));
    printf("Nomor Rekening: %ld\n", getNoRek(nasabah));
    printf("Saldo Anda: %d\n", getSaldo(nasabah));
    printf("kembali ke menu lainnya? (y/t): ");scanf(" %c",&back);
    if (back=='y'){
    	menuLainnya(nasabah);
	}
}
void transfer(Nasabah *nasabah) {
	char back;
    char tujuanUsername[50];
    long tujuanNoRek;
    int jumlahTransfer;
    int found = 0;
    Nasabah penerima;
	system("cls");
    printTopCenter("TRANSFER SALDO");
    printf("\t\t\t\t\t<=====================================>\n");
    printf("\n\nMasukkan username penerima: ");
    scanf("%s", tujuanUsername);
    printf("Masukkan nomor rekening penerima: ");
    scanf("%ld", &tujuanNoRek);
    printf("Masukkan jumlah transfer: ");
    scanf("%d", &jumlahTransfer);

    // Cek apakah saldo cukup
    if (jumlahTransfer > getSaldo(nasabah)) {
        printf("Saldo tidak mencukupi untuk transfer sebesar %d.\n", jumlahTransfer);
        return;
    }

    // Buka file untuk membaca data nasabah
    FILE *file = fopen("datanasabah.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file! Pastikan file datanasabah.txt ada.\n");
        return;
    }

    // Baca data nasabah dan cari penerima
    FILE *tempFile = fopen("temp.txt", "w"); // File sementara untuk update data
    if (tempFile == NULL) {
        printf("Gagal membuat file sementara.\n");
        fclose(file);
        return;
    }

    while (bacaNasabah(file, &penerima)) {
        if (strcmp(getUsername(&penerima), tujuanUsername) == 0 && getNoRek(&penerima) == tujuanNoRek) {
            found = 1;
            setSaldo(&penerima, getSaldo(&penerima) + jumlahTransfer); // Tambahkan saldo ke penerima
        }

        // Simpan data nasabah ke file sementara
        simpanNasabah(tempFile, &penerima);
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        // Kurangi saldo pengirim
        setSaldo(nasabah, getSaldo(nasabah) - jumlahTransfer);

        // Update file dengan saldo pengirim
        tempFile = fopen("temp.txt", "r");
        file = fopen("datanasabah.txt", "w");

        if (file == NULL || tempFile == NULL) {
            printf("Gagal memperbarui data nasabah.\n");
            if (file) fclose(file);
            if (tempFile) fclose(tempFile);
            return;
        }

        while (bacaNasabah(tempFile, &penerima)) {
            if (strcmp(getUsername(&penerima), getUsername(nasabah)) == 0) {
                // Simpan data pengirim yang diperbarui
                simpanNasabah(file, nasabah);
            } else {
                // Simpan data lainnya
                simpanNasabah(file, &penerima);
            }
        }

        fclose(file);
        fclose(tempFile);
        remove("temp.txt"); // Hapus file sementara

        printf("Transfer berhasil sebesar %d ke %s (Rek: %ld).\n", jumlahTransfer, tujuanUsername, tujuanNoRek);
    } else {
        printf("Transfer gagal! Penerima dengan username dan nomor rekening tersebut tidak ditemukan.\n");
        remove("temp.txt"); // Hapus file sementara
    }
    printf("kembali ke menu lainnya? (y/t): ");scanf(" %c",&back);
    if (back=='y'){
    	menuLainnya(nasabah);
	}
}
// Modul pembelian
void pembelian(Nasabah *nasabah) {
	char back;
	int choice;
    int jumlah=0;
    int noPelanggan=0;
    
    system("cls");
    printTopCenter("PEMBELIAN / PEMBAYARAN");
    printf("\t\t\t\t\t<=====================================>\n");
    printf("\n\n\n 1. <-- TAGIHAN LISTRIK\t\t\t\t\t\t\t\t\t\t      BELI PULSA --> 4.\n\n\n");
    printf("\n 2. <-- TAGIHAN AIR\t\t\t\t\t\t\t\t\t\t      BAYAR BPJS --> 5.\n\n\n");
    printf("\n 3. <-- TOP E-MONEY\t\t\t\t\t\t\t\t\t\t\t KEMBALI --> 6.\n\n\n");
    printf("\nPILIHAN: ");scanf("%d",&choice);
    switch(choice){
    	case 1:
    		system("cls");
    		printTopCenter("TAGIHAN LISTRIK\n");
    		printf("\t\t\t\t\t<=====================================>\n");
    		printf("\n\nMasukkan Nomor Pelanggan: ");scanf("%d",&noPelanggan);
    		printf("Masukkan Jumlah tagihan: Rp. ");scanf("%d",&jumlah);
    		break;
    	case 2:
    		system("cls");
    		printTopCenter("TAGIHAN AIR\n");
    		printf("\t\t\t\t\t<=====================================>\n");
    		printf("\n\nMasukkan Nomor Pelanggan: ");scanf("%d",&noPelanggan);
    		printf("Masukkan Jumlah tagihan: Rp. ");scanf("%d",&jumlah);
    		break;
    	case 3:
    		system("cls");
    		printTopCenter("TOP-UP E-MONEY\n");
    		printf("\t\t\t\t\t<=====================================>\n");
    		printf("\n\nMasukkan ID  E-money: ");scanf("%d",&noPelanggan);
    		printf("Masukkan Jumlah nominal Top-up: Rp. ");scanf("%d",&jumlah);
    		break;
    	case 4:
    		system("cls");
    		printTopCenter("ISI PULSA\n");
    		printf("\t\t\t\t\t<=====================================>\n");
    		printf("\n\nMasukkan Nomor HP: ");scanf("%d",&noPelanggan);
    		printf("Masukkan Jumlah Nominal Topup Rp. ");scanf("%d",&jumlah);
    		break;
    	case 5:
    		system("cls");
    		printTopCenter("BAYAR BPJS\n");
    		printf("\t\t\t\t\t<=====================================>\n");
    		printf("\n\nMasukkan Nomor BPJS: ");scanf("%d",&noPelanggan);
    		printf("Masukkan Jumlah nominal: Rp. ");scanf("%d",&jumlah);
    		break;
    	case 6:
    		printf("kembali ke menu utama");
    		menuLainnya(nasabah);
    		
    	default:
    		printf("masukkan angka yang ada di pilihan");
    		return;
	}

    if (jumlah > 0) {
        if (nasabah->saldo >= jumlah) {
            // Kurangi saldo nasabah
            nasabah->saldo -= jumlah;

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
                    bufferNasabah[i].saldo = nasabah->saldo;
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
			printf("Pembelian / Pembayaran sebesar Rp.%d berhasil!",jumlah);
            
        }
		 else {
            printf("Saldo tidak mencukupi untuk melakukan pembelian.\n");
        }
    } else {
        printf("Jumlah saldo yang ditarik harus lebih dari 0.\n");
    }
    printf("\nkembali ke menu pembelian/pembayaran? (y/t): ");scanf(" %c",&back);
    if (back=='y'){
    	pembelian(nasabah);
	}
}




