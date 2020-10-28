//***************************************************************************
//
//
// @PROJECT  :	Diercke Globus Online
// @VERSION  :	3.0
// @FILENAME :	NGLprimitives.cpp
// @DATE     :	18.8.2014
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#include <GLinclude.h>
#include <CRectT.h>
#include <CVector3T.h>


//---------------------------------------------------------------------------
//
// Namespace: NGLprimitives
// Methode:   DrawRect2D
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------


namespace NGLprimitives
{
    void DrawRect(const CRectT<float>* vrc)
    {
        glBegin(GL_TRIANGLE_STRIP);

        glVertex2f(vrc->left, vrc->top);
        glVertex2f(vrc->right, vrc->top);
        glVertex2f(vrc->left, vrc->bottom);
        glVertex2f(vrc->right, vrc->bottom);

        glEnd();
    }


    void DrawFrame(const CRectT<float>* vrc)
    {
        glBegin(GL_LINE_LOOP);

        glVertex2f(vrc->left, vrc->top);
        glVertex2f(vrc->right, vrc->top);
        glVertex2f(vrc->right, vrc->bottom);
        glVertex2f(vrc->left, vrc->bottom);

        glEnd();
    }


    void DrawFrame(const CRectT<double>* vrc)
    {
        glBegin(GL_LINE_LOOP);

        glVertex2d(vrc->left, vrc->top);
        glVertex2d(vrc->right, vrc->top);
        glVertex2d(vrc->right, vrc->bottom);
        glVertex2d(vrc->left, vrc->bottom);

        glEnd();
    }


    void DrawFrame(const CRectT<int>* vrc)
    {
        glBegin(GL_LINE_LOOP);

        glVertex2i(vrc->left, vrc->top);
        glVertex2i(vrc->right, vrc->top);
        glVertex2i(vrc->right, vrc->bottom);
        glVertex2i(vrc->left, vrc->bottom);

        glEnd();
    }


    // vrc = Vertex rectangle
    // trc = Texture rectangle

    void DrawRect(const CRectT<float>* vrc, const CRectT<float>* trc)
    {
        glBegin(GL_TRIANGLE_STRIP);

        glTexCoord2f(trc->left, trc->top);
        glVertex2f(vrc->left, vrc->top);

        glTexCoord2f(trc->right, trc->top);
        glVertex2f(vrc->right, vrc->top);

        glTexCoord2f(trc->left, trc->bottom);
        glVertex2f(vrc->left, vrc->bottom);

        glTexCoord2f(trc->right, trc->bottom);
        glVertex2f(vrc->right, vrc->bottom);


        glEnd();
    }


    void DrawTexture(const CRectT<float>* vrc)
    {
        CRectT<float> trc;

        trc.Set(0.0f, 0.0f, 1.0f, 1.0f);

        glBegin(GL_TRIANGLE_STRIP);

        //sglMultiTexCoord2f(GL_TEXTURE0, trc.left, trc.top);
        glTexCoord2f(trc.left, trc.top);
        glVertex2f(vrc->left, vrc->top);

        //glMultiTexCoord2f(GL_TEXTURE0, trc.right, trc.top);
        glTexCoord2f(trc.right, trc.top);
        glVertex2f(vrc->right, vrc->top);

        //glMultiTexCoord2f(GL_TEXTURE0, trc.left, trc.bottom);
        glTexCoord2f(trc.left, trc.bottom);
        glVertex2f(vrc->left, vrc->bottom);

        glTexCoord2f(trc.right, trc.bottom);
        //glMultiTexCoord2f(GL_TEXTURE0, trc.right, trc.bottom);
        glVertex2f(vrc->right, vrc->bottom);


        glEnd();
    }


    // vrc = Vertex rectangle
    // trc = Texture rectangle

    void DrawRect(const CRectT<double>* vrc, const CRectT<float>* trc, bool yFlipped = false)
    {
        glBegin(GL_TRIANGLE_STRIP);

        if (yFlipped)
        {
            glTexCoord2f(trc->left, trc->bottom);
            glVertex2d(vrc->left, vrc->top);

            glTexCoord2f(trc->right, trc->bottom);
            glVertex2d(vrc->right, vrc->top);

            glTexCoord2f(trc->left, trc->top);
            glVertex2d(vrc->left, vrc->bottom);

            glTexCoord2f(trc->right, trc->top);
            glVertex2d(vrc->right, vrc->bottom);
        }
        else
        {
            glTexCoord2f(trc->left, trc->top);
            glVertex2d(vrc->left, vrc->top);

            glTexCoord2f(trc->right, trc->top);
            glVertex2d(vrc->right, vrc->top);

            glTexCoord2f(trc->left, trc->bottom);
            glVertex2d(vrc->left, vrc->bottom);

            glTexCoord2f(trc->right, trc->bottom);
            glVertex2d(vrc->right, vrc->bottom);
        }



        glEnd();
    }


    void DrawLine(float VonX, float VonY, float BisX, float BisY)
    {
        glBegin(GL_LINES);
        glVertex2f(VonX, VonY);
        glVertex2f(BisX, BisY);
        glEnd();
    }


    void DrawRectTranslated(const CRectT<float>* vrc, const CRectT<float>* trc)
    {
        glBegin(GL_TRIANGLE_STRIP);

        glTexCoord2f(trc->left, trc->top);
        glVertex2f(0, 0);

        glTexCoord2f(trc->right, trc->top);
        glVertex2f(vrc->Width(), 0);

        glTexCoord2f(trc->left, trc->bottom);
        glVertex2f(0, vrc->Height());

        glTexCoord2f(trc->right, trc->bottom);
        glVertex2f(vrc->Width(), vrc->Height());


        glEnd();
    }


    //---------------------------------------------------------------------------
    //
    // Klasse:    CGL_Patch
    // Methode:   SetSphereVertex
    //
    //
    //---------------------------------------------------------------------------

    void SetPatchVertex(const CVector3<float>* Origin, const CVector3<float>* cf)
    {
        CVector3<float> norm; // (0,0,1);

        norm.SubtractVector(cf, Origin);
        norm.Normalize();

        glNormal3fv(norm.v(0));
        glVertex3fv(cf->v(0));
    }


    //---------------------------------------------------------------------------
    //
    // Klasse:    CGL_Patch
    // Methode:   SetSphereVertex
    //
    //
    //---------------------------------------------------------------------------

    void SetSphereVertex(const CVector3<double>* Origin, const CVector3<double>* cf)
    {
        CVector3<double> norm; // (0,0,1);

        norm.SubtractVector(cf, Origin);
        norm.Normalize();

        glNormal3dv(norm.v(0));
        glVertex3dv(cf->v(0));
    }
}
