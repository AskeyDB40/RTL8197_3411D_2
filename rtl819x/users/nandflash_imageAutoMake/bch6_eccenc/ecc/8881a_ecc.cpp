#include <stdio.h>
#include <string.h>

unsigned short int GO7[10] ; 
unsigned short int GO6[10] ; 
unsigned short int GO5[10] ; 
unsigned short int GO4[10] ; 
unsigned short int GO3[10] ; 
unsigned short int GO2[10] ; 
unsigned short int GO1[10] ; 
unsigned short int GO0[10] ;  

static void ReedSolomonWr(unsigned short int DataIn)
{ 
    
	unsigned short int GIN; // 16bit, only use 10bit: [9:0]. bit [15:10] and bit[9:8] are always zero.    
	GIN = DataIn & 0x00FF;

	unsigned short int G0710bit = 
	((GO7[0] & 0x0001) << 0) | 
	((GO7[1] & 0x0001) << 1) |
	((GO7[2] & 0x0001) << 2) | 
	((GO7[3] & 0x0001) << 3) |
	((GO7[4] & 0x0001) << 4) |  
	((GO7[5] & 0x0001) << 5) |
	((GO7[6] & 0x0001) << 6) | 
	((GO7[7] & 0x0001) << 7) |
	((GO7[8] & 0x0001) << 8) | 
	((GO7[9] & 0x0001) << 9) ;

	unsigned short int I10bit = GIN ^ G0710bit;
	unsigned short int I[10];
	I[0] = (I10bit >> 0) & 0x0001;
	I[1] = (I10bit >> 1) & 0x0001;
	I[2] = (I10bit >> 2) & 0x0001;
	I[3] = (I10bit >> 3) & 0x0001;
	I[4] = (I10bit >> 4) & 0x0001;
	I[5] = (I10bit >> 5) & 0x0001;
	I[6] = (I10bit >> 6) & 0x0001;
	I[7] = (I10bit >> 7) & 0x0001;
	I[8] = (I10bit >> 8) & 0x0001;
	I[9] = (I10bit >> 9) & 0x0001;

	unsigned short int xa36[10];
	unsigned short int xa567[10];
	unsigned short int xa403[10];
	unsigned short int xa407[10];
	unsigned short int xa607[10];
	unsigned short int xa398[10];
	unsigned short int xa385[10];
	unsigned short int xa540[10];

	xa36[0] = I[1] ^ I[2] ^ I[4] ^ I[9] ;
	xa36[1] = I[2] ^ I[3] ^ I[5] ;
	xa36[2] = I[0] ^ I[3] ^ I[4] ^ I[6] ;
	xa36[3] = I[2] ^ I[5] ^ I[7] ^ I[9] ;
	xa36[4] = I[3] ^ I[6] ^ I[8] ;
	xa36[5] = I[4] ^ I[7] ^ I[9] ;
	xa36[6] = I[0] ^ I[5] ^ I[8] ;
	xa36[7] = I[1] ^ I[6] ^ I[9] ;
	xa36[8] = I[0] ^ I[2] ^ I[7] ;
	xa36[9] = I[0] ^ I[1] ^ I[3] ^ I[8] ;

	xa567[0] = I[0] ^ I[1] ^ I[4] ^ I[7] ^ I[8] ^ I[9] ;
	xa567[1] = I[0] ^ I[1] ^ I[2] ^ I[5] ^ I[8] ^ I[9] ;
	xa567[2] = I[1] ^ I[2] ^ I[3] ^ I[6] ^ I[9] ;
	xa567[3] = I[0] ^ I[1] ^ I[2] ^ I[3] ^ I[8] ^ I[9] ;
	xa567[4] = I[1] ^ I[2] ^ I[3] ^ I[4] ^ I[9] ;
	xa567[5] = I[2] ^ I[3] ^ I[4] ^ I[5] ;
	xa567[6] = I[0] ^ I[3] ^ I[4] ^ I[5] ^ I[6] ;
	xa567[7] = I[1] ^ I[4] ^ I[5] ^ I[6] ^ I[7] ;
	xa567[8] = I[2] ^ I[5] ^ I[6] ^ I[7] ^ I[8] ;
	xa567[9] = I[0] ^ I[3] ^ I[6] ^ I[7] ^ I[8] ^ I[9] ;

	xa403[0] = I[5] ^ I[6] ^ I[7] ^ I[9] ;
	xa403[1] = I[0] ^ I[6] ^ I[7] ^ I[8] ;
	xa403[2] = I[1] ^ I[7] ^ I[8] ^ I[9] ;
	xa403[3] = I[0] ^ I[2] ^ I[5] ^ I[6] ^ I[7] ^ I[8] ;
	xa403[4] = I[0] ^ I[1] ^ I[3] ^ I[6] ^ I[7] ^ I[8] ^ I[9] ;
	xa403[5] = I[0] ^ I[1] ^ I[2] ^ I[4] ^ I[7] ^ I[8] ^ I[9] ;
	xa403[6] = I[1] ^ I[2] ^ I[3] ^ I[5] ^ I[8] ^ I[9] ;
	xa403[7] = I[2] ^ I[3] ^ I[4] ^ I[6] ^ I[9] ;
	xa403[8] = I[3] ^ I[4] ^ I[5] ^ I[7] ;
	xa403[9] = I[4] ^ I[5] ^ I[6] ^ I[8] ;

	xa407[0] = I[1] ^ I[2] ^ I[3] ^ I[5] ^ I[8] ^ I[9] ;
	xa407[1] = I[2] ^ I[3] ^ I[4] ^ I[6] ^ I[9] ;
	xa407[2] = I[3] ^ I[4] ^ I[5] ^ I[7] ;
	xa407[3] = I[1] ^ I[2] ^ I[3] ^ I[4] ^ I[6] ^ I[9] ;
	xa407[4] = I[2] ^ I[3] ^ I[4] ^ I[5] ^ I[7] ;
	xa407[5] = I[0] ^ I[3] ^ I[4] ^ I[5] ^ I[6] ^ I[8] ;
	xa407[6] = I[1] ^ I[4] ^ I[5] ^ I[6] ^ I[7] ^ I[9] ;
	xa407[7] = I[0] ^ I[2] ^ I[5] ^ I[6] ^ I[7] ^ I[8] ;
	xa407[8] = I[0] ^ I[1] ^ I[3] ^ I[6] ^ I[7] ^ I[8] ^ I[9] ;
	xa407[9] = I[0] ^ I[1] ^ I[2] ^ I[4] ^ I[7] ^ I[8] ^ I[9] ;

	xa607[0] = I[0] ^ I[1] ^ I[3] ^ I[6] ^ I[9] ;
	xa607[1] = I[0] ^ I[1] ^ I[2] ^ I[4] ^ I[7] ;
	xa607[2] = I[0] ^ I[1] ^ I[2] ^ I[3] ^ I[5] ^ I[8] ;
	xa607[3] = I[2] ^ I[4] ;
	xa607[4] = I[0] ^ I[3] ^ I[5] ;
	xa607[5] = I[1] ^ I[4] ^ I[6] ;
	xa607[6] = I[2] ^ I[5] ^ I[7] ;
	xa607[7] = I[0] ^ I[3] ^ I[6] ^ I[8] ;
	xa607[8] = I[1] ^ I[4] ^ I[7] ^ I[9] ;
	xa607[9] = I[0] ^ I[2] ^ I[5] ^ I[8] ;

	xa398[0] = I[0] ^ I[2] ^ I[4] ;
	xa398[1] = I[0] ^ I[1] ^ I[3] ^ I[5] ;
	xa398[2] = I[1] ^ I[2] ^ I[4] ^ I[6] ;
	xa398[3] = I[3] ^ I[4] ^ I[5] ^ I[7] ;
	xa398[4] = I[4] ^ I[5] ^ I[6] ^ I[8] ;
	xa398[5] = I[5] ^ I[6] ^ I[7] ^ I[9] ;
	xa398[6] = I[0] ^ I[6] ^ I[7] ^ I[8] ;
	xa398[7] = I[1] ^ I[7] ^ I[8] ^ I[9] ;
	xa398[8] = I[0] ^ I[2] ^ I[8] ^ I[9] ;
	xa398[9] = I[1] ^ I[3] ^ I[9] ;

	xa385[0] = I[0] ^ I[5] ^ I[6] ^ I[9] ;
	xa385[1] = I[0] ^ I[1] ^ I[6] ^ I[7] ;
	xa385[2] = I[1] ^ I[2] ^ I[7] ^ I[8] ;
	xa385[3] = I[2] ^ I[3] ^ I[5] ^ I[6] ^ I[8] ;
	xa385[4] = I[0] ^ I[3] ^ I[4] ^ I[6] ^ I[7] ^ I[9] ;
	xa385[5] = I[0] ^ I[1] ^ I[4] ^ I[5] ^ I[7] ^ I[8] ;
	xa385[6] = I[1] ^ I[2] ^ I[5] ^ I[6] ^ I[8] ^ I[9] ;
	xa385[7] = I[2] ^ I[3] ^ I[6] ^ I[7] ^ I[9] ;
	xa385[8] = I[3] ^ I[4] ^ I[7] ^ I[8] ;
	xa385[9] = I[4] ^ I[5] ^ I[8] ^ I[9] ;

	xa540[0] = I[2] ^ I[3] ^ I[4] ^ I[5] ^ I[6] ^ I[7] ^ I[8] ;
	xa540[1] = I[0] ^ I[3] ^ I[4] ^ I[5] ^ I[6] ^ I[7] ^ I[8] ^ I[9] ;
	xa540[2] = I[0] ^ I[1] ^ I[4] ^ I[5] ^ I[6] ^ I[7] ^ I[8] ^ I[9] ;
	xa540[3] = I[0] ^ I[1] ^ I[3] ^ I[4] ^ I[9] ;
	xa540[4] = I[0] ^ I[1] ^ I[2] ^ I[4] ^ I[5] ;
	xa540[5] = I[0] ^ I[1] ^ I[2] ^ I[3] ^ I[5] ^ I[6] ;
	xa540[6] = I[0] ^ I[1] ^ I[2] ^ I[3] ^ I[4] ^ I[6] ^ I[7] ;
	xa540[7] = I[0] ^ I[1] ^ I[2] ^ I[3] ^ I[4] ^ I[5] ^ I[7] ^ I[8] ;
	xa540[8] = I[0] ^ I[1] ^ I[2] ^ I[3] ^ I[4] ^ I[5] ^ I[6] ^ I[8] ^ I[9] ;
	xa540[9] = I[1] ^ I[2] ^ I[3] ^ I[4] ^ I[5] ^ I[6] ^ I[7] ^ I[9] ;

	unsigned short int xa1[10];
	unsigned short int xa2[10];
	unsigned short int xa3[10];
	unsigned short int xa4[10];
	unsigned short int xa5[10];
	unsigned short int xa6[10];
	unsigned short int xa7[10];
	unsigned short int xa8[10];

	xa1[0] = GO0[9] ;
	xa1[1] = GO0[0];
	xa1[2] = GO0[1];
	xa1[3] = GO0[2] ^ GO0[9] ;
	xa1[4] = GO0[3] ;
	xa1[5] = GO0[4] ;
	xa1[6] = GO0[5] ;
	xa1[7] = GO0[6] ;
	xa1[8] = GO0[7] ;
	xa1[9] = GO0[8] ;

	xa2[0] = GO1[8] ;
	xa2[1] = GO1[9] ;
	xa2[2] = GO1[0] ;
	xa2[3] = GO1[1] ^ GO1[8] ;
	xa2[4] = GO1[2] ^ GO1[9] ;
	xa2[5] = GO1[3] ;
	xa2[6] = GO1[4] ;
	xa2[7] = GO1[5] ;
	xa2[8] = GO1[6] ;
	xa2[9] = GO1[7] ;

	xa3[0] = GO2[7] ;
	xa3[1] = GO2[8] ;
	xa3[2] = GO2[9] ;
	xa3[3] = GO2[0] ^ GO2[7] ;
	xa3[4] = GO2[1] ^ GO2[8] ;
	xa3[5] = GO2[2] ^ GO2[9] ;
	xa3[6] = GO2[3] ;
	xa3[7] = GO2[4] ;
	xa3[8] = GO2[5] ;
	xa3[9] = GO2[6] ;

	xa4[0] = GO3[6] ;
	xa4[1] = GO3[7] ;
	xa4[2] = GO3[8] ;
	xa4[3] = GO3[6] ^ GO3[9] ;
	xa4[4] = GO3[0] ^ GO3[7] ;
	xa4[5] = GO3[1] ^ GO3[8] ;
	xa4[6] = GO3[2] ^ GO3[9] ;
	xa4[7] = GO3[3] ;
	xa4[8] = GO3[4] ;
	xa4[9] = GO3[5] ;

	xa5[0] = GO4[5] ;
	xa5[1] = GO4[6] ;
	xa5[2] = GO4[7] ;
	xa5[3] = GO4[5] ^ GO4[8] ;
	xa5[4] = GO4[6] ^ GO4[9] ;
	xa5[5] = GO4[0] ^ GO4[7] ;
	xa5[6] = GO4[1] ^ GO4[8] ;
	xa5[7] = GO4[2] ^ GO4[9] ;
	xa5[8] = GO4[3] ;
	xa5[9] = GO4[4] ;

	xa6[0] = GO5[4] ;
	xa6[1] = GO5[5] ;
	xa6[2] = GO5[6] ;
	xa6[3] = GO5[4] ^ GO5[7] ;
	xa6[4] = GO5[5] ^ GO5[8] ;
	xa6[5] = GO5[6] ^ GO5[9] ;
	xa6[6] = GO5[0] ^ GO5[7] ;
	xa6[7] = GO5[1] ^ GO5[8] ;
	xa6[8] = GO5[2] ^ GO5[9] ;
	xa6[9] = GO5[3] ;

	xa7[0] = GO6[3] ;
	xa7[1] = GO6[4] ;
	xa7[2] = GO6[5] ;
	xa7[3] = GO6[3] ^ GO6[6] ;
	xa7[4] = GO6[4] ^ GO6[7] ;
	xa7[5] = GO6[5] ^ GO6[8] ;
	xa7[6] = GO6[6] ^ GO6[9] ;
	xa7[7] = GO6[0] ^ GO6[7] ;
	xa7[8] = GO6[1] ^ GO6[8] ;
	xa7[9] = GO6[2] ^ GO6[9] ;

	xa8[0] = GO7[2] ^ GO7[9] ;
	xa8[1] = GO7[3] ;
	xa8[2] = GO7[4] ;
	xa8[3] = GO7[2] ^ GO7[5] ^ GO7[9];
	xa8[4] = GO7[3] ^ GO7[6] ;
	xa8[5] = GO7[4] ^ GO7[7] ;
	xa8[6] = GO7[5] ^ GO7[8] ;
	xa8[7] = GO7[6] ^ GO7[9] ;
	xa8[8] = GO7[0] ^ GO7[7] ;
	xa8[9] = GO7[1] ^ GO7[8] ;

	//GO0 = xa36 ; 
	//GO1 = xa567 ^ GO0 ;
	//GO2 = xa403 ^ GO1 ;
	//GO3 = xa407 ^ GO2 ;
	//GO4 = xa607 ^ GO3 ;
	//GO5 = xa398 ^ GO4 ;
	//GO6 = xa385 ^ GO5 ;
	//GO7 = xa540 ^ GO6 ;
	int i;
	for(i=0; i < 10 ; i++)
	{
		// Note: order is important, can not change 
		GO7[i] = xa540[i] ^ GO6[i] ;
		GO6[i] = xa385[i] ^ GO5[i] ;
		GO5[i] = xa398[i] ^ GO4[i] ;
		GO4[i] = xa607[i] ^ GO3[i] ;
		GO3[i] = xa407[i] ^ GO2[i] ;
		GO2[i] = xa403[i] ^ GO1[i] ;
		GO1[i] = xa567[i] ^ GO0[i] ;
		GO0[i] = xa36[i];   	
	} // for j


} // ReedSolomonWr


void
bch6_ecc_512B_encode(unsigned char *ecc,  // ecc: output 10 bytes of ECC code
	const unsigned char *input_buf, 	// input_buf: the 512 bytes input data (BCH6_PAGE_SIZE bytes)
	const unsigned char *oob){			// oob: 6 bytes out-of-band for input (BCH6_OOB_SIZE bytes)

	int i;
	unsigned char raw_data[512+16];
	unsigned short int EccOut[10]; // only use 8bit [7:0]. total 10byte.	
		
	for(i=0; i<10; i++)
	{
		GO7[i] = 0x0;
		GO6[i] = 0x0;
		GO5[i] = 0x0;
		GO4[i] = 0x0;
		GO3[i] = 0x0;
		GO2[i] = 0x0;
		GO1[i] = 0x0;
		GO0[i] = 0x0;
	}
			
	memset(raw_data,0xff,528);
	memcpy(raw_data, input_buf, 512);
	memcpy(raw_data+512, oob, 6);

	for(i=0; i<(512+6) ; i++)
	{    
		ReedSolomonWr(raw_data[i]);  
	}

	EccOut[0] = // {GO7[9:2]};         
	//(char)( // ?? how to transfer 16bit unsigned short int [15:0] as 8 bit [7:0]
	((GO7[2] & 0x0001) << 0) | 
	((GO7[3] & 0x0001) << 1) |
	((GO7[4] & 0x0001) << 2) | 
	((GO7[5] & 0x0001) << 3) |
	((GO7[6] & 0x0001) << 4) | 
	((GO7[7] & 0x0001) << 5) |
	((GO7[8] & 0x0001) << 6) | 
	((GO7[9] & 0x0001) << 7) ;
		//);

	EccOut[1] = // {GO7[1:0],GO6[9:4]};
	((GO6[4] & 0x0001) << 0) | 
	((GO6[5] & 0x0001) << 1) |
	((GO6[6] & 0x0001) << 2) | 
	((GO6[7] & 0x0001) << 3) |
	((GO6[8] & 0x0001) << 4) | 
	((GO6[9] & 0x0001) << 5) |
	((GO7[0] & 0x0001) << 6) | 
	((GO7[1] & 0x0001) << 7) ;

	EccOut[2] = // {GO6[3:0],GO5[9:6]};
	((GO5[6] & 0x0001) << 0) | 
	((GO5[7] & 0x0001) << 1) |
	((GO5[8] & 0x0001) << 2) | 
	((GO5[9] & 0x0001) << 3) |
	((GO6[0] & 0x0001) << 4) | 
	((GO6[1] & 0x0001) << 5) |
	((GO6[2] & 0x0001) << 6) | 
	((GO6[3] & 0x0001) << 7) ;

	EccOut[3] = // {GO5[5:0],GO4[9:8]};
	((GO4[8] & 0x0001) << 0) | 
	((GO4[9] & 0x0001) << 1) |
	((GO5[0] & 0x0001) << 2) | 
	((GO5[1] & 0x0001) << 3) |
	((GO5[2] & 0x0001) << 4) | 
	((GO5[3] & 0x0001) << 5) |
	((GO5[4] & 0x0001) << 6) | 
	((GO5[5] & 0x0001) << 7) ;

	EccOut[4] = // {GO4[7:0]};         
	((GO4[0] & 0x0001) << 0) | 
	((GO4[1] & 0x0001) << 1) |
	((GO4[2] & 0x0001) << 2) | 
	((GO4[3] & 0x0001) << 3) |
	((GO4[4] & 0x0001) << 4) | 
	((GO4[5] & 0x0001) << 5) |
	((GO4[6] & 0x0001) << 6) | 
	((GO4[7] & 0x0001) << 7) ;

	EccOut[5] = // {GO3[9:2]};         
	((GO3[2] & 0x0001) << 0) | 
	((GO3[3] & 0x0001) << 1) |
	((GO3[4] & 0x0001) << 2) | 
	((GO3[5] & 0x0001) << 3) |
	((GO3[6] & 0x0001) << 4) | 
	((GO3[7] & 0x0001) << 5) |
	((GO3[8] & 0x0001) << 6) | 
	((GO3[9] & 0x0001) << 7) ;

	EccOut[6] = // {GO3[1:0],GO2[9:4]};
	((GO2[4] & 0x0001) << 0) | 
	((GO2[5] & 0x0001) << 1) |
	((GO2[6] & 0x0001) << 2) | 
	((GO2[7] & 0x0001) << 3) |
	((GO2[8] & 0x0001) << 4) | 
	((GO2[9] & 0x0001) << 5) |
	((GO3[0] & 0x0001) << 6) | 
	((GO3[1] & 0x0001) << 7) ;
	
	EccOut[7] = // {GO2[3:0],GO1[9:6]};
	((GO1[6] & 0x0001) << 0) | 
	((GO1[7] & 0x0001) << 1) |
	((GO1[8] & 0x0001) << 2) | 
	((GO1[9] & 0x0001) << 3) |
	((GO2[0] & 0x0001) << 4) | 
	((GO2[1] & 0x0001) << 5) |
	((GO2[2] & 0x0001) << 6) | 
	((GO2[3] & 0x0001) << 7) ;

	EccOut[8] = // {GO1[5:0],GO0[9:8]};
	((GO0[8] & 0x0001) << 0) | 
	((GO0[9] & 0x0001) << 1) |
	((GO1[0] & 0x0001) << 2) | 
	((GO1[1] & 0x0001) << 3) |
	((GO1[2] & 0x0001) << 4) | 
	((GO1[3] & 0x0001) << 5) |
	((GO1[4] & 0x0001) << 6) | 
	((GO1[5] & 0x0001) << 7) ;

	EccOut[9] = // {GO0[7:0]};         
	((GO0[0] & 0x0001) << 0) | 
	((GO0[1] & 0x0001) << 1) |
	((GO0[2] & 0x0001) << 2) | 
	((GO0[3] & 0x0001) << 3) |
	((GO0[4] & 0x0001) << 4) | 
	((GO0[5] & 0x0001) << 5) |
	((GO0[6] & 0x0001) << 6) | 
	((GO0[7] & 0x0001) << 7) ;

	//printf("\n============== Generate 10 bytes ECC ==============\n");
	for(i=0;i<10;i++){
		//printf("EccOut:0x%02X ",EccOut[i]);
		ecc[i] = (unsigned char)EccOut[i];
		//printf("ECC:0x%02X \n",ECC[i]);
	}
}

