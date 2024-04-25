//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CGL_Freetype.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#ifndef CGL_FREETYPE_H
#define CGL_FREETYPE_H

#include <set>
#include <vector>
#include <CRectT.h>
#include <CSetPointer.h>
#include <CGL_Context.h>
#include <NStringTool.h>
#include <CGL_FrameBuffer.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H


struct SGlyphTexture
{
    bool operator<(const SGlyphTexture& k) const
    {
        return mGlyphIndex < k.mGlyphIndex;
    }


    int				mGlyphIndex;
    unsigned int	mTexHandle;
    CRectT<float>	mTexRect;
    int				mWidth;
    int				mHeight;
    float			mAdvanceX;
    float			mAdvanceY;
    int				mEx;
    int				mEy;
    int				mDesc;
    float			mBitmapLeft;
    float			mBitmapTop;
    float			mBitmapBottom;
};

typedef CSetPointer<SGlyphTexture> TGlyphTexture;

class CGL_Freetype
{
    public:

        CGL_Freetype();

        void Init(const char* Fontfile, int FontSize);
        int RenderString(const CGL_Context* glcon, float x, float y, const unsigned char* text, int w);
        int RenderStringScaled(const CGL_Context* glcon, float x, float y, const unsigned char* text, int b, float ScaleFaktor);
        int RenderStringRects(int x, int y, const int WindowWidth, const int WindowHeight, const unsigned char* text, CRectT<float>* rc);


        float GetStringWidth(const CGL_Context* glcon, const unsigned char* text, int b);
        float GetStringWidth(const CGL_Context* glcon, const unsigned char* text, std::vector<float>& vw);
        float GetStringWidthFb(const CGL_Context* glcon, const char* text, std::vector<float>& vw);
        float GetStringHeight() const;
        int FontSize() const;


        int GetStringLimit(const CGL_Context* glcon, const unsigned char* text, float lim, float f);



        int RenderStringFrame(const CGL_Context* glcon, float x, float y, const float* FillColor, const float* FrameColor, const unsigned char* text, int b);
        bool Umbruch(const CGL_Context* glcon, const std::string& text, int Limit, stringvector* utext);
        void RenderUmbruch(const CGL_Context* glcon, int xpos, int ypos, const std::string& text, int wLimit, int LineHeight, int MaxLines);
        void RenderUmbruch2(const CGL_Context* glcon, int center_xpos, int ypos, const std::string& text, int wLimit, int LineHeight, int MaxLines);

        /*
         * int GetStringLimitFb(const CGL_Context* glcon, const char* text, float lim);
         * void RenderToFrameBuffer(const CGL_Context* glcon, float Scale, const unsigned char* txt);
         * void DrawFrameBuffer(const CGL_Context* glcon, float xpos, float ypos) const;
         * void DrawFrameBufferRotated(const CGL_Context* glcon, float xpos, float ypos) const;
         * void DrawFrameBufferRotatedCenter(const CGL_Context* glcon, const char* txt, float xpos, float ycenter);
         *
         * static void ResetFrameBuffer();
         */



        void ResetGlyphs();

        float LineOffset() const { return mFontSize * 0.6; }
        void SetRenderModeMonochrome(bool b)        { mRenderModeMonochrome = b; }



    protected:

        const SGlyphTexture* GetGlyphTex(const int c);
        float RenderGlyph(const CGL_Context* glcon, float x, float y, const int c, int* LastIndex, CRectT<float>* vrOut = NULL);
        float RenderGlyphScaled(const CGL_Context* glcon, float x, float y, const int c, int* LastIndex, float ScaleFaktor);
        int GetGlyphWidth(const CGL_Context* glcon, const int c, int* LastIndex);
        float GetGlyphHeight(const CGL_Context* glcon);

        //void RenderToFrameBuffer(const CGL_Context* glcon, CFrameBuffer* fb, const unsigned char* txt);

        std::set<TGlyphTexture> mGlyphSet;
        FT_Face mFace;
        int mFontSize;
        float mFontSizeScaled;
        bool mRenderModeMonochrome;
};


#endif
