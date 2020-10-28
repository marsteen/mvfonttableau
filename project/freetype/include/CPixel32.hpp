//------------------------------------------------------------------------------
//
// PROJECT : GLUT test application
//
// FILE    : CPixel32.cpp
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


#ifndef CPIXEL32_HPP
#define CPIXEL32_HPP

//---------------------------------------------------------------------------
//
// KLASSE        : CPixel32T
// METHODE       : operator ==
//
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
bool CPixel32T<Tpixel>::operator==(const CPixel32T<Tpixel>& p2) const
{
    return (rgba[0] == p2.rgba[0]) &&
           (rgba[1] == p2.rgba[1]) &&
           (rgba[2] == p2.rgba[2]) &&
           (rgba[3] == p2.rgba[3]);
}


//---------------------------------------------------------------------------
//
// Klasse:    CPixel32T
// Methode:		MixPixel
//
// g1 = 255 - Farbe bleibt wie sie ist
// g1 =   0 - Farbe wird komplett schwarz
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
void CPixel32T<Tpixel>::MixBlack(unsigned char g1)
{
    if (g1 != 255)
    {
        if (g1 == 0)
        {
            rgba[0] = rgba[1] = rgba[2] = rgba[3] = 0;
        }
        else
        {
            rgba[0] = ((rgba[0] * g1) / 255);
            rgba[1] = ((rgba[1] * g1) / 255);
            rgba[2] = ((rgba[2] * g1) / 255);
            rgba[3] = ((rgba[3] * g1) / 255);
        }
    }
}


template<typename T>
T Saturate(T a, T b)
{
    return ((a + b) > 255) ? 255 : a + b;
}


//---------------------------------------------------------------------------
//
// Klasse:    CPixel32T
// Methode:		MixPixel
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
void CPixel32T<Tpixel>::MixPixel(CPixel32T<Tpixel>*  p2)
{
    Tpixel Alpha1 = 255 - p2->rgba[3];
    Tpixel Alpha2 = p2->rgba[3];
    Tpixel Alpha3 = Saturate(rgba[3], p2->rgba[3]);

    for (int i = 0; i < 3; i++)
    {
        rgba[i] = ((rgba[i] * Alpha1) / 255) + ((p2->rgba[i] * Alpha2) / 255);
    }
    //rgba[1] = ((rgba[1] * Alpha1) / 255) + ((p2->rgba[1] * Alpha2) / 255);
    //rgba[2] = ((rgba[2] * Alpha1) / 255) + ((p2->rgba[2] * Alpha2) / 255);


    rgba[3] = Alpha3;
}


//---------------------------------------------------------------------------
//
// Klasse:    CPixel32T
// Methode:		MultPixel
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
void CPixel32T<Tpixel>::MultPixel(const CPixel32T<Tpixel>*  p2)
{
    if (p2->rgba[3] > 0)
    {
        float Alpha = float(p2->rgba[3]) / 255;

        for (int i = 0; i < 3; i++)
        {
            float f = float(p2->rgba[i]) / 255;
            //rgba[i] *= f;

            rgba[i] = (rgba[i] * f * Alpha) +
                (rgba[i] * (1.0f - Alpha));
        }
    }
}


//---------------------------------------------------------------------------
//
// KLASSE        : CPixel32T
// METHODE       : operator!=
//
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
bool CPixel32T<Tpixel>::operator!=(const CPixel32T<Tpixel>& p2) const
{
    return (rgba[0] != p2.rgba[0]) ||
           (rgba[1] != p2.rgba[1]) ||
           (rgba[2] != p2.rgba[2]) ||
           (rgba[3] != p2.rgba[3]);
}


//---------------------------------------------------------------------------
//
// KLASSE        : CPixel32T
// METHODE       : SwapRedBlue
//
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
void CPixel32T<Tpixel>::SwapRedBlue()
{
    Tpixel swap = rgba[0];

    rgba[0] = rgba[2];
    rgba[2] = swap;
}


//---------------------------------------------------------------------------
//
// KLASSE        : CPixel32T
// METHODE       : Set
//
//
//
//---------------------------------------------------------------------------

template<typename Tpixel>
void CPixel32T<Tpixel>::Set(Tpixel r, Tpixel g, Tpixel b, Tpixel a)
{
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = a;
}


//---------------------------------------------------------------------------
//
// Klasse:    CPixel32
// Methode:		PrepareAlpha
//
//
//---------------------------------------------------------------------------


template<typename Tpixel>
void CPixel32T<Tpixel>::PrepareAlpha(Tpixel* Alpha1)
{
    Tpixel Alpha2 = rgba[3];

    *Alpha1 = 255 - rgba[3];


    rgba[0] = (rgba[0] * Alpha2) / 255;
    rgba[1] = (rgba[1] * Alpha2) / 255;
    rgba[2] = (rgba[2] * Alpha2) / 255;
}


#endif
