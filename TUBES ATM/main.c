#include <stdio.h>
#include <windows.h>
#include "admin.h"
#include "styleText.h"
#include "nasabah.h"

int main() {
    char kembali,choice1,choice2;
    start:
    system("color 1F");
    printCentered("Selamat Datang di ATM!");
    Sleep(3000);
    system("cls");
    printTopCenter("1.LOGIN / 2.REGISTRASI");
    printf("\t\t\t\t\t<=====================================>\n");
    printf("\n\n\n\nPilihan: ");scanf(" %c", &choice1);
	system("cls");
    if (choice1 == '1') {
    	printTopCenter("1.ADMIN / 2.NASABAH");
    	printf("\t\t\t\t\t<=====================================>\n");
    	printf("\n\n\n\nPilihan: ");scanf(" %c",&choice2);
    	system("cls");
    	if(choice2 == '1'){
    		loginAdmin();
		}
        else if(choice2 == '2'){
        	loginNasabah();
	}
    } else if (choice1 == '2') {
    	system("cls");
        registrasi();
    } else {
        printf("Pilihan tidak valid. Silakan jalankan ulang program.\n");
    }
    printBottomLeftCorner("tekan 1 untuk kembali ke menu awal: ");scanf(" %c",&kembali);
    if(kembali=='1'){
    system("cls");
    goto start;	
	}
    return 0;
}
