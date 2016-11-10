//allgemeine Fragen.
//was heist das _t bei den uint?
//was für einen Editor empfiehlst du? codeblocks
//was ist das password der maschine?
//gibt es möglichkeiten zu testen ob das bisherige stimmt?
//was heit shift to the left? wie macht man das?
//was ist die S matrix? / die erste operation?


// OPTIONS: -O2 -std=gnu99
#include "student.h"
#include <inttypes.h>

void *aes128_init(void *key)
{
	return key;                 //Den Key weitergeben und jedesmal neu rechnen
}

void *shiftRows(void *input);


void aes128_encrypt(void *buffer, void *param)
{
	uint8_t key[176]; 	 //ein key ist [16] => man braucht für 11 runden 176
	uint8_t mes[16];
	for (i=0;i<16;i++){
        key[i] = (uint8_t)*param;
	}
	for (i=0;i<16;i++){
        mes[i] = (uint8_t)*buffer;
	}

	//key espansion
	for (i=1;i<=10;i++){
		if (i mod 4 = 0){//sonderfall alle 4
            key[i*16]=key...;

		}
		else{
			for (j=(i*16+4); j<=(i*16+15); j++){
				key[j]=key[j-4] ^ keys[j-16];
			}
		}
	}
}



void *shiftRows(void *input){   //input ist int[16]
    uint8_t eingang = *input;
    uint8_t result[16];
    for (i=1;i<=4;i++){
        for (j=1;j<=4;j++){
            result[i*4-4+((j-i) modulus 4)+1]=eingang[i*4-4+j]
        }
    }
    *input = result;
    return &result;
}

void *addRoundKey(void *input, void *key){
    uint8_t result[16];
    for (i=1; i<=16; i++){
        result[i]=*input[i] ^ *key;
    }
    *input = result
    return &result;
}

