//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CFrameBuffer.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#ifndef CGL_FrameBuffer_H
#define CGL_FrameBuffer_H

#include <CPixel32.h>
#include <CDataRect.h>
#include <GLinclude.h>

class CGL_FrameBuffer
{
	
  public:
  
  
    CGL_FrameBuffer()
    {
      mWidth  = 0;
      mHeight = 0;
      mScale  = 1.0f;
      mOldFramebuffer = 0;
    
    }
    
    void CreateFramebufferObject(int Width, int Height);
		void DeleteFramebuffer();
    void DrawToFrameBuffer(bool Enable);    
		void DrawTexture(float xpos, float ypos) const;
		void DrawTexture(float xpos, float ypos, float w, float h) const;
		void DrawTextureRotated(float xpos, float ypos) const;
		void SetScale(float f) { mScale = f; }
    void SetViewport();
    void RestoreViewport() const;
    void ClearBuffer() const;
		float Width()  const { return mScale * mWidth; }
		float Height() const { return mScale * mHeight; }
		float UnscaledWidth()  const { return mWidth; }
		float UnscaledHeight()  const { return mHeight; }
		float Scale() const  { return mScale; }
		void ReadBuffer(CDataRect* dr);
		
  
  protected:
    
    GLuint mFBhandle;
    GLuint mColorRenderbuffer;
    GLuint mDepthRenderbuffer;
    GLuint mTexture;
    GLint  mOldFramebuffer;	
    GLint  mOldVp[4]; // Alter Viewport
    
    unsigned int mWidth;
    unsigned int mHeight;
    float        mScale;
    float        mAspect;
    
    
   
    
};


#endif
