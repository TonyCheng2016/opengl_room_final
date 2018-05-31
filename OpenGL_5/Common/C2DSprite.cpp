#include "C2DSprite.h"

C2DSprite::C2DSprite()
{
	m_iNumVtx = QUAD_NUM;
	m_pPoints = NULL; m_pNormals = NULL; m_pTex1 = NULL; m_pColors = NULL;

	m_pPoints = new vec4[m_iNumVtx];
	m_pNormals = new vec3[m_iNumVtx];
	m_pColors = new vec4[m_iNumVtx];
	// 此處改成正面為面向正 Z 方向 
	m_pPoints[0] = vec4(-0.5f, -0.5f, 0.0f, 1.0f);
	m_pPoints[1] = vec4( 0.5f, -0.5f, 0.0f, 1.0f);
	m_pPoints[2] = vec4( 0.5f,  0.5f, 0.0f, 1.0f);
	m_pPoints[3] = vec4(-0.5f, -0.5f, 0.0f, 1.0f);
	m_pPoints[4] = vec4( 0.5f,  0.5f, 0.0f, 1.0f);
	m_pPoints[5] = vec4(-0.5f,  0.5f, 0.0f, 1.0f);

	m_pNormals[0] = vec3(0, 0, 1.0f);  // Normal Vector 的 W 為 0
	m_pNormals[1] = vec3(0, 0, 1.0f);
	m_pNormals[2] = vec3(0, 0, 1.0f);
	m_pNormals[3] = vec3(0, 0, 1.0f);
	m_pNormals[4] = vec3(0, 0, 1.0f);
	m_pNormals[5] = vec3(0, 0, 1.0f);

	//m_pColors[0] = vec4(1.0f, 1.0f, 1.0f, 1.0f);  // (r, g, b, a)
	//m_pColors[1] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//m_pColors[2] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//m_pColors[3] = vec4( 1.0f, 1.0f,  1.0f, 1.0f);
	//m_pColors[4] = vec4( 1.0f, 1.0f,  1.0f, 1.0f);
	//m_pColors[5] = vec4( 1.0f, 1.0f,  1.0f, 1.0f);

	//for (int i = 0; i < m_iNumVtx; i++) m_pColors[i] = vec4(-1.0f, -1.0f, -1.0f, 1.0f);
	//// Set shader's name
	//SetShaderName("vsVtxNormal.glsl", "fsVtxNormal.glsl");

#ifdef LIGHTING_WITHCPU
	// Default Set shader's name
	SetShaderName("vsLighting_CPU.glsl", "fsLighting_CPU.glsl");
#else // lighting with GPU
#ifdef PERVERTEX_LIGHTING
	SetShaderName("vsLighting_GPU.glsl", "fsLighting_GPU.glsl");
#else
	SetShaderName("vsPerPixelLighting.glsl", "fsPerPixelLighting.glsl");
	//SetShaderName("vsVtxNormal.glsl","fsVtxNormal.glsl");
#endif
#endif  
	// Create and initialize a buffer object 
	CreateBufferObject();

	// 預設為 -1.0f, 由 Normal 決定顏色
	m_fColor[0] = -1.0f; m_fColor[1] = -1.0f; m_fColor[2] = -1.0f; m_fColor[3] = 1;

	// Sprite 的左下與右上角的 local 座標
	m_OriginalLf = vec4(-0.5f, -0.5f, 0.0f, 1.0f);
	m_OriginalTR = vec4( 0.5f, 0.5f, 0.0f, 1.0f);

	m_bPushed = false;

}

C2DSprite::~C2DSprite()
{
	
}
void C2DSprite::RenderWithFlatShading(vec4 vLightPos, color4 vLightI)
{
	// 以每一個面的三個頂點計算其重心，以該重心作為顏色計算的點頂
	// 根據 Phong lighting model 計算相對應的顏色，並將顏色儲存到此三個點頂
	// 因為每一個平面的頂點的 Normal 都相同，所以此處並沒有計算此三個頂點的平均 Normal

	vec4 vCentroidP;
	for( int i = 0 ; i < m_iNumVtx ; i += 3 ) {
		// 計算三角形的重心
		vCentroidP = (m_pPoints[i] + m_pPoints[i+1] + m_pPoints[i+2])/3.0f;
		//m_pColors[i] = m_pColors[i + 1] = m_pColors[i + 2] = PhongReflectionModel(vCentroidP, m_pNormals[i], vLightPos, vLightI);
	}
	glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx+sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors ); // vertcies' Color
}

void C2DSprite::RenderWithFlatShading(const LightSource &Lights)
{
	// 以每一個面的三個頂點計算其重心，以該重心作為顏色計算的點頂
	// 根據 Phong lighting model 計算相對應的顏色，並將顏色儲存到此三個點頂
	// 因為每一個平面的頂點的 Normal 都相同，所以此處並沒有計算此三個頂點的平均 Normal
	vec4 vCentroidP;
	for (int i = 0; i < m_iNumVtx; i += 3) {
		// 計算三角形的重心
		vCentroidP = (m_pPoints[i] + m_pPoints[i + 1] + m_pPoints[i + 2]) / 3.0f;
		//m_pColors[i] = m_pColors[i + 1] = m_pColors[i + 2] = PhongReflectionModel(vCentroidP, m_pNormals[i], Lights.position, Lights.diffuse);
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}

void C2DSprite::RenderWithGouraudShading(vec4 vLightPos, color4 vLightI)
{
	// 以 vertex 為單位，根據該點的位置與其法向量，利用 Phong lighting model 計算相對應的顏色
	// 將該顏色儲存回該頂點

	// 六個頂點構成的兩個三角形
	// 計算 0 1 2 5 四個頂點的顏色即可，0 與 3、2 與 4 的顏色相同
	/*m_pColors[0] = m_pColors[3] = PhongReflectionModel(m_pPoints[0], m_pNormals[0], vLightPos, vLightI);
	m_pColors[2] = m_pColors[4] = PhongReflectionModel(m_pPoints[2], m_pNormals[2], vLightPos, vLightI);
	m_pColors[1] = PhongReflectionModel(m_pPoints[1], m_pNormals[1], vLightPos, vLightI);
	m_pColors[5] = PhongReflectionModel(m_pPoints[5], m_pNormals[5], vLightPos, vLightI);*/

	// 如果要設定 Spot Light，在此處計算光源與被照明點的夾角，改變 vLightI 的內容，上面在呼叫傳入即可

	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}

void C2DSprite::RenderWithGouraudShading(const LightSource &Lights)
{
	// 以 vertex 為單位，根據該點的位置與其法向量，利用 Phong lighting model 計算相對應的顏色
	// 將該顏色儲存回該頂點

	// 六個頂點構成的兩個三角形
	// 計算 0 1 2 5 四個頂點的顏色即可，0 與 3、2 與 4 的顏色相同
	m_pColors[0] = m_pColors[3] = PhongReflectionModel(m_pPoints[0], m_pNormals[0], Lights);
	m_pColors[2] = m_pColors[4] = PhongReflectionModel(m_pPoints[2], m_pNormals[2], Lights);
	m_pColors[1] = PhongReflectionModel(m_pPoints[1], m_pNormals[1], Lights);
	m_pColors[5] = PhongReflectionModel(m_pPoints[5], m_pNormals[5], Lights);

	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
}

void C2DSprite::Update(float dt, point4 vLightPos, color4 vLightI)
{
#ifdef LIGHTING_WITHCPU
	if (m_bViewUpdated || m_bTRSUpdated) { // Model View 的相關矩陣內容有更動
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxMV3X3Final = mat3(	// 只取前面的 3X3 矩陣的內容
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
	m_vLightInView = m_mxView * vLightPos;		// 將 Light 轉換到鏡頭座標再傳入
	// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
	m_AmbientProduct = m_Material.ka * m_Material.ambient  * vLightI;
	m_DiffuseProduct = m_Material.kd * m_Material.diffuse  * vLightI;
	m_SpecularProduct = m_Material.ks * m_Material.specular * vLightI;
#endif

}

void C2DSprite::Update(float dt, const LightSource &Lights)
{
#ifdef LIGHTING_WITHCPU
	if (m_bViewUpdated || m_bTRSUpdated) { // Model View 的相關矩陣內容有更動
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
	m_vLightInView = m_mxView * Lights.position;		// 將 Light 轉換到鏡頭座標再傳入
	// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
	m_AmbientProduct = m_Material.ka * m_Material.ambient  * Lights.ambient;
	m_DiffuseProduct = m_Material.kd * m_Material.diffuse  * Lights.diffuse;
	m_SpecularProduct = m_Material.ks * m_Material.specular * Lights.specular;
#endif

}
void C2DSprite::Update(float dt)
{
	if (m_bViewUpdated || m_bTRSUpdated) { // Model View 的相關矩陣內容有更動
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_mxITMV = InverseTransposeMatrix(m_mxMVFinal);
		m_bViewUpdated = m_bTRSUpdated = false;
	}
}

void C2DSprite::SetTRSMatrix(mat4 &mat)
{
	m_mxTRS = mat;
	m_bTRSUpdated = true;
	// 計算 Sprite 在世界座標的左下角與右上角的位置
	m_Lf = m_mxTRS*m_OriginalLf;
	m_TR = m_mxTRS*m_OriginalTR;
}

// 傳入的點已經是對應到 (-1,-1) (1,1) 之間的 2D 座標
bool C2DSprite::OnTouches(const vec2 &tp)
{
	if (m_Lf.x <= tp.x && tp.x <= m_TR.x && m_Lf.y <= tp.y && tp.y <= m_TR.y) // 該按鈕被按到
	{
		if ( !m_bPushed ) {
			// 變成灰階
			SetColor(vec4(0.5, 0.5, 0.5, 1.0f));
			m_bPushed = true;
		}
		else {
			m_bPushed = false;
			SetColor(m_DefaultColor);
		}
		return true;		// 傳回有點到按鈕
	}
	else return false;	// 傳回沒有點到按鈕
}

void C2DSprite::SetDefaultColor(vec4 vColor)
{
	m_DefaultColor = vColor;
	SetColor(vColor);
}
//void C2DSprite::SetVtxColors(vec4 vLFColor, vec4 vLRColor, vec4 vTRColor, vec4 vTLColor)
//{
//	m_pColors[3] = m_pColors[0] = vLFColor;
//	m_pColors[1] = vLRColor;
//	m_pColors[4] = m_pColors[2] = vTRColor;
//	m_pColors[5] = vTLColor;
//
//	// 將所有 vertices 顏色更新到 VBO 中
//	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
//}

//void C2DSprite::SetVtxColors(vec4 vFColor, vec4 vSColor) // 設定兩個三角形的顏色
//{
//	m_pColors[0] = m_pColors[1] = m_pColors[2] = vFColor;
//	m_pColors[3] = m_pColors[4] = m_pColors[5] = vSColor;
//
//	// 將所有 vertices 顏色更新到 VBO 中
//	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
//	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, sizeof(vec4)*m_iNumVtx, m_pColors); // vertcies' Color
//}
void C2DSprite::Draw()
{
	DrawingSetShader();
	glDrawArrays(GL_TRIANGLES, 0, QUAD_NUM);
}

void C2DSprite::DrawW()
{
	DrawingWithoutSetShader();
	glDrawArrays(GL_TRIANGLES, 0, QUAD_NUM);
}