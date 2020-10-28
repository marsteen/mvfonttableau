//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CGL_Freetype.cpp
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#include <CGL_Freetype.h>


#include <CGL_Texture.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftbitmap.h>
#include <NGlobalLog.h>
#include <CGraflibPng.h>
//#include <NGLprimitives.h>
#include <CGL_Basic.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <NUtf.h>
#include <NGlobalLog.h>

static FT_Library StaticLibFreetype;

static int StaticWindowHeight;
static CFrameBuffer* StaticFrameBuffer = NULL;
static bool StaticFreetypeInit = false;


using namespace std;
using namespace NGlobalLog;


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   CGL_Freetype
//
//
//---------------------------------------------------------------------------

CGL_Freetype::CGL_Freetype()
{
    mFace = NULL;
    mFontSize = 0;
    mFontSizeScaled = 0.0f;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   Init
//
//
//---------------------------------------------------------------------------

void CGL_Freetype::Init(const char* Fontfile, int FontSize)
{
    gdstr << "CGL_Freetype::Init file:" << Fontfile << " FontSize=" << FontSize;
    gdlog();

    int error = 0;



    if (!StaticFreetypeInit)
    {
        error = FT_Init_FreeType(&StaticLibFreetype);
        StaticFreetypeInit = true;
    }


    if (error)
    {
        gdstr << " CGL_Freetype::Init FAILED file:" << Fontfile << " Size=" << FontSize;
        gderr();
    }
    else
    {
        mFontSize = FontSize;
        error = FT_New_Face(StaticLibFreetype, Fontfile, 0, &mFace);
        if (error == FT_Err_Unknown_File_Format)
        {
            gdstr << " FT_Err_Unknown_File_Format:" << Fontfile << " Size=" << FontSize;
            gderr();


            //... the font file could be opened and read, but it appears ... that its font format is unsupported
        }
        else
        if (error)
        {
            //... another error code means that the font file could not ... be opened or read, or simply that it is broken...
            gdstr << "CGL_Freetype::Init error:" << Fontfile << " Size=" << FontSize;
            gderr();
        }
        else
        {
            //cout << "ok" << endl;

            error = FT_Set_Char_Size(mFace, /* handle to face object */
                    0,                      /* char_width in 1/64th of points */
                    mFontSize * 64,         /* char_height in 1/64th of points */
                    40,                     /* horizontal device resolution */
                    40);                    /* vertical device resolution */
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   FontSize
//
//
//---------------------------------------------------------------------------


int CGL_Freetype::FontSize() const
{
    return mFontSize;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   GetStringWidth
//
//
//---------------------------------------------------------------------------

float CGL_Freetype::GetStringWidth(const CGL_Context* glcon, const unsigned char* text, int b)
{
#if 1
    //gdstr << "RenderString:" << text;
    //gdlog();

    float TotalWidth = 0;

    int LastIndex = 0;

    for (int i = 0; text[i] != 0;)
    {
        int ThisIndex;
        int cs;
        int Utf32 = NUtf::Utf8_to_Utf32(text + i, &cs);
        float w = GetGlyphWidth(glcon, Utf32, &LastIndex);
        i += cs;

        if (b <= 0)
        {
            TotalWidth += w - b;
        }
        else
        {
            TotalWidth += b;
        }
    }
#endif

    return TotalWidth;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   GetStringWidth
//
//
//---------------------------------------------------------------------------

float CGL_Freetype::GetStringWidth(const CGL_Context* glcon, const unsigned char* text, std::vector<float>& vw)
{
#if 1
    //gdstr << "RenderString:" << text;
    //gdlog();

    float TotalWidth = 0;

    int LastIndex = 0;

    for (int i = 0; text[i] != 0;)
    {
        int ThisIndex;
        int cs;
        int Utf32 = NUtf::Utf8_to_Utf32(text + i, &cs);
        float w = GetGlyphWidth(glcon, Utf32, &LastIndex);
        i += cs;

        TotalWidth += w;
        vw.push_back(TotalWidth);
    }
#endif

    return TotalWidth;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   GetStringLimit
//
//
//---------------------------------------------------------------------------

int CGL_Freetype::GetStringLimit(const CGL_Context* glcon, const unsigned char* text, float lim, float f)
{
#if 1
    //gdstr << "RenderString:" << text;
    //gdlog();

    float TotalWidth = 0;

    int LastIndex = 0;
    int r = -1;

    for (int i = 0; text[i] != 0;)
    {
        int ThisIndex;
        int cs;
        int Utf32 = NUtf::Utf8_to_Utf32(text + i, &cs);
        float w = GetGlyphWidth(glcon, Utf32, &LastIndex);
        i += cs;

        TotalWidth += w * f;
        if (TotalWidth > lim)
        {
            r = i;
            break;
        }
    }
#endif

    return r;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   RenderStringFrame
//
//
//---------------------------------------------------------------------------

int CGL_Freetype::RenderStringFrame(const CGL_Context* glcon, float x, float y, const float* FillColor, const float* FrameColor, const unsigned char* text, int b)
{
    CRectT<float> rc;

    rc.left = x;
    rc.right = x + GetStringWidth(glcon, text, b);
    rc.top = y;
    rc.bottom = y + (mFace->ascender - mFace->descender) / 64.0;

    rc.Expand(8, 8);
    rc.top -= 8;


    glEnable(GL_BLEND);
    glcon->SetUniformInt(UNILOC_SHADER_MODE, ESHADER_MODE_COLOR);

    glcon->SetUniformVec4(UNILOC_COLOR, FrameColor);
    CGL_Basic::DrawFrame(glcon, rc);

    glcon->SetUniformVec4(UNILOC_COLOR, FillColor);
    CGL_Basic::DrawQuad(glcon, rc);
    return 0;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   RenderString
//
//
//---------------------------------------------------------------------------

int CGL_Freetype::RenderString(const CGL_Context* glcon, float x, float y, const unsigned char* text, int b)
{
    //gdstr << "RenderString:" << text;
    //gdlog();

    int LastIndex = 0;

    for (int i = 0; text[i] != 0;)
    {
        int ThisIndex;
        int cs;
        int Utf32 = NUtf::Utf8_to_Utf32(text + i, &cs);
        float w = RenderGlyph(glcon, x, y, Utf32, &LastIndex);
        i += cs;

        if (b <= 0)
        {
            x += w - b;
        }
        else
        {
            x += b;
        }
    }
    return 0;
}


//


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   RenderStringScaled
//
//
//---------------------------------------------------------------------------

int CGL_Freetype::RenderStringScaled(const CGL_Context* glcon, float x, float y, const unsigned char* text, int b, float ScaleFaktor)
{
    //gdstr << "RenderString:" << text;
    //gdlog();

    int LastIndex = 0;

    for (int i = 0; text[i] != 0;)
    {
        int ThisIndex;
        int cs;
        int Utf32 = NUtf::Utf8_to_Utf32(text + i, &cs);
        float w = RenderGlyphScaled(glcon, x, y, Utf32, &LastIndex, ScaleFaktor);
        i += cs;

        if (b <= 0)
        {
            x += w - b;
        }
        else
        {
            x += b;
        }
    }
    return 0;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   GetGlyphTex
//
//
//---------------------------------------------------------------------------

const SGlyphTexture* CGL_Freetype::GetGlyphTex(const int c)
{
    SGlyphTexture* GlyphTex = NULL;

    if (mFace != NULL)
    {
        int error;
        int glyph_index = FT_Get_Char_Index(mFace, c);

        if (glyph_index != 0)
        {
            GlyphTex = new SGlyphTexture;
            GlyphTex->mGlyphIndex = glyph_index;
            TGlyphTexture GlyphPtr(GlyphTex);


            set<TGlyphTexture>::iterator it = mGlyphSet.find(GlyphPtr);

            if (it == mGlyphSet.end())
            {
                error = FT_Load_Glyph(mFace,                    /* handle to face object */
                        glyph_index,                            /* glyph index */
                        FT_LOAD_DEFAULT);                       /* load flags, see below */

                error = FT_Render_Glyph(mFace->glyph,           /* glyph slot */
                        FT_RENDER_MODE_NORMAL);                 /* render mode */

                CDataRect dr;
                CGL_Texture tex;

                dr.mWidth = mFace->glyph->bitmap.pitch;
                dr.mHeight = mFace->glyph->bitmap.rows;
                dr.mBits = 8;
                dr.mData = (CPixel*)mFace->glyph->bitmap.buffer;

                dr.AlphaTo32(255, 255, 255);

                int OrigWidth = dr.mWidth;
                int OrigHeight = dr.mHeight;
                dr.EnhanceBlock(4, 4, &GlyphTex->mEx, &GlyphTex->mEy);



                tex.CreateTexture(&dr);

                GlyphTex->mWidth = mFace->glyph->bitmap.width;
                GlyphTex->mHeight = dr.mHeight;
                GlyphTex->mBitmapLeft = mFace->glyph->bitmap_left;
                GlyphTex->mBitmapTop = mFace->glyph->bitmap_top;

                GlyphTex->mTexHandle = tex.mTexHandle;
                GlyphTex->mTexRect.left = 0;
                GlyphTex->mTexRect.right = float(OrigWidth) / dr.mWidth;
                GlyphTex->mTexRect.top = 1.0f;
                GlyphTex->mTexRect.bottom = 0.0f;
                GlyphTex->mAdvanceX = float(mFace->glyph->advance.x) / 64.0f;
                GlyphTex->mAdvanceY = OrigHeight;
                GlyphTex->mDesc = float(mFace->glyph->metrics.height - mFace->glyph->metrics.horiBearingY) / 64.0f;

                dr.DeleteData();

                mGlyphSet.insert(GlyphPtr);
            }
            else
            {
                delete GlyphTex;
                GlyphTex = it->mPtr;
            }
        }
    }
    return GlyphTex;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   RenderGlyphScaled
//
//
//---------------------------------------------------------------------------

float CGL_Freetype::RenderGlyphScaled(const CGL_Context* glcon, float x, float y, const int c, int* LastIndex, float ScaleFaktor)
{
    float Kerning = 0;
    int error;
    float Advance;
    //int glyph_index = FT_Get_Char_Index(mFace, c);


    const SGlyphTexture* GlyphTex = GetGlyphTex(c);

    if (GlyphTex != NULL)
    {
        {
            CRectT<float> vr;

            vr.left = x + GlyphTex->mBitmapLeft;
            vr.top = y - GlyphTex->mDesc - GlyphTex->mEy;
            vr.right = vr.left + GlyphTex->mWidth * ScaleFaktor;
            vr.bottom = vr.top + GlyphTex->mHeight * ScaleFaktor;

            if (*LastIndex > 0)
            {
                if (*LastIndex != GlyphTex->mGlyphIndex)
                {
                    FT_Vector delta;
                    FT_Get_Kerning(mFace, *LastIndex, GlyphTex->mGlyphIndex, FT_KERNING_DEFAULT, &delta);
                    Kerning = float(delta.x) / 64.0f * ScaleFaktor;
                    vr.left += Kerning;
                    vr.right += Kerning;

                    //gdstr << "L=" << *LastIndex << " G=" << GlyphTex->mGlyphIndex;
                    //gdlog();
                }
            }
            *LastIndex = GlyphTex->mGlyphIndex;

            //vr.Set(x - GlyphTex->mEx, y - GlyphTex->mEy - GlyphTex->mDesc, x + GlyphTex->mWidth, y + GlyphTex->mHeight);  //* glcon->mAspect);
            //vr.Set(x, y, x + GlyphTex->mWidth, y + GlyphTex->mHeight);
            glBindTexture(GL_TEXTURE_2D, GlyphTex->mTexHandle);
            CGL_Basic::DrawTextureQuad(glcon, vr, GlyphTex->mTexRect);
        }
        Advance = ScaleFaktor * GlyphTex->mAdvanceX + Kerning;
    }
    else
    {
        Advance = float(mFontSize) / 64 * ScaleFaktor;
    }
    return Advance;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   RenderGlyph
//
//
//---------------------------------------------------------------------------

float CGL_Freetype::RenderGlyph(const CGL_Context* glcon, float x, float y, const int c, int* LastIndex)
{
    float Kerning = 0;
    int error;
    int Advance;
    //int glyph_index = FT_Get_Char_Index(mFace, c);


    const SGlyphTexture* GlyphTex = GetGlyphTex(c);

    if (GlyphTex != NULL)
    {
        {
            CRectT<float> vr;

            vr.left = x + GlyphTex->mBitmapLeft;
            vr.top = y - GlyphTex->mDesc - GlyphTex->mEy;
            vr.right = vr.left + GlyphTex->mWidth;
            vr.bottom = vr.top + GlyphTex->mHeight;

            if (*LastIndex > 0)
            {
                if (*LastIndex != GlyphTex->mGlyphIndex)
                {
                    FT_Vector delta;
                    FT_Get_Kerning(mFace, *LastIndex, GlyphTex->mGlyphIndex, FT_KERNING_DEFAULT, &delta);
                    Kerning = float(delta.x) / 64;
                    vr.left += Kerning;
                    vr.right += Kerning;

                    //gdstr << "L=" << *LastIndex << " G=" << GlyphTex->mGlyphIndex;
                    //gdlog();
                }
            }
            *LastIndex = GlyphTex->mGlyphIndex;

            //vr.Set(x - GlyphTex->mEx, y - GlyphTex->mEy - GlyphTex->mDesc, x + GlyphTex->mWidth, y + GlyphTex->mHeight);  //* glcon->mAspect);
            //vr.Set(x, y, x + GlyphTex->mWidth, y + GlyphTex->mHeight);
            glBindTexture(GL_TEXTURE_2D, GlyphTex->mTexHandle);
            CGL_Basic::DrawTextureQuad(glcon, vr, GlyphTex->mTexRect);
        }
        Advance = GlyphTex->mAdvanceX + Kerning;
    }
    else
    {
        Advance = float(mFontSize) / 64;
    }
    return Advance;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   GetGlyphWidth
//
//
//---------------------------------------------------------------------------


int CGL_Freetype::GetGlyphWidth(const CGL_Context* glcon, const int c, int* LastIndex)
{
    int Advance = 0;
    const SGlyphTexture* GlyphTex = GetGlyphTex(c);


    if (GlyphTex != NULL)
    {
        float Kerning = 0;
        if (*LastIndex > 0)
        {
            if (*LastIndex != GlyphTex->mGlyphIndex)
            {
                FT_Vector delta;
                FT_Get_Kerning(mFace, *LastIndex, GlyphTex->mGlyphIndex, FT_KERNING_DEFAULT, &delta);
                Kerning = float(delta.x) / 64;
            }
        }
        *LastIndex = GlyphTex->mGlyphIndex;
        Advance = GlyphTex->mAdvanceX + Kerning;
    }
    else
    {
        Advance = float(mFontSize) / 64;
    }
    return Advance;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   ResetGlyphs
//
//
//---------------------------------------------------------------------------

void CGL_Freetype::ResetGlyphs()
{
    StaticFreetypeInit = false;
    set<TGlyphTexture>::iterator it;

    for (it = mGlyphSet.begin(); it != mGlyphSet.end(); ++it)
    {
        glDeleteTextures(1, &(it->mPtr->mTexHandle));
    }
    mGlyphSet.clear();
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   ResetFrameBuffer
//
//
//---------------------------------------------------------------------------

void CGL_Freetype::ResetFrameBuffer()
{
    if (StaticFrameBuffer != NULL)
    {
        StaticFrameBuffer->DeleteFramebuffer();
    }
    delete StaticFrameBuffer;
    StaticFrameBuffer = NULL;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   RenderToFrameBuffer
//
//
//---------------------------------------------------------------------------


void CGL_Freetype::RenderToFrameBuffer(const CGL_Context* glcon, float Scale, const unsigned char* txt)
{
    //gdstr << "CGL_Freetype::RenderToFrameBuffer START:" << txt;
    //gdlog();


    if (StaticFrameBuffer == NULL)
    {
        //float fbScale = 1.0 / Scale;

        StaticFrameBuffer = new CFrameBuffer;
        //StaticFrameBuffer->CreateFramebufferObject(512 * Scale, 128 * Scale);
        //StaticFrameBuffer->CreateFramebufferObject(512 * Scale, 128 * Scale);
        StaticFrameBuffer->CreateFramebufferObject(1024, 128);
        //StaticFrameBuffer->SetScale(fbScale);
        StaticFrameBuffer->SetScale(Scale);
    }


    RenderToFrameBuffer(glcon, StaticFrameBuffer, txt);
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   RenderToFrameBuffer
//
//
//---------------------------------------------------------------------------

void CGL_Freetype::RenderToFrameBuffer(const CGL_Context* glcon, CFrameBuffer* fb, const unsigned char* txt)
{
    //
    // Der String wird rechts unten in die Ecke des Framebuffer gerendert
    //


    fb->SetViewport();

    glcon->ViewportUniformSet(fb->UnscaledWidth(), fb->UnscaledHeight());

    //float h = GetGlyphHeight(glcon);

    fb->DrawToFrameBuffer(true);
    fb->ClearBuffer();
    RenderString(glcon, 0, 0, txt, 0);
    fb->DrawToFrameBuffer(false);
    fb->RestoreViewport();

    glcon->ViewportUniformRestore();
}


#if 0
//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   RenderToFrameBuffer
//
//
//---------------------------------------------------------------------------

void CGL_Freetype::RenderToFrameBuffer(const CGL_Context* glcon, CFrameBuffer* fb, const unsigned char* txt)
{
    fb->SetViewport();

    glcon->ViewportUniformSet(fb->UnscaledWidth(), fb->UnscaledHeight());

    //int width = GetStringWidth(glcon, txt, 0);
    //float xpos = ((right + left) / 2) - width / 2;

    static int n;
    //int width  = GetStringWidth(glcon, txt, 0);
    //int height = GetStringHeight();

    float xpos = 0; // (StaticFrameBuffer->UnscaledWidth() / 2) - (width / 2);

    //float xpos = 0; // - width / 2 - n++;

    fb->DrawToFrameBuffer(true);
    fb->ClearBuffer();
    RenderString(glcon, xpos, fb->UnscaledHeight() / 2, txt, 0);
    fb->DrawToFrameBuffer(false);
    fb->RestoreViewport();

    glcon->ViewportUniformRestore();
}


#endif



//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   DrawFrameBuffer
//
//
//---------------------------------------------------------------------------


void CGL_Freetype::DrawFrameBuffer(const CGL_Context* glcon, float xpos, float ypos) const
{
    StaticFrameBuffer->DrawTexture(glcon, xpos, ypos);
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   DrawFrameBufferRotated
//
//
//---------------------------------------------------------------------------


void CGL_Freetype::DrawFrameBufferRotated(const CGL_Context* glcon, float xpos, float ypos) const
{
    StaticFrameBuffer->DrawTextureRotated(glcon, xpos, ypos);
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   GetGlyphHeight
//
//
//---------------------------------------------------------------------------

float CGL_Freetype::GetGlyphHeight(const CGL_Context* glcon)
{
    float f = 0.0f;
    const SGlyphTexture* GlyphTex = GetGlyphTex('M');

    if (GlyphTex != NULL)
    {
        f = GlyphTex->mAdvanceY;
    }
    return f;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   DrawFrameBufferRotatedCenter
//
//
//---------------------------------------------------------------------------

void CGL_Freetype::DrawFrameBufferRotatedCenter(const CGL_Context* glcon, const char* text, float xcenter, float ycenter)
{
    float w = GetStringWidth(glcon, (const unsigned char*)text, 0) * StaticFrameBuffer->Scale();
    float h = GetGlyphHeight(glcon) * StaticFrameBuffer->Scale();

    StaticFrameBuffer->DrawTextureRotated(glcon, xcenter + (h / 2) - StaticFrameBuffer->Height(), ycenter - (w  / 2));
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   GetStringWidthFb
//
//
//---------------------------------------------------------------------------

float CGL_Freetype::GetStringWidthFb(const CGL_Context* glcon, const char* text, std::vector<float>& vw)
{
    float f = 1.0f;

    if (StaticFrameBuffer != NULL)
    {
        f = GetStringWidth(glcon, (const unsigned char*)text, vw) * StaticFrameBuffer->Scale();
    }
    return f;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   GetStringWidthFb
//
//
//---------------------------------------------------------------------------

int CGL_Freetype::GetStringLimitFb(const CGL_Context* glcon, const char* text, float lim)
{
    int r = -1;

    if (StaticFrameBuffer != NULL)
    {
        r = GetStringLimit(glcon, (const unsigned char*)text, lim, StaticFrameBuffer->Scale());
        //r = GetStringLimit(glcon, (const unsigned char*) text, lim, 1.0f);
    }
    return r;
}


#if 0

if (glyph_index != 0)
{
    SGlyphTexture* GlyphTex = new SGlyphTexture;
    GlyphTex->mGlyphIndex = glyph_index;
    TGlyphTexture GlyphPtr(GlyphTex);


    set<TGlyphTexture>::iterator it = mGlyphSet.find(GlyphPtr);

    if (it == mGlyphSet.end())
    {
        error = FT_Load_Glyph(mFace,                    /* handle to face object */
                glyph_index,                            /* glyph index */
                FT_LOAD_DEFAULT);                       /* load flags, see below */

        error = FT_Render_Glyph(mFace->glyph,           /* glyph slot */
                FT_RENDER_MODE_NORMAL);                 /* render mode */

        CDataRect dr;
        CGL_Texture tex;

        dr.mWidth = mFace->glyph->bitmap.pitch;
        dr.mHeight = mFace->glyph->bitmap.rows;
        dr.mBits = 8;
        dr.mData = (CPixel*)mFace->glyph->bitmap.buffer;

        dr.AlphaTo32(255, 255, 255);

        //gdstr << "C=" << (char) c << " w=" << dr.mWidth << " h=" << dr.mHeight;
        //gdlog();

        /*
         *      {
         *          stringstream fname;
         *          CGraflibPng png;
         *
         *          fname << "char_" << (char) c << ".png";
         *          png.CopyDataFrom(&dr);
         *          png.Write(fname.str().c_str(), false, NULL);
         *      }
         */
        //int OrigHeight = dr.mHeight;

        int OrigWidth = dr.mWidth;
        int OrigHeight = dr.mHeight;
        dr.EnhanceBlock(4, 4, &GlyphTex->mEx, &GlyphTex->mEy);



        tex.CreateTexture(&dr);

        /*
         * dr.DeleteData();
         *
         * GlyphTex->mWidth          = OrigWidth;
         * GlyphTex->mHeight         = OrigHeight;
         *
         * GlyphTex->mTexHandle      = tex.mTexHandle;
         * GlyphTex->mTexRect.left   = 0;
         * GlyphTex->mTexRect.right  = float(OrigWidth)  / float(mFace->glyph->bitmap.pitch);
         * GlyphTex->mTexRect.top    = float(OrigWidth)  / float(dr.mHeight);
         * GlyphTex->mTexRect.bottom = 0.0f;
         * GlyphTex->mAdvanceX       = mFace->glyph->advance.x >> 6;
         * GlyphTex->mDesc           = (mFace->glyph->metrics.height - mFace->glyph->metrics.horiBearingY) / 64;
         */

        GlyphTex->mWidth = mFace->glyph->bitmap.width;
        GlyphTex->mHeight = dr.mHeight;
        GlyphTex->mBitmapLeft = mFace->glyph->bitmap_left;
        GlyphTex->mBitmapTop = mFace->glyph->bitmap_top;

        GlyphTex->mTexHandle = tex.mTexHandle;
        GlyphTex->mTexRect.left = 0;
        GlyphTex->mTexRect.right = float(mFace->glyph->bitmap.width) / dr.mWidth;   //mFace->glyph->bitmap.pitch;
        GlyphTex->mTexRect.top = 1.0;                                               // 1.0;
        GlyphTex->mTexRect.bottom = 0.0;                                            // 0.0f;
        GlyphTex->mAdvanceX = float(mFace->glyph->advance.x) / 64.0;
        GlyphTex->mDesc = float(mFace->glyph->metrics.height - mFace->glyph->metrics.horiBearingY) / 64.0;


        //cout << "c=" << (char) c << " d=" << GlyphTex->mDesc << endl;



        mGlyphSet.insert(GlyphPtr);
    }
    else
    {
        delete GlyphTex;
        GlyphTex = it->mPtr;
    }


//		NGLprimitives::DrawTexture(GlyphTex->mTexHandle, x - GlyphTex->mEx, y - GlyphTex->mEy - GlyphTex->mDesc, GlyphTex->mWidth, GlyphTex->mHeight, 1);

    {
        CRectT<float> vr;

        vr.left = x + GlyphTex->mBitmapLeft;
        vr.top = y - GlyphTex->mDesc - GlyphTex->mEy;
        vr.right = vr.left + GlyphTex->mWidth;
        vr.bottom = vr.top + GlyphTex->mHeight;

        if (*LastIndex > 0)
        {
            if (*LastIndex != GlyphTex->mGlyphIndex)
            {
                FT_Vector delta;
                FT_Get_Kerning(mFace, *LastIndex, GlyphTex->mGlyphIndex, FT_KERNING_DEFAULT, &delta);
                Kerning = delta.x >> 6;
                vr.left += Kerning;
                vr.right += Kerning;

                //gdstr << "L=" << *LastIndex << " G=" << GlyphTex->mGlyphIndex;
                //gdlog();
            }
        }
        *LastIndex = GlyphTex->mGlyphIndex;

        //vr.Set(x - GlyphTex->mEx, y - GlyphTex->mEy - GlyphTex->mDesc, x + GlyphTex->mWidth, y + GlyphTex->mHeight);  //* glcon->mAspect);
        //vr.Set(x, y, x + GlyphTex->mWidth, y + GlyphTex->mHeight);
        glBindTexture(GL_TEXTURE_2D, GlyphTex->mTexHandle);
        CGL_Basic::DrawTextureQuad(glcon, vr, GlyphTex->mTexRect);
    }


    Advance = GlyphTex->mAdvanceX + Kerning;
}
else
{
    Advance = mFontSize;
}

return Advance;
}

#endif

//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   Umbruch
//
// Return: true  = text wurde umgebrochen, Ergebnis steht in utext
//
//
//---------------------------------------------------------------------------

bool CGL_Freetype::Umbruch(const CGL_Context* glcon, const std::string& text, int wLimit, stringvector* utext)
{
    bool r = false;
    int w = GetStringWidth(glcon, (unsigned char*)text.c_str(), 0);

    if (w < wLimit)
    {
        utext->push_back(text);
        r = true;
    }
    else
    {
        stringvector sv;

        NStringTool::Split(text, &sv, " /-");
        r = true;

        if (sv.size() > 0)
        {
            string atext = sv[0];
            string btext = atext;

            for (int i = 1; i < sv.size(); i++)
            {
                btext = btext + sv[i];


                int wp = GetStringWidth(glcon, (unsigned char*)btext.c_str(), 0);
                if (wp > wLimit)
                {
                    NStringTool::RemoveLeading(atext, ' ');
                    utext->push_back(atext);
                    atext = sv[i];
                    btext = atext;
                }
                else
                {
                    atext = btext;
                }
            }
            //utext->push_back(sv[sv.size()-1]); // Letztes Element speichern
            NStringTool::RemoveLeading(btext, ' ');
            utext->push_back(btext); // Letztes Element speichern
        }
    }
    return r;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   RenderUmbruch
//
// wLimit:      Maximale Breite einer Zeile in Pixel
// LineHeight:  Abstand zur naechsten Linie
// MaxLines:    Maximale Anzahl der Linien
//
//---------------------------------------------------------------------------

void CGL_Freetype::RenderUmbruch(const CGL_Context* glcon, int xpos, int ypos, const std::string& text, int wLimit, int LineHeight, int MaxLines)
{
    stringvector utext;

    if (Umbruch(glcon, text, wLimit, &utext))
    {
        for (int i = 0; i < utext.size() && i < MaxLines; i++)
        {
            std::string stext;
            if ((utext.size() > MaxLines) && (i == MaxLines - 1))
            {
                stext = utext[i] + " ...";
            }
            else
            {
                stext = utext[i];
            }


            RenderString(glcon, xpos, ypos, (unsigned char*)stext.c_str(), 0);
            ypos -= LineHeight;
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGL_Freetype
// Methode:   RenderUmbruch2
//
// wLimit:      Maximale Breite einer Zeile in Pixel
// LineHeight:  Abstand zur naechsten Linie
// MaxLines:    Maximale Anzahl der Linien
//
// Die einzelnen Zeilen werden linksbuendig ausgerichtet, aber die Position des linken
// Randes richtet sich nach der laengsten Zeile. Diese wird zentriert (Zentrum = center_xpos)
//
//---------------------------------------------------------------------------

void CGL_Freetype::RenderUmbruch2(const CGL_Context* glcon, int center_xpos, int ypos, const std::string& text, int wLimit, int LineHeight, int MaxLines)
{
    stringvector utext;

    if (Umbruch(glcon, text, wLimit, &utext))
    {
        vector<string> vtext;
        float MaxWidth = -1; // Maximale Breite eines Strings

        for (int i = 0; i < utext.size() && i < MaxLines; i++)
        {
            std::string stext;
            if ((utext.size() > MaxLines) && (i == MaxLines - 1))
            {
                stext = utext[i] + " ...";
            }
            else
            {
                stext = utext[i];
            }
            vtext.push_back(stext);


            float w = GetStringWidth(glcon, (unsigned char*)stext.c_str(), 0);
            if (w > MaxWidth)
            {
                MaxWidth = w;
            }
        }

        float tposx = float(center_xpos) - MaxWidth / 2;



        for (int i = 0; i < vtext.size(); i++)
        {
            RenderString(glcon, tposx, ypos, (unsigned char*)vtext[i].c_str(), 0);
            ypos -= LineHeight;
        }
    }
}
