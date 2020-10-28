//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CPolygon.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#ifndef CPOLYGON_H
#define CPOLYGON_H

#include <vector>
#include <CVector2T.h>
#include <CRectT.h>

struct SPolyPoint : public CVector2<float>
{
    SPolyPoint()
    {
        t = 0.0f;
    }


    float t; // Koordinate des Punktes innerhalb des Polygons (0..1)
};

class CPolygon
{
    public:

        CPolygon()
        {
            mTotalLength = 0.0;
        }


        void CalcLength();

        int Size() const
        {
            return mPoints.size();
        }


        float ix(int i) const
        {
            return mPoints[i].x;
        }


        float iy(int i) const
        {
            return mPoints[i].y;
        }


        float it(int i) const
        {
            return mPoints[i].t;
        }


        float CenterX() const
        {
            return mCenter.x;
        }


        float CenterY() const
        {
            return mCenter.y;
        }


        void AddPoint(float x, float y)
        {
            SPolyPoint pt;

            pt.Set(x, y);
            mPoints.push_back(pt);
        }


        void AddPoint(SPolyPoint& pt)
        {
            mPoints.push_back(pt);
        }


        float BoxWidth() const
        {
            return mBBox.Width();
        }


        float BoxHeight() const
        {
            return mBBox.Height();
        }


        void Clear()
        {
            mPoints.clear();
            mTotalLength = 0.0;
        }


        std::vector<SPolyPoint> mPoints;
        CVector2<float> mCenter;        // Zentrum aller Punkte
        CRectT<float> mBBox;            // Bounding Box

        float mTotalLength;

    protected:

        void CalcCenter();
        void CalcBBox();
};

#endif
