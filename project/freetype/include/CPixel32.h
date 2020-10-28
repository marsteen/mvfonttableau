#ifndef CPIXEL32_H
#define CPIXEL32_H

#include <CPixel.h>
#include <CPixel24.h>

template<typename Tpixel>
class CPixel32T : public CPixel
{
    public:

        unsigned char rgba[4];

        void operator=(const CPixel32T<Tpixel>& p2)
        {
            rgba[0] = p2.rgba[0];
            rgba[1] = p2.rgba[1];
            rgba[2] = p2.rgba[2];
            rgba[3] = p2.rgba[3];
        }


        void operator=(const CPixel24T<Tpixel>& p2)
        {
            rgba[0] = p2.rgb[0];
            rgba[1] = p2.rgb[1];
            rgba[2] = p2.rgb[2];
            rgba[3] = 0xFF;
        }


        void operator=(int i)
        {
            rgba[0] = i >> 24;
            rgba[1] = (i >> 16) & 0xFF;
            rgba[2] = (i >> 8) & 0xFF;
            rgba[3] = i & 0xFF;
        }


        unsigned int GetInt() const
        {
            return (rgba[3] << 24) | (rgba[2] << 16) | (rgba[1] << 8) | rgba[0];
        }


        bool operator<(const CPixel32T<Tpixel>& p) const
        {
            return GetInt() < p.GetInt();
        }


        bool operator==(const CPixel32T<Tpixel>& p2) const;
        bool operator!=(const CPixel32T<Tpixel>& p2) const;
        void Set(Tpixel r, Tpixel g, Tpixel b, Tpixel a);
        void SwapRedBlue(void);
        void PrepareAlpha(Tpixel* Alpha1);
        void MixBlack(unsigned char g1);
        void MixPixel(CPixel32T<Tpixel>*  p2);
        void MultPixel(const CPixel32T<Tpixel>*  p2);
};

typedef CPixel32T<unsigned char> CPixel32;

#include <CPixel32.hpp>

#endif
