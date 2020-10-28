
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <CDataRect.h>

#include <libpng_16/png.h>
#include <libpng_16/pngstruct.h>
#include <libpng_16/pnginfo.h>


extern "C"
{
  #include <PnglibInterface.h>
};

/*
	typedef struct
	{
		png_structp    png_ptr;
		png_infop      info_ptr;
		png_color*     palette;
		int            num_palette;
		unsigned char* trans_pal;
		int            num_trans;

	}
	PngInfoStruct;
*/

namespace NPng16
{



	static void (*PngStatusFunction)(int Status);
	bool png_read_filepointer(FILE* fp, size_t file_pos, CDataRect& dr);

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
	
	
	//---------------------------------------------------------------------------
	//
	// Klasse:    CGraflibPng
	// Methode:   Read
	//
	//
	//---------------------------------------------------------------------------

	bool png_read_file(const char* Filename, size_t file_pos, CDataRect& dr)
	{
		bool r = false;
		FILE* fp = (FILE*) fopen(Filename, "rb");  
		
		if (fp != NULL)
		{
			r = png_read_filepointer(fp, file_pos, dr);
		}
		else
		{

		}
		
		return r;
	}


	//---------------------------------------------------------------------------
	//
	// Klasse:    CGraflibPng
	// Methode:   Read
	//
	//
	//---------------------------------------------------------------------------

	bool png_read_filepointer(FILE* fp, size_t file_pos, CDataRect& dr)
	{
		
		int r = false;

		//PngStatusFunction = MyPngStatusFunction;
		
		PngInfoStruct pinfo;
		int FileSize;
		unsigned char* PicData = NULL;

		fseek(fp, file_pos, 0);

		//GlobalDebug("  open ok");

		if (fp == NULL)
		{
		
			
		//	GlobalDebugT("***** fopen error:", Filename, mDebugLevel);
		}
		else
		{
			//cout  << "start read_png" << endl;

			if (read_png(fp, 0, &pinfo))
			{
				//GlobalDebug("  read_png ok");



		
		


				dr.mWidth  = pinfo.info_ptr->width;
				dr.mHeight = pinfo.info_ptr->height;

		/*
				cout << "pinfo.bit_depth         =" <<  pinfo.bit_depth << endl;
				cout << "pinfo.info_ptr->channels=" <<  (int) pinfo.info_ptr->channels << endl;
				cout << "pinfo.info_ptr->width   =" << pinfo.info_ptr->width << endl;
				cout << "pinfo.info_ptr->height  =" << pinfo.info_ptr->height << endl;
				cout << "width =" << mWidth << endl;
				cout << "height=" << mHeight << endl;
	*/      



				dr.mBits   = pinfo.bit_depth *  pinfo.info_ptr->channels;
				int ByteProPixel = dr.mBits / 8;
				dr.mData   = (CPixel*) new unsigned char[dr.mWidth * dr.mHeight * ByteProPixel];
				
				//cout << "W=" << mWidth << " H=" << mHeight << " Bits=" << mBits << endl;
				//gdlog();
				

				unsigned char* PicDataPtr = (unsigned char*) dr.mData;

				//GlobalDebugEnabled = true;
				//gdstr << "File:" << Filename << "  W=" << mWidth << " H=" << mHeight << " Bits=" << mBits;
				//gdlog();
				//GlobalDebug(mstr, DBG_TEMP2);

				int BytesPerLine = (dr.mWidth * dr.mBits) / 8;
				
				//cout << "BytesPerLine=" << BytesPerLine << endl;

				for (int i = 0; i < dr.mHeight; i++)
				{
					//cout << "i=" << i << endl;
					memcpy(PicDataPtr, pinfo.info_ptr->row_pointers[i], BytesPerLine);
					PicDataPtr += BytesPerLine;
				}
				//TestPalette(&pinfo);
				r = true;

				finish_read(&pinfo);
				

			}
			else
			{
			}
			fclose(fp);







		}

		/*
		if (mDebugLevel > 0)
		{
			ShowDebugInfo();
			GlobalLeave("CGraflibPng::Read START",  mDebugLevel);
		}
		*/


		return r;
	}
	
	
	//---------------------------------------------------------------------------
	//
	// Klasse:    
	// Methode:   png_write
	//
	//
	//---------------------------------------------------------------------------

	int png_write(const char* Filename, CDataRect& dr)
	{

	#if 1
	switch (dr.mBits)
	{

		case  8: write_pngGrayscale(Filename, dr.mWidth, dr.mHeight, dr.mData, 1);
							break;

		case 16: write_pngGrayscale(Filename, dr.mWidth, dr.mHeight, dr.mData, 2);
							break;

		case 24: write_png(Filename, dr.mWidth, dr.mHeight, 3, dr.mData);
							break;



		case 32: write_png(Filename, dr.mWidth, dr.mHeight, 4, dr.mData);
							break;
	}
	#endif
	return 0;
	}

	
	
};

