#ifndef CSOLIDSPHERE_H
#define CSOLIDSPHERE_H
#include "../header/Angel.h"
#include "CShape.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

class CSolidSphere : public CShape
{
private:
	GLfloat m_fRadius;
	GLint m_iSlices, m_iStacks;

public:
	CSolidSphere(const GLfloat fRadius=1.0f, const int iSlices=12,const  int iStacks = 6);
	~CSolidSphere();

	void Update(float dt, const LightSource &lights);
	void Update(float dt); // ���p��������ө�
	void UpdateTwoLights(float dt, const LightSource &light1, const LightSource &light2);//light1 and light2
	// Sphere ��ø�s��l�ϥΦh�ժ� GL_TRIANGLE_STRIP ��ø�s, �]���S����k���� Flat Shading�A
	// �u���H vertex ����¦���p���C�⪺ Ground Shading
	void RenderWithFlatShading(const LightSource &lights);
	void RenderWithGouraudShading(const LightSource &lights);
	void RenderWithGouraudShading(const LightSource &light1, const LightSource &light2);//light1 and light2
	void Draw();
	void DrawW(); // �I�s���A���]�w Shader ���yø�覡

};

#endif