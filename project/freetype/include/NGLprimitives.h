#ifndef NGLPRIMITIVES_H
#define NGLPRIMITIVES_H

#include <CRectT.h>
#include <CVector3T.h>

namespace NGLprimitives
{
	void DrawRect(const CRectT<float>* vrc);

	void DrawRect(const CRectT<float>* vrc, const CRectT<float>* trc);
	void DrawRect(const CRectT<double>* vrc, const CRectT<float>* trc, bool yFlipped=false);

	void DrawFrame(const CRectT<int>* vrc);
  void DrawFrame(const CRectT<float>* vrc);
  void DrawFrame(const CRectT<double>* vrc);
  void DrawTexture(const CRectT<float>* vrc);

  void DrawLine(float VonX, float VonY, float BisX, float BisY);
	void DrawRectTranslated(const CRectT<float>* vrc, const CRectT<float>* nrc);

	void SetSphereVertex(const CVector3<float>* Origin, const CVector3<float>* cf);
	void SetSphereVertex(const CVector3<double>* Origin, const CVector3<double>* cf);

};

#endif
