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

typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;  //specifies the file type
    DWORD bfSize;  //specifies the size in bytes of the bitmap file
    WORD bfReserved1;  //reserved; must be 0
    WORD bfReserved2;  //reserved; must be 0
    DWORD bOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    
    /*signed int*/ WORD  width;  //specifies width in pixels
    /*signed int*/ WORD dummy2;  //species height in pixels
    WORD height;
    DWORD dummy3;
    WORD biPlanes; //specifies the number of color planes, must be 1
    WORD biBitCount; //specifies the number of bit per pixel
    DWORD biCompression;//spcifies the type of compression
    DWORD biSizeImage;  //size of image in bytes
    signed int biXPelsPerMeter;  //number of pixels per meter in x axis
    signed int biYPelsPerMeter;  //number of pixels per meter in y axis
    DWORD biClrUsed;  //number of colors used by th ebitmap
    DWORD biClrImportant;  //number of colors that are important
}BITMAPINFOHEADER;

void test32( PCIE_HANDLE hPCIe, DWORD addr );
void testDMA( PCIE_HANDLE hPCIe, DWORD addr);

BOOL WriteImage(PCIE_HANDLE hPCIe, char *filename, BITMAPINFOHEADER *info);
BOOL checkImageDone(PCIE_HANDLE hPCIe);
BOOL NewReadImage(PCIE_HANDLE hPCIe, BITMAPINFOHEADER *info);
void Demo(PCIE_HANDLE hPCIe, char *filename);
BOOL WriteInfo2(PCIE_HANDLE hPCIe, BITMAPINFOHEADER *info);

int main(void)
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

	if (!hPCIe)
	{
		printf("PCIE_Open failed\n");
		return 0;
	}
/*
	//test CRA
	test32(hPCIe, CRA);			// Test the Configuration Registers for reads and writes
	PCIE_Write32( hPCIe, pcie_bars[0], CRA, START_BYTE);
	//test SDRAM
	testDMA(hPCIe,SDRAM);			// Test the SDRAM for reads and writes

	PCIE_Write32( hPCIe, pcie_bars[0], CRA, STOP_BYTE);
	printf("\nPush up SW[16] to view data stored in SDRAM and use SW[3:0] to select different addresses.\n");
*/  
    //test Custom Logic
   
    
	return 0;
}

/*
//Tests 16 consecutive PCIE_Write32 to address

void test32( PCIE_HANDLE hPCIe, DWORD addr )
{
	BOOL bPass;
	DWORD testVal = 0xf;
	DWORD readVal;

	WORD i = 0;
	for (i = 0; i < 16 ; i++ )
	{
		bPass = PCIE_Write32( hPCIe, pcie_bars[0], addr, testVal);
		if (!bPass)
		{
			printf("test FAILED: write did not return success");
			return;
		}
		bPass = PCIE_Read32( hPCIe, pcie_bars[0], addr, &readVal);
		if (!bPass)
		{
			printf("test FAILED: read did not return success");
			return;
		}
		printf("Testing register %d at addr %x with value %x: ", i, addr, testVal);

		if (testVal == readVal)
		{
			printf("Test PASSED: expected %x, received %x\n", testVal, readVal);
		}
		else
		{
			printf("Test FAILED: expected %x, received %x\n", testVal, readVal);
		}
		testVal = testVal + 1;
		addr = addr + 4;
	}
	return;
}

//tests DMA write of buffer to address
void testDMA( PCIE_HANDLE hPCIe, DWORD addr)
{
	BOOL bPass;
	DWORD testArray[MAXDMA];
	DWORD readArray[MAXDMA];
	
	WORD i = 1;
	testArray[0] = START_BYTE;
	while ( i < MAXDMA )
	{
		testArray[i] = i  + 0x0f;
		i++;
	}

	//bPass = PCIE_DmaWrite(hPCIe, addr, testArray, MAXDMA * RWSIZE );
	//if (!bPass)
	//{
	//	printf("test FAILED: write did not return success");
	//	return;
	//}
	bPass = PCIE_DmaRead(hPCIe, addr, readArray, MAXDMA * RWSIZE );
	if (!bPass)
	{
		printf("test FAILED: read did not return success");
		return;
	}
	i = 0;
	while ( i < MAXDMA )
	{
		printf("Testing SDRAM at addr %x: ", addr);
		if (testArray[i] == readArray[i])
		{
			printf("Test PASSED: expected %x, received %x\n", testArray[i], readArray[i]);
		}
		else
		{
			printf("Test FAILED: expected %x, received %x\n", testArray[i], readArray[i]);
		}
		i++;
		addr = addr + 4;
	}
	return;
}
*/
BOOL NewReadImage(PCIE_HANDLE hPCIe, BITMAPINFOHEADER *info) 
{
	//checkImageDone(hPCIe);
	FILE* fp = fopen("out.bmp", "wb");

    static unsigned char //color[1], /* 1 byte = 8 bits */
                         byte[1];
    unsigned short word[1]; /* 2 bytes */
    unsigned long  dword[1], /* 4 bytes */
                   bpp=1,
                   /* in bytes */
                   FileHeaderSize=14, 
                   InfoHeaderSize=40, // header_bytes
                   PaletteSize=255*4, // = 8 = number of bytes in palette, 256 colors
                   BytesPerRow,
                   FileSize,
                   OffBits,
                   BytesSize, // bytes in image portion
                   //iByte, // number of byte
                   /* in pixels */
                   Width= info->width, 
                   Height= info->height;
                   /* in ... */
                   //ix, iy, ix_,
                   //bit=0, /* bit value */
                   //bitNumber; /* bits are numberd from 0 to 7 */

    BytesPerRow=(((Width * bpp)+31)/32)*4; 
    BytesSize=BytesPerRow*Height;
    FileSize=FileHeaderSize+InfoHeaderSize+PaletteSize+BytesSize;
    OffBits=FileHeaderSize+InfoHeaderSize+PaletteSize;

    word[0]=19778;                                         fwrite(word,1,2,fp); /* file Type signature = BM */
    dword[0]=FileSize;                                     fwrite(dword,1,4,fp); /* FileSize */
    word[0]=0;                                             fwrite(word,1,2,fp); /* reserved1 */
    word[0]=0;                                             fwrite(word,1,2,fp); /* reserved2 */
    dword[0]=OffBits;                                      fwrite(dword,1,4,fp); /* OffBits */
    dword[0]=InfoHeaderSize;                               fwrite(dword,1,4,fp); 
    dword[0]=Width;                                        fwrite(dword,1,4,fp); 
    dword[0]=Height;                                       fwrite(dword,1,4,fp); 
    word[0]=1;                                             fwrite(word,1,2,fp); /* planes */
    word[0]=8;                                             fwrite(word,1,2,fp); /* Bits of color per pixel */
    dword[0]=0;                                            fwrite(dword,1,4,fp); /* compression type */
    dword[0]=0;                                            fwrite(dword,1,4,fp); /* Image Data Size, set to 0 when no compression */
    dword[0]=0;                                            fwrite(dword,1,4,fp); /*  */
    dword[0]=0;                                            fwrite(dword,1,4,fp); /*  */
    dword[0]=255;                                          fwrite(dword,1,4,fp); /*  number of used coloors*/
    dword[0]=0;                                            fwrite(dword,1,4,fp); /*  */
 
    int i;
    for (i = 0; i < 256; i++)
    {
    	byte[0]=i;                                            fwrite(byte,1,1,fp); /* R */ 
		byte[0]=i;                                            fwrite(byte,1,1,fp); /* R */                                     
		byte[0]=i;                                            fwrite(byte,1,1,fp); /* G */    
		byte[0]=i;                                            fwrite(byte,1,1,fp); /* B */ 
    }          

    DWORD addr = 0x08500000; //new image starts from 0x08500000

	unsigned char *testImage;//test image buffer
	testImage = (unsigned char*)malloc(info->width*info->height*4*sizeof(unsigned char));
	BOOL bPass = PCIE_DmaRead(hPCIe, addr, testImage, info->width*info->height*4);
	if(!bPass)
	{
		printf("ERROR: unsuccessful image reading.\n");
		return FALSE;
	}
	else
		printf("Image read by the Atom.\n");


	unsigned char *bitmapImage;//image buffer
	bitmapImage = (unsigned char*)malloc(info->width*info->height*sizeof(unsigned char));
	int index = 0;
	int j;
	// Store only the LSByte of the 32-bit data into the new image file
	for (j = 0; j < info->width*info->height; ++j)
	{
		unsigned char buff;
		buff = testImage[index++];
		bitmapImage[j] = buff;
		buff = testImage[index++];
		buff = testImage[index++];
		buff = testImage[index++];
	}

	if(!bitmapImage)
	{
		printf("ERROR: new image is empty.\n");
		return FALSE;
	}
	fwrite(bitmapImage, info->width*info->height*sizeof(unsigned char),1,fp);
	free(bitmapImage);
	free(testImage);
	fclose(fp);
	printf("New image written to **out.bmp**.\n");
	return TRUE;
}

