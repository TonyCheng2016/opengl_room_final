#include "CObj2.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#pragma warning(disable: 4996)
CObj2::CObj2(char *filename)
{
	loadObj(filename);
	m_pPoints = NULL; m_pNormals = NULL; m_pTex1 = NULL;

	m_pPoints = new vec4[m_iNumVtx];
	m_pNormals = new vec3[m_iNumNormals];
	m_pColors = new vec4[m_iNumVtx];
	m_pTex1 = new vec2[m_iNumVtx];

	/*m_vertices[0] = point4(-0.5, -0.5, 0.5, 1.0);
	m_vertices[1] = point4(-0.5, 0.5, 0.5, 1.0);
	m_vertices[2] = point4(0.5, 0.5, 0.5, 1.0);
	m_vertices[3] = point4(0.5, -0.5, 0.5, 1.0);
	m_vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
	m_vertices[5] = point4(-0.5, 0.5, -0.5, 1.0);
	m_vertices[6] = point4(0.5, 0.5, -0.5, 1.0);
	m_vertices[7] = point4(0.5, -0.5, -0.5, 1.0);*/
	for (int i = 0; i < m_iNumVtx; i++) { m_pPoints[i] = m_vertices[i]; }
	//for (int i = 0; i < m_iNumVtx; i++) {  m_pPoints[i] = m_vertices[i]=m_Face[i]; }
	for (int i = 0; i < m_iNumVtx; i++) { m_pNormals[i] = m_Normals[i]; }
	m_iIndex = 0;
	// generate 12 triangles: 36 vertices and 36 colors
	/*Quad(1, 0, 3, 2);
	Quad(2, 3, 7, 6);
	Quad(3, 0, 4, 7);
	Quad(6, 5, 1, 2);
	Quad(4, 5, 6, 7);
	Quad(5, 4, 0, 1);*/

	// �w�]�N�Ҧ��������]�w���Ǧ�
	//for (int i = 0; i < m_iNumVtx; i++) m_pColors[i] = vec4(-1.0f, -1.0f, -1.0f, 1.0f);

#ifdef LIGHTING_WITHCPU
	// Default Set shader's name
	SetShaderName("vsLighting_CPU.glsl", "fsLighting_CPU.glsl");
#else // lighting with GPU
#ifdef PERVERTEX_LIGHTING
	SetShaderName("vsLighting_GPU.glsl", "fsLighting_GPU.glsl");
#else
	SetShaderName("vsPerPixelLighting.glsl", "fsPerPixelLighting.glsl");
#endif
#endif  

	// Create and initialize a buffer object �A�N���������]�w���J SetShader ��
	//	CreateBufferObject();

	// �]�w����
	SetMaterials(vec4(0), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	SetKaKdKsShini(0, 0.8f, 0.2f, 1);
}

//loadobj
void CObj2::loadObj(char *fname)
{
	FILE *fp;
	int read;
	GLfloat x[3], y[2], z[3];
	char ch[100];
	//string ch2[5];
	elephant = glGenLists(1);
	fp = fopen(fname, "r");
	if (!fp)
	{
		printf("can't open file %s\n", fname);
		exit(1);
	}
	glPointSize(2.0);
	glNewList(elephant, GL_COMPILE);
	{
		glPushMatrix();
		glBegin(GL_POINTS);
		m_iNumVtx = 0;
		m_iNumNormals = 0;
		m_iNumface = 0;
		m_iNumCount = 0;
		while (!(feof(fp)))
		{
			//read = fscanf(fp, "%c %f %f %f", &ch, &x, &y, &z);
					//glVertex3f(x, y, z);
			fscanf(fp,"%s",&ch);
			
			//for (int i = 0; i < 10; i++)
			//{
			if (ch[0] == 'v'&& ch[1] !='n')
				{
					read = fscanf(fp, "%f %f %f", &x, &y, &z);
					if (read == 3 ){
						m_vertices2[m_iNumVtx] = vec4(x[0], y[0], z[0], 1.0);
						m_iNumVtx++;
						//glVertex3f(x, y, z);
						//continue;
					}
					
				}
			if (ch[0]=='v' && ch[1] =='n')
			{
				read = fscanf(fp, "%f %f %f", &x, &y, &z);
				if (read == 3){
					m_Normals2[m_iNumVtx] = vec3(x[0], y[0], z[0]);
					m_iNumNormals++;
				}
			}
			if (ch[0] == 'f')
			{
				read = fscanf(fp, "%f//%f %f//%f %f//%f", &x[0], &z[0], &x[1], &z[1], &x[2], &z[2]);
				if (read == 6){
					m_Face[m_iNumface] = vec3(x[0], 0, z[0]);
					m_Face[m_iNumface+1] = vec3(x[1], 0, z[1]);
					m_Face[m_iNumface+2] = vec3(x[2], 0, z[2]);
					
					
					m_vertices[m_iNumCount] = m_vertices2[int(m_Face[m_iNumface].x)-1];
					m_vertices[m_iNumCount+1] = m_vertices2[int(m_Face[m_iNumface+1].x)-1];
					m_vertices[m_iNumCount+2] = m_vertices2[int(m_Face[m_iNumface+2].x)-1];
					
					m_Normals[m_iNumCount] = m_Normals2[int(m_Face[m_iNumface].z) - 1];
					m_Normals[m_iNumCount+1] = m_Normals2[int(m_Face[m_iNumface+1].z) - 1];
					m_Normals[m_iNumCount+2] = m_Normals2[int(m_Face[m_iNumface+2].z) - 1];
					//m_Normals[0] = m_Normals2[2];
					m_iNumface = m_iNumface + 3;
					m_iNumCount = m_iNumCount + 3;


				}
				//for (int i = 0; i < m_iNumVtx; i++) { m_vertices[i] = m_vertices2[int(m_Face[m_iNumface].x)+i]; }
				//for (int i = 0; i < m_iNumVtx; i++) { m_Normals[i] = m_Normals[int(m_Face[m_iNumface+i].z) - 1]; }
			}
			//}
		}
		glEnd();
	}
	glPopMatrix();
	glEndList();
	fclose(fp);
}

//void CObj2::Quad(int a, int b, int c, int d)
//{
//	// Initialize temporary vectors along the quad's edge to
//	//   compute its face normal 
//	vec4 u = m_vertices[b] - m_vertices[a];
//	vec4 v = m_vertices[c] - m_vertices[b];
//	vec3 normal = normalize(cross(u, v));
//
//	m_pNormals[m_iIndex] = normal; m_pPoints[m_iIndex] = m_vertices[a]; m_iIndex++;
//	m_pNormals[m_iIndex] = normal; m_pPoints[m_iIndex] = m_vertices[b]; m_iIndex++;
//	m_pNormals[m_iIndex] = normal; m_pPoints[m_iIndex] = m_vertices[c]; m_iIndex++;
//	m_pNormals[m_iIndex] = normal; m_pPoints[m_iIndex] = m_vertices[a]; m_iIndex++;
//	m_pNormals[m_iIndex] = normal; m_pPoints[m_iIndex] = m_vertices[c]; m_iIndex++;
//	m_pNormals[m_iIndex] = normal; m_pPoints[m_iIndex] = m_vertices[d]; m_iIndex++;
//}

void CObj2::Draw()
{
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Change to wireframe mode
	DrawingSetShader();
	glDrawArrays(GL_TRIANGLES, 0, SOLIDCUBE_NUM);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Return to solid mode
}


void CObj2::DrawW()
{
	DrawingWithoutSetShader();
	glDrawArrays(GL_TRIANGLES, 0, SOLIDCUBE_NUM);
}


void CObj2::RenderWithFlatShading(vec4 vLightPos, color4 vLightI)
{
	// �H�C�@�ӭ����T�ӳ��I�p��䭫�ߡA�H�ӭ��ߧ@���C��p�⪺�I��
	// �ھ� Phong lighting model �p��۹������C��A�ñN�C���x�s�즹�T���I��
	// �]���C�@�ӥ��������I�� Normal ���ۦP�A�ҥH���B�èS���p�⦹�T�ӳ��I������ Normal

	vec4 vCentroidP;
	for (int i = 0; i < m_iNumVtx; i += 3) {
		// �p��T���Ϊ�����
		vCentroidP = (m_pPoints[i] + m_pPoints[i + 1] + m_pPoints[i + 2]) / 3.0f;
		m_pColors[i] = m_pColors[i + 1] = m_pColors[i + 2] = PhongReflectionModel(vCentroidP, m_pNormals[i], vLightPos, vLightI);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}

void CObj2::RenderWithFlatShading(const LightSource &Lights)
{
	// �H�C�@�ӭ����T�ӳ��I�p��䭫�ߡA�H�ӭ��ߧ@���C��p�⪺�I��
	// �ھ� Phong lighting model �p��۹������C��A�ñN�C���x�s�즹�T���I��
	// �]���C�@�ӥ��������I�� Normal ���ۦP�A�ҥH���B�èS���p�⦹�T�ӳ��I������ Normal

	vec4 vCentroidP;
	for (int i = 0; i < m_iNumVtx; i += 3) {
		// �p��T���Ϊ�����
		vCentroidP = (m_pPoints[i] + m_pPoints[i + 1] + m_pPoints[i + 2]) / 3.0f;
		m_pColors[i] = m_pColors[i + 1] = m_pColors[i + 2] = PhongReflectionModel(vCentroidP, m_pNormals[i], Lights.position, Lights.diffuse);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}

void CObj2::RenderWithGouraudShading(vec4 vLightPos, color4 vLightI)
{
	vec4 vCentroidP;
	for (int i = 0; i < m_iNumVtx; i += 6) {
		m_pColors[i] = m_pColors[i + 3] = PhongReflectionModel(m_pPoints[i], m_pNormals[i], vLightPos, vLightI);
		m_pColors[i + 2] = m_pColors[i + 4] = PhongReflectionModel(m_pPoints[i + 2], m_pNormals[i + 2], vLightPos, vLightI);
		m_pColors[i + 1] = PhongReflectionModel(m_pPoints[i + 1], m_pNormals[i + 1], vLightPos, vLightI);
		m_pColors[i + 5] = PhongReflectionModel(m_pPoints[i + 5], m_pNormals[i + 5], vLightPos, vLightI);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumNormals, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}

void CObj2::RenderWithGouraudShading(const LightSource &Lights)
{
	vec4 vCentroidP;
	for (int i = 0; i < m_iNumVtx; i += 6) {
		m_pColors[i] = m_pColors[i + 3] = PhongReflectionModel(m_pPoints[i], m_pNormals[i], Lights);
		m_pColors[i + 2] = m_pColors[i + 4] = PhongReflectionModel(m_pPoints[i + 2], m_pNormals[i + 2], Lights);
		m_pColors[i + 1] = PhongReflectionModel(m_pPoints[i + 1], m_pNormals[i + 1], Lights);
		m_pColors[i + 5] = PhongReflectionModel(m_pPoints[i + 5], m_pNormals[i + 5], Lights);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}
void CObj2::RenderWithGouraudShading(const LightSource &light1, const LightSource &light2)
{
	vec4 vCentroidP;
	for (int i = 0; i < m_iNumVtx; i += 6) {
		m_pColors[i] = m_pColors[i + 3] = PhongReflectionModelTwoLights(m_pPoints[i], m_pNormals[i], light1, light2);
		m_pColors[i + 2] = m_pColors[i + 4] = PhongReflectionModelTwoLights(m_pPoints[i + 2], m_pNormals[i + 2], light1, light2);
		m_pColors[i + 1] = PhongReflectionModelTwoLights(m_pPoints[i + 1], m_pNormals[i + 1], light1, light2);
		m_pColors[i + 5] = PhongReflectionModelTwoLights(m_pPoints[i + 5], m_pNormals[i + 5], light1, light2);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}

// ���B�ҵ��� vLightPos �����O�@�ɮy�Ъ��T�w�����m
void CObj2::Update(float dt, point4 vLightPos, color4 vLightI)
{
#ifdef LIGHTING_WITHCPU
	if (m_bViewUpdated || m_bTRSUpdated) { // Model View �������x�}���e�����
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxMV3X3Final = mat3(
			m_mxMVFinal._m[0].x, m_mxMVFinal._m[1].x, m_mxMVFinal._m[2].x,
			m_mxMVFinal._m[0].y, m_mxMVFinal._m[1].y, m_mxMVFinal._m[2].y,
			m_mxMVFinal._m[0].z, m_mxMVFinal._m[1].z, m_mxMVFinal._m[2].z);

#ifdef GENERAL_CASE
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal);
#endif

		m_bViewUpdated = m_bTRSUpdated = false;
	}
	if (m_iMode == FLAT_SHADING) RenderWithFlatShading(vLightPos, vLightI);
	else RenderWithGouraudShading(vLightPos, vLightI);

#else // Lighting With GPU
	if (m_bViewUpdated || m_bTRSUpdated) {
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	m_vLightInView = m_mxView * vLightPos;		// �N Light �ഫ�����Y�y�ЦA�ǤJ
	// ��X AmbientProduct DiffuseProduct �P SpecularProduct �����e
	m_AmbientProduct = m_Material.ka * m_Material.ambient  * vLightI;
	m_DiffuseProduct = m_Material.kd * m_Material.diffuse  * vLightI;
	m_SpecularProduct = m_Material.ks * m_Material.specular * vLightI;
#endif
}


void CObj2::Update(float dt, const LightSource &Lights)
{
#ifdef LIGHTING_WITHCPU
	if (m_bViewUpdated || m_bTRSUpdated) { // Model View �������x�}���e�����
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxMV3X3Final = mat3(
			m_mxMVFinal._m[0].x, m_mxMVFinal._m[1].x, m_mxMVFinal._m[2].x,
			m_mxMVFinal._m[0].y, m_mxMVFinal._m[1].y, m_mxMVFinal._m[2].y,
			m_mxMVFinal._m[0].z, m_mxMVFinal._m[1].z, m_mxMVFinal._m[2].z);

#ifdef GENERAL_CASE
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal);
#endif

		m_bViewUpdated = m_bTRSUpdated = false;
	}
	if (m_iMode == FLAT_SHADING) RenderWithFlatShading(Lights);
	else RenderWithGouraudShading(Lights);

#else // Lighting With GPU
	if (m_bViewUpdated || m_bTRSUpdated) {
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	m_vLightInView = m_mxView * Lights.position;		// �N Light �ഫ�����Y�y�ЦA�ǤJ
	// ��X AmbientProduct DiffuseProduct �P SpecularProduct �����e
	m_AmbientProduct = m_Material.ka * m_Material.ambient  *  Lights.ambient;
	m_DiffuseProduct = m_Material.kd * m_Material.diffuse  *  Lights.diffuse;
	m_SpecularProduct = m_Material.ks * m_Material.specular * Lights.specular;
#endif

}
//--------------------light1�Mlight2----------------
void CObj2::UpdateTwoLights(float dt, const LightSource &light1, const LightSource &light2)
{
#ifdef LIGHTING_WITHCPU
	if (m_bViewUpdated || m_bTRSUpdated) { // Model View �������x�}���e�����
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxMV3X3Final = mat3(
			m_mxMVFinal._m[0].x, m_mxMVFinal._m[1].x, m_mxMVFinal._m[2].x,
			m_mxMVFinal._m[0].y, m_mxMVFinal._m[1].y, m_mxMVFinal._m[2].y,
			m_mxMVFinal._m[0].z, m_mxMVFinal._m[1].z, m_mxMVFinal._m[2].z);

#ifdef GENERAL_CASE
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal);
#endif

		m_bViewUpdated = m_bTRSUpdated = false;
	}
	if (m_iMode == FLAT_SHADING) RenderWithFlatShading(light1);
	else RenderWithGouraudShading(light1);

#else // Lighting With GPU
	if (m_bViewUpdated || m_bTRSUpdated) {
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	m_vLightInView = m_mxView * light1.position;// �N light1 �ഫ�����Y�y�ЦA�ǤJ
	m_vLightInView2 = m_mxView * light2.position;// �N light2 �ഫ�����Y�y�ЦA�ǤJ
	//// ��X AmbientProduct DiffuseProduct �P SpecularProduct �����e
	m_AmbientProduct = m_Material.ka * m_Material.ambient  *  light1.ambient; //light1 a
	m_DiffuseProduct = m_Material.kd * m_Material.diffuse  *  light1.diffuse; //light1 d
	m_SpecularProduct = m_Material.ks * m_Material.specular * light1.specular;//light1 s
	m_AmbientProduct2 = m_Material.ka * m_Material.ambient  *  light2.ambient;//light2 a
	m_DiffuseProduct2 = m_Material.kd * m_Material.diffuse  *  light2.diffuse;//light2 d
	m_SpecularProduct2 = m_Material.ks * m_Material.specular * light2.specular;//light2 s
#endif

}
void CObj2::Update(float dt)
{
	if (m_bViewUpdated || m_bTRSUpdated) { // Model View �������x�}���e�����
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal);
		m_bViewUpdated = m_bTRSUpdated = false;
	}
}