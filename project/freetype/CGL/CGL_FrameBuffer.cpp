//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CGL_FrameBuffer.cpp
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************

#include <iostream>
#include <CGL_FrameBuffer.h>
#include <GLinclude.h>
#include <NGLprimitives.h>


using namespace std;



static void checkGlError(const char* func)
{
}


static void checkGlErrorCritical(const char* func)
{
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_FrameBuffer
// Methode: CreateFramebufferObject
//
//
//---------------------------------------------------------------------------

void CGL_FrameBuffer::CreateFramebufferObject(int Width, int Height)
{
    cout << "CGL_FrameBuffer::CreateFramebufferObject START" << endl;
    cout << "w=" << Width << " h=" << Height << endl;

    mWidth = Width;
    mHeight = Height;

    mFBhandle = 0;

    glGenFramebuffers(1, &mFBhandle);
    checkGlErrorCritical("glGenFramebuffers");
    glBindFramebuffer(GL_FRAMEBUFFER, mFBhandle);
    checkGlErrorCritical("glBindFramebuffer");

    //Create a color renderbuffer, allocate storage for it, and attach it to the framebuffer’s color attachment point.
    //GLuint colorRenderbuffer;

    glGenRenderbuffers(1, &mColorRenderbuffer);
    checkGlErrorCritical("glGenRenderbuffers");
    glBindRenderbuffer(GL_RENDERBUFFER, mColorRenderbuffer);
    checkGlErrorCritical("glBindRenderbuffer");
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, Width, Height);
    checkGlErrorCritical("glRenderbufferStorage");
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, mColorRenderbuffer);
    checkGlErrorCritical("glFramebufferRenderbuffer");

#if 0
    //Create a depth or depth/stencil renderbuffer, allocate storage for it, and attach it to the framebuffers depth attachment point.

    glGenRenderbuffers(1, &mDepthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, Width, Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthRenderbuffer);
#endif
    // Textur erzeugen

    glGenTextures(1, &mTexture);
    checkGlErrorCritical("glGenTextures");
    glBindTexture(GL_TEXTURE_2D, mTexture);
    checkGlErrorCritical("glBindTexture");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // GL_NEAREST
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    checkGlErrorCritical("glTexImage2D");
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);
    checkGlErrorCritical("glFramebufferTexture2D");

    //Test the framebuffer for completeness. This test only needs to be performed when the framebuffers configuration changes.

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "GL_FRAMEBUFFER_COMPLETE failed: Status=" <<  status << endl;
    }
    else
    {
        cout << "GL_FRAMEBUFFER_COMPLETE ok" << endl;
    }


    // Textur und Framebuffer deaktivieren:

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    cout << "CGL_FrameBuffer::CreateFramebufferObject OK" << endl;



    // Debug("CreateFramebufferObject OK");
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_FrameBuffer
// Methode: DeleteFramebuffer
//
//
//---------------------------------------------------------------------------

void CGL_FrameBuffer::DeleteFramebuffer()
{
    glDeleteFramebuffers(1, &mFBhandle);
    mFBhandle = 0;
    glDeleteTextures(1, &mTexture);
    mTexture = 0;
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_FrameBuffer
// Methode: DrawToFrameBuffer
//
//
//---------------------------------------------------------------------------

// Zeichnen in den Framebuffer aktivieren:

void CGL_FrameBuffer::DrawToFrameBuffer(bool Enable)
{
    if (Enable)
    {
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mOldFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, mFBhandle);
    }
    else
    {
        //glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, mOldFramebuffer);
    }
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_FrameBuffer
// Methode: ClearBuffer
//
//
//---------------------------------------------------------------------------

void CGL_FrameBuffer::ClearBuffer() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_FrameBuffer
// Methode: ReadBuffer
//
//
//---------------------------------------------------------------------------


void CGL_FrameBuffer::ReadBuffer(CDataRect* dr)
{
    dr->mBits = 32;
    dr->mWidth = mWidth;
    dr->mHeight = mHeight;
    dr->mData = new CPixel32[mWidth * mHeight];

    DrawToFrameBuffer(true);
    glReadPixels(0, 0, mWidth, mHeight, GL_RGBA, GL_UNSIGNED_BYTE, dr->mData);
    DrawToFrameBuffer(false);
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_FrameBuffer
// Methode: DrawTexture
//
//
//---------------------------------------------------------------------------


// Textur zeichnen

void CGL_FrameBuffer::DrawTexture(float xpos, float ypos) const
{
    glBindTexture(GL_TEXTURE_2D, mTexture);
    //CGL_Basic::DrawTextureQuad(glcon, xpos, ypos, xpos + mWidth, ypos + mHeight);

    CRectT<float> vrc;

    vrc.Set(xpos, ypos, xpos + mWidth, ypos + mHeight);
    NGLprimitives::DrawTexture(&vrc);
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_FrameBuffer
// Methode: DrawTexture
//
//
//---------------------------------------------------------------------------

// Textur zeichnen

void CGL_FrameBuffer::DrawTexture(float xpos, float ypos, float w, float h) const
{
    glBindTexture(GL_TEXTURE_2D, mTexture);
    //CGL_Basic::DrawTextureQuad(glcon, xpos, ypos, xpos + w, ypos + h);

    CRectT<float> vrc;

    vrc.Set(xpos, ypos, xpos + w, ypos + h);
    NGLprimitives::DrawTexture(&vrc);
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_FrameBuffer
// Methode: SetViewport
//
//
//---------------------------------------------------------------------------

void CGL_FrameBuffer::SetViewport()
{
    glGetIntegerv(GL_VIEWPORT, mOldVp);
    glViewport(0, 0, mWidth, mHeight);
}


//---------------------------------------------------------------------------
//
// Klasse:  CGL_FrameBuffer
// Methode: RestoreViewport
//
//
//---------------------------------------------------------------------------

void CGL_FrameBuffer::RestoreViewport() const
{
    glViewport(mOldVp[0], mOldVp[1], mOldVp[2], mOldVp[3]);
}
