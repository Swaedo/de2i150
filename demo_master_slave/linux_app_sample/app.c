#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "PCIE.h"

//MAX BUFFER FOR DMA
#define MAXDMA 16

//BASE ADDRESS FOR CONTROL REGISTER
//#define CRA 0x00004080		// This is the starting address of the Custom Slave module. This maps to the address space of the custom module in the Qsys subsystem.
#define CRA 0x00000000		// This is the starting address of the Custom Slave module. This maps to the address space of the custom module in the Qsys subsystem.

//BASE ADDRESS TO SDRAM
#define SDRAM 0x08000000	// This is the starting address of the SDRAM controller. This maps to the address space of the SDRAM controller in the Qsys subsystem.
#define START_BYTE 0xF00BF00B
#define STOP_BYTE 0xDEADF00B
#define RWSIZE (32 / 8)
PCIE_BAR pcie_bars[] = { PCIE_BAR0, PCIE_BAR1 , PCIE_BAR2 , PCIE_BAR3 , PCIE_BAR4 , PCIE_BAR5 };

void test32( PCIE_HANDLE hPCIe, DWORD addr );
void testDMA( PCIE_HANDLE hPCIe, DWORD addr);

// BOOL WriteImage(PCIE_HANDLE hPCIe, char *filename);
// BOOL checkImageDone(PCIE_HANDLE hPCIe);
// BOOL NewReadImage(PCIE_HANDLE hPCIe);
// void Demo(PCIE_HANDLE hPCIe, char *filename);
// BOOL WriteInfo2(PCIE_HANDLE hPCIe);

/*
demo

write image 
    write header
    save image to sdram
send start signal 
read image
    save image to out
    Write Info
    */

int main(int arc, char* argv[])
{
	void *lib_handle;
	PCIE_HANDLE hPCIe;

	lib_handle = PCIE_Load();		// Dynamically Load the PCIE library
	if (!lib_handle)
	{
		printf("PCIE_Load failed\n");
		return 0;
	}
	hPCIe = PCIE_Open(0,0,0);		// Every device is a like a file in UNIX. Opens the PCIE device for reading/writing
    /*PCIE_Write32( hPCIe, pcie_bars[0], addr, *((unsigned char *)&tempw + 1) ) ;  //01
    PCIE_Write32( hPCIe, pcie_bars[0], addr, *((unsigned char *)&tempw + 1) ) ;  //01
    PCIE_Write32( hPCIe, pcie_bars[0], addr, *((unsigned char *)&tempw + 1) ) ;  //01
    PCIE_Write32( hPCIe, pcie_bars[0], addr, *((unsigned char *)&tempw + 1) ) ;  //01 */
    
	if (!hPCIe)
	{
		printf("PCIE_Open failed\n");
		return 0;
	}
    
  // Custom Logic
    unsigned char magicNumb[2];  
    unsigned char width; 
    unsigned char height;
    unsigned char maxInt;
    unsigned long* Image = malloc(width * height * sizeof(unsigned char));
  
    //open image
    printf("a");
    if (argv[1] == NULL){
        printf("include file name\n"); 
        return 1;
    }
    printf("b");
    FILE * pFile = fopen(argv[1], "r");
    if (pFile == NULL) 
        return 1;
    
    //strip image
        printf("1");
        fread(magicNumb, sizeof(unsigned char), 2, pFile);
        printf("2");
        fread(width, sizeof(unsigned char), 1, pFile);
        printf("3");
        fread(height, sizeof(unsigned char), 1, pFile);
        printf("4");
        fread(maxInt, sizeof(unsigned char), 1, pFile);
        printf("5");
        printf("%s %c %c %c\n", magicNumb, width, height, maxInt);
        printf("6");
        fread(Image, sizeof(unsigned short), (width*height), pFile);
        printf("7");
        
        DWORD addr = 0x04; //hope this is right for PPM
        //get width (0008/32)
        //send width
        //get height 
        //send height 
        //max intens = 255
        //loop save image array (whats the terminiating condition? = height * width * thing) (0008/32)
        
    
    BOOL bPass;
    //bPass = PCIE_DmaRead(hPCIe, addr, readArray, MAXDMA * RWSIZE );

    //save image to atom 
            //open image
            //p6, width, height, intensity
            //read remainder from SDRAM
            
        
    /*
	//test CRA
	test32(hPCIe, CRA);			// Test the Configuration Registers for reads and writes
	PCIE_Write32( hPCIe, pcie_bars[0], CRA, START_BYTE);
	//test SDRAM
	testDMA(hPCIe,SDRAM);			// Test the SDRAM for reads and writes

	PCIE_Write32( hPCIe, pcie_bars[0], CRA, STOP_BYTE);
	printf("\nPush up SW[16] to view data stored in SDRAM and use SW[3:0] to select different addresses.\n");
*/  
    
    
    fclose(pFile);
    free(Image);
    printf("done\n");
	return 0;
}
