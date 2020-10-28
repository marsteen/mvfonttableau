//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CGL_Basic.cpp
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************
//
// Inhalt: Implementation von CDataRect
//
//  Klasse fuer einfache OpenGL/ES Operationen
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 01.06.2013 erstellt- Martin Steen
//
//------------------------------------------------------------------------------

#include <NGlobalLog.h>
#include <GLinclude.h>
#include <SVertArray.h>
#include <CGL_Basic.h>
#include <CVector2T.h>
#include <CVector3T.h>
#include <CGL_Context.h>

extern void checkGlError(const char* func);

using namespace NGlobalLog;

// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawQuad
//
// Zeichnen eines Quads als Trifan (Counterclockwise)
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawQuad(const CGL_Context* glcon, float x1, float y1, float x2, float y2)
{
	SVertArray ar;
	CVector3<float> v;
	
	ar.MakeVerts(4);
	
	v.Set(x1, y1, 0);	
	ar.AddVert(v);
	
	v.Set(x2, y1, 0);
	ar.AddVert(v);
	
	v.Set(x2, y2, 0);
	ar.AddVert(v);

	v.Set(x1, y2, 0);
	ar.AddVert(v);
	
	glFrontFace(GL_CCW);
	glDisable(GL_CULL_FACE);
	
	glVertexAttribPointer(glcon->VertAttrib(), 3, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
	glEnableVertexAttribArray(glcon->VertAttrib());


	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

 	glDisableVertexAttribArray(glcon->VertAttrib());
 	
 	//checkGlError("DrawQuad");
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawQuad

//  const float* Colors: Zeiger auf 3*4 floats
//  vier Farbewerte fuer die vier Ecken des Rechtecks
//
//  Die ersten beiden Farbwerte sind den UNTEREN Ecken zugeordnet
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawQuad(const CGL_Context* glcon, float x1, float y1, float x2, float y2, const float* Colors)
{
	SVertArray ar;
	CVector3<float> v;
	
	ar.MakeVerts(4);
	
	/*
	v.Set(x1, y1, 0);	
	ar.AddVert(v);

	v.Set(x1, y2, 0);
	ar.AddVert(v);	

	v.Set(x2, y2, 0);
	ar.AddVert(v);
	
	v.Set(x2, y1, 0);
	ar.AddVert(v);
	*/
	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);	
	
	v.Set(x1, y1, 0);	
	ar.AddVert(v);
	
	v.Set(x2, y1, 0);
	ar.AddVert(v);
	
	v.Set(x2, y2, 0);
	ar.AddVert(v);

	v.Set(x1, y2, 0);
	ar.AddVert(v);
	
	glVertexAttribPointer(glcon->VertAttrib(), 3, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
	glVertexAttribPointer(glcon->ColorAttrib(), 3, GL_FLOAT, GL_FALSE, 0, Colors);
	
	glEnableVertexAttribArray(glcon->VertAttrib());
	glEnableVertexAttribArray(glcon->ColorAttrib());


	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

 	glDisableVertexAttribArray(glcon->VertAttrib());
	glDisableVertexAttribArray(glcon->ColorAttrib());
 	
 	//checkGlError("DrawQuad");
}

// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawQuad

//  const float* Colors: Zeiger auf 16 floats
//  vier Farbewerte fuer die vier Ecken des Rechtecks
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawQuad(const CGL_Context* glcon, const CRectT<float>& vr, const float* Colors)
{
	DrawQuad(glcon, vr.left, vr.top, vr.right, vr.bottom, Colors);	
}

// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawQuad
//
// ---------------------------------------------------------------------------


void CGL_Basic::DrawQuad(const CGL_Context* glcon, const CRectT<float>& vr)
{
	DrawQuad(glcon, vr.left, vr.top, vr.right, vr.bottom);	
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : AddVertToPolygon
//
// ---------------------------------------------------------------------------


static void AddPointToArray(int i, const CPolygon* Polygon,  float sx, float sy, const float& TexOffset, SVertArray2D& ar)
{
	CVector2<float> v;
	CVector2<float> t;

	float px = Polygon->ix(i) * sx;
	float py = Polygon->iy(i) * sy;
	float tx = Polygon->it(i);

	
	v.Set(px, py);
	t.Set(tx, TexOffset);	
	ar.AddVert(v);
	ar.AddText(t);
}





// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawPolygon
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawPolygon(const CGL_Context* glcon, const CPolygon* Polygon, float sx, float sy, float& TexOffset)
{

  if (Polygon->Size() > 0)
  {
		static bool sFirst = true;
		SVertArray2D ar;
		CVector2<float> v;
		CVector2<float> t;
	
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
		ar.MakeVerts(Polygon->Size());
		ar.MakeTexts(Polygon->Size());
	
		for (int i = 0; i < Polygon->Size(); i++)
		{
			AddPointToArray(i, Polygon, sx, sy, TexOffset, ar);
	
		/*
			float px = Polygon->ix(i) * sx;
			float py = Polygon->iy(i) * sy;
			float tx = Polygon->it(i);
		
			if (sFirst)
			{
				//gdstr << "px=" << px << " py=" << py;
				//gdlog();
			}
	
			v.Set(px, py, 0);
			t.Set(tx, TexOffset);	
			ar.AddVert(v);
			ar.AddText(t);
	*/		
		}
	
		//AddPointToArray(0, Polygon, sx, sy, TexOffset, ar); // Polygon schliessen

	
	
		TexOffset += 0.002;
		if (TexOffset >= 100000.0f)
		{
			TexOffset -= 100000.0f;
		}
	
		
		glVertexAttribPointer(glcon->VertAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
		glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mText->v());
	
		glEnableVertexAttribArray(glcon->VertAttrib());
		glEnableVertexAttribArray(glcon->TextAttrib());


		glDrawArrays(GL_LINE_STRIP, 0, ar.mCount);

		glDisableVertexAttribArray(glcon->VertAttrib());
		glDisableVertexAttribArray(glcon->TextAttrib());
	
		glDisable(GL_BLEND);

		checkGlError("DrawPolygon");
		sFirst = false;
	}
}

// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawLine
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawLine(const CGL_Context* glcon, float x1, float y1, float x2, float y2) 
{
	SVertArray ar;
	CVector3<float> v;
	
	ar.MakeVerts(2);
	
	v.Set(x1, y1, 0);	
	ar.AddVert(v);
	v.Set(x2, y2, 0);
	ar.AddVert(v);	
	
	checkGlError("VOR  DrawLine");
		
	glVertexAttribPointer(glcon->VertAttrib(), 3, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());	
	glEnableVertexAttribArray(glcon->VertAttrib());
	glDrawArrays(GL_LINES, 0, 2);

 	glDisableVertexAttribArray(glcon->VertAttrib());	

 	checkGlError("NACH DrawLine");
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawLines
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawLines(const CGL_Context* glcon, const std::vector<	CVector2<float> >& LineVec) 
{
	SVertArray2D ar;
	CVector2<float> v;
	
	ar.MakeVerts(LineVec.size());
	for (int i = 0; i < LineVec.size(); i++)
	{	
	  ar.AddVert(LineVec[i]);
	}
		
	glVertexAttribPointer(glcon->VertAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());	
	glEnableVertexAttribArray(glcon->VertAttrib());
	glDrawArrays(GL_LINES, 0, LineVec.size());
 	glDisableVertexAttribArray(glcon->VertAttrib());	

 	checkGlError("DrawLine"); 	
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawFrame
//
// ---------------------------------------------------------------------------


void CGL_Basic::DrawFrame(const CGL_Context* glcon, const CRectT<float>& rc)
{
	SVertArray ar;
	CVector3<float> v;
	
	ar.MakeVerts(4);
	
	v.Set(rc.left, rc.top, 0);	
	ar.AddVert(v);
	v.Set(rc.right, rc.top, 0);
	ar.AddVert(v);	
	v.Set(rc.right, rc.bottom, 0);
	ar.AddVert(v);
	v.Set(rc.left, rc.bottom, 0);
	ar.AddVert(v);
		
	glVertexAttribPointer(glcon->VertAttrib(), 3, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
	
	glEnableVertexAttribArray(glcon->VertAttrib());


	glDrawArrays(GL_LINE_LOOP, 0, 4);

 	glDisableVertexAttribArray(glcon->VertAttrib());	

 	checkGlError("DrawFrame"); 	
}



// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawTextureQuad
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawTextureQuad(const CGL_Context* glcon, float x1, float y1, float x2, float y2)
{
	SVertArray ar;
	CVector3<float> v;
	CVector2<float> t;
	
	ar.MakeVerts(4);
	ar.MakeTexts(4);
	
	v.Set(x1, y1, 0);	
	t.Set(0.0f, 0.0f);
	ar.AddVert(v);
	ar.AddText(t);
	
	
	v.Set(x2, y1, 0);
	t.Set(1.0f, 0.0f);
	ar.AddVert(v);
	ar.AddText(t);
	
	
	v.Set(x2, y2, 0);
	t.Set(1.0f, 1.0f);
	ar.AddVert(v);
	ar.AddText(t);

	v.Set(x1, y2, 0);
	t.Set(0.0f, 1.0f);
	ar.AddVert(v);
	ar.AddText(t);
	
	glVertexAttribPointer(glcon->VertAttrib(), 3, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
  glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mText->v());
	
	glEnableVertexAttribArray(glcon->VertAttrib());
	glEnableVertexAttribArray(glcon->TextAttrib());
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

 	glDisableVertexAttribArray(glcon->VertAttrib());
	glDisableVertexAttribArray(glcon->TextAttrib());
	
 	checkGlError("DrawTextureQuad 2");

}

// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawTextureQuad
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawTextureQuad(const CGL_Context* glcon, const CRectT<float>& rc)
{
	DrawTextureQuad(glcon, rc.left, rc.top, rc.right, rc.bottom);
}

// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawTextureQuadRotated
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawTextureQuadRotated(const CGL_Context* glcon, float x1, float y1, float x2, float y2)
{
	SVertArray ar;
	CVector3<float> v;
	CVector2<float> t;
	
	float w = x2 - x1;
	float h = y2 - y1;

	
	x2 = x1 + h;
	y2 = y1 + w;
	
	
	ar.MakeVerts(4);
	ar.MakeTexts(4);
	
	v.Set(x1, y1, 0);	
	t.Set(0.0f, 1.0f);
	ar.AddVert(v);
	ar.AddText(t);
	
	
	v.Set(x2, y1, 0);
	t.Set(0.0f, 0.0f);
	ar.AddVert(v);
	ar.AddText(t);
	
	
	v.Set(x2, y2, 0);
	t.Set(1.0f, 0.0f);
	ar.AddVert(v);
	ar.AddText(t);

	v.Set(x1, y2, 0);
	t.Set(1.0f, 1.0f);
	ar.AddVert(v);
	ar.AddText(t);
	
	glVertexAttribPointer(glcon->VertAttrib(), 3, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
  glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mText->v());
	
	glEnableVertexAttribArray(glcon->VertAttrib());
	glEnableVertexAttribArray(glcon->TextAttrib());
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

 	glDisableVertexAttribArray(glcon->VertAttrib());
	glDisableVertexAttribArray(glcon->TextAttrib());
	
 	checkGlError("DrawTextureQuadRotated");

}



// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawTextureQuadAngle
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawTextureQuadAngle(const CGL_Context* glcon, const CRectT<float>& vr, const CRectT<float>& tr, float Angle, const CVector2<float>* org1)
{

	SVertArray2D ar;
	CVector2<float> v;
	CVector2<float> t;
	CVector2<float> org;

	ar.MakeVerts(4);
	ar.MakeTexts(4);
	
	if (org1 == NULL)
	{
		org.x = vr.CenterX();
		org.y = vr.CenterY();	
	}
	else
	{
	  org = *org1;
	}
	
	float sinA = sin(DEG_TO_RAD(Angle));
	float cosA = cos(DEG_TO_RAD(Angle));
	

	

	v.Set(vr.left, vr.top);
	v.RotationXY(&org, sinA, cosA);	
	t.Set(tr.left, tr.top);
	ar.AddVert(v);
	ar.AddText(t);


	v.Set(vr.right, vr.top);
	v.RotationXY(&org, sinA, cosA);	
	t.Set(tr.right, tr.top);
	ar.AddVert(v);
	ar.AddText(t);


	v.Set(vr.right, vr.bottom);
	v.RotationXY(&org, sinA, cosA);	
	t.Set(tr.right, tr.bottom);
	ar.AddVert(v);
	ar.AddText(t);

	v.Set(vr.left, vr.bottom);
	v.RotationXY(&org, sinA, cosA);	
	t.Set(tr.left, tr.bottom);
	ar.AddVert(v);
	ar.AddText(t);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glVertexAttribPointer(glcon->VertAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
  glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mText->v());

	glEnableVertexAttribArray(glcon->VertAttrib());
	glEnableVertexAttribArray(glcon->TextAttrib());

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

 	glDisableVertexAttribArray(glcon->VertAttrib());
	glDisableVertexAttribArray(glcon->TextAttrib());

	glDisable(GL_BLEND);

 	checkGlError("DrawTextureQuadAngle");

}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawQuadAngle
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawQuadAngle(const CGL_Context* glcon, const CRectT<float>& vr, float Angle, const CVector2<float>* org1)
{

	SVertArray2D ar;
	CVector2<float> v;
	CVector2<float> org;

	ar.MakeVerts(4);
	
	
	if (org1 == NULL)
	{
		org.x = vr.CenterX();
		org.y = vr.CenterY();	
	}
	else
	{
	  org = *org1;
	}
	
	float sinA = sin(DEG_TO_RAD(Angle));
	float cosA = cos(DEG_TO_RAD(Angle));
	

	

	v.Set(vr.left, vr.top);
	v.RotationXY(&org, sinA, cosA);	
	ar.AddVert(v);


	v.Set(vr.right, vr.top);
	v.RotationXY(&org, sinA, cosA);	
	ar.AddVert(v);


	v.Set(vr.right, vr.bottom);
	v.RotationXY(&org, sinA, cosA);	
	ar.AddVert(v);
	

	v.Set(vr.left, vr.bottom);
	v.RotationXY(&org, sinA, cosA);		
	ar.AddVert(v);
	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glVertexAttribPointer(glcon->VertAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());  
	glEnableVertexAttribArray(glcon->VertAttrib());
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
 	glDisableVertexAttribArray(glcon->VertAttrib());

	glDisable(GL_BLEND);

 	checkGlError("DrawQuadAngle");

}



// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawTextureQuad
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawTextureQuad(const CGL_Context* glcon, const CRectT<float>& vr, const CRectT<float>& tr)
{

	SVertArray2D ar;
	CVector2<float> v;
	CVector2<float> t;

	ar.MakeVerts(4);
	ar.MakeTexts(4);

	v.Set(vr.left, vr.top);
	t.Set(tr.left, tr.top);
	ar.AddVert(v);
	ar.AddText(t);


	v.Set(vr.right, vr.top);
	t.Set(tr.right, tr.top);
	ar.AddVert(v);
	ar.AddText(t);


	v.Set(vr.right, vr.bottom);
	t.Set(tr.right, tr.bottom);
	ar.AddVert(v);
	ar.AddText(t);

	v.Set(vr.left, vr.bottom);
	t.Set(tr.left, tr.bottom);
	ar.AddVert(v);
	ar.AddText(t);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glVertexAttribPointer(glcon->VertAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
  glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mText->v());

	glEnableVertexAttribArray(glcon->VertAttrib());
	glEnableVertexAttribArray(glcon->TextAttrib());

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

 	glDisableVertexAttribArray(glcon->VertAttrib());
	glDisableVertexAttribArray(glcon->TextAttrib());

	glDisable(GL_BLEND);

	checkGlError("DrawTextureQuad 1");

}



// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawDisk
//
// ---------------------------------------------------------------------------


void CGL_Basic::DrawDisk(const CGL_Context* glcon, 
  float Radius, 
  float a_start,                      // Start-Winkel in Grad 0..360
  float a_size,                       // Groesse in Grad (0..360)
  const CVector2<float>& origin, 
  int segs)
{

	SVertArray2D ar;
	CVector2<float> v;

	ar.MakeVerts(segs+2);
	
	float a_start_rad = DEG_TO_RAD(a_start);
	float a_size_rad  = DEG_TO_RAD(a_size);
	float step_rad    = a_size_rad / segs;

	//glDisable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	
	ar.AddVert(origin);		
	
	for (int i = 0; i <=  segs; i++)
	{
	  v.x = origin.x + cos(a_start_rad) * Radius;
	  v.y = origin.y + sin(a_start_rad) * Radius;
		ar.AddVert(v);
		a_start_rad += step_rad;
	}
	

	glVertexAttribPointer(glcon->VertAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());

	glEnableVertexAttribArray(glcon->VertAttrib());

	glDrawArrays(GL_TRIANGLE_FAN, 0, ar.mCount);

 	glDisableVertexAttribArray(glcon->VertAttrib());
	
	glFrontFace(GL_CCW);

}



// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawRing
//
// ---------------------------------------------------------------------------


void CGL_Basic::DrawRing(const CGL_Context* glcon, 
  float Radius_inner, 
  float Radius_outer, 
  float a_start,                      // Start-Winkel in Grad 0..360
  float a_size,                       // Groesse in Grad (0..360)
  const CVector2<float>& origin, 
  int segs)
{

	SVertArray2D ar;
	CVector2<float> v;

	ar.MakeVerts((segs+1) * 2);
	
	float a_start_rad = DEG_TO_RAD(a_start);
	float a_size_rad  = DEG_TO_RAD(a_size);
	float step_rad    = a_size_rad / segs;

	
	for (int i = 0; i <=  segs; i++)
	{
	  v.x = origin.x + cos(a_start_rad) * Radius_inner;
	  v.y = origin.y + sin(a_start_rad) * Radius_inner;
		ar.AddVert(v);

	  v.x = origin.x + cos(a_start_rad) * Radius_outer;
	  v.y = origin.y + sin(a_start_rad) * Radius_outer;
		ar.AddVert(v);	
		
		a_start_rad += step_rad;
	}
	

	glVertexAttribPointer(glcon->VertAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());

	glEnableVertexAttribArray(glcon->VertAttrib());

	glDrawArrays(GL_TRIANGLE_STRIP, 0, ar.mCount);

 	glDisableVertexAttribArray(glcon->VertAttrib());

}



// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawCircle
//
// ---------------------------------------------------------------------------


void CGL_Basic::DrawCircle(const CGL_Context* glcon, 
  float Radius, 
  float a_start,                      // Start-Winkel in Grad 0..360
  float a_size,                       // Groesse in Grad (0..360)
  const CVector2<float>& origin, 
  int segs)
{

	SVertArray2D ar;
	CVector2<float> v;

	ar.MakeVerts(segs+1);
	
	float a_start_rad = DEG_TO_RAD(a_start);
	float a_size_rad  = DEG_TO_RAD(a_size);
	float step_rad    = a_size_rad / segs;

	
	for (int i = 0; i <=  segs; i++)
	{
	  v.x = origin.x + cos(a_start_rad) * Radius;
	  v.y = origin.y + sin(a_start_rad) * Radius;
		ar.AddVert(v);
		a_start_rad += step_rad;
	}
	

	glVertexAttribPointer(glcon->VertAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());

	glEnableVertexAttribArray(glcon->VertAttrib());

	glDrawArrays(GL_LINE_STRIP, 0, ar.mCount);

 	glDisableVertexAttribArray(glcon->VertAttrib());

}



