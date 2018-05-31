#ifndef CSOLIDSPHEREMT_H
#define CSOLIDSPHEREMT_H
#include "../header/Angel.h"
#include "CShapeMT.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

class CSolidSphereMT : public CShapeMT
{
private:
	GLfloat m_fRadius;
	GLint m_iSlices, m_iStacks;

public:
	CSolidSphereMT(const GLfloat fRadius=1.0f, const int iSlices=12,const  int iStacks = 6);
	~CSolidSphereMT();

	void Update(float dt, const LightSource &lights);
	void Update(float dt); // ���p��������ө�

	// Sphere ��ø�s��l�ϥΦh�ժ� GL_TRIANGLE_STRIP ��ø�s, �]���S����k���� Flat Shading�A
	// �u���H vertex ����¦���p���C�⪺ Ground Shading
	void RenderWithFlatShading(const LightSource &lights);
	void RenderWithGouraudShading(const LightSource &lights);
	void Draw();
	void DrawW(); // �I�s���A���]�w Shader ���yø�覡

};

#endif