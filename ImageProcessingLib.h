#ifndef IMAGEPROCESSINGLIB_H
#define IMAGEPROCESSINGLIB_H



#define BMP_IMAGE_TYPE_ERROR          0
#define BMP_IMAGE_TYPE_RGB            1
#define BMP_IMAGE_TYPE_GREY_SCALE     2

#define BMP_HEADER_SIZE         54
#define BMP_COLOR_TABLE_SIZE    1024

#define BMP_HEADER_WIDTH_DATA_OFFSET            0x12    /* the bitmap width in pixels (signed integer) */
#define BMP_HEADER_WIDTH_DATA_SIZE              0x04    /* the bitmap width data size */
#define BMP_HEADER_HEIGHT_DATA_OFFSET           0x16    /* the bitmap height in pixels (signed integer) */
#define BMP_HEADER_HEIGHT_DATA_SIZE             0x04    /* the bitmap height data size */
#define BMP_HEADER_BITDEPTH_DATA_OFFSET         0x1C    /* the bitmap bit depth data offset */
#define BMP_HEADER_BITDEPTH_DATA_SIZE           0x02    /* the bitmap bit depth data size */

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

extern int  imageReader(const char *imgName, int *_height, int *_width, int *_bitDepth, unsigned char *_header, unsigned char *_colorTable, unsigned char *_buf);
extern void imageWriter(const char *imgName, unsigned char *header, unsigned char *colorTable, unsigned char *buf, int bitDepth); 
extern int ImageRgbToGrey(const char *imgName);
extern int ImageRgbOrGreyScaleCheck(const char *imgName);

#endif // IMAGEPROCESSINGLIB_H
