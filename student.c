                                                    //Hausaufgabe AES programmieren
// OPTIONS: -O2 -std=gnu99
#include "student.h"
#include <inttypes.h>

void *aes128_init(void *key)                        //Die vorgegebene initialisierungsfunktion reicht den key nur weiter.
{                                                   //Die möglichkeit von precomputation wird nicht genutzt
	return key;                                     //Den Key weitergeben und jedesmal neu rechnen. einfacher als malloc
}

                                                    //Deklaration aller funktionen
uint8_t sBoxLookup(uint8_t input);
uint8_t rConLookup( uint8_t input);
uint8_t xtime(uint8_t input);

void aes128_encrypt(void *buffer, void *param)      //die "eigentliche" Funktion. Hier wird der gesammte AES gemanaged.
{                                                   //zunächst werden alle verwendeten Variablen initialisiert
	uint8_t key[176]; 	                            //ein key ist [16] => man braucht für 11 runden 176
	uint8_t *mes;                                   //Da buffer nur die zu verschlüsselnde Nachricht enthält wird buffer umbenannt.
	mes = buffer;                                   //Es macht Sinn die Nachricht nicht "neu" anzulegen, sondern mit dem pointer auf
	for (int i=0;i<16;i++){                         //der ursprünglichen Nachhricht zu arbeiten, da buffer die Ausgabe sein soll. (den Cifer enthalten soll)
        key[i] = ((uint8_t*)param)[i];
	}

                                                    //key expansion
	for (int i=1;i<=10;i++){                        //sonderfall alle 4 // modolo mit 4 entspricht bitweiser addition mit 3!!! i & 3 == 0 wird aber nicht benötigt
        key[i*16]  =(sBoxLookup(key[i*16-3]))^key[i*16-16]^rConLookup(i-1);
        key[i*16+1]=(sBoxLookup(key[i*16-2]))^key[i*16-15];
        key[i*16+2]=(sBoxLookup(key[i*16-1]))^key[i*16-14];
        key[i*16+3]=(sBoxLookup(key[i*16-4]))^key[i*16-13];

		for (int j=(i*16+4); j<=(i*16+15); j++){
			key[j]=key[j-4] ^ key[j-16];
		}                                           //ende key expansion
	}

    for (int j=0; j<=15; j++){                      //loop 0 (in der ersten loop nur Schritt 4 AddRoundkey
        mes[j]=mes[j] ^ key[j];
    }
                                                    //Standard loop 1 - 9
    for (int i=1; i<=9; i++){
        for (int j=0; j<=15; j++){
            mes[j] = sBoxLookup(mes[j]);            //Schritt 1 SubBytes
        }
        uint8_t mestmp[16];                         //Schritt 2 Row shift
        mestmp[0] = mes[0]; mestmp[4] = mes[4]; mestmp[8] = mes[8]; mestmp[12] = mes[12];   //erste Reihe von Rowshift. wird nicht benötigt.
        mestmp[1] = mes[5]; mestmp[5] = mes[9]; mestmp[9] = mes[13]; mestmp[13] = mes[1];   //zweite Reihe vom rowshift
        mestmp[2] = mes[10]; mestmp[6] = mes[14]; mestmp[10] = mes[2]; mestmp[14] = mes[6]; //dritte Reihe vom rowshift
        mestmp[3] = mes[15]; mestmp[7] = mes[3]; mestmp[11] = mes[7]; mestmp[15] = mes[11]; //vierte Reihe vom rowshift

                                                   //Schritt 3 Mix colums
        for (int j=0; j<4; j++){                                                            //für jede Spalte
            int k=j*4;
            mes[k+0] = xtime(mestmp[k+0])                   ^ (xtime(mestmp[k+1]) ^ mestmp[k+1])    ^ mestmp[k+2]                           ^ mestmp[k+3];
            mes[k+1] = mestmp[k+0]                          ^ xtime(mestmp[k+1])                    ^ (xtime(mestmp[k+2]) ^ mestmp[k+2])    ^ mestmp[k+3];
            mes[k+2] = mestmp[k+0]                          ^ mestmp[k+1]                           ^ xtime(mestmp[k+2])                    ^ (xtime(mestmp[k+3]) ^ mestmp[k+3]);
            mes[k+3] = (xtime(mestmp[k+0]) ^ mestmp[k+0])   ^ mestmp[k+1]                           ^ mestmp[k+2]                           ^ xtime(mestmp[k+3]);
        }
        for (int j=0; j<=15; j++){                  //Schritt 4 AddRoundkey
            mes[j]=mes[j] ^ key[j+16*i];
        }
    }                                               //ende Standard loop

    for (int j=0; j<=15; j++){              //loop 10 (nur Schritt 1, 2 & 4)
        mes[j] = sBoxLookup(mes[j]);                //Schritt 1 SubBytes
    }
    uint8_t mestmp[16];                         //Schritt 2 Row shift
    mestmp[0] = mes[0]; mestmp[4] = mes[4]; mestmp[8] = mes[8]; mestmp[12] = mes[12];   //erste Reihe von Rowshift. wird nicht benötigt.
    mestmp[1] = mes[5]; mestmp[5] = mes[9]; mestmp[9] = mes[13]; mestmp[13] = mes[1];   //zweite Reihe vom rowshift
    mestmp[2] = mes[10]; mestmp[6] = mes[14]; mestmp[10] = mes[2]; mestmp[14] = mes[6]; //dritte Reihe vom rowshift
    mestmp[3] = mes[15]; mestmp[7] = mes[3]; mestmp[11] = mes[7]; mestmp[15] = mes[11]; //vierte Reihe vom rowshift
    for (int j=0; j<=15; j++){                  //Schritt 4 AddRoundkey
        mes[j]=mestmp[j] ^ key[j+16*10];
    }
  /*  for (int k=0; k<=15; k++){                      //Prüffunktion zur kontrolle der ausgerechneten Keys
        mes[k] = key[k+32];
    } */
}

uint8_t sBoxLookup(uint8_t input){                  //Funktion die den Lookup aus der Sbox ausgibt.
    uint8_t Sbox[256] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16,
    };
    return Sbox[input];
}

uint8_t rConLookup( uint8_t input){                 //Funktion die als Lookup den Rcon ausgibt.
    uint8_t Rcon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36,
    };
    return Rcon[input];
}

uint8_t xtime(uint8_t input){
    uint8_t output = (input<<1);
    if (((input>>7) & 1) == 1){
        output = output ^ 0x1b;
    }
    return output;
}
