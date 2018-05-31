#ifndef CCHECKER_H
#define CCHECKER_H

#include "TypeDefine.h"
#include "CQuad.h"


// �H (0,0) ������ ���X�@�� nXn ���ѽL���
// n �b�ŧi�ɵ��w
class CChecker {

private:
	CQuad *m_pSquares;
	int    m_iGridSize;
	float  *m_pfSquaresT;
	GLuint m_uiShaderHandle;
	float  m_fYPos;
	int    m_iMode;


public:
	CChecker(int iSize = 6, float fYPos = 0.0f); // �w�]�� 6 X 6 ���, �C�@�Ӥ��������O 1
	~CChecker();

	void SetShader();
	void SetShaderName(const char vxShader[], const char fsShader[]);
	void SetProjectionMatrix(mat4 &mat);
	void SetViewMatrix(mat4 &mat);
	void SetTRSMatrix(mat4 &mat);
	void SetShadingMode(int iMode) {m_iMode = iMode;}
	void SetTextureLayer(int texlayer);
	void Update(float dt, const LightSource &lights);
	void Draw();

	// For setting materials 
	void SetMaterials(color4 ambient, color4 diffuse, color4 specular);
	void SetKaKdKsShini(float ka, float kd, float ks, float shininess); // ka kd ks shininess
};

#endif