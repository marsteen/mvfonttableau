//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CPixel24fixT.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#ifndef CPIXEL24FIXT_H
#define CPIXEL24FIXT_H



#include <CFixpointT.h>

//
// 1 Kanal
//
template <typename Tpixeltype>
struct SPixel1i
{
	Tpixeltype v;
};


template <int Tbits>
class CPixel1fixT
{
	public:

		void Init()
		{
			v = 0;
		}

		template <typename Tpixeltype>
		void CopyTo(SPixel1i<Tpixeltype>* pf)
		{
			pf->v = v.RoundIntU();
		}

		template <typename Tpixeltype>
		void Add(SPixel1i<Tpixeltype>* p, CFixpointT<Tbits> f)
		{
			v += f * p->v;
		}

		template <typename Tpixeltype>
		void Add(SPixel1i<Tpixeltype>* p)
		{
			v += p->v;
		}

		void Add(CPixel1fixT<Tbits>* p)
		{
			v += p->v;
		}
		void Div(CFixpointT<Tbits> f)
		{
			v /= f;
		}
	  void Mult(CFixpointT<Tbits> f, CPixel1fixT<Tbits>* pf)
	  {
			pf->v = v * f;
		}

	protected:

		CFixpointT<Tbits> v;
};


//
// 3 Kanäle
//
template <typename Tpixeltype>
struct SPixel3i
{
	Tpixeltype rgb[3];
};

template <int Tbits>
class CPixel3fixT
{
	public:

		void Init()
		{
			rgb[0] = 0;
			rgb[1] = 0;
			rgb[2] = 0;
		}

		//
		// CopyTo
		//

		template <typename Tpixeltype>
	  void CopyTo(SPixel3i<Tpixeltype>* pf)
	  {
			pf->rgb[0] = rgb[0].RoundIntU();
			pf->rgb[1] = rgb[1].RoundIntU();
			pf->rgb[2] = rgb[2].RoundIntU();
		}


		//
		// Add
		//
		template <typename Tpixeltype>
		void Add(SPixel3i<Tpixeltype>* p, CFixpointT<Tbits> f)
		{
			rgb[0] += f * p->rgb[0];
			rgb[1] += f * p->rgb[1];
			rgb[2] += f * p->rgb[2];
		}

		template <typename Tpixeltype>
		void Add(SPixel3i<Tpixeltype>* p)
		{
			rgb[0] += p->rgb[0];
			rgb[1] += p->rgb[1];
			rgb[2] += p->rgb[2];
		}


		//
		// Add
		//
		void Add(CPixel3fixT<Tbits>* p)
		{
			rgb[0] += p->rgb[0];
			rgb[1] += p->rgb[1];
			rgb[2] += p->rgb[2];
		}
		void Div(CFixpointT<Tbits> f)
		{
			rgb[0] /= f;
			rgb[1] /= f;
			rgb[2] /= f;
		}
	  void Mult(CFixpointT<Tbits> f, CPixel3fixT<Tbits>* pf)
	  {
			pf->rgb[0] = rgb[0] * f;
			pf->rgb[1] = rgb[1] * f;
			pf->rgb[2] = rgb[2] * f;
		}

	protected:

		CFixpointT<Tbits> rgb[4];

};


//
// 4 Kanäle
//
template <typename Tpixeltype>
struct SPixel4i
{
	Tpixeltype rgba[4];
};


template <int Tbits>
class CPixel4fixT
{
	public:

		void Init()
		{
			rgba[0] = 0;
			rgba[1] = 0;
			rgba[2] = 0;
			rgba[3] = 0;
		}


		//
		// CopyTo
		//
		template <typename Tpixeltype>
	  void CopyTo(SPixel4i<Tpixeltype>* pf)
	  {
			pf->rgba[0] = rgba[0].RoundIntU();
			pf->rgba[1] = rgba[1].RoundIntU();
			pf->rgba[2] = rgba[2].RoundIntU();
			pf->rgba[3] = rgba[3].RoundIntU();
		}

		//
		// Add
		//
		template <typename Tpixeltype>
		void Add(SPixel4i<Tpixeltype>* p, CFixpointT<Tbits> f)
		{
			rgba[0] += f * p->rgba[0];
			rgba[1] += f * p->rgba[1];
			rgba[2] += f * p->rgba[2];
			rgba[3] += f * p->rgba[3];
		}

		template <typename Tpixeltype>
		void Add(SPixel4i<Tpixeltype>* p)
		{
			rgba[0] += p->rgba[0];
			rgba[1] += p->rgba[1];
			rgba[2] += p->rgba[2];
			rgba[3] += p->rgba[3];
		}

		void Add(CPixel4fixT<Tbits>* p)
		{
			rgba[0] += p->rgba[0];
			rgba[1] += p->rgba[1];
			rgba[2] += p->rgba[2];
			rgba[3] += p->rgba[3];
		}

		void Div(CFixpointT<Tbits> f)
		{
			rgba[0] /= f;
			rgba[1] /= f;
			rgba[2] /= f;
			rgba[3] /= f;
		}

	  void Mult(CFixpointT<Tbits> f, CPixel4fixT<Tbits>* pf)
	  {
			pf->rgba[0] = rgba[0] * f;
			pf->rgba[1] = rgba[1] * f;
			pf->rgba[2] = rgba[2] * f;
			pf->rgba[3] = rgba[3] * f;
		}

	protected:

		CFixpointT<Tbits> rgba[4];

};


#endif
