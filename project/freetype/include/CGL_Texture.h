//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CGL_Texture.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#ifndef CGL_TEXTURE_H
#define CGL_TEXTURE_H

#include <CDataRect.h>

class CGL_Texture : public CDataRect
{
	public:

		CGL_Texture()
		{
			mTexHandle = 0;
		}

		bool LoadTextureTga(const char* TextureFilename);
		bool CreateTexture(const CDataRect* glib, bool CreateMipFap=false);
		void DeleteTexture();


		unsigned int mTexHandle;

};


#endif
