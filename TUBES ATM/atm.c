#include <stdio.h>
#include "atm.h"
// Fungsi setter untuk Nasabah
void setUsername(Nasabah *n, const char *username) {
    strcpy(n->username, username);
}

void setNoRek(Nasabah *n, long noRek) {
    n->noRek = noRek;
}

void setPin(Nasabah *n, int pin) {
    n->pin = pin;
}

void setSaldo(Nasabah *n, int saldo) {
    n->saldo = saldo;
}
void setIsBlocked(Nasabah *n, int isBlocked){
	n->isBlocked = isBlocked;
}

// Fungsi getter untuk Nasabah
char* getUsername(Nasabah *n) {
    return n->username;
}

long getNoRek(Nasabah *n) {
    return n->noRek;
}

int getPin(Nasabah *n) {
    return n->pin;
}

int getSaldo(Nasabah *n) {
    return n->saldo;
}
int getIsBlocked(Nasabah *n){
	return n->isBlocked;
}

// Fungsi untuk menyimpan data nasabah ke file
void simpanNasabah(FILE *file, Nasabah *n) {
    fprintf(file, "Username: %s\n", n->username);
    fprintf(file, "No Rekening: %ld\n", n->noRek);
    fprintf(file, "PIN: %d\n", n->pin);
    fprintf(file, "Saldo: %d\n", n->saldo);
    fprintf(file, "Status: %d\n\n", n->isBlocked);
}

// Fungsi untuk membaca data nasabah dari file
int bacaNasabah(FILE *file, Nasabah *n) {
    return fscanf(file, "Username: %s\n", n->username) != EOF &&
           fscanf(file, "No Rekening: %ld\n", &n->noRek) &&
           fscanf(file, "PIN: %d\n", &n->pin) &&
           fscanf(file, "Saldo: %d\n\n", &n->saldo) &&
           fscanf(file, "Status: %d\n\n", &n->isBlocked);
}
float bacaSaldoATM() {
    FILE *file = fopen("saldoATM.txt", "r");
    float saldo = 0.0;

    if (file != NULL) {
        fscanf(file, "%f", &saldo); // Membaca saldo dari file
        fclose(file);
    } else {
        saldo = 1000000.0; // Default saldo jika file tidak ada
        printf("File saldoATM.txt tidak ditemukan. Menggunakan saldo default: Rp %.2f\n", saldo);
    }

    return saldo;
}
void simpanSaldoATM(float saldo) {
    FILE *file = fopen("saldoATM.txt", "w");

    if (file != NULL) {
        fprintf(file, "%.2f", saldo); // Menulis saldo ke file
        fclose(file);
    } else {
        printf("Gagal menyimpan saldo ke file.\n");
    }
}


