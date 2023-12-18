#ifndef IMAGEPROCESSINGLIB_H
#define IMAGEPROCESSINGLIB_H

/*+++++++++++++++++++++++++++++++ under constuction +++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* BMP HEADER offset*/
#define BMP_HEADER_NAME_OFFSET                              0x00
#define BMP_HEADER_FILE_SIZE_OFFSET                         0x02
#define BMP_HEADER_IMAGE_DATA_START_ADDR_OFFSET             0x0A

/* BMP HEADER data offset size*/
#define BMP_HEADER_NAME_OFFSET_SIZE                         0x02    /* BMP_HEADER_NAME_OFFSET */
#define BMP_HEADER_FILE_SIZE_OFFSET_SIZE                    0x04    /* BMP_HEADER_FILE_SIZE_OFFSET */
#define BMP_HEADER_IMAGE_DATA_START_ADDR_OFFSET_SIZE        0x04    /* BMP_HEADER_IMAGE_DATA_START_ADDR_OFFSET */

/* DIB HEADER data offset */
#define DIB_HEADER_SIZE_OFFSET                          0x0E    /* the size of this header, in bytes (40) */
#define DIB_HEADER_WIDTH_DATA_OFFSET                    0x12    /* the bitmap width in pixels (signed integer) */
#define DIB_HEADER_HEIGHT_DATA_OFFSET                   0x16    /* the bitmap height in pixels (signed integer) */
#define DIB_HEADER_COLOR_PLANE_NUM_OFFSET               0x1A    /* the number of color planes (must be 1) */
#define DIB_HEADER_BITDEPTH_DATA_OFFSET                 0x1C    /* the number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32 */
#define DIB_HEADER_COMPRESS_METHOD_OFFSET               0x1E    /* the compression method being used. See the next table for a list of possible values */
#define DIB_HEADER_IMAGE_SIZE_OFFSET			        0x22	/*	the image size. This is the size of the raw bitmap data; a dummy 0 can be given for BI_RGB bitmaps.	*/
#define DIB_HEADER_HORIZONTAL_RESOLUTION_OFFSET			0x26	/*	the horizontal resolution of the image. (pixel per metre, signed integer)	*/
#define DIB_HEADER_VERTICAL_RESOLUTION_OFFSET			0x2A	/*	the vertical resolution of the image. (pixel per metre, signed integer)	*/
#define DIB_HEADER_NUM_COLOR_IN_COLOR_PALLETTE_OFFSET	0x2E	/*	the number of colors in the color palette, or 0 to default to 2n	*/
#define DIB_HEADER_IMPORTANT_COLOR_USED_OFFSET			0x32	/*	the number of important colors used, or 0 when every color is important; generally ignored	*/


/* DIB HEADER data offset size*/
#define DIB_HEADER_SIZE_OFFSET_SIZE                         0x04    /* DIB_HEADER_SIZE_OFFSET*/
#define DIB_HEADER_WIDTH_DATA_OFFSET_SIZE                   0x04    /* DIB_HEADER_WIDTH_DATA_OFFSET */
#define DIB_HEADER_HEIGHT_DATA_OFFSET_SIZE                  0x04    /* DIB_HEADER_HEIGHT_DATA_OFFSET */
#define DIB_HEADER_COLOR_PLANE_NUM_OFFSET_SIZE              0x02    /* DIB_HEADER_COLOR_PLANE_NUM_OFFSET */
#define DIB_hEADER_BITDEPTH_DATA_OFFSET_SIZE                0x02    /* DIB_HEADER_BITDEPTH_DATA_OFFSET */
#define DIB_HEADER_COMPRESS_METHOD_OFFSET_SIZE              0x04    /* DIB_HEADER_COMPRESS_METHOD_OFFSET */
#define DIB_HEADER_IMAGE_SIZE_OFFSET_SIZE			        0x04	/* DIB_HEADER_IMAGE_SIZE_OFFSET */
#define DIB_HEADER_HORIZONTAL_RESOLUTION_OFFSET_SIZE       	0x04	/* DIB_HEADER_HORIZONTAL_RESOLUTION_OFFSET */
#define DIB_HEADER_VERTICAL_RESOLUTION_OFFSET_SIZE			0x04	/* DIB_HEADER_VERTICAL_RESOLUTION_OFFSET */
#define DIB_HEADER_NUM_COLOR_IN_COLOR_PALLETTE_OFFSET_SIZE	0x04	/* DIB_HEADER_NUM_COLOR_IN_COLOR_PALLETTE_OFFSET */
#define DIB_HEADER_IMPORTANT_COLOR_USED_OFFSET_SIZE			0x04	/* DIB_HEADER_IMPORTANT_COLOR_USED_OFFSET */



typedef struct BMP_header{
    char   name[2];
    unsigned int size;      //	The size of the BMP file in bytes
    unsigned int offset;    //The offset, i.e. starting address, of the byte where the bitmap image data (pixel array) can be found.
};

struct DIB_header{
    unsigned int header_size;           /* the size of this header, in bytes (40) */
    unsigned int width;				    /* the bitmap width in pixels (signed integer) */
    unsigned int height;			    /*	the bitmap height in pixels (signed integer) */
    unsigned int color_planes_num;	    /* the number of color planes (must be 1) */
    unsigned int bitDepth;			    /* the number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32. */
    unsigned int compression_method;    /* the compression method being used. See the next table for a list of possible values */
    unsigned int imgae_size;			/* the image size. This is the size of the raw bitmap data; a dummy 0 can be given for BI_RGB bitmaps. */
    unsigned int horizontal_resolution;	/*	the horizontal resolution of the image. (pixel per metre, signed integer) */
    unsigned int vertical_resolution;	/* the vertical resolution of the image. (pixel per metre, signed integer) */
    unsigned int color_num;				/* the number of colors in the color palette, or 0 to default to 2n */
    unsigned int important_color_used;	/* the number of important colors used, or 0 when every color is important; generally ignored */
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/* will remove after done define */
#define BMP_HEADER_WIDTH_DATA_OFFSET            0x12    /* the bitmap width in pixels (signed integer) */
#define BMP_HEADER_WIDTH_DATA_SIZE              0x04    /* the bitmap width data size */
#define BMP_HEADER_HEIGHT_DATA_OFFSET           0x16    /* the bitmap height in pixels (signed integer) */
#define BMP_HEADER_HEIGHT_DATA_SIZE             0x04    /* the bitmap height data size */
#define BMP_HEADER_BITDEPTH_DATA_OFFSET         0x1C    /* the bitmap bit depth data offset */
#define BMP_HEADER_BITDEPTH_DATA_SIZE           0x02    /* the bitmap bit depth data size */

#define BMP_IMAGE_TYPE_ERROR          0
#define BMP_IMAGE_TYPE_RGB            1
#define BMP_IMAGE_TYPE_GREY_SCALE     2
#define BMP_IMAGE_TYPE_BINARY         3

#define BMP_HEADER_SIZE         54
#define BMP_COLOR_TABLE_SIZE    1024


#define BMP_BIT_DEPTH_1         1
#define BMP_BIT_DEPTH_4         4
#define BMP_BIT_DEPTH_8         8
#define BMP_BIT_DEPTH_16        16
#define BMP_BIT_DEPTH_24        24
#define BMP_BIT_DEPTH_32        32


#define BMP_RGB_CHANNEL_RED     0
#define BMP_RGB_CHANNEL_GREEN   1
#define BMP_RGB_CHANNEL_BLUE    2
#define BMP_RGB_CHANNEL_NUM     3

#define WHITE   255
#define BLACK   0


extern int  imageReader(const char *imgName, int *_height, int *_width, int *_bitDepth, unsigned char *_header, unsigned char *_colorTable, unsigned char *_buf);
extern void imageWriter(const char *imgName, unsigned char *header, unsigned char *colorTable, unsigned char *buf, int bitDepth); 
extern int ImageRgbToGrey(const char *imgName);
extern int ImageRgbOrGreyScaleCheck(const char *imgName);
extern int ImageBinarization(const char *imgName, int threshold);

#endif // IMAGEPROCESSINGLIB_H
