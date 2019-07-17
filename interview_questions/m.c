#include <stdio.h>
#include <stdint.h>
#include <math.h>


void pbb (int32_t i, int8_t b) {
	int32_t j = i;
	int c = 0 ; 
	int h = ((int)pow(2,b) - 1) << (32 - b) ; 
        int t = ((double)(32.0 / (double)b)) ;

	i = j ;
	printf("\r\nBase [%d] t[%d] h[%x]\t", (int)pow(2,b), t, h);
	for (c = 0 ; c < t ; c++) {
		if ((0 == c) && (32 % b)) {
		  printf("%1X ", (int)((i & h) >> ((int)(32 / b)) * b) & (int)(pow(2,b) - 1)) ;
		  i = i << (32 % b) ;
			t++ ;
		} else {
		  printf("%1X ", (int)((i & h) >> (32 - b)) & (int)(pow(2,b) - 1)) ;
		  i = i << b ;
		}
	}
}


void pb (int32_t i) {
	int32_t j = i;
	printf("int\t[%d]", (int)i) ;
	printf("\r\nint\t%08X\r\n", (int)i) ;

	int c = 0 ; 


	// byte
	i = j ;
	printf("byte\t");
	for (c = 0 ; c < 4 ; c++) {
		printf("%02X ", (int)((i & 0xFF000000) >> 24)) ;
		i = i << 8 ;
	}
	
	// nibble 
	printf("\r\nnibble\t");
	i = j ;
	for (c = 0 ; c < 8 ; c++) {
		printf("%1X ", (int)((i & 0xF0000000) >> 28)) ;
		i = i << 4 ;
	}
	
	// oct
	i = j ;
	printf("\r\noct\t");
	for (c = 0 ; c < 11 ; c++) {
		if (0 == c) {
		  printf("%1X ", (int)((i & 0xe0000000) >> 30)) ;
		  i = i << 2 ;
		} else {
		  printf("%1X ", (int)((i & 0xe0000000) >> 29)) ;
		  i = i << 3 ;
		}
	}
	
	// 1/2nib 
	i = j ;
	printf("\r\n1\\2nib\t");
	for (c = 0 ; c < 16 ; c++) {
		printf("%1X ", (int)((i & 0xC0000000) >> 30)) ;
		i = i << 2 ;
	}
	
	// bit
	i = j ;
	printf("\r\nbit\t");
	for (c = 0 ; c < 32 ; c++) {
		if (i & 0x80000000) {
			printf("1") ;
		} else {
			printf("0") ;
		}
		
		i = i << 1 ;
		if ((c + 1) % 8) {
		} else {
			printf(" ") ;
		}
	}
} 

void main (void) {

	printf("Hello\r\n") ;


	pb(1234) ;

	pbb(1234, 8) ;
	pbb(1234, 7) ;
	pbb(1234, 6) ;
	pbb(1234, 5) ;
	pbb(1234, 4) ;
	pbb(1234, 3) ;
	pbb(1234, 2) ;
	pbb(1234, 1) ;
}
