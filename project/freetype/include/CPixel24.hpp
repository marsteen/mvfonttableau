//------------------------------------------------------------------------------
//
// PROJECT : GLUT test application
//
// FILE    : CPixel24.cpp
//
// VERSION : 1.0
//
// AUTHOR  : Martin Steen
//
//
//
//
//
//---------------------------------------------------------------------------


#ifndef CPIXEL24_HPP
#define CPIXEL24_HPP

//---------------------------------------------------------------------------
//
// KLASSE        : CPixel24
// METHODE       : operator ==
//
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
bool CPixel24T<Tpixel>::operator==(const CPixel24T<Tpixel>& p2) const
{
    return (rgb[0] == p2.rgb[0]) &&
           (rgb[1] == p2.rgb[1]) &&
           (rgb[2] == p2.rgb[2]);
}


template<typename Tpixel>
void CPixel24T<Tpixel>::operator=(const CPixel32T<Tpixel>& p2)
{
    rgb[0] = p2.rgba[0];
    rgb[1] = p2.rgba[1];
    rgb[2] = p2.rgba[2];
}


//---------------------------------------------------------------------------
//
// KLASSE        : CPixel24
// METHODE       : operator!=
//
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
bool CPixel24T<Tpixel>::operator!=(const CPixel24T<Tpixel>& p2) const
{
    return (rgb[0] != p2.rgb[0]) ||
           (rgb[1] != p2.rgb[1]) ||
           (rgb[2] != p2.rgb[2]);
}


//---------------------------------------------------------------------------
//
// KLASSE        : CPixel24
// METHODE       : SwapRedBlue
//
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
void CPixel24T<Tpixel>::SwapRedBlue()
{
    Tpixel swap = rgb[0];

    rgb[0] = rgb[2];
    rgb[2] = swap;
}


//---------------------------------------------------------------------------
//
// KLASSE        : CPixel24
// METHODE       : Set
//
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
void CPixel24T<Tpixel>::Set(Tpixel r, Tpixel g, Tpixel b)
{
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
}


//---------------------------------------------------------------------------
//
// KLASSE        : CPixel24
// METHODE       : Set
//
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
void CPixel24T<Tpixel>::Set(unsigned int b)
{
    Tpixel* c = (Tpixel*)&b;

    rgb[0] = c[0];
    rgb[1] = c[1];
    rgb[2] = c[2];
}


//---------------------------------------------------------------------------
//
// KLASSE        : CPixel24
// METHODE       : Brightness
//
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
int CPixel24T<Tpixel>::Brightness()
{
    return rgb[0] + rgb[1] + rgb[2];
}


//---------------------------------------------------------------------------
//
// Klasse:    CPixel24
// Methode:		MixPixel
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template<typename Tpixel>
void CPixel24T<Tpixel>::MixPixel(const CPixel32T<Tpixel>*  p2)
{
    Tpixel Alpha1 = 255 - p2->rgba[3];
    Tpixel Alpha2 = p2->rgba[3];

    if (Alpha2 == 0)
    {
    }
    else
    if (Alpha2 < 255)
    {
        rgb[0] = ((rgb[0] * Alpha1) / 255) + ((p2->rgba[0] * Alpha2) / 255);
        rgb[1] = ((rgb[1] * Alpha1) / 255) + ((p2->rgba[1] * Alpha2) / 255);
        rgb[2] = ((rgb[2] * Alpha1) / 255) + ((p2->rgba[2] * Alpha2) / 255);
    }
    else
    {
        *this = *p2;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CPixel24
// Methode:		MixPixelRB
//
// Wie MixPixel, aber roter und blauer Farbkanal werden vertauscht
//
//---------------------------------------------------------------------------

template<typename Tpixel>
void CPixel24T<Tpixel>::MixPixelRB(const CPixel32T<Tpixel>*  p2)
{
    Tpixel Alpha1 = 255 - p2->rgba[3];
    Tpixel Alpha2 = p2->rgba[3];

    if (Alpha2 == 0)
    {
    }
    else
    if (Alpha2 < 255)
    {
        rgb[0] = ((rgb[0] * Alpha1) / 255) + ((p2->rgba[2] * Alpha2) / 255);
        rgb[1] = ((rgb[1] * Alpha1) / 255) + ((p2->rgba[1] * Alpha2) / 255);
        rgb[2] = ((rgb[2] * Alpha1) / 255) + ((p2->rgba[0] * Alpha2) / 255);
    }
    else
    {
        rgb[0] = p2->rgba[2];
        rgb[1] = p2->rgba[1];
        rgb[2] = p2->rgba[0];
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CPixel24T
// Methode:		MixPixel
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
void CPixel24T<Tpixel>::MixPixel(CPixel32T<Tpixel>*  p2, Tpixel Alpha1)
{
    rgb[0] = ((rgb[0] * Alpha1) / 255) + p2->rgba[0];
    rgb[1] = ((rgb[1] * Alpha1) / 255) + p2->rgba[1];
    rgb[2] = ((rgb[2] * Alpha1) / 255) + p2->rgba[2];
}


//---------------------------------------------------------------------------
//
// Klasse:    CPixel24T
// Methode:		MixPixel
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
void CPixel24T<Tpixel>::MixPixel(CPixel24T<Tpixel>*  p2, Tpixel Alpha1)
{
    rgb[0] = ((rgb[0] * Alpha1) / 255) + p2->rgb[0];
    rgb[1] = ((rgb[1] * Alpha1) / 255) + p2->rgb[1];
    rgb[2] = ((rgb[2] * Alpha1) / 255) + p2->rgb[2];
}


//---------------------------------------------------------------------------
//
// Klasse:    CPixel24T
// Methode:		MultPixel
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
void CPixel24T<Tpixel>::MultPixel(const CPixel32T<Tpixel>*  p2)
{
    if (p2->rgba[3] > 0) // Alpha groesser als 0
    {
        float Alpha = float(p2->rgba[3]) / 255;

        for (int i = 0; i < 3; i++)
        {
            float f = float(p2->rgba[i]) / 255;
            //rgb[i] *= f;

            rgb[i] = (rgb[i] * f * Alpha) +
                (rgb[i] * (1.0 - Alpha));
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CPixel24T
// Methode:		MixPixel
//
// g1 = 255 - Farbe bleibt wie sie ist
// g1 =   0 - Farbe wird komplett schwarz
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
void CPixel24T<Tpixel>::MixBlack(unsigned char g1)
{
    if (g1 != 255)
    {
        if (g1 == 0)
        {
            rgb[0] = rgb[1] = rgb[2] = 0;
        }
        else
        {
            rgb[0] = ((rgb[0] * g1) / 255);
            rgb[1] = ((rgb[1] * g1) / 255);
            rgb[2] = ((rgb[2] * g1) / 255);
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CPixel24T
// Methode:		MixFolie
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
void CPixel24T<Tpixel>::MixFolie(CPixel32T<Tpixel>*  p2)
{
    int r = rgb[0] - (255 - p2->rgba[0]);
    int g = rgb[1] - (255 - p2->rgba[1]);
    int b = rgb[2] - (255 - p2->rgba[2]);

    rgb[0] = (r < 0) ? 0 : r;
    rgb[1] = (g < 0) ? 0 : g;
    rgb[2] = (b < 0) ? 0 : b;
}


#endif
