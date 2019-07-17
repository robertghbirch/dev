#include <stdio.h>


double getT(unsigned char l, unsigned char h)
{
	unsigned short s ;
	((unsigned char *)(&s))[1] = h ;
	((unsigned char *)(&s))[0] = l ;

	if (h & 0x08) {
		// -
		return(((((double)s) - 4096)/16)) ;

	} else {
		// +
		return((double)s/16) ;
	}
}


void main(void) {

	unsigned char l ;
	unsigned char h ;

	h = 0x0C;
	l = 0x90;
	printf("Temp = [%lf]\r\n", getT(l,h)) ;

	h = 0x0e;
	l = 0x70;
	printf("Temp = [%lf]\r\n", getT(l,h)) ;

	h = 0x0F;
	l = 0xFF;
	printf("Temp = [%lf]\r\n", getT(l,h)) ;

	h = 0x00;
	l = 0x00;
	printf("Temp = [%lf]\r\n", getT(l,h)) ;

	h = 0x00;
	l = 0x01;
	printf("Temp = [%lf]\r\n", getT(l,h)) ;

	h = 0x01;
	l = 0x90;
	printf("Temp = [%lf]\r\n", getT(l,h)) ;

	h = 0x04;
	l = 0xb4;
	printf("Temp = [%lf]\r\n", getT(l,h)) ;

	h = 0x06;
	l = 0x40;
	printf("Temp = [%lf]\r\n", getT(l,h)) ;

	h = 0x07;
	l = 0xd0;
	printf("Temp = [%lf]\r\n", getT(l,h)) ;
}
