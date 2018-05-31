#ifndef COBJ2_H
#define COBJ2_H
#include "../header/Angel.h"
#include "CShape.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#define SOLIDCUBE_NUM 7500  // 6 faces, 2 triangles/face , 3 vertices/triangle //car
//#define SOLIDCUBE_NUM 800//monkey
class CObj2 : public CShape
{
private:
	//car
	vec4 m_vertices2[5000];
	vec4 m_vertices[5000];
	vec3 m_Normals2[6000];
	vec3 m_Normals[6000];
	vec3 m_Face[22000];
	////monkey
	//vec4 m_vertices2[1000];
	//vec4 m_vertices[1000];
	//vec3 m_Normals2[1000];
	//vec3 m_Normals[1000];
	//vec3 m_Face[3000];
	int  m_iIndex;

	//void Quad(int a, int b, int c, int d);
public:
	CObj2(char *filename);
	~CObj2(){};

	void Update(float dt, point4 vLightPos, color4 vLightI);
	void Update(float dt, const LightSource &Lights);
	void Update(float dt); // 不計算光源的照明

	void UpdateTwoLights(float dt, const LightSource &light1, const LightSource &light2);//light1 and light2

	void RenderWithFlatShading(point4 vLightPos, color4 vLightI);
	void RenderWithGouraudShading(point4 vLightPos, color4 vLightI);
	void RenderWithFlatShading(const LightSource &Lights);
	void RenderWithGouraudShading(const LightSource &Lights);

	void RenderWithGouraudShading(const LightSource &light1, const LightSource &light2);//light1 and light2
	void Draw();
	void DrawW();

	//loadobj
	GLuint elephant;
	float elephantrot;
	char ch = '1';
	void loadObj(char *fname);//obj reader	
};

#endif