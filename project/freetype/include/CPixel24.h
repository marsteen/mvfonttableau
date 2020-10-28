

#ifndef CPIXEL24_H
#define CPIXEL24_H

#include <CPixel.h>

template <typename Tpixel>
class CPixel32T;

template <typename Tpixel=unsigned char>
class CPixel24T : public CPixel
{
	public:



  	Tpixel rgb[3];

    void operator=(const CPixel24T<Tpixel>& p2)
    {
			rgb[0] = p2.rgb[0];
			rgb[1] = p2.rgb[1];
			rgb[2] = p2.rgb[2];
		}

  	void operator=(const CPixel32T<Tpixel>& p2);

    void operator=(int i)
    {
			rgb[0] = i >> 16;
			rgb[1] = (i >> 8) & 0xFF;
			rgb[2] = i & 0xFF;
		}

		bool operator<(const CPixel24T<Tpixel>& p) const
		{
			return GetInt() < p.GetInt();
		}

		unsigned int GetInt() const
		{
			return (rgb[2] << 16) | (rgb[1] << 8) | rgb[0];
		}

		void MixPixel(const CPixel32T<Tpixel>* p2);
		void MixFolie(CPixel32T<Tpixel>* p2);
		void MixPixelRB(const CPixel32T<Tpixel>* p2);
		void MixPixel(CPixel32T<Tpixel>*  p2, Tpixel Alpha1);
		void MixPixel(CPixel24T<Tpixel>*  p2, Tpixel Alpha1);
		void MixBlack(unsigned char g1);
		void MultPixel(const CPixel32T<Tpixel>*  p2);




    bool operator==(const CPixel24T<Tpixel>& p2) const;
    bool operator!=(const CPixel24T<Tpixel>& p2) const;
		void Set(Tpixel r, Tpixel g, Tpixel b);
		void Set(unsigned int b);
		void SwapRedBlue(void);
		int  Brightness(void);
};

#include <CPixel32.h>
#include <CPixel24.hpp>

typedef CPixel24T<> CPixel24;

#endif
