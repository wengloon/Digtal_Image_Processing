#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ImageProcessingLib.h"


/*
* Bmp image file read
*/
int  imageReader(const char *imgName, int *_height, int *_width, int *_bitDepth, unsigned char *_header, unsigned char *_colorTable, unsigned char *_buf)
{
FILE *streamIn;
int ret, imageSize;

    streamIn = fopen(imgName,"rb");

    ret = true;
    if(streamIn ==(FILE *)NULL)
    {
        ret = false;
        printf("Unable to read image \n");
    }
    else
    {
        for(int i =0;i<BMP_HEADER_SIZE;i++)
        {
            _header[i] = getc(streamIn);
        }

        *_width = *(int *)&_header[BMP_HEADER_WIDTH_DATA_OFFSET];
        *_height = *(int *)&_header[BMP_HEADER_HEIGHT_DATA_OFFSET];
        *_bitDepth = *(int *)&_header[BMP_HEADER_BITDEPTH_DATA_OFFSET];
        imageSize  = (*_width)*(*_height);
        printf(" image height %d\n", *_height);
        printf(" image width %d\n", *_width);

        if(*_bitDepth <= BMP_BIT_DEPTH_8)
        { /* below or equal to 8bit depth, means this image contain color table */
            fread(_colorTable,sizeof(unsigned char),BMP_COLOR_TABLE_SIZE,streamIn);
        }

        fread(_buf,sizeof(unsigned char),imageSize,streamIn);
        printf("success read image \n");

    }

    fclose(streamIn);

    return ret;
}

/*
* Bmp image file write
*/
void imageWriter(const char *imgName, unsigned char *header, unsigned char *colorTable, unsigned char *buf, int bitDepth)
{
int imageSize, imageWidth, imageHeight;
FILE *fo;

	fo = fopen(imgName,"wb");
    imageWidth = *(int *)&header[BMP_HEADER_WIDTH_DATA_OFFSET];
    imageHeight = *(int *)&header[BMP_HEADER_HEIGHT_DATA_OFFSET];
    imageSize  = imageWidth*imageHeight;
    fwrite(header,sizeof(unsigned char),BMP_HEADER_SIZE,fo);
    if(bitDepth <= BMP_HEADER_SIZE)
    { /* below or equal to 8bit depth, means this image contain color table */
        fwrite(colorTable,sizeof(unsigned char),BMP_COLOR_TABLE_SIZE,fo);
    }
    fwrite(buf,sizeof(unsigned char),imageSize, fo);
    fclose(fo);
	printf("success write image \n");

}

/*
* convert Bmp color image to grey scale
*/
int ImageRgbToGrey(const char *imgName)
{
unsigned char imgHeader[BMP_HEADER_SIZE];
unsigned char colorTable[BMP_COLOR_TABLE_SIZE];
FILE *fIn;
FILE *fOut;
int ret, imageSize, imageHeight, imageWidth, imageBitDepth, index, temp, len;
char *dot;
char greyName[] = "_Grey.bmp";
char newName[strlen(imgName) + strlen(greyName)];

	ret = true;
    dot = strchr(imgName, '.');
    index = (int)(dot - imgName);
    strcpy(newName, imgName);
    for(int i=0; i< sizeof(greyName); i++)
    {
        newName[index + i] = greyName[i];
    }

    fIn = fopen(imgName,"rb");

    if(fIn == NULL)
    {
        printf("Unable to open image\n");
		ret = false;
    }
    else
    {
		fOut = fopen(newName,"wb");
        for(int i =0;i<BMP_HEADER_SIZE;i++)
        {
            imgHeader[i] = getc(fIn);
        }
        fwrite(imgHeader,sizeof(unsigned char),BMP_HEADER_SIZE,fOut);

        imageHeight = *(int*)&imgHeader[BMP_HEADER_HEIGHT_DATA_OFFSET];
        imageWidth  = *(int*)&imgHeader[BMP_HEADER_WIDTH_DATA_OFFSET];
        imageBitDepth = *(int*)&imgHeader[BMP_HEADER_BITDEPTH_DATA_OFFSET];

        if(imageBitDepth <= BMP_BIT_DEPTH_8)
        {   /* below or equal to 8bit depth, means this image contain color table */
            fread(colorTable,sizeof(unsigned char),BMP_COLOR_TABLE_SIZE,fIn);
            fwrite(colorTable,sizeof(unsigned char),BMP_COLOR_TABLE_SIZE,fOut);
        }

        int imageSize = imageHeight * imageWidth;
        unsigned char buffer[imageSize][BMP_RGB_CHANNEL_NUM];

        for(int i =0;i<imageSize;i++)
        {
            temp = 0;
            for(int j = 0; j < BMP_RGB_CHANNEL_NUM; j++)
            {   /* get RGB channel value */
                buffer[i][j] = getc(fIn);
            }
            temp = (buffer[i][BMP_RGB_CHANNEL_RED]*0.3 )+(buffer[i][BMP_RGB_CHANNEL_GREEN]*0.59)+(buffer[i][BMP_RGB_CHANNEL_BLUE]*0.11);
            for(int j = 0; j < BMP_RGB_CHANNEL_NUM; j++)
            {
                putc(temp,fOut);
            }
        }
		fclose(fOut);
        printf("Bmp RGB to grey Success!\n");
    }

    fclose(fIn);
	return ret;
}

/*
* check bmp image type, RGB or greyscale
*/
int ImageRgbOrGreyScaleCheck(const char *imgName)
{
unsigned char imgHeader[BMP_HEADER_SIZE];
unsigned char colorTable[BMP_COLOR_TABLE_SIZE];
FILE *fIn;
int ret, imageSize, imageHeight, imageWidth, imageBitDepth, index, temp, len;

    fIn = fopen(imgName,"rb");

    if(fIn == NULL)
    {
        printf("Unable to open image\n");
		ret = BMP_IMAGE_TYPE_ERROR;
    }
    else
    {
        ret = BMP_IMAGE_TYPE_GREY_RGB;
        for(int i =0;i<BMP_HEADER_SIZE;i++)
        {
            imgHeader[i] = getc(fIn);
        }

        imageHeight = *(int*)&imgHeader[BMP_HEADER_HEIGHT_DATA_OFFSET];
        imageWidth  = *(int*)&imgHeader[BMP_HEADER_WIDTH_DATA_OFFSET];
        imageBitDepth = *(int*)&imgHeader[BMP_HEADER_BITDEPTH_DATA_OFFSET];
		
		ret = (imageBitDepth <= BMP_BIT_DEPTH_8) ? BMP_IMAGE_TYPE_GREY : BMP_IMAGE_TYPE_RGB;
		if(ret == BMP_IMAGE_TYPE_GREY )
		{
			printf("it is a greyscale");
		}
		else if(ret == BMP_IMAGE_TYPE_RGB )
		{
			printf("it is a RGB");
		}
		else
		{	/* should not reach here */
			printf("image error");
		}
    }
	fclose(fIn);
    return ret;
}

int ImageBinarization(const char *imgName, int threshold)
{
unsigned char imgHeader[BMP_HEADER_SIZE];
unsigned char colorTable[BMP_COLOR_TABLE_SIZE];
FILE *fIn;
FILE *fOut;
int ret, imageSize, imageHeight, imageWidth, imageBitDepth, index, temp, len;
char *dot;
char BlackWhiteName[] = "_Bw.bmp";
char newName[strlen(imgName) + strlen(BlackWhiteName)];

	ret = true;
    dot = strchr(imgName, '.');
    index = (int)(dot - imgName);
    strcpy(newName, imgName);
    for(int i=0; i< sizeof(BlackWhiteName); i++)
    {
        newName[index + i] = BlackWhiteName[i];
    }
    printf("Holla %s", newName);

    fIn = fopen(imgName,"rb");

    if(fIn == NULL)
    {
        printf("Unable to open image\n");
        ret = false;
    }
    else
    {
        fOut = fopen(newName,"wb");
        for(int i =0;i<BMP_HEADER_SIZE;i++)
        {
           imgHeader[i] = getc(fIn);
        }
        fwrite(imgHeader,sizeof(unsigned char),BMP_HEADER_SIZE,fOut);

        imageHeight = *(int*)&imgHeader[BMP_HEADER_HEIGHT_DATA_OFFSET];
        imageWidth  = *(int*)&imgHeader[BMP_HEADER_WIDTH_DATA_OFFSET];
        imageBitDepth = *(int*)&imgHeader[BMP_HEADER_BITDEPTH_DATA_OFFSET];

        if(imageBitDepth <= BMP_BIT_DEPTH_8)
        {   /* below or equal to 8bit depth, means this image contain color table */
            fread(colorTable,sizeof(unsigned char),BMP_COLOR_TABLE_SIZE,fIn);
            fwrite(colorTable,sizeof(unsigned char),BMP_COLOR_TABLE_SIZE,fOut);
        }

        int imageSize = imageHeight * imageWidth;
        unsigned char buffer[imageSize];
        fread(buffer,sizeof(unsigned char),imageSize,fIn);

        for(int i =0;i<imageSize;i++)
        {
            buffer[i] = (buffer[i]>threshold)? WHITE : BLACK;
        }
        fwrite(buffer,sizeof(unsigned char), imageSize,fOut);
        fclose(fIn);
        fclose(fOut);
        printf("Bmp binarization Success!\n");
    }

	return ret;
}