//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CGL_Basic.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#ifndef CGL_Basic_H
#define CGL_Basic_H

#include <GLinclude.h>
#include <CGL_Context.h>
#include <CPolygon.h>
#include <CRectT.h>
#include <CVector2T.h>


class CGL_Basic
{
	public:
		
		// Flaechen:
	
		static void DrawQuad(const CGL_Context* glcon, float x1, float y1, float x2, float y2);
		static void DrawQuad(const CGL_Context* glcon, const CRectT<float>& vr);
		static void DrawQuad(const CGL_Context* glcon, float x1, float y1, float x2, float y2, const float* Colors);
		static void DrawQuad(const CGL_Context* glcon, const CRectT<float>& vr, const float* Colors);
		static void DrawQuadAngle(const CGL_Context* glcon, const CRectT<float>& vr, float Angle, const CVector2<float>* org1);

		
		// Flaechen mit Textur:
		
		static void DrawTextureQuad(const CGL_Context* glcon, float x1, float y1, float x2, float y2);
		static void DrawTextureQuad(const CGL_Context* glcon, const CRectT<float>& rc);
		static void DrawTextureQuad(const CGL_Context* glcon, const CRectT<float>& vr, const CRectT<float>& tr);
  	static void DrawTextureQuad(const CGL_Context* glcon, const CVector2<float>* vr, const CRectT<float>& tr);
   	static void DrawTextureQuadRotated(const CGL_Context* glcon, float x1, float y1, float x2, float y2);
		static void DrawTextureQuadAngle(const CGL_Context* glcon, const CRectT<float>& vr, const CRectT<float>& tr, float Angle, const CVector2<float>* org1=NULL);


		
		// Linien:
		
		static void DrawLine(const CGL_Context* glcon, float x1, float y1, float x2, float y2);
		static void DrawLines(const CGL_Context* glcon, const std::vector<	CVector2<float> >& LineVec);
		static void DrawPolygon(const CGL_Context* glcon, const CPolygon* Polygon, float sx, float sy, float& TexOffset);
		static void DrawFrame(const CGL_Context* glcon, const CRectT<float>& rc);

   // Kreise, Kreisflaechen, Ringe:
		
		static void DrawDisk(const CGL_Context* glcon, 
						float Radius, 
						float a_start,                      // Start-Winkel inn Grad 0..360
						float a_end,                        // End-Winkel in Grad 0..360
						const CVector2<float>& origin, 
						int segs);

		static void DrawCircle(const CGL_Context* glcon, 
						float Radius, 
						float a_start,                      // Start-Winkel inn Grad 0..360
						float a_end,                        // End-Winkel in Grad 0..360
						const CVector2<float>& origin, 
						int segs);
						
		static void DrawRing(const CGL_Context* glcon, 
			float Radius_inner, 
			float Radius_outer, 
			float a_start,                      // Start-Winkel in Grad 0..360
			float a_size,                       // Groesse in Grad (0..360)
			const CVector2<float>& origin, 
			int segs);
						
						


		



};

#endif
