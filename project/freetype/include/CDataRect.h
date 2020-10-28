//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CDataRect.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************

#ifndef CDATARECT_H
#define CDATARECT_H

#include <CPixel8.h>
#include <stdio.h>
#include <string.h>
#include <string>

class CDataRect
{
	public:

		CDataRect()
		{
			mData    = (CPixel*) NULL;
			mPalette = NULL;
			mWidth   = 0;
			mHeight  = 0;
			mBits    = 0;
			mTransp  = false;
			mStatus  = 0;
		}

		void operator=(CDataRect& dr)
		{
			mPalette   = dr.mPalette;
			mWidth     = dr.mWidth;
			mHeight    = dr.mHeight;
			mBits      = dr.mBits;
		}

		unsigned int DataSize() const
		{
			return mWidth * mHeight * (mBits / 8);
		}

		void SetData(int b)
		{
			memset(mData, b, DataSize());
		}
		void InitData(int b=0)
		
		{
			
		   mData = new CPixel8[DataSize()];
			
		   SetData(b);
		
	 }

		void CopyDataFrom(const CDataRect* dr)
		{
			mData      = dr->mData;
			mPalette   = dr->mPalette;
			mWidth     = dr->mWidth;
			mHeight    = dr->mHeight;
			mBits      = dr->mBits;

			//mData = new unsigned char[mWidth * mHeight * (mBits/8)];
		}

		bool InterleaveAlpha(const CDataRect* Alpha);
		bool InterleaveAlpha();
		bool AlphaTo32();
		bool AlphaTo32(unsigned char r, unsigned char g, unsigned char b);
		void Enhance(int twidth, int theight, bool CenterX=false, bool CenterY=false);
		void EnhanceBlock(int BlockW, int BlockH, int* ex=(int*)NULL, int* ey=(int*) NULL, bool CenterX=false, bool CenterY=false);
		void EnhanceCenter(int twidth, int theight, int* ex=(int*)NULL, int* ey=(int*)NULL);
		void Blur(int stages);
		int LineSize() const { return mWidth * (mBits / 8); }
		bool TestDataEqual() const;
		unsigned char* Convert8BitTo24() const;
		bool TestZero();
		void Show() const;
		std::string Print() const;
		void Convert32to24();
  	void Convert24to32();
		void Convert24to8();
		void Swap16Bits();
		void Scale2();
		

		int  Offset(int x, int y) const
		{
			return (y * mWidth) + x;
		}



		void SwapRedBlue();
		void Xflip();
		void Yflip();
		void ExtractAlpha();

		void DeleteData()
		{
			delete[] mData;
			mData = 0L;
		}

		CPixel* mData;
		void*        mPalette;
		int          mWidth;
		int          mHeight;
		int          mBits;
		int          mStatus;    // Status Bit
		bool         mTransp;    // 100% Transparent

	protected:

		
		int Offset(int x, int y, int c, int b);
		
		template <typename Tpixel>
		void Scale2T();

		
		template <typename Tpixel>
		void SwapPixelT(int x1, int y1, int x2, int y2);

		template <typename Tpixel>
		void PutPixelT(int x, int y, const Tpixel* Pixel);

		template <typename Tpixel>
		Tpixel GetPixelT(int x, int y) const;


};


//---------------------------------------------------------------------------
//
// Klasse:    CDataRect
// Methode:		GetPixelT
//
//---------------------------------------------------------------------------

template <typename Tpixel>
Tpixel CDataRect::GetPixelT(int x, int y) const
{
	Tpixel* ptr =  ((Tpixel*) mData) + (y * mWidth) + x;
	return *ptr;
}

//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		PutPixelT
//
//---------------------------------------------------------------------------

template <typename Tpixel>
void CDataRect::PutPixelT(int x, int y, const Tpixel* Pixel)
{
	Tpixel* ptr =  ((Tpixel*) mData) + (y * mWidth) + x;
	*ptr = *Pixel;
}


#endif

