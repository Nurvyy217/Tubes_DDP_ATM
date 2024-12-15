#ifndef ATM_H
#define ATM_H
#include "struct.h"

// Deklarasi fungsi setter dan getter untuk Nasabah
void setUsername(Nasabah *n, const char *username);
void setNoRek(Nasabah *n, long noRek);
void setPin(Nasabah *n, int pin);
void setSaldo(Nasabah *n, int saldo);

char* getUsername(Nasabah *n);
long getNoRek(Nasabah *n);
int getPin(Nasabah *n);
int getSaldo(Nasabah *n);

// Fungsi untuk menyimpan data nasabah ke file
void simpanNasabah(FILE *file, Nasabah *n);

// Fungsi untuk membaca data nasabah dari file
int bacaNasabah(FILE *file, Nasabah *n);

float bacaSaldoATM();
void simpanSaldoATM(float saldo);
#endif
