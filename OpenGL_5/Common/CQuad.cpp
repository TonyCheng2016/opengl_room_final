#include "CQuad.h"
// Example 4 �}�l
// ���P Example 3 �¤W(Y�b)
// �C�@�� Vertex �W�[ Normal �A�令�~���� CShape�A�@�ֳB�z�������]�w�ݨD


CQuad::CQuad()
{
	m_iNumVtx = QUAD_NUM;
	m_pPoints = NULL; m_pNormals = NULL; m_pTex1 = NULL;

	m_pPoints  = new vec4[m_iNumVtx];
	m_pNormals = new vec3[m_iNumVtx];
	m_pColors  = new vec4[m_iNumVtx]; 
	m_pTex1 = new vec2[m_iNumVtx];		// ���ͩһݪ��K�Ϯy��
#if MULTITEXTURE >= LIGHT_MAP
	m_pTex2 = new vec2[m_iNumVtx];  // ���� light map �һݪ��K�Ϯy��
#endif
#if MULTITEXTURE >= NORMAL_MAP
	m_pTex3 = new vec2[m_iNumVtx];	// ���� normal map �һݪ��K�Ϯy��
	// ���ͩһݭn�� Tanget vector for each vertex
	m_pTangentV = new vec3[m_iNumVtx];
#endif

	m_pPoints[0] = vec4( -0.5f, 0.0f,  0.5f, 1.0f);
	m_pPoints[1] = vec4(  0.5f, 0.0f,  0.5f, 1.0f);
	m_pPoints[2] = vec4(  0.5f, 0.0f, -0.5f, 1.0f);
	m_pPoints[3] = vec4( -0.5f, 0.0f,  0.5f, 1.0f);
	m_pPoints[4] = vec4(  0.5f, 0.0f, -0.5f, 1.0f);
	m_pPoints[5] = vec4( -0.5f, 0.0f, -0.5f, 1.0f);

	m_pNormals[0] = vec3(  0, 1.0f, 0);  // Normal Vector �� W �� 0
	m_pNormals[1] = vec3(  0, 1.0f, 0);
	m_pNormals[2] = vec3(  0, 1.0f, 0);
	m_pNormals[3] = vec3(  0, 1.0f, 0);
	m_pNormals[4] = vec3(  0, 1.0f, 0);
	m_pNormals[5] = vec3(  0, 1.0f, 0);

	m_pColors[0] = vec4( 1.0f, 1.0f,  1.0f, 1.0f);  // (r, g, b, a)
	m_pColors[1] = vec4( 1.0f, 1.0f,  1.0f, 1.0f);
	m_pColors[2] = vec4( 1.0f, 1.0f,  1.0f, 1.0f);
	m_pColors[3] = vec4( 1.0f, 1.0f,  1.0f, 1.0f);
	m_pColors[4] = vec4( 1.0f, 1.0f,  1.0f, 1.0f);
	m_pColors[5] = vec4( 1.0f, 1.0f,  1.0f, 1.0f);

	m_pTex1[0] = vec2(0.0f, 0.0f);
	m_pTex1[1] = vec2(1.0f, 0.0f);
	m_pTex1[2] = vec2(1.0f, 1.0f);
	m_pTex1[3] = vec2(0.0f, 0.0f);
	m_pTex1[4] = vec2(1.0f, 1.0f);
	m_pTex1[5] = vec2(0.0f, 1.0f);

#if MULTITEXTURE >= LIGHT_MAP
	m_pTex2[0] = vec2(0.0f, 0.0f);
	m_pTex2[1] = vec2(1.0f, 0.0f);
	m_pTex2[2] = vec2(1.0f, 1.0f);
	m_pTex2[3] = vec2(0.0f, 0.0f);
	m_pTex2[4] = vec2(1.0f, 1.0f);
	m_pTex2[5] = vec2(0.0f, 1.0f);
#endif
#if MULTITEXTURE >= NORMAL_MAP
	m_pTex3[0] = vec2(0.0f, 0.0f);
	m_pTex3[1] = vec2(1.0f, 0.0f);
	m_pTex3[2] = vec2(1.0f, 1.0f);
	m_pTex3[3] = vec2(0.0f, 0.0f);
	m_pTex3[4] = vec2(1.0f, 1.0f);
	m_pTex3[5] = vec2(0.0f, 1.0f);
	// �p�� tangent vector
	for (int i = 0; i < 6; i += 3) { // �T�� vertex �@��
		float dU1 = m_pTex1[i + 1].x - m_pTex1[i].x;
		float dV1 = m_pTex1[i + 1].y - m_pTex1[i].y;
		float dU2 = m_pTex1[i + 2].x - m_pTex1[i].x;
		float dV2 = m_pTex1[i + 2].y - m_pTex1[i].y;
		float f = 1.0f/(dU1 * dV2 - dU2*dV1);
		vec4 E1 = m_pPoints[i + 1] - m_pPoints[i];
		vec4 E2 = m_pPoints[i + 2] - m_pPoints[i];

		vec3 tangent;
		tangent.x = f*(dV2 * E1.x + E2.x * (-dV1));
		tangent.y = f*(dV2 * E1.y + E2.y * (-dV1));
		tangent.z = f*(dV2 * E1.z + E2.z * (-dV1));

		m_pTangentV[i] += tangent;
		m_pTangentV[i + 1] += tangent;
		m_pTangentV[i + 2] = tangent;
	}
	for (int i = 0; i < 6; i++)
		m_pTangentV[i] = normalize(m_pTangentV[i]);
#endif

	for( int i = 0 ; i < m_iNumVtx ; i++ ) m_pColors[i] = vec4(-1.0f,-1.0f,-1.0f,1.0f);

#ifdef PERVERTEX_LIGHTING
	SetShaderName("vsPerVtxLighting.glsl", "fsPerVtxLighting.glsl");
#else
	SetShaderName("vsPerPixelLighting.glsl", "fsPerPixelLighting.glsl");
#endif

}

void CQuad::RenderWithFlatShading(const LightSource &lights)
{
	// �H�C�@�ӭ����T�ӳ��I�p��䭫�ߡA�H�ӭ��ߧ@���C��p�⪺�I��
	// �ھ� Phong lighting model �p��۹������C��A�ñN�C���x�s�즹�T���I��
	// �]���C�@�ӥ��������I�� Normal ���ۦP�A�ҥH���B�èS���p�⦹�T�ӳ��I������ Normal
	vec4 vCentroidP;
	for (int i = 0; i < m_iNumVtx; i += 3) {
		// �p��T���Ϊ�����
		vCentroidP = (m_pPoints[i] + m_pPoints[i + 1] + m_pPoints[i + 2]) / 3.0f;
		m_pColors[i] = m_pColors[i + 1] = m_pColors[i + 2] = PhongReflectionModel(vCentroidP, m_pNormals[i], lights);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}


void CQuad::RenderWithGouraudShading(const LightSource &lights)
{
	// �H vertex �����A�ھڸ��I����m�P��k�V�q�A�Q�� Phong lighting model �p��۹������C��
	// �N���C���x�s�^�ӳ��I

	// �Ĥ����I���c������ӤT����
	// �p�� 0 1 2 5 �|�ӳ��I���C��Y�i�A0 �P 3�B2 �P 4 ���C��ۦP

	m_pColors[0] = m_pColors[3] = PhongReflectionModel(m_pPoints[0], m_pNormals[0], lights);
	m_pColors[2] = m_pColors[4] = PhongReflectionModel(m_pPoints[2], m_pNormals[2], lights);
	m_pColors[1] = PhongReflectionModel(m_pPoints[1], m_pNormals[1], lights);
	m_pColors[5] = PhongReflectionModel(m_pPoints[5], m_pNormals[5], lights);

	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}
//-------------------------light1 and light2---------------
void CQuad::RenderWithGouraudShading(const LightSource &light1, const LightSource &light2)
{
	// �H vertex �����A�ھڸ��I����m�P��k�V�q�A�Q�� Phong lighting model �p��۹������C��
	// �N���C���x�s�^�ӳ��I

	// �Ĥ����I���c������ӤT����
	// �p�� 0 1 2 5 �|�ӳ��I���C��Y�i�A0 �P 3�B2 �P 4 ���C��ۦP

	m_pColors[0] = m_pColors[3] = PhongReflectionModelTwoLights(m_pPoints[0], m_pNormals[0], light1, light2);
	m_pColors[2] = m_pColors[4] = PhongReflectionModelTwoLights(m_pPoints[2], m_pNormals[2], light1, light2);
	m_pColors[1] = PhongReflectionModelTwoLights(m_pPoints[1], m_pNormals[1], light1,light2);
	m_pColors[5] = PhongReflectionModelTwoLights(m_pPoints[5], m_pNormals[5], light1, light2);

	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}

void CQuad::Update(float dt, const LightSource &lights)
{
#ifdef LIGHTING_WITHCPU
	if( m_bViewUpdated || m_bTRSUpdated  ) { // Model View �������x�}���e�����
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxMV3X3Final = mat3(
			m_mxMVFinal._m[0].x,  m_mxMVFinal._m[1].x, m_mxMVFinal._m[2].x,
			m_mxMVFinal._m[0].y,  m_mxMVFinal._m[1].y, m_mxMVFinal._m[2].y,
			m_mxMVFinal._m[0].z,  m_mxMVFinal._m[1].z, m_mxMVFinal._m[2].z);
#ifdef GENERAL_CASE
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal); 
#endif
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	if (m_iMode == FLAT_SHADING) RenderWithFlatShading(lights);
	else RenderWithGouraudShading(lights);

#else // Lighting With GPU
	if (m_bViewUpdated || m_bTRSUpdated) {
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	m_vLightInView = m_mxView * lights.position;		// �N Light �ഫ�����Y�y�ЦA�ǤJ
	// ��X AmbientProduct DiffuseProduct �P SpecularProduct �����e
	m_AmbientProduct = m_Material.ka * m_Material.ambient  * lights.ambient;
	m_AmbientProduct.w = m_Material.ambient.w;
	m_DiffuseProduct = m_Material.kd * m_Material.diffuse  * lights.diffuse;
	m_DiffuseProduct.w = m_Material.diffuse.w;
	m_SpecularProduct = m_Material.ks * m_Material.specular * lights.specular;
	m_SpecularProduct.w = m_Material.specular.w;
#endif

}
//--------------------light1�Mlight2----------------
void CQuad::UpdateTwoLights(float dt, const LightSource &light1, const LightSource &light2)
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

void CQuad::Update(float dt)
{
	if (m_bViewUpdated || m_bTRSUpdated) { // Model View �������x�}���e�����
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal); 
		m_bViewUpdated = m_bTRSUpdated = false;
	}
}

void CQuad::SetVtxColors(vec4 vLFColor, vec4 vLRColor, vec4 vTRColor, vec4 vTLColor)
{
	m_pColors[3] = m_pColors[0] = vLFColor;
	m_pColors[1] = vLRColor;
	m_pColors[4] = m_pColors[2] = vTRColor;
	m_pColors[5] = vTLColor;

	// �N�Ҧ� vertices �C���s�� VBO ��
	glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx+sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors ); // vertcies' Color
}

void CQuad::SetVtxColors(vec4 vFColor, vec4 vSColor) // �]�w��ӤT���Ϊ��C��
{
	m_pColors[0] = m_pColors[1] = m_pColors[2] = vFColor;
	m_pColors[3] = m_pColors[4] = m_pColors[5] = vSColor;

	// �N�Ҧ� vertices �C���s�� VBO ��
	glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx+sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors ); // vertcies' Color
}

void CQuad::Draw()
{
	DrawingSetShader();
//	glBindTexture(GL_TEXTURE_2D, m_uiTexObject[0]); 
	glDrawArrays( GL_TRIANGLES, 0, QUAD_NUM );
//	glBindTexture(GL_TEXTURE_2D, 0);  // ����{�����K�ϳ]�w
}

void CQuad::DrawW()
{
	DrawingWithoutSetShader();
//	glBindTexture(GL_TEXTURE_2D, m_uiTexObject[0]); 
	glDrawArrays( GL_TRIANGLES, 0, QUAD_NUM );
//	glBindTexture(GL_TEXTURE_2D, 0);  // ����{�����K�ϳ]�w
}