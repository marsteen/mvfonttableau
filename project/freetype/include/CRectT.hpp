//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CRectT.hpp
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************

#include <MathDefines.h>
#include <template_macros.hpp>


//---------------------------------------------------------------------------
//
// Klasse:    CRectT
// Methode:   IntersectLine
//
// Die Schnittmenge zweier eindimensionaler Linien  wird berechnet.
//
// Linie 1: von A1 nach B1
// Linie 2: von A2 nach B2
// Linie 3: von A3 nach B3
//
// Linie 3 ist die Schnittmenge der Linien 1 und 2. Für den Fall, dass
// die beiden Linien sich nicht überschneiden, ist das Ergebnis
// undefiniert und false wird zurückgegeben.
//
// Rueckgabewert: true - Linien überschneiden sich, Linie 3 (A3/B3)
//                      enthält die Schnittmenge
//
//               false - Schnittmenge ist leer
//
//---------------------------------------------------------------------------

template <class T>
bool CRectT<T>::IntersectLine(T A1, T B1, T A2, T B2, T* A3, T* B3) const
{
	*A3 = Tmax(A1, A2);
	if ((*A3 > B1) || (*A3 > B2)) return false;

	*B3 = Tmin(B1, B2);
	if ((*B3 < A1) || (*B3 < A2)) return false;

	return true;
}


//---------------------------------------------------------------------------
//
// Klasse:    CRectT
// Methode:   TotalInside
//
// 
// Test, ob Rechteck r2 sich komplett im Rechteck befindet
//
//---------------------------------------------------------------------------

template <class T>
bool CRectT<T>::TotalInside(const CRectT& r2) const
{
	return  (r2.left >= left) && (r2.right <= right) && 
	        (r2.top >= top) && (r2.bottom <= bottom);	
	
}

//---------------------------------------------------------------------------
//
// Klasse:    CRectT
// Methode:   InterSectRect
//
// Die Schnittmenge zweier Rechtecke wird berechnet.
//
// ri enthalt die Schnittmenge der Rechtecke r1 und r2
//
// Rückgabewert: true - r1 und r2 haben Schnittmenge
//
//               false - Schnittmenge leer
//
//---------------------------------------------------------------------------




template <class T>
bool CRectT<T>::InterSectRect(const CRectT* r2, CRectT* ri) const
{
	
	bool r = false;
	
	if (TotalInside(*r2))
	{
		*ri = *r2;
		r = true;
	}
	else
	if (r2->TotalInside(*this))		
	{
		r = true;
	}
	else
	{
		if (IntersectLine(left, right, r2->left, r2->right, &(ri->left), &(ri->right)))
		{
			if (IntersectLine(top, bottom, r2->top, r2->bottom, &(ri->top), &(ri->bottom)))
			{
				r = true;
			}
		}
	}
	return r;
}

//---------------------------------------------------------------------------
//
// Klasse:    CRectT
// Methode:   TrimRect
//
//---------------------------------------------------------------------------

template <class T>
void CRectT<T>::TrimRect()
{
	T SwapVal;

	if (right < left)
	{
		SwapVal = left; left = right; right = SwapVal;
	}
	if (bottom < top)
	{
		SwapVal = top; top = bottom; bottom = SwapVal;
	}
}

//---------------------------------------------------------------------------
//
// Klasse:    CRectT
// Methode:   InRect
//
// Rückgabewert: true - Punkt x,y ist im Rechteck
//
//               false - nicht im Rechteck
//---------------------------------------------------------------------------

template <class T>
bool CRectT<T>::InRect(T x, T y) const
{
	return (x >= left) && (x <= right) &&
	       (y >= top)  && (y <= bottom);
}



//---------------------------------------------------------------------------
//
// Klasse:    CRectT
// Methode:   Add
//
//---------------------------------------------------------------------------

template <class T>
void CRectT<T>::Add(T xoff, T yoff)
{
  left   += xoff;
  top    += yoff;
  right  += xoff;
  bottom += yoff;
}

//---------------------------------------------------------------------------
//
// Klasse:    CRectT
// Methode:   AddPercent
//
//---------------------------------------------------------------------------

template <class T>
void CRectT<T>::AddPercent(T xp, T yp)
{
	T xoff = Width()  * xp;
  T yoff = Height() * yp;
	
	Add(xoff, yoff);
}


//---------------------------------------------------------------------------
//
// Klasse:    CRectT
// Methode:   Expand
//
//---------------------------------------------------------------------------

template <class T>
void CRectT<T>::Expand(T xoff, T yoff)
{
  left   -= xoff;
  top    -= yoff;
  right  += xoff;
  bottom += yoff;
}



//---------------------------------------------------------------------------
//
// Klasse:    CRect
// Methode:   Show
//
//
//---------------------------------------------------------------------------

template <class T>
void CRectT<T>::Show(const char* title, std::stringstream& mstr) const
{
	mstr << title
	     << " l=" << left
	     << " t=" << top
	     << " r=" << right
	     << " b=" << bottom;
}



template <class T>
std::string CRectT<T>::Show() const
{
	std::stringstream mstr;
	mstr << "l=" << left
	     << " t=" << top
	     << " r=" << right
	     << " b=" << bottom;
			 
	return mstr.str();
}

//---------------------------------------------------------------------------
//
// Klasse:    CRect
// Methode:   Stretch
//
//
//---------------------------------------------------------------------------

template <class T>
void CRectT<T>::Stretch(T xs, T ys)
{
	left   = CenterX() - (Width()  / 2) * xs;
	right  = CenterX() + (Width()  / 2) * xs;
	top    = CenterY() - (Height() / 2) * ys;
	bottom = CenterY() + (Height() / 2) * ys;
	
}

//---------------------------------------------------------------------------
//
// Klasse:    CRect
// Methode:   Stretch
//
// Flags z.B. top + left = 5
//
//---------------------------------------------------------------------------

template <class T>
void CRectT<T>::Stretch(T xs, T ys, unsigned int f)
{
	if (f & 1) left   = CenterX() - (Width()  / 2) * xs;
	if (f & 2) right  = CenterX() + (Width()  / 2) * xs;
	if (f & 4) top    = CenterY() - (Height() / 2) * ys;
	if (f & 8 )bottom = CenterY() + (Height() / 2) * ys;
	
}


//---------------------------------------------------------------------------
//
// Klasse:    CRect
// Methode:   JoinRect
//
//
//---------------------------------------------------------------------------

template <class T>
void CRectT<T>::JoinRect(const CRectT<T>& rc)
{
	if (!isset)
	{
		left   = rc.left;
		right  = rc.right;
		top    = rc.top;
		bottom = rc.bottom;
		isset = true;
	}
		
	
	left   = Tmin(left,   rc.left);
	right  = Tmax(right,  rc.right);
	top    = Tmin(top,    rc.top);
	bottom = Tmax(bottom, rc.bottom);
	
	
	
	
}


//---------------------------------------------------------------------------
//
// Klasse:    CRect
// Methode:   Rotate
//
// vec - Zeiger auf vier Vectoren, in denen das Ergebnis der 
// Rotation gespeichert wird
//
//---------------------------------------------------------------------------

template <class T>
void CRectT<T>::Rotate(float r_grad, CVector2<T>* vec)
{

  CVector2<T> Center;
  float r_rad = DEG_TO_RAD(r_grad);
  float sin_r = sin(r_rad);
  float cos_r = cos(r_rad);
  Center.x = CenterX();
	Center.y = CenterY();
	
	vec[0].x = left;
	vec[0].y = top;

	vec[1].x = right;
	vec[1].y = top;
	
	vec[2].x = right;
	vec[2].y = bottom;

	vec[3].x = left;
	vec[3].y = bottom;
	
	for (int i = 0; i < 4; i++)
	{
		vec[i].RotationXY(&Center, sin_r, cos_r);
	}
}



