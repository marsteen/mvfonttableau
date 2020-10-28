//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CRectT.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#ifndef CRECTT_H
#define CRECTT_H

#include <CVector2T.h>
#include <string>
#include <sstream>


template<class T>
class CRectT
{
    public:

        CRectT()
        {
            isset = false;
        }


        void Set(T _left, T _top, T _right, T _bottom)
        {
            left = _left;
            top = _top;
            right = _right;
            bottom = _bottom;
            isset = true;
        }


        bool InterSectRect(const CRectT* r2, CRectT* ri) const;
        bool InRect(T x, T y) const;
        void TrimRect(void);
        void Show(const char* title, std::stringstream& mstr) const;
        std::string Show() const;
        void Add(T xoff, T yoff);
        void AddPercent(T xp, T yp);
        void Expand(T xoff, T yoff);
        void Stretch(T xs, T ys);
        void Stretch(T xs, T ys, unsigned int f);
        void JoinRect(const CRectT<T>& rc);
        bool TotalInside(const CRectT& r2) const;
        void Rotate(float grad, CVector2<T>* vec);

        T Area() const { return Width() * Height(); }

        T CenterX() const { return (left + right) / 2; }
        T CenterY() const { return (bottom + top) / 2; }
        void CopyFrom(const CRectT<T>& v)
        {
            left = v.left;
            right = v.right;
            top = v.top;
            bottom = v.bottom;
        }


        void FlipY()
        {
            T tmp = bottom;

            bottom = top;
            top = tmp;
        }


        T Width()  const { return right - left; }
        T Height() const { return bottom - top; }

        bool isset; // Flag, ob das Rechteck gesetzt wurde
        T left;
        T top;
        T right;
        T bottom;

    protected:

        bool IntersectLine(T A1, T B1, T A2, T B2, T* A3, T* B3) const;
};

#include <CRectT.hpp>

#endif
