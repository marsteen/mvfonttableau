//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CGL_Texture.cpp
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************
//
// Inhalt: Implementation von CGL_Texture
//
//   Klasse zur Verwaltung von OpenGL-Texturen
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 01.06.2013 erstellt- Martin Steen
//
//------------------------------------------------------------------------------


#include <iostream>
#include <fstream>

#include <GLinclude.h>
#include <CGL_Texture.h>

using namespace std;


static void checkGlError(const char* func)
{
	
}



//---------------------------------------------------------------------------
//
// Klasse:    CGL_Texture
// Methode:		DeleteTexture
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CGL_Texture::DeleteTexture()
{
	if (mTexHandle != 0)
	{
		glDeleteTextures(1, &mTexHandle);
		mTexHandle = 0;
	}
}

//---------------------------------------------------------------------------
//
// Klasse:    CGL_Texture
// Methode:		CreateTexture
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CGL_Texture::CreateTexture(const CDataRect* glib, bool CreateMipMap)
{
	//glEnable(GL_TEXTURE_2D); // Veraltet
  //checkGlError("glEnable(GL_TEXTURE_2D)");
  
  int format;
  bool r = true;

	glGenTextures(1, &mTexHandle);				// Create One Texture
  checkGlError("glGenTextures");


	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, (int) mTexHandle);
  checkGlError("glBindTexture");
	
	switch (glib->mBits)
	{
		case 24: format = GL_RGB;  break;
		case 32: format = GL_RGBA; break;
		
		default:
			
			cout << "CGL_Texture::CreateTexture Bit format not supported:" << glib->mBits << endl;
			r = false;
			break;
	}

	if (r)
	{
		//       GL_LINEAR
		// oder  GL_NEAREST
		
		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //
		
		if (CreateMipMap)
		{		
#if 0
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, format, glib->mWidth, glib->mHeight, 0, format, GL_UNSIGNED_BYTE, glib->mData);
			glGenerateMipmap(GL_TEXTURE_2D);			
#endif			
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, format, glib->mWidth, glib->mHeight, 0, format, GL_UNSIGNED_BYTE, glib->mData);
		}
		

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		checkGlError("glTexParameteri (GL_TEXTURE_MIN_FILTER)");


		checkGlError("glTexParameteri (GL_TEXTURE_WRAP_T)");


		checkGlError("glTexImage2D");
		CopyDataFrom(glib);
		
	}
	
	
	return r;

}


#if 0

//---------------------------------------------------------------------------
//
// Klasse:    CGL_Texture
// Methode:		LoadTexture
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CGL_Texture::LoadTextureTga(const char* TextureFilename)
{
	bool r = false;
	CGraflibTga tga;

	//cout << "CGL_Texture::LoadTextureTga START: " << TextureFilename << endl;

	tga.Read(TextureFilename);
	tga.SwapRedBlue();
	tga.Yflip();
	//tga.Write("test.tga", false);

	//cout << "CGL_Texture Width=" << tga.mWidth << " Height=" << tga.mHeight << endl;

	if (tga.mData != NULL)	//
	{
		//glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &mTexHandle);				// Create One Texture
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, (int) mTexHandle);

			//       GL_LINEAR
			// oder  GL_NEAREST

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tga.mWidth, tga.mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tga.mData);

		mWidth    = tga.mWidth;
		mHeight   = tga.mHeight;
		mBits     = tga.mBits;

		delete[] tga.mData;

		//glDisable(GL_TEXTURE_2D);

		r = true;
	}
	return r;									// Return The Status
}

#endif
