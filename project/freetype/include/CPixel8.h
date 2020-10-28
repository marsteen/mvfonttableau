//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CPixel8.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#ifndef CPIXEL8_H
#define CPIXEL8_H

#include <CPixel.h>


class CPixel8 : public CPixel
{
	public:

	  void operator=(const CPixel8& p2)
	  {
			a = p2.a;
		}


	  bool operator==(const CPixel8& p2) const
	  {
			return a == p2.a;
		}

	  bool operator!=(const CPixel8& p2) const
	  {
			return a != p2.a;
		}



		unsigned char a;
};

#endif
