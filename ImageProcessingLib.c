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
//        printf(" image height %d\n", *_height);
//        printf(" image width %d\n", *_width);

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
int PixelArrayOffset;

    fIn = fopen(imgName,"rb");

    if(fIn == NULL)
    {
        printf("Unable to open image\n");
		ret = BMP_IMAGE_TYPE_ERROR;
    }
    else
    {
        ret = BMP_IMAGE_TYPE_ERROR;
        for(int i =0;i<BMP_HEADER_SIZE;i++)
        {
            imgHeader[i] = getc(fIn);
        }
        
		PixelArrayOffset = *(int*)&imgHeader[BMP_HEADER_IMAGE_DATA_START_ADDR_OFFSET];
        imageHeight = *(int*)&imgHeader[BMP_HEADER_HEIGHT_DATA_OFFSET];
        imageWidth  = *(int*)&imgHeader[BMP_HEADER_WIDTH_DATA_OFFSET];
        imageBitDepth = *(int*)&imgHeader[BMP_HEADER_BITDEPTH_DATA_OFFSET];

        int imageSize = imageHeight * imageWidth;
        unsigned char buffer[imageSize][BMP_RGB_CHANNEL_NUM];
        unsigned char buffer1channel[imageSize];

        switch (imageBitDepth)
        {
            case BMP_BIT_DEPTH_1:
                ret = BMP_IMAGE_TYPE_STANDARD_BINARY;
                break;
            case BMP_BIT_DEPTH_8:
                ret = BMP_IMAGE_TYPE_NOT_STANDARD_BINARY;
                for(int i =0;i<( PixelArrayOffset-BMP_HEADER_SIZE) ;i++)
                {   /*data dump*/
                     temp = getc(fIn);
                }
                for(int j=0; j< imageSize; j++)
                {
                    buffer1channel[j] = getc(fIn);
                    temp = buffer1channel[j];
                    if(((buffer1channel[j]) != BMP_COLOR_BLACK ) && 
                        ((buffer1channel[j]) != BMP_COLOR_WHITE ))
                    {
                        ret = BMP_IMAGE_TYPE_STANDARD_GREY_SCALE;
                        break;
                    }
                }
                break;
            case BMP_BIT_DEPTH_16:
            case BMP_BIT_DEPTH_24:
            case BMP_BIT_DEPTH_32:
                ret = BMP_IMAGE_TYPE_NOT_STANDARD_GREY_SCALE;
                for(int i =0;i<( PixelArrayOffset-BMP_HEADER_SIZE) ;i++)
                {   /*data dump*/
                     temp = getc(fIn);
                }
                for(int i =0;i<imageSize;i++)
                {
                    temp = 0;
                    for(int j = 0; j < BMP_RGB_CHANNEL_NUM; j++)
                    {   /* get RGB channel value */
                        buffer[i][j] = getc(fIn);
                        //printf("channel %d\n", buffer[i][j]);
                    }
                    //printf("\n");
                    if( ( buffer[i][BMP_RGB_CHANNEL_RED] != buffer[i][BMP_RGB_CHANNEL_GREEN] ) ||
                        ( buffer[i][BMP_RGB_CHANNEL_RED] != buffer[i][BMP_RGB_CHANNEL_BLUE] ) ||
                        ( buffer[i][BMP_RGB_CHANNEL_BLUE] != buffer[i][BMP_RGB_CHANNEL_GREEN] ))
                    {
                        /* 3 channel not same, not grey scale */
                        ret = BMP_IMAGE_TYPE_RGB;
                        break;
                    }
                }
                break;
            default:
                /* should not reach here */
                printf("image error\n");
                break;
        }
        switch (ret)
        {
            case BMP_IMAGE_TYPE_ERROR:
                printf("image error\n");
                break;
            case BMP_IMAGE_TYPE_RGB:
                printf("is it a RGB bmp image\n");
                break;
            case BMP_IMAGE_TYPE_STANDARD_GREY_SCALE:
                printf("it is a standard greyscale image\n");
                break;
            case BMP_IMAGE_TYPE_NOT_STANDARD_GREY_SCALE:
                printf("it is not a standard greyscale image\n");
                break;
            case BMP_IMAGE_TYPE_STANDARD_BINARY:
                printf("it is a standard binary image\n");
                break;
            case BMP_IMAGE_TYPE_NOT_STANDARD_BINARY:
                printf("it is not a standard binary image\n");
                break;
        
        default:
            break;
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
//    printf("Holla %s", newName);

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
            buffer[i] = (buffer[i]>threshold)? BMP_COLOR_WHITE : BMP_COLOR_BLACK;
        }
        fwrite(buffer,sizeof(unsigned char), imageSize,fOut);
        fclose(fIn);
        fclose(fOut);
        printf("Bmp binarization Success!\n");
    }

	return ret;
}