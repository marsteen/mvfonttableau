//***************************************************************************
//
//
// @PROJECT  :	Create Container
// @VERSION  :	1.0
// @FILENAME :	CGraflibPng.cpp
// @DATE     :	16.9.2014
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************

#include <iostream>
#include <fstream>
#include <string>

#include <pnglib/png.h>
#include <CException.h>
#include <CFileIO.h>
#include <SFileReadInfo.h>
#include <CGraflibPng.h>

using namespace std;



typedef struct
{
    png_structp		png_ptr;
    png_infop		info_ptr;
    png_color*		palette;
    int				num_palette;
    unsigned char*	trans_pal;
    int				num_trans;
}
PngInfoStruct;



static void (* PngStatusFunction)(int Status);


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   SendPngStatus
//
//
//---------------------------------------------------------------------------

static void SendPngStatus(int Status)
{
    if (PngStatusFunction != NULL)
    {
        (*PngStatusFunction)(Status);
    }
}


#define streams
#define entire
//#define hilevel


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   write_png
//
//
//---------------------------------------------------------------------------

static int write_png(const char* file_name, int Width, int Height, int BytePerPixel, void* PixelData)
{
    FILE* fp;
    int i;
    png_bytepp RowPointers;
    png_structp png_ptr;
    png_infop info_ptr;
    png_colorp palette;
    int png_transforms = PNG_TRANSFORM_BGR;

    /* open the file */
    fp = fopen(file_name, "wb");
    if (fp == NULL)
    {
        return 0;
    }

    RowPointers = (png_bytepp)malloc(Height * sizeof(void*));

    for (i = 0; i < Height; i++)
    {
        RowPointers[i] = (png_bytep)((char*)PixelData) + (i * Width * BytePerPixel);
    }

    /* Create and initialize the png_struct with the desired error handler
     * functions.  If you want to use the default stderr and longjump method,
     * you can supply NULL for the last three parameters.  We also check that
     * the library version is compatible with the one used at compile time,
     * in case we are using dynamically linked libraries.  REQUIRED.
     */

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (png_ptr == NULL)
    {
        fclose(fp);
        return 0;
    }

    /* Allocate/initialize the image information data.  REQUIRED */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        fclose(fp);
        png_destroy_write_struct(&png_ptr, png_infopp_NULL);
        return 0;
    }

    png_set_rows(png_ptr, info_ptr, RowPointers);

    switch (BytePerPixel)
    {
        case 3:

            png_set_IHDR(png_ptr, info_ptr, Width, Height, 8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
            break;


        case 4:

            png_set_IHDR(png_ptr, info_ptr, Width, Height, 8, PNG_COLOR_TYPE_RGBA,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
            break;

        default:

            /* unsupported Pixel format */
            break;
    }



    /* Set error handling.  REQUIRED if you aren't supplying your own
     * error handling functions in the png_create_write_struct() call.
     */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        /* If we get here, we had a problem reading the file */
        fclose(fp);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    /* This is the easy way.  Use it if you already have all the
     * image info living info in the structure.  You could "|" many
     * PNG_TRANSFORM flags into the png_transforms integer here.
     */
    png_write_png(png_ptr, info_ptr, png_transforms, png_voidp_NULL);


    /* clean up after the write, and free any memory allocated */
    png_destroy_write_struct(&png_ptr, &info_ptr);

    free(RowPointers);

    /* close the file */
    fclose(fp);

    /* that's it */
    return 1;
}


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   read_png
//
//
//---------------------------------------------------------------------------

static void read_png(FILE* fp, unsigned int sig_read, PngInfoStruct* pinf)
{
    //png_structp png_ptr;
    //png_infop info_ptr;
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;

    /* Create and initialize the png_struct with the desired error handler
     * functions.  If you want to use the default stderr and longjump method,
     * you can supply NULL for the last three parameters.  We also supply the
     * the compiler header file version, so that we know if the application
     * was compiled with a compatible version of the library.  REQUIRED
     */
    pinf->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
            //(png_voidp) user_error_ptr, user_error_fn, user_warning_fn);
            NULL, NULL, NULL);

    if (pinf->png_ptr == NULL)
    {
        fclose(fp);
        return;
    }

    /* Allocate/initialize the memory for image information.  REQUIRED. */
    pinf->info_ptr = png_create_info_struct(pinf->png_ptr);
    if (pinf->info_ptr == NULL)
    {
        fclose(fp);
        png_destroy_read_struct(&pinf->png_ptr, png_infopp_NULL, png_infopp_NULL);
        return;
    }

    /* Set error handling if you are using the setjmp/longjmp method (this is
     * the normal method of doing things with libpng).  REQUIRED unless you
     * set up your own error handlers in the png_create_read_struct() earlier.
     */

    if (setjmp(png_jmpbuf(pinf->png_ptr)))
    {
        /* Free all of the memory associated with the png_ptr and info_ptr */
        png_destroy_read_struct(&pinf->png_ptr, &pinf->info_ptr, png_infopp_NULL);
        fclose(fp);
        /* If we get here, we had a problem reading the file */
        return;
    }

    png_init_io(pinf->png_ptr, fp);

    /* If we have already read some of the signature */
    png_set_sig_bytes(pinf->png_ptr, sig_read);

    /* #ifdef hilevel */

    /*
     * If you have enough memory to read in the entire image at once,
     * and you need to specify only transforms that can be controlled
     * with one of the PNG_TRANSFORM_* bits (this presently excludes
     * dithering, filling, setting background, and doing gamma
     * adjustment), then you can read the entire image (including
     * pixels) into the info structure with this call:
     */
    //png_read_png(png_ptr, info_ptr, png_transforms, png_voidp_NULL);



    png_read_png(pinf->png_ptr, pinf->info_ptr, 0, png_voidp_NULL);

    pinf->palette = NULL;
    pinf->trans_pal = NULL;


    if (pinf->info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
    {
        //cout << "!!! png_get_PLTE" << endl;
        png_get_PLTE(pinf->png_ptr, pinf->info_ptr, &pinf->palette, &pinf->num_palette);
        //cout << "pinf->num_palette=" << pinf->num_palette << endl;
    }
    else
    {
        pinf->palette = NULL;
    }

    if (png_get_valid(pinf->png_ptr, pinf->info_ptr, PNG_INFO_tRNS))
    {
        if (pinf->info_ptr->valid & PNG_INFO_tRNS)
        {
            png_bytep trans;
            int num_trans;
            png_color_16p trans_values;

            cout << "!!! png_get_tRNS"<< endl;

            png_get_tRNS(pinf->png_ptr, pinf->info_ptr, &trans, &num_trans, &trans_values);

            cout << "num_trans=" << num_trans << endl;
            cout << "trans[0]=" << (int)trans[0] << endl;
            cout << "trans_values=" << (int)trans_values[0].index << endl;

            /*
             *      for (int n = 0; n < num_trans; n++)
             *      {
             *          cout << "  " << n << " trans=" << (int) trans[n] << endl;
             *      }
             */
            pinf->trans_pal = new unsigned char[256];
            memset(pinf->trans_pal, 255, 256);
            pinf->num_trans = num_trans;
            memcpy(pinf->trans_pal, trans, num_trans);
        }
        else
        {
            pinf->trans_pal = NULL;
        }
    }

/*
 *   printf("Breite    : %d\n",   pinf->info_ptr->width);
 * printf("Hoehe     : %d\n",   pinf->info_ptr->height);
 *   printf("Channels  : %d\n",   pinf->info_ptr->channels);
 * printf("Bits/Pixel: %d\n",   pinf->info_ptr->pixel_depth);
 * printf("Bits/Channel: %d\n", pinf->info_ptr->bit_depth);
 * printf("Rowbytes  : %d\n",  png_get_rowbytes(pinf->png_ptr, pinf->info_ptr));
 *   printf("Colortype : %d\n",  png_get_color_type(pinf->png_ptr, pinf->info_ptr));
 */



    /* close the file */

    fclose(fp);

    SendPngStatus(1);



    /* that's it */
    return;
}


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   finish_read
//
//
//---------------------------------------------------------------------------

static void finish_read(PngInfoStruct* pinf)
{
    png_destroy_read_struct(&pinf->png_ptr, &pinf->info_ptr, png_infopp_NULL);
    delete[] pinf->trans_pal;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflibPng
// Methode:   Read
//
//
//---------------------------------------------------------------------------

bool CGraflibPng::Read(const char* Filename, SFileReadInfo* fri)
{
    //Debug2 << DBON;
    //Debug2 << "CGraflibPng::Read START " << Filename << DBLF;

    PngInfoStruct pinfo;
    int FileSize;
    unsigned char* PicData = NULL;

    FILE* fp = (FILE*)fopen(Filename, "rb");

    if (fp == NULL)
    {
        SetError(EGRAFLIB_ERROR_READ, __func__, Filename);
        return false;
    }
    else
    {
        int ByteProPixel;

        read_png(fp, 0, &pinfo);

        mWidth = pinfo.info_ptr->width;
        mHeight = pinfo.info_ptr->height;


        switch (pinfo.info_ptr->color_type)
        {
            case PNG_COLOR_TYPE_GRAY:
            case PNG_COLOR_TYPE_GRAY_ALPHA:

                ByteProPixel = 1;
                break;

            case PNG_COLOR_TYPE_RGB:

                ByteProPixel = 3;
                break;

            case PNG_COLOR_TYPE_RGB_ALPHA:

                ByteProPixel = 4;
                break;

            case PNG_COLOR_TYPE_PALETTE:

                //cout << "***** CGraflibPng::Read unsupported color type: PNG_COLOR_TYPE_PALETTE"  << endl;
                ByteProPixel = 1;
                break;

            default:

                cout << "***** CGraflibPng::Read unsupported color type " << (int)pinfo.info_ptr->color_type << endl;
                exit(0);
                break;
        }


        mBits = ByteProPixel * 8;
        mData = (CPixelT*)new unsigned char[pinfo.info_ptr->width *  pinfo.info_ptr->height * ByteProPixel];

        unsigned char* PicDataPtr = (unsigned char*)mData;

/*
 *      cout << "CGraflibPng::Read Width =" << mWidth << endl;
 *      cout << "CGraflibPng::Read Height=" << mHeight << endl;
 *      cout << "CGraflibPng::Read Bits  =" << mBits << endl;
 *      cout << "pinf->info_ptr->pixel_depth=" << (int) pinfo.info_ptr->pixel_depth << endl;
 *      cout << "pinfo.info_ptr->channels   =" << (int) pinfo.info_ptr->channels << endl;
 *      cout << "pinfo.info_ptr->color_type   =" << pinfo.info_ptr->color_type << endl;
 */

        int BytesPerLine = mWidth * ByteProPixel;

        for (int i = 0; i < mHeight; i++)
        {
            memcpy(PicDataPtr, pinfo.info_ptr->row_pointers[i], BytesPerLine);
            PicDataPtr += BytesPerLine;
        }



        if (pinfo.info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
        {
            if (pinfo.palette != NULL)
            {
                if (pinfo.trans_pal != NULL)
                {
                    cout << "ConvertPalToRGBA START" << endl;
                    ConvertPalToRGBA((CPixel24*)pinfo.palette, pinfo.trans_pal);
                    cout << "ok" << endl;
                }
                else
                {
                    cout << "ConvertPalToRGB START" << endl;
                    ConvertPalToRGB((CPixel24*)pinfo.palette);
                    cout << "ok" << endl;
                }
            }
        }


        finish_read(&pinfo);

        //SwapRedBlue();

        return true;
    }
    return false;
}


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   write_pngGrayscale
//
//
//---------------------------------------------------------------------------

static int write_pngGrayscale(const char* file_name, int Width, int Height, void* PixelData, int ByteProPixel)
{
    FILE* fp;
    int i;
    png_bytepp RowPointers;
    png_structp png_ptr;
    png_infop info_ptr;
    png_colorp palette;
    int png_transforms = PNG_TRANSFORM_BGR;

    /* open the file */
    fp = fopen(file_name, "wb");
    if (fp == NULL)
    {
        return 0;
    }

    RowPointers = (png_bytepp)malloc(Height * sizeof(void*));

    for (i = 0; i < Height; i++)
    {
        RowPointers[i] = (png_bytep)((char*)PixelData) + (i * Width * ByteProPixel);
    }

    /* Create and initialize the png_struct with the desired error handler
     * functions.  If you want to use the default stderr and longjump method,
     * you can supply NULL for the last three parameters.  We also check that
     * the library version is compatible with the one used at compile time,
     * in case we are using dynamically linked libraries.  REQUIRED.
     */

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (png_ptr == NULL)
    {
        fclose(fp);
        return 0;
    }

    /* Allocate/initialize the image information data.  REQUIRED */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        fclose(fp);
        png_destroy_write_struct(&png_ptr, png_infopp_NULL);
        return 0;
    }

    png_set_rows(png_ptr, info_ptr, RowPointers);

    png_set_IHDR(png_ptr, info_ptr, Width, Height, ByteProPixel * 8, PNG_COLOR_TYPE_GRAY,
        PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);


    /* Set error handling.  REQUIRED if you aren't supplying your own
     * error handling functions in the png_create_write_struct() call.
     */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        /* If we get here, we had a problem reading the file */
        fclose(fp);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    /* This is the easy way.  Use it if you already have all the
     * image info living info in the structure.  You could "|" many
     * PNG_TRANSFORM flags into the png_transforms integer here.
     */
    png_write_png(png_ptr, info_ptr, png_transforms, png_voidp_NULL);


    /* clean up after the write, and free any memory allocated */
    png_destroy_write_struct(&png_ptr, &info_ptr);

    free(RowPointers);

    /* close the file */
    fclose(fp);

    /* that's it */
    return 1;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflibPng
// Methode:   Write
//
//
//---------------------------------------------------------------------------

int CGraflibPng::Write(const char* Filename, bool bCompFlag, CFileIO* fio)
{
    SwapRedBlue();

    switch (mBits)
    {
        case 8:
            write_pngGrayscale(Filename, mWidth, mHeight, mData, 1);
            break;

        case 16:
            write_pngGrayscale(Filename, mWidth, mHeight, mData, 2);
            break;

        case 24:
            write_png(Filename, mWidth, mHeight, 3, mData);
            break;



        case 32:
            write_png(Filename, mWidth, mHeight, 4, mData);
            break;
    }
    SwapRedBlue();
}
