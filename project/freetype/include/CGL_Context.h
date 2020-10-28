//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CGL_Context.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************

#ifndef CGL_Context_H
#define CGL_Context_H


class CGL_Context
{
	public:
	
		CGL_Context();
		virtual ~CGL_Context() { };

		virtual bool Init(int w, int h);
		virtual void Draw2D() { };
	
		int              mWidth;
		int              mHeight;
		float            mAspect;
		
};

#endif

