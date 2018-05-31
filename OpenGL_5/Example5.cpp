// 
// Cubic mapping (Environment Mapping)
// 
// 執行前的準備工作
// 關閉 CShape.h 中的 #define PERVERTEX_LIGHTING，使用 PERPIXEL_LIGHTING 才會有作用
// 設定 #define MULTITEXTURE  (DIFFUSE_MAP|LIGHT_MAP|NORMAL_MAP)
// 開啟 #define CUBIC_MAP 1

#include "header/Angel.h"
#include "Common/CQuad.h"
#include "Common/CSolidCube.h"
#include "Common/CSolidSphere.h"
#include "Common/CWireSphere.h"
#include "Common/CWireCube.h"
#include "Common/CChecker.h"
#include "Common/CCamera.h"
#include "Common/CTexturePool.h"
#include "png_loader.h"
#include "Common/C2DSprite.h" 
#include "Common/CObj.h"
#include "Common/CObj2.h"
#define SPACE_KEY 32
#define SCREEN_SIZE 800
#define HALF_SIZE SCREEN_SIZE /2 
#define VP_HALFWIDTH  20.0f
#define VP_HALFHEIGHT 20.0f
#define GRID_SIZE 20 // must be an even number


// For Model View and Projection Matrix
mat4 g_mxModelView(1.0f);
mat4 g_mxProjection;
CObj          *g_pObj;//monkey
CObj2         *g_pObj2;//car
// For Objects
CQuad		  *g_pFloor;
CSolidCube    *g_pCube;
CSolidSphere  *g_pSphere;
CSolidCube    *g_pCube2, *g_pCube3, *g_pCube4, *g_pCube5, *g_pCube6;
CSolidSphere  *g_pSphere2, *g_pSphere3, *g_pSphere4, *g_pSphere5, *g_pSphere6;
//------------------animal--------------
CQuad         *g_pAimal[3];
CQuad         *g_pDoor[12];
GLuint        g_uiAimalTexID[4];
GLuint        g_uiAimalNormalTexID[3];
//------------------------op4 room1 to room 6
CQuad         *g_LeftWall, *g_RightWall;
CQuad         *g_FrontWall, *g_BackWall;
CQuad         *g_CellingWall;
CQuad         *g_LeftWall2, *g_RightWall2, *g_FrontWall2, *g_BackWall2, *g_CellingWall2, *g_Floor2;
CQuad         *g_LeftWall3, *g_RightWall3, *g_FrontWall3, *g_BackWall3, *g_CellingWall3, *g_Floor3;
CQuad         *g_LeftWall4, *g_RightWall4, *g_FrontWall4, *g_BackWall4, *g_CellingWall4, *g_Floor4;
CQuad         *g_LeftWall5, *g_RightWall5, *g_FrontWall5, *g_BackWall5, *g_CellingWall5, *g_Floor5;
CQuad         *g_LeftWall6, *g_RightWall6, *g_FrontWall6, *g_BackWall6, *g_CellingWall6, *g_Floor6;
// For View Point
GLfloat g_fRadius = 8.0;
GLfloat g_fTheta = 45.0f*DegreesToRadians;
GLfloat g_fPhi = 45.0f*DegreesToRadians;

//----------------------------------------------------------------------------
// Part 2 : for single light source
bool g_bAutoRotating = false;
float g_fElapsedTime = 0;
float g_fLightRadius = 6;
float g_fLightTheta = 0;

float g_fLightR = 0.85f;
float g_fLightG = 0.85f;
float g_fLightB = 0.85f;

LightSource g_Light1 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(6.0f, 9.0f, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};
//-----------------------room 2 to room6
LightSource g_LightRoom2 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(15.0f, g_fLightRadius, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  //spotTarget
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	1.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource g_LightRoom3 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(40.0f, g_fLightRadius, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  //spotTarget
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	1.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};
LightSource g_LightRoom4 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(40.0f, g_fLightRadius, 20.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  //spotTarget
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	1.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

LightSource g_LightRoom5 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(15.0f, g_fLightRadius, 20.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  //spotTarget
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	1.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};
LightSource g_LightRoom6 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(5.0f, g_fLightRadius, 25.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  //spotTarget
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	1.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};
CWireSphere *g_pLight; 
CWireSphere *g_pLightR2, *g_pLightR3, *g_pLightR4, *g_pLightR5, *g_pLightR6;//room2 to room6
//----------------------------------------------------------------------------
//2D-------------------------
C2DSprite *g_p2DBtn[4];
mat4  g_2DView;
mat4  g_2DProj;
// Texture 
GLuint g_uiFTexID[20]; // 三個物件分別給不同的貼圖
int g_iTexWidth,  g_iTexHeight;
GLuint g_uiSphereCubeMap; // for Cubic Texture
//新增2個全域變數(代表相機所在位置):
float g_camerax = 0.0f;
float g_cameray = 0.0f;
//----------------------------------------------------------------------------
// 函式的原型宣告
extern void IdleProcess();

void init( void )
{
	
	mat4 mxT, mxS;
	vec4 vT, vColor;
	// 產生所需之 Model View 與 Projection Matrix
	// 產生所需之 Model View 與 Projection Matrix
	point4  eye(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);
	point4  at(0.0f, 0.0f, 0.0f, 1.0f);
	auto camera = CCamera::create();
	camera->updateViewLookAt(eye, at);
	camera->updatePerspective(60.0, (GLfloat)SCREEN_SIZE / (GLfloat)SCREEN_SIZE, 1.0, 1000.0);

	auto texturepool = CTexturePool::create();
	g_uiFTexID[0] = texturepool->AddTexture("texture/checker.png");
	g_uiFTexID[1] = texturepool->AddTexture("texture/Masonry.Brick.png");
	g_uiFTexID[3] = texturepool->AddTexture("texture/metal.png");
	g_uiFTexID[4] = texturepool->AddTexture("texture/Masonry.Brick.normal.png");
	g_uiFTexID[5] = texturepool->AddTexture("texture/gold.png");
	g_uiFTexID[6] = texturepool->AddTexture("texture/earth.png");
	g_uiFTexID[7] = texturepool->AddTexture("texture/floor.png");
	g_uiFTexID[8] = texturepool->AddTexture("texture/old_stone_wall.png");
	g_uiFTexID[9] = texturepool->AddTexture("texture/NormalMapstone.png");

	g_uiFTexID[10] = texturepool->AddTexture("texture/wall.png");
	g_uiFTexID[11] = texturepool->AddTexture("texture/NormalMapwall.png");
#ifdef MULTITEXTURE
	g_uiFTexID[2] = texturepool->AddTexture("texture/lightMap1.png");
#endif
	//animal png--------------------
	g_uiAimalTexID[0] = texturepool->AddTexture("texture/wi_tata.png");
	g_uiAimalNormalTexID[0] = texturepool->AddTexture("texture/wi_tata_normal.png");
	//door
	g_uiAimalTexID[1] = texturepool->AddTexture("texture/door.png");
	g_uiAimalNormalTexID[1] = texturepool->AddTexture("texture/NormalMapdoor.png");
	//-----------------------
	g_uiSphereCubeMap = CubeMap_load_SOIL();
	//2D---------------------------------------
	
	mxS = Scale(0.1f, 0.1f, 1.0f);
	g_p2DBtn[3] = new C2DSprite; g_p2DBtn[3]->SetShader();
	vColor.x = 1; vColor.y = 1; vColor.z = 1; g_p2DBtn[3]->SetDefaultColor(vColor);
	mxT = Translate(0.90f, 0.90f, 0);
	g_p2DBtn[3]->SetTRSMatrix(mxT*mxS);
	g_p2DBtn[3]->SetViewMatrix(g_2DView);
	g_p2DBtn[3]->SetViewMatrix(g_2DProj); g_p2DBtn[3]->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));

//Animal 1----------------------------------
	// For g_pAimal1
	g_pAimal[0] = new CQuad;

#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pAimal[0]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pAimal[0]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pAimal[0]->SetShader();
	mxT = Translate(5.0f, 3.0f, -2.0f);
	g_pAimal[0]->SetTRSMatrix(mxT* RotateX(90)*Scale(3.0f, 3.0f, 3.0f));
	g_pAimal[0]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pAimal[0]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pAimal[0]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//door 1----------------------------------
	// For door 1
	g_pDoor[0] = new CQuad;

#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pDoor[0]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pDoor[0]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pDoor[0]->SetShader();
	mxT = Translate(9.7f, 3.0f, 0.0f);
	g_pDoor[0]->SetTRSMatrix(mxT* RotateY(-90)*RotateX(90)*Scale(5.0f, 5.0f, 5.0f));
	g_pDoor[0]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pDoor[0]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[0]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	// =============For door 2
	g_pDoor[1] = new CQuad;

#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pDoor[1]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pDoor[1]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pDoor[1]->SetShader();
	mxT = Translate(11.0f, 3.0f, 0.0f);
	g_pDoor[1]->SetTRSMatrix(mxT* RotateY(90)*RotateX(90)*Scale(5.0f, 5.0f, 5.0f));
	g_pDoor[1]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pDoor[1]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[1]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	// ================For door 3
	g_pDoor[2] = new CQuad;

#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pDoor[2]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pDoor[2]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pDoor[2]->SetShader();
	mxT = Translate(29.5f, 3.0f, 5.0f);
	g_pDoor[2]->SetTRSMatrix(mxT* RotateY(-90)*RotateX(90)*Scale(5.0f, 5.0f, 5.0f));
	g_pDoor[2]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pDoor[2]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[2]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	// For door 4
	g_pDoor[3] = new CQuad;

#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pDoor[3]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pDoor[3]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pDoor[3]->SetShader();
	mxT = Translate(31.0f, 3.0f, 5.0f);
	g_pDoor[3]->SetTRSMatrix(mxT* RotateY(90)*RotateX(90)*Scale(5.0f, 5.0f, 5.0f));
	g_pDoor[3]->SetShadingMode(GOURAUD_SHADING);
	// For door 5
	g_pDoor[4] = new CQuad;

#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pDoor[4]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pDoor[4]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pDoor[4]->SetShader();
	mxT = Translate(45.0f, 3.0f, 9.7f);
	g_pDoor[4]->SetTRSMatrix(mxT* RotateY(180)*RotateX(90)*Scale(5.0f, 5.0f, 5.0f));
	g_pDoor[4]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pDoor[4]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[4]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	// 設定貼圖
	g_pDoor[4]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[4]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	// For door 6
	g_pDoor[5] = new CQuad;

#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pDoor[5]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pDoor[5]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pDoor[5]->SetShader();
	mxT = Translate(45.0f, 3.0f, 10.5f);
	g_pDoor[5]->SetTRSMatrix(mxT* RotateY(0)*RotateX(90)*Scale(5.0f, 5.0f, 5.0f));
	g_pDoor[5]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pDoor[5]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[5]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	// For door 7
	g_pDoor[6] = new CQuad;

#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pDoor[6]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pDoor[6]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pDoor[6]->SetShader();
	mxT = Translate(30.7f, 3.0f, 22.0f);
	g_pDoor[6]->SetTRSMatrix(mxT* RotateY(90)*RotateX(90)*Scale(5.0f, 5.0f, 5.0f));
	g_pDoor[6]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pDoor[6]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[6]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	// For door 8
	g_pDoor[7] = new CQuad;

#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pDoor[7]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pDoor[7]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pDoor[7]->SetShader();
	mxT = Translate(29.2f, 3.0f, 22.0f);
	g_pDoor[7]->SetTRSMatrix(mxT* RotateY(-90)*RotateX(90)*Scale(5.0f, 5.0f, 5.0f));
	g_pDoor[7]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pDoor[7]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[7]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	// For door 9
	g_pDoor[8] = new CQuad;

#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pDoor[8]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pDoor[8]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pDoor[8]->SetShader();
	mxT = Translate(10.5f, 3.0f, 27.0f);
	g_pDoor[8]->SetTRSMatrix(mxT* RotateY(90)*RotateX(90)*Scale(5.0f, 5.0f, 5.0f));
	g_pDoor[8]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pDoor[8]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[8]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	// For door 10
	g_pDoor[9] = new CQuad;

#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pDoor[9]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pDoor[9]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pDoor[9]->SetShader();
	mxT = Translate(9.7f, 3.0f, 27.0f);
	g_pDoor[9]->SetTRSMatrix(mxT* RotateY(-90)*RotateX(90)*Scale(5.0f, 5.0f, 5.0f));
	g_pDoor[9]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pDoor[9]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[9]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	// For door 11
	g_pDoor[10] = new CQuad;

#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pDoor[10]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pDoor[10]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pDoor[10]->SetShader();
	mxT = Translate(0.0f, 3.0f, 11.0f);
	g_pDoor[10]->SetTRSMatrix(mxT* RotateY(0)*RotateX(90)*Scale(5.0f, 5.0f, 5.0f));
	g_pDoor[10]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pDoor[10]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[10]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	// For door 12
	g_pDoor[11] = new CQuad;

#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pDoor[11]->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pDoor[11]->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif

	g_pDoor[11]->SetShader();
	mxT = Translate(0.0f, 3.0f, 9.7f);
	g_pDoor[11]->SetTRSMatrix(mxT* RotateY(180)*RotateX(90)*Scale(5.5f, 5.5f, 5.5f));
	g_pDoor[11]->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pDoor[11]->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pDoor[11]->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	//--------------------obj reader---------------------------
	g_pObj = new CObj("monkey.obj");//replace porsche.obj with radar.obj or any other .obj to display it
	// Part 3 : materials
#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pObj->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pObj->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_pObj->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pObj->SetKaKdKsShini(0.15f, 0.8f, 0.2f, 50);

	g_pObj->SetShader();
	vT.x = 20.0f; vT.y = 2.0f; vT.z = 0.0f;   //monkey 位移中心
	mxT = Translate(vT);
	//mxS = Scale(0.1, 0.1, 0.1);//car
	//mxS = Scale(1, 1, 1);//monkey
	mxS = Scale(1, 1, 1);//
	g_pObj->SetTRSMatrix(mxT*mxS);
	g_pObj->SetShadingMode(GOURAUD_SHADING);

	//--------------------obj2 reader---------------------------
	g_pObj2 = new CObj2("car.obj");//replace porsche.obj with radar.obj or any other .obj to display it
	// Part 3 : materials
#if MULTITEXTURE >= (DIFFUSE_MAP | NORMAL_MAP)
	g_pObj2->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pObj2->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_pObj2->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pObj2->SetKaKdKsShini(0.15f, 0.8f, 0.2f, 50);

	g_pObj2->SetShader();
	vT.x = 2.0f; vT.y = 2.0f; vT.z = 21.5f;   //monkey 位移中心
	mxT = Translate(vT);
	mxS = Scale(0.1, 0.1, 0.1);//car
	//mxS = Scale(1, 1, 1);//monkey
	//mxS = Scale(1, 1, 1);//
	g_pObj2->SetTRSMatrix(mxT*mxS);
	g_pObj2->SetShadingMode(GOURAUD_SHADING);
// Room1--------------------------------
	g_pFloor = new CQuad;
#ifdef MULTITEXTURE
	g_pFloor->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
#endif
	g_pFloor->SetShader();
	g_pFloor->SetTRSMatrix(Scale(20,1,20));
	g_pFloor->SetShadingMode(GOURAUD_SHADING);
	g_pFloor->SetTiling(40,40);
	// 設定貼圖
	g_pFloor->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pFloor->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = -10.0f; vT.y = 10.0f; vT.z = 0;
	mxT = Translate(vT);
	g_LeftWall = new CQuad;
#ifdef MULTITEXTURE
	g_LeftWall->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
#endif
	g_LeftWall->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_LeftWall->SetShadingMode(GOURAUD_SHADING);
	g_LeftWall->SetShader();
	g_LeftWall->SetColor(vec4(0.6f));
	g_LeftWall->SetTRSMatrix(mxT*RotateZ(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_LeftWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 9.9f; vT.y = 10.0f; vT.z = 0;
	mxT = Translate(vT);
	g_RightWall = new CQuad;
#ifdef MULTITEXTURE
	//g_RightWall->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
	g_RightWall->SetTextureLayer(DIFFUSE_MAP);
#endif
	g_RightWall->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 1.0f, 1, 0.5f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_RightWall->SetShadingMode(GOURAUD_SHADING);
	g_RightWall->SetShader();
	//g_RightWall->SetColor(vec4(0.6f));
	g_RightWall->SetTRSMatrix(mxT*RotateZ(90.0f)*Scale(20.0f, 1, 20.0f));
	g_RightWall->SetKaKdKsShini(0, 0.8f, 0.5f, 50);

	vT.x = 0.0f; vT.y = 10.0f; vT.z = 10.0f;
	mxT = Translate(vT);
	g_FrontWall = new CQuad;
#ifdef MULTITEXTURE
	g_FrontWall->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
#endif
	g_FrontWall->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_FrontWall->SetShadingMode(GOURAUD_SHADING);
	g_FrontWall->SetShader();
	g_FrontWall->SetColor(vec4(0.6f));
	g_FrontWall->SetTRSMatrix(mxT*RotateX(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_FrontWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 0.0f; vT.y = 10.0f; vT.z = -10.0f;
	mxT = Translate(vT);
	g_BackWall = new CQuad;
#ifdef MULTITEXTURE
	g_BackWall->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
#endif
	g_BackWall->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BackWall->SetShadingMode(GOURAUD_SHADING);
	g_BackWall->SetShader();
	g_BackWall->SetColor(vec4(0.6f));
	g_BackWall->SetTRSMatrix(mxT*RotateX(90.0f)*Scale(20.0f, 1, 20.0f));
	g_BackWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 0.0f; vT.y = 20.0f; vT.z = 0.0f;//天花板
	mxT = Translate(vT);
	g_CellingWall = new CQuad;
#ifdef MULTITEXTURE
	g_CellingWall->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
#endif
	g_CellingWall->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_CellingWall->SetShadingMode(GOURAUD_SHADING);
	g_CellingWall->SetShader();
	g_CellingWall->SetColor(vec4(0.5f));
	g_CellingWall->SetTRSMatrix(mxT*RotateX(180.0f)*Scale(20.0f, 1, 20.0f));
	g_CellingWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//----------------Room2___________________________________
	vT.x = 10.0f; vT.y = 10.0f; vT.z = 0.0f;
	mxT = Translate(vT);
	g_LeftWall2 = new CQuad;
#ifdef MULTITEXTURE
	//g_LeftWall2->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
	g_LeftWall2->SetTextureLayer(DIFFUSE_MAP);
#endif
	g_LeftWall2->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 1.0f, 1, 0.5), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_LeftWall2->SetShadingMode(GOURAUD_SHADING);
	g_LeftWall2->SetShader();
	g_LeftWall2->SetColor(vec4(1.0f));
	g_LeftWall2->SetTRSMatrix(mxT*RotateZ(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_LeftWall2->SetKaKdKsShini(0, 0.8f, 0.5f, 50);

	vT.x = 30.0f; vT.y = 10.0f; vT.z = 0;
	mxT = Translate(vT);
	g_RightWall2 = new CQuad;
#ifdef MULTITEXTURE
	g_RightWall2->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP | NORMAL_MAP);//three png
#endif
	g_RightWall2->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 0.0f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_RightWall2->SetShadingMode(GOURAUD_SHADING);
	g_RightWall2->SetShader();
	g_RightWall2->SetColor(vec4(0.6f));
	g_RightWall2->SetTRSMatrix(mxT*RotateZ(90.0f)*Scale(20.0f, 1, 20.0f));
	g_RightWall2->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 20.0f; vT.y = 10.0f; vT.z = 10.0f;
	mxT = Translate(vT);
	g_FrontWall2 = new CQuad;
#ifdef MULTITEXTURE
	//g_FrontWall2->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
	g_FrontWall2->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_FrontWall2->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_FrontWall2->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 0.0f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_FrontWall2->SetShadingMode(GOURAUD_SHADING);
	g_FrontWall2->SetShader();
	g_FrontWall2->SetColor(vec4(0.6f));
	g_FrontWall2->SetTRSMatrix(mxT*RotateX(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_FrontWall2->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 20.0f; vT.y = 10.0f; vT.z = -10.0f;
	mxT = Translate(vT);
	g_BackWall2 = new CQuad;
#ifdef MULTITEXTURE
	g_BackWall2->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP );
	//g_BackWall2->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_BackWall2->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 0.0f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BackWall2->SetShadingMode(GOURAUD_SHADING);
	g_BackWall2->SetShader();
	g_BackWall2->SetColor(vec4(0.6f));
	g_BackWall2->SetTRSMatrix(mxT*RotateX(90.0f)*Scale(20.0f, 1, 20.0f));
	g_BackWall2->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 20.0f; vT.y = 20.0f; vT.z = 0.0f;//天花板
	mxT = Translate(vT);
	g_CellingWall2 = new CQuad;
#ifdef MULTITEXTURE
	//g_CellingWall2->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP);
	g_CellingWall2->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_CellingWall2->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_CellingWall2->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 0.0f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_CellingWall2->SetShadingMode(GOURAUD_SHADING);
	g_CellingWall2->SetShader();
	g_CellingWall2->SetColor(vec4(0.5f));
	g_CellingWall2->SetTRSMatrix(mxT*RotateX(180.0f)*Scale(20.0f, 1, 20.0f));
	g_CellingWall2->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 20.0f; vT.y = 0.0f; vT.z = 0.0f;
	mxT = Translate(vT);
	g_Floor2 = new CQuad;
#ifdef MULTITEXTURE
	//g_Floor2->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
	g_Floor2->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_Floor2->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_Floor2->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 0.0f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_Floor2->SetShadingMode(GOURAUD_SHADING);
	g_Floor2->SetShader();
	g_Floor2->SetColor(vec4(0.5f));
	g_Floor2->SetTRSMatrix(mxT*Scale(20.0f, 1, 20.0f));
	g_Floor2->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_Floor2->SetTiling(40, 40);//--------------light map

	//Room3___________________________________
	vT.x = 30.0f; vT.y = 10.0f; vT.z = 0.0f;
	mxT = Translate(vT);
	g_LeftWall3 = new CQuad;
#ifdef MULTITEXTURE
	//g_LeftWall3->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
	g_LeftWall3->SetTextureLayer(DIFFUSE_MAP); // 有兩張貼圖
#endif
	g_LeftWall3->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.0f, 0.85f, 0.5), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_LeftWall3->SetShadingMode(GOURAUD_SHADING);
	g_LeftWall3->SetShader();
	g_LeftWall3->SetColor(vec4(1.0f));
	g_LeftWall3->SetTRSMatrix(mxT*RotateZ(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_LeftWall3->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 50.0f; vT.y = 10.0f; vT.z = 0;
	mxT = Translate(vT);
	g_RightWall3 = new CQuad;
#ifdef MULTITEXTURE
	g_RightWall3->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
#endif
	g_RightWall3->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.0f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_RightWall3->SetShadingMode(GOURAUD_SHADING);
	g_RightWall3->SetShader();
	g_RightWall3->SetColor(vec4(0.6f));
	g_RightWall3->SetTRSMatrix(mxT*RotateZ(90.0f)*Scale(20.0f, 1, 20.0f));
	g_RightWall3->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 40.0f; vT.y = 10.0f; vT.z = 10.0f;
	mxT = Translate(vT);
	g_FrontWall3 = new CQuad;
#ifdef MULTITEXTURE
	g_FrontWall3->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
#endif
	g_FrontWall3->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.0f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_FrontWall3->SetShadingMode(GOURAUD_SHADING);
	g_FrontWall3->SetShader();
	g_FrontWall3->SetColor(vec4(0.6f));
	g_FrontWall3->SetTRSMatrix(mxT*RotateX(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_FrontWall3->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 40.0f; vT.y = 10.0f; vT.z = -10.0f;
	mxT = Translate(vT);
	g_BackWall3 = new CQuad;
#ifdef MULTITEXTURE
	g_BackWall3->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
#endif
	g_BackWall3->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.0f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BackWall3->SetShadingMode(GOURAUD_SHADING);
	g_BackWall3->SetShader();
	g_BackWall3->SetColor(vec4(0.6f));
	g_BackWall3->SetTRSMatrix(mxT*RotateX(90.0f)*Scale(20.0f, 1, 20.0f));
	g_BackWall3->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 40.0f; vT.y = 20.0f; vT.z = 0.0f;//天花板
	mxT = Translate(vT);
	g_CellingWall3 = new CQuad;
#ifdef MULTITEXTURE
	g_CellingWall3->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
#endif
	g_CellingWall3->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.0f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_CellingWall3->SetShadingMode(GOURAUD_SHADING);
	g_CellingWall3->SetShader();
	g_CellingWall3->SetColor(vec4(0.5f));
	g_CellingWall3->SetTRSMatrix(mxT*RotateX(180.0f)*Scale(20.0f, 1, 20.0f));
	g_CellingWall3->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 40.0f; vT.y = 0.0f; vT.z = 0.0f;
	mxT = Translate(vT);
	g_Floor3 = new CQuad;
#ifdef MULTITEXTURE
	g_Floor3->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
#endif
	g_Floor3->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.0f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_Floor3->SetShadingMode(GOURAUD_SHADING);
	g_Floor3->SetShader();
	g_Floor3->SetColor(vec4(0.5f));
	g_Floor3->SetTRSMatrix(mxT*Scale(20.0f, 1, 20.0f));
	g_Floor3->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	//Room4___________________________________
	vT.x = 30.0f; vT.y = 10.0f; vT.z = 20.0f;
	mxT = Translate(vT);
	g_LeftWall4 = new CQuad;
#ifdef MULTITEXTURE
	//g_LeftWall4->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
	g_LeftWall4->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_LeftWall4->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_LeftWall4->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 1.0f, 0, 0.8), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_LeftWall4->SetShadingMode(GOURAUD_SHADING);
	g_LeftWall4->SetShader();
	g_LeftWall4->SetColor(vec4(0.8f));
	g_LeftWall4->SetTRSMatrix(mxT*RotateZ(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_LeftWall4->SetKaKdKsShini(0, 0.8f, 0.5f, 60);

	vT.x = 50.0f; vT.y = 10.0f; vT.z = 20.0f;
	mxT = Translate(vT);
	g_RightWall4 = new CQuad;
#ifdef MULTITEXTURE
	//g_RightWall4->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
	g_RightWall4->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_RightWall4->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_RightWall4->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 1.0f, 0, 0.8), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_RightWall4->SetShadingMode(GOURAUD_SHADING);
	g_RightWall4->SetShader();
	g_RightWall4->SetColor(vec4(0.6f));
	g_RightWall4->SetTRSMatrix(mxT*RotateZ(90.0f)*Scale(20.0f, 1, 20.0f));
	g_RightWall4->SetKaKdKsShini(0, 0.8f, 0.5f, 60);

	vT.x = 40.0f; vT.y = 10.0f; vT.z = 30.0f;
	mxT = Translate(vT);
	g_FrontWall4 = new CQuad;
#ifdef MULTITEXTURE
	//g_FrontWall4->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
	g_FrontWall4->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_FrontWall4->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_FrontWall4->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 1.0f, 0, 0.8), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_FrontWall4->SetShadingMode(GOURAUD_SHADING);
	g_FrontWall4->SetShader();
	g_FrontWall4->SetColor(vec4(0.6f));
	g_FrontWall4->SetTRSMatrix(mxT*RotateX(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_FrontWall4->SetKaKdKsShini(0, 0.8f, 0.5f, 60);

	vT.x = 40.0f; vT.y = 10.0f; vT.z = 10.0f;
	mxT = Translate(vT);
	g_BackWall4 = new CQuad;
#ifdef MULTITEXTURE
	//g_BackWall4->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
	g_BackWall4->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_BackWall4->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_BackWall4->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 1.0f, 0, 0.8), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BackWall4->SetShadingMode(GOURAUD_SHADING);
	g_BackWall4->SetShader();
	g_BackWall4->SetColor(vec4(0.6f));
	g_BackWall4->SetTRSMatrix(mxT*RotateX(90.0f)*Scale(20.0f, 1, 20.0f));
	g_BackWall4->SetKaKdKsShini(0, 0.8f, 0.5f, 60);

	vT.x = 40.0f; vT.y = 20.0f; vT.z = 20.0f;//天花板
	mxT = Translate(vT);
	g_CellingWall4 = new CQuad;
#ifdef MULTITEXTURE
	//g_CellingWall4->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
	g_CellingWall4->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_CellingWall4->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_CellingWall4->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 1.0f, 0, 0.8), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_CellingWall4->SetShadingMode(GOURAUD_SHADING);
	g_CellingWall4->SetShader();
	g_CellingWall4->SetColor(vec4(0.5f));
	g_CellingWall4->SetTRSMatrix(mxT*RotateX(180.0f)*Scale(20.0f, 1, 20.0f));
	g_CellingWall4->SetKaKdKsShini(0, 0.8f, 0.5f, 60);

	vT.x = 40.0f; vT.y = 0.0f; vT.z = 20.0f;
	mxT = Translate(vT);
	g_Floor4 = new CQuad;
#ifdef MULTITEXTURE
	//g_Floor4->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
	g_Floor4->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_Floor4->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_Floor4->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 1.0f, 0, 0.8), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_Floor4->SetShadingMode(GOURAUD_SHADING);
	g_Floor4->SetShader();
	g_Floor4->SetColor(vec4(0.5f));
	g_Floor4->SetTRSMatrix(mxT*Scale(20.0f, 1, 20.0f));
	g_Floor4->SetKaKdKsShini(0, 0.8f, 0.5f, 60);


	//Room5___________________________________
	vT.x = 10.0f; vT.y = 10.0f; vT.z = 20.0f;
	mxT = Translate(vT);
	g_LeftWall5 = new CQuad;
#ifdef MULTITEXTURE
	//g_LeftWall5->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
	g_LeftWall5->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_LeftWall5->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_LeftWall5->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 1.0f, 1, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_LeftWall5->SetShadingMode(GOURAUD_SHADING);
	g_LeftWall5->SetShader();
	g_LeftWall5->SetColor(vec4(0.8f));
	g_LeftWall5->SetTRSMatrix(mxT*RotateZ(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_LeftWall5->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 30.0f; vT.y = 10.0f; vT.z = 20.0f;
	mxT = Translate(vT);
	g_RightWall5 = new CQuad;
#ifdef MULTITEXTURE
	//g_RightWall5->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
	g_RightWall5->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_RightWall5->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_RightWall5->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 1.0f, 1, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_RightWall5->SetShadingMode(GOURAUD_SHADING);
	g_RightWall5->SetShader();
	g_RightWall5->SetColor(vec4(0.6f));
	g_RightWall5->SetTRSMatrix(mxT*RotateZ(90.0f)*Scale(20.0f, 1, 20.0f));
	g_RightWall5->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 20.0f; vT.y = 10.0f; vT.z = 30.0f;
	mxT = Translate(vT);
	g_FrontWall5 = new CQuad;
#ifdef MULTITEXTURE
	//g_FrontWall5->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
	g_FrontWall5->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_FrontWall5->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_FrontWall5->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 1.0f, 1, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_FrontWall5->SetShadingMode(GOURAUD_SHADING);
	g_FrontWall5->SetShader();
	g_FrontWall5->SetColor(vec4(0.5f));
	g_FrontWall5->SetTRSMatrix(mxT*RotateX(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_FrontWall5->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 20.0f; vT.y = 10.0f; vT.z = 10.0f;
	mxT = Translate(vT);
	g_BackWall5 = new CQuad;
#ifdef MULTITEXTURE
	//g_BackWall5->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
	g_BackWall5->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_BackWall5->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_BackWall5->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 1.0f, 1, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BackWall5->SetShadingMode(GOURAUD_SHADING);
	g_BackWall5->SetShader();
	g_BackWall5->SetColor(vec4(0.5f));
	g_BackWall5->SetTRSMatrix(mxT*RotateX(90.0f)*Scale(20.0f, 1, 20.0f));
	g_BackWall5->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 20.0f; vT.y = 20.0f; vT.z = 20.0f;//天花板
	mxT = Translate(vT);
	g_CellingWall5 = new CQuad;
#ifdef MULTITEXTURE
	//g_CellingWall5->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
	g_CellingWall5->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_CellingWall5->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_CellingWall5->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 1.0f, 1, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_CellingWall5->SetShadingMode(GOURAUD_SHADING);
	g_CellingWall5->SetShader();
	g_CellingWall5->SetColor(vec4(0.5f));
	g_CellingWall5->SetTRSMatrix(mxT*RotateX(180.0f)*Scale(20.0f, 1, 20.0f));
	g_CellingWall5->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 20.0f; vT.y = 0.0f; vT.z = 20.0f;
	mxT = Translate(vT);
	g_Floor5 = new CQuad;
#ifdef MULTITEXTURE
	//g_Floor5->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
	g_Floor5->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_Floor5->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
#endif
	g_Floor5->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 1.0f, 1, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_Floor5->SetShadingMode(GOURAUD_SHADING);
	g_Floor5->SetShader();
	g_Floor5->SetColor(vec4(0.5f));
	g_Floor5->SetTRSMatrix(mxT*Scale(20.0f, 1, 20.0f));
	g_Floor5->SetKaKdKsShini(0, 0.8f, 0.5f, 1);


	//Room6___________________________________
	vT.x = -10.0f; vT.y = 10.0f; vT.z = 20.0f;
	mxT = Translate(vT);
	g_LeftWall6 = new CQuad;
#ifdef MULTITEXTURE
	g_LeftWall6->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
#endif
	g_LeftWall6->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 0.0f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_LeftWall6->SetShadingMode(GOURAUD_SHADING);
	g_LeftWall6->SetShader();
	g_LeftWall6->SetColor(vec4(0.8f));
	g_LeftWall6->SetTRSMatrix(mxT*RotateZ(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_LeftWall6->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 10.0f; vT.y = 10.0f; vT.z = 20;
	mxT = Translate(vT);
	g_RightWall6 = new CQuad;
#ifdef MULTITEXTURE
	g_RightWall6->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
#endif
	g_RightWall6->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 0.0f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_RightWall6->SetShadingMode(GOURAUD_SHADING);
	g_RightWall6->SetShader();
	g_RightWall6->SetColor(vec4(0.6f));
	g_RightWall6->SetTRSMatrix(mxT*RotateZ(90.0f)*Scale(20.0f, 1, 20.0f));
	g_RightWall6->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 0.0f; vT.y = 10.0f; vT.z = 30.0f;
	mxT = Translate(vT);
	g_FrontWall6 = new CQuad;
#ifdef MULTITEXTURE
	g_FrontWall6->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
#endif
	g_FrontWall6->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 0.0f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_FrontWall6->SetShadingMode(GOURAUD_SHADING);
	g_FrontWall6->SetShader();
	g_FrontWall6->SetColor(vec4(0.6f));
	g_FrontWall6->SetTRSMatrix(mxT*RotateX(-90.0f)*Scale(20.0f, 1, 20.0f));
	g_FrontWall6->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 0.0f; vT.y = 10.0f; vT.z = 10.0f;
	mxT = Translate(vT);
	g_BackWall6 = new CQuad;
#ifdef MULTITEXTURE
	//g_BackWall6->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
	g_BackWall6->SetTextureLayer(DIFFUSE_MAP ); // 有1張貼圖
#endif
	g_BackWall6->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 1.0f, 1, 0.5), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_BackWall6->SetShadingMode(GOURAUD_SHADING);
	g_BackWall6->SetShader();
	g_BackWall6->SetColor(vec4(1.0f));
	g_BackWall6->SetTRSMatrix(mxT*RotateX(90.0f)*Scale(20.0f, 1, 20.0f));
	g_BackWall6->SetKaKdKsShini(0, 0.8f, 0.5f, 50);

	vT.x = 0.0f; vT.y = 20.0f; vT.z = 20.0f;//天花板
	mxT = Translate(vT);
	g_CellingWall6 = new CQuad;
#ifdef MULTITEXTURE
	g_CellingWall6->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
#endif
	g_CellingWall6->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 0.0f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_CellingWall6->SetShadingMode(GOURAUD_SHADING);
	g_CellingWall6->SetShader();
	g_CellingWall6->SetColor(vec4(0.5f));
	g_CellingWall6->SetTRSMatrix(mxT*RotateX(180.0f)*Scale(20.0f, 1, 20.0f));
	g_CellingWall6->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	vT.x = 0.0f; vT.y = 0.0f; vT.z = 20.0f;
	mxT = Translate(vT);
	g_Floor6 = new CQuad;
#ifdef MULTITEXTURE
	g_Floor6->SetTextureLayer(DIFFUSE_MAP | LIGHT_MAP); // 有兩張貼圖
#endif
	g_Floor6->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(1, 0.0f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_Floor6->SetShadingMode(GOURAUD_SHADING);
	g_Floor6->SetShader();
	g_Floor6->SetColor(vec4(0.5f));
	g_Floor6->SetTRSMatrix(mxT*Scale(20.0f, 1, 20.0f));
	g_Floor6->SetKaKdKsShini(0, 0.8f, 0.5f, 1);


	g_pCube = new CSolidCube;
	g_pCube->SetTextureLayer( DIFFUSE_MAP | NORMAL_MAP);
	g_pCube->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_pCube->SetShader();
	// 設定 Cube
	vT.x = 4.0f; vT.y = 1.0f; vT.z = -0.5f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 2.0f; mxT._m[2][2] = 2.0f;
	g_pCube->SetTRSMatrix(mxT );
	g_pCube->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pCube->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pCube->SetKaKdKsShini(0.25f, 0.8f, 0.2f, 2);
	//================cube2==============
	g_pCube2 = new CSolidCube;
	g_pCube2->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pCube2->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_pCube2->SetShader();
	// 設定 Cube
	vT.x = 22.0f; vT.y = 1.0f; vT.z = 2.5f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 2.0f; mxT._m[2][2] = 2.0f;
	g_pCube2->SetTRSMatrix(mxT);
	g_pCube2->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pCube2->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pCube2->SetKaKdKsShini(0.25f, 0.8f, 0.2f, 200);

	//================cube3==============
	g_pCube3 = new CSolidCube;
	g_pCube3->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pCube3->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_pCube3->SetShader();
	// 設定 Cube
	vT.x = 42.0f; vT.y = 1.0f; vT.z = -6.5f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 2.0f; mxT._m[2][2] = 2.0f;
	g_pCube3->SetTRSMatrix(mxT);
	g_pCube3->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pCube3->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pCube3->SetKaKdKsShini(0.25f, 0.8f, 0.2f, 200);
	//================cube4==============
	g_pCube4 = new CSolidCube;
	g_pCube4->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pCube4->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_pCube4->SetShader();
	// 設定 Cube
	vT.x = 32.0f; vT.y = 1.0f; vT.z = 15.5f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 2.0f; mxT._m[2][2] = 2.0f;
	g_pCube4->SetTRSMatrix(mxT);
	g_pCube4->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pCube4->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pCube4->SetKaKdKsShini(0.25f, 0.8f, 0.2f, 200);
	//================cube5==============
	g_pCube5 = new CSolidCube;
	g_pCube5->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pCube5->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_pCube5->SetShader();
	// 設定 Cube
	vT.x = 27.0f; vT.y = 2.0f; vT.z = 25.0f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 2.0f; mxT._m[2][2] = 2.0f;
	g_pCube5->SetTRSMatrix(mxT);
	g_pCube5->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pCube5->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(0.85f, 0.85f, 0.85f, 0.7), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pCube5->SetKaKdKsShini(0.25f, 0.8f, 0.2f, 150);
	//================cube6==============
	g_pCube6 = new CSolidCube;
	g_pCube6->SetTextureLayer(DIFFUSE_MAP | NORMAL_MAP);
	g_pCube6->SetShaderName("vsNormalMapLighting.glsl", "fsNormalMapLighting.glsl");
	g_pCube6->SetShader();
	// 設定 Cube
	vT.x = 8.0f; vT.y = 3.0f; vT.z = 17.5f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 2.0f; mxT._m[2][2] = 2.0f;
	g_pCube6->SetTRSMatrix(mxT);
	g_pCube6->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pCube6->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pCube6->SetKaKdKsShini(0.25f, 0.8f, 0.2f, 500);
			// For Reflecting Sphere
	g_pSphere = new CSolidSphere(1.0f, 24, 12);
	g_pSphere->SetTextureLayer(DIFFUSE_MAP);  // 使用 
	g_pSphere->SetCubeMapTexName(1);
	g_pSphere->SetViewPosition(eye);
	g_pSphere->SetShaderName("vsCubeMapping.glsl", "fsCubeMapping.glsl");
	g_pSphere->SetShader();
	vT.x = 0.0f; vT.y = 2.0f; vT.z = 0.0f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 2.0f; mxT._m[2][2] = 2.0f;
	g_pSphere->SetTRSMatrix(mxT*RotateX(90.0f));
	g_pSphere->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pSphere->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pSphere->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_pSphere->SetColor(vec4(0.9f, 0.9f, 0.9f, 1.0f));
	//-------------------------------------ball 2-------------
	g_pSphere2 = new CSolidSphere(1.0f, 36, 24);
	g_pSphere2->SetTextureLayer(DIFFUSE_MAP);  // 使用 
	g_pSphere2->SetCubeMapTexName(1);
	g_pSphere2->SetViewPosition(eye);
	//g_pSphere2->SetShaderName("vsCubeMapping.glsl", "fsCubeMapping.glsl");
	g_pSphere2->SetShader();
	vT.x = 27.0f; vT.y = 3.0f; vT.z = -8.0f;
	mxT = Translate(vT);
	mxT._m[0][0] = 3.0f; mxT._m[1][1] = 3.0f; mxT._m[2][2] = 3.0f;
	g_pSphere2->SetTRSMatrix(mxT*RotateX(-90.0f));
	g_pSphere2->SetMirror(1,1);
	g_pSphere2->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pSphere2->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pSphere2->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_pSphere2->SetColor(vec4(0.9f, 0.9f, 0.9f, 1.0f));
	//-------------------------------------ball 3-------------
	g_pSphere3 = new CSolidSphere(1.0f, 24, 12);
	g_pSphere3->SetTextureLayer(DIFFUSE_MAP);  // 使用 
	g_pSphere3->SetCubeMapTexName(1);
	g_pSphere3->SetViewPosition(eye);
	g_pSphere3->SetShaderName("vsCubeMapping.glsl", "fsCubeMapping.glsl");
	g_pSphere3->SetShader();
	vT.x = 33.0f; vT.y = 2.0f; vT.z = -2.0f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 2.0f; mxT._m[2][2] = 2.0f;
	g_pSphere3->SetTRSMatrix(mxT*RotateX(90.0f));
	g_pSphere3->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pSphere3->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pSphere3->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_pSphere3->SetColor(vec4(0.9f, 0.9f, 0.9f, 1.0f));
	//-------------------------------------ball 4-------------
	g_pSphere4 = new CSolidSphere(1.0f, 24, 12);
	g_pSphere4->SetTextureLayer(DIFFUSE_MAP);  // 使用 
	g_pSphere4->SetCubeMapTexName(1);
	g_pSphere4->SetViewPosition(eye);
	g_pSphere4->SetShaderName("vsCubeMapping.glsl", "fsCubeMapping.glsl");
	g_pSphere4->SetShader();
	vT.x = 47.5f; vT.y = 5.0f; vT.z = 27.0f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 2.0f; mxT._m[2][2] = 2.0f;
	g_pSphere4->SetTRSMatrix(mxT*RotateX(-90.0f));
	g_pSphere4->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pSphere4->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pSphere4->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
	g_pSphere4->SetColor(vec4(0.9f, 0.9f, 0.9f, 1.0f));
	//-------------------------------------ball 5-------------
	g_pSphere5 = new CSolidSphere(1.0f, 24, 12);
	g_pSphere5->SetTextureLayer(DIFFUSE_MAP);  // 使用 
	g_pSphere5->SetCubeMapTexName(1);
	g_pSphere5->SetViewPosition(eye);
	g_pSphere5->SetShaderName("vsCubeMapping.glsl", "fsCubeMapping.glsl");
	g_pSphere5->SetShader();
	vT.x = 23.5f; vT.y = 3.45f; vT.z = 28.3f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 2.0f; mxT._m[2][2] = 2.0f;
	g_pSphere5->SetTRSMatrix(mxT*RotateX(90.0f));
	g_pSphere5->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pSphere5->SetMaterials(vec4(0), vec4(0.85f, 0.0f, 0.6f, 0.65), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pSphere5->SetKaKdKsShini(0, 0.8f, 0.5f, 20);
	g_pSphere5->SetColor(vec4(0.9f, 0.9f, 0.9f, 1.0f));
	//-------------------------------------ball 6-------------
	g_pSphere6 = new CSolidSphere(1.0f, 24, 12);
	g_pSphere6->SetTextureLayer(DIFFUSE_MAP);  // 使用 
	g_pSphere6->SetCubeMapTexName(1);
	g_pSphere6->SetViewPosition(eye);
	g_pSphere6->SetShaderName("vsCubeMapping.glsl", "fsCubeMapping.glsl");
	g_pSphere6->SetShader();
	vT.x = -5.0f; vT.y = 2.0f; vT.z = 27.0f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 2.0f; mxT._m[2][2] = 2.0f;
	g_pSphere6->SetTRSMatrix(mxT*RotateX(90.0f));
	g_pSphere6->SetShadingMode(GOURAUD_SHADING);
	// 設定貼圖
	g_pSphere6->SetMaterials(vec4(0), vec4(0.85f, 0.85f, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pSphere6->SetKaKdKsShini(0, 0.6f, 0.4f, 70);
	g_pSphere6->SetColor(vec4(0.9f, 0.9f, 0.9f, 1.0f));
	//-------------room2 to room6 light ------------------
	// 設定 代表 Light 的 WireSphere
	g_pLight = new CWireSphere(0.25f, 6, 3);
	g_pLight->SetLightingDisable();
	g_pLight->SetTextureLayer(NONE_MAP);	// 沒有貼圖
	g_pLight->SetShader();
	mxT = Translate(g_Light1.position);
	g_pLight->SetTRSMatrix(mxT);
	g_pLight->SetColor(g_Light1.diffuse);

	g_pLightR2 = new CWireSphere(0.25f, 6, 3);
	g_pLightR2->SetShader();
	mxT = Translate(g_LightRoom2.position);
	g_pLightR2->SetTRSMatrix(mxT);
	g_pLightR2->SetColor(g_LightRoom2.diffuse);
#ifdef LIGHTING_WITHGPU
	g_pLightR2->SetLightingDisable();
#endif

	g_pLightR3 = new CWireSphere(0.25f, 6, 3);
	g_pLightR3->SetShader();
	mxT = Translate(g_LightRoom3.position);
	g_pLightR3->SetTRSMatrix(mxT);
	g_pLightR3->SetColor(g_LightRoom3.diffuse);
#ifdef LIGHTING_WITHGPU
	g_pLightR3->SetLightingDisable();
#endif

	g_pLightR4 = new CWireSphere(0.25f, 6, 3);
	g_pLightR4->SetShader();
	mxT = Translate(g_LightRoom4.position);
	g_pLightR4->SetTRSMatrix(mxT);
	g_pLightR4->SetColor(g_LightRoom4.diffuse);
#ifdef LIGHTING_WITHGPU
	g_pLightR4->SetLightingDisable();
#endif

	g_pLightR5 = new CWireSphere(0.25f, 6, 3);
	g_pLightR5->SetShader();
	mxT = Translate(g_LightRoom5.position);
	g_pLightR5->SetTRSMatrix(mxT);
	g_pLightR5->SetColor(g_LightRoom5.diffuse);
#ifdef LIGHTING_WITHGPU
	g_pLightR5->SetLightingDisable();
#endif

	g_pLightR6 = new CWireSphere(0.25f, 6, 3);
	g_pLightR6->SetShader();
	mxT = Translate(g_LightRoom6.position);
	g_pLightR6->SetTRSMatrix(mxT);
	g_pLightR6->SetColor(g_LightRoom6.diffuse);
#ifdef LIGHTING_WITHGPU
	g_pLightR6->SetLightingDisable();
#endif

	// 因為本範例不會動到 Projection Matrix 所以在這裡設定一次即可
	// 就不寫在 OnFrameMove 中每次都 Check
	bool bPDirty;
	mat4 mpx = camera->getProjectionMatrix(bPDirty); 
	//animal -------------
	g_pAimal[0]->SetProjectionMatrix(mpx);
	//door
	for (int i = 0; i < 12; i++)
	{
		g_pDoor[i]->SetProjectionMatrix(mpx);
	}
	//2DButton
	//g_p2DBtn[3]->SetProjectionMatrix(mpx);
	g_pObj->SetProjectionMatrix(mpx);//obj reader
	g_pObj2->SetProjectionMatrix(mpx);//obj2 reader
	//-------------------room1
	g_pFloor->SetProjectionMatrix(mpx);
	g_LeftWall->SetProjectionMatrix(mpx);
	g_RightWall->SetProjectionMatrix(mpx);
	g_FrontWall->SetProjectionMatrix(mpx);
	g_BackWall->SetProjectionMatrix(mpx);
	g_CellingWall->SetProjectionMatrix(mpx);//天花板
	g_pCube->SetProjectionMatrix(mpx);
	g_pSphere->SetProjectionMatrix(mpx);
	g_pLight->SetProjectionMatrix(mpx);
	//---------------------room2 to room6
	//room2
	g_LeftWall2->SetProjectionMatrix(mpx);
	g_RightWall2->SetProjectionMatrix(mpx);
	g_FrontWall2->SetProjectionMatrix(mpx);
	g_BackWall2->SetProjectionMatrix(mpx);
	g_CellingWall2->SetProjectionMatrix(mpx);//天花板
	g_Floor2->SetProjectionMatrix(mpx);
	g_pCube2->SetProjectionMatrix(mpx);//cube2
	g_pSphere2->SetProjectionMatrix(mpx);//ball 2
	//room 3
	g_LeftWall3->SetProjectionMatrix(mpx);
	g_RightWall3->SetProjectionMatrix(mpx);
	g_FrontWall3->SetProjectionMatrix(mpx);
	g_BackWall3->SetProjectionMatrix(mpx);
	g_CellingWall3->SetProjectionMatrix(mpx);//天花板
	g_Floor3->SetProjectionMatrix(mpx);
	g_pCube3->SetProjectionMatrix(mpx);//cube3
	g_pSphere3->SetProjectionMatrix(mpx);//ball 3
	//room 4
	g_LeftWall4->SetProjectionMatrix(mpx);
	g_RightWall4->SetProjectionMatrix(mpx);
	g_FrontWall4->SetProjectionMatrix(mpx);
	g_BackWall4->SetProjectionMatrix(mpx);
	g_CellingWall4->SetProjectionMatrix(mpx);//天花板
	g_Floor4->SetProjectionMatrix(mpx);
	g_pCube4->SetProjectionMatrix(mpx);//cube4
	g_pSphere4->SetProjectionMatrix(mpx);//ball 4
	//room 5
	g_LeftWall5->SetProjectionMatrix(mpx);
	g_RightWall5->SetProjectionMatrix(mpx);
	g_FrontWall5->SetProjectionMatrix(mpx);
	g_BackWall5->SetProjectionMatrix(mpx);
	g_CellingWall5->SetProjectionMatrix(mpx);//天花板
	g_Floor5->SetProjectionMatrix(mpx);
	g_pCube5->SetProjectionMatrix(mpx);//cube5
	g_pSphere5->SetProjectionMatrix(mpx);//ball 5
	//room 6
	g_LeftWall6->SetProjectionMatrix(mpx);
	g_RightWall6->SetProjectionMatrix(mpx);
	g_FrontWall6->SetProjectionMatrix(mpx);
	g_BackWall6->SetProjectionMatrix(mpx);
	g_CellingWall6->SetProjectionMatrix(mpx);//天花板
	g_Floor6->SetProjectionMatrix(mpx);
	g_pCube6->SetProjectionMatrix(mpx);//cube6
	g_pSphere6->SetProjectionMatrix(mpx);//ball6
	//-------------room2 to room 6 lights
	g_pLightR2->SetProjectionMatrix(mpx);
	g_pLightR3->SetProjectionMatrix(mpx);
	g_pLightR4->SetProjectionMatrix(mpx);
	g_pLightR5->SetProjectionMatrix(mpx);
	g_pLightR6->SetProjectionMatrix(mpx);
}

//----------------------------------------------------------------------------
void GL_Display( void )
{

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear the window
	g_pObj->Draw();//obj reader
	g_pObj2->Draw();//obj2 reader
	//2D draw
	g_p2DBtn[3]->Draw();
	
	glEnable(GL_BLEND);  // 設定2D Texure Mapping 有作用
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifndef  MULTITEXTURE
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[0]); 
	g_pFloor->Draw();
#else 
	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[0]); // 與 Diffuse Map 結合
	glActiveTexture(GL_TEXTURE1); // select active texture 1
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]); // 與 Light Map 結合

	//-------------room1d
	g_pFloor->Draw();
	g_LeftWall->Draw();
	//g_RightWall->Draw();
	//g_FrontWall->Draw();
	g_BackWall->Draw();
	g_CellingWall->Draw();//天花板
	
	
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[4]);
	//=============room3
	
	g_LeftWall3->Draw();
	g_RightWall3->Draw();
	//g_FrontWall3->Draw();
	g_BackWall3->Draw();
	g_CellingWall3->Draw();//天花板
	g_Floor3->Draw();
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[10]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[11]);
	//===============room5
	
	g_LeftWall5->Draw();
	g_RightWall5->Draw();
	g_FrontWall5->Draw();
	g_BackWall5->Draw();
	g_CellingWall5->Draw();//天花板
	g_Floor5->Draw();
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[8]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[9]);
	//=============room4

	g_LeftWall4->Draw();
	g_RightWall4->Draw();
	g_FrontWall4->Draw();
	g_BackWall4->Draw();
	g_CellingWall4->Draw();//天花板
	g_Floor4->Draw();
	glBindTexture(GL_TEXTURE_2D, 0);

#endif
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[1]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[4]);
	g_pCube->Draw();
	
	//===============room2===============
	
	g_LeftWall2->Draw();
	//g_RightWall2->Draw();
	//g_FrontWall2->Draw();
	g_BackWall2->Draw();
	g_CellingWall2->Draw();//天花板
	g_Floor2->Draw();
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[5]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[1]);
	g_pCube2->Draw();//cube2
	g_pCube3->Draw();//cube3
	g_pCube4->Draw();//cube4
	g_pCube5->Draw();//cube5
	g_pCube6->Draw();//cube6
	//================room6
	
	g_LeftWall6->Draw();
	//g_RightWall6->Draw();
	g_FrontWall6->Draw();
	g_BackWall6->Draw();
	g_CellingWall6->Draw();//天花板
	g_Floor6->Draw();
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[3]); // 與 Diffuse Map 結合
	glActiveTexture(GL_TEXTURE1); // select active texture 1
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_uiSphereCubeMap); // 與 Light Map 結合
	g_pSphere->Draw();
	
	g_pSphere3->Draw();//ball 3
	g_pSphere4->Draw();//ball 4
	g_pSphere5->Draw();//ball 5
	g_pSphere6->Draw();//ball 6
	glBindTexture(GL_TEXTURE_2D, 0);
	g_pLight->Draw();
	g_pLightR2->Draw();
	g_pLightR3->Draw();
	g_pLightR4->Draw();
	g_pLightR5->Draw();
	g_pLightR6->Draw();
	glActiveTexture(GL_TEXTURE0); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[6]); // 與 Diffuse Map 結合//earth.png
	g_pSphere2->Draw();//===============ball 2
	glBindTexture(GL_TEXTURE_2D, 0);

	glDepthMask(GL_FALSE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiAimalTexID[0]);
	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiAimalNormalTexID[0]);
	g_pAimal[0]->Draw();
	
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_uiAimalTexID[1]);
	glActiveTexture(GL_TEXTURE2); // select active texture 0
	glBindTexture(GL_TEXTURE_2D, g_uiAimalNormalTexID[1]);
	for (int i = 0; i < 12; i++)
	{
		g_pDoor[i]->Draw();
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glDepthMask(GL_TRUE);// 開啟對 Z-Buffer 的寫入操作
	
	glutSwapBuffers();	// 交換 Frame Buffer
}

//----------------------------------------------------------------------------
// Part 2 : for single light source
void UpdateLightPosition(float dt)
{
	mat4 mxT;
	// 每秒繞 Y 軸轉 90 度
	g_fElapsedTime += dt;
	g_fLightTheta = g_fElapsedTime*(float)M_PI_2;
	if( g_fLightTheta >= (float)M_PI*2.0f ) {
		g_fLightTheta -= (float)M_PI*2.0f;
		g_fElapsedTime -= 4.0f;
	}
	g_Light1.position.x = g_fLightRadius * cosf(g_fLightTheta);
	g_Light1.position.z = g_fLightRadius * sinf(g_fLightTheta);
	mxT = Translate(g_Light1.position);
	g_pLight->SetTRSMatrix(mxT);
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//--------------------相機移動----------------------
void Camera_Move(float x, float y, float z, float w){
	g_camerax += sin(g_fPhi) * x; g_cameray += cos(g_fPhi)* y;
	g_camerax += sin(g_fPhi - (float)M_PI / 2) * z; g_cameray += cos(g_fPhi + (float)M_PI / 2)* w;

	//if (g_camerax >= (float)GRID_SIZE / 2 - 1.0f) g_camerax = (float)GRID_SIZE / 2 - 1.0f;
	//if (g_camerax <= 1.0f - (float)GRID_SIZE / 2) g_camerax = 1.0f - (float)GRID_SIZE / 2;
	//if (g_cameray >= (float)GRID_SIZE / 2 - 1.0f) g_cameray = (float)GRID_SIZE / 2 - 1.0f;
	//if (g_cameray <= 1.0f - (float)GRID_SIZE / 2) g_cameray = 1.0f - (float)GRID_SIZE / 2;

	if (g_camerax >= (float)GRID_SIZE*2.5 - 1.0f) g_camerax = (float)GRID_SIZE*2.5 - 1.0f;
	if (g_camerax <= 1.0f - (float)GRID_SIZE / 2) g_camerax = 1.0f - (float)GRID_SIZE / 2;
	if (g_cameray >= (float)GRID_SIZE*1.5 - 1.0f) g_cameray = (float)GRID_SIZE*1.5 - 1.0f;
	if (g_cameray <= 1.0f - (float)GRID_SIZE / 2) g_cameray = 1.0f - (float)GRID_SIZE / 2;

	if (g_camerax >= (float)GRID_SIZE / 2 - 1.0f&&g_camerax<10.0f &&g_cameray<10.0f&&(g_cameray<-3.0f || g_cameray >= 3.0f)) g_camerax = (float)GRID_SIZE / 2 - 1.0f;//右1
	if (g_camerax >= 10.0f &&g_camerax < 11.0f&& g_cameray<10.0f && (g_cameray<-3.0f || g_cameray >= 3.0f)) g_camerax = (float)GRID_SIZE / 2 + 1.0f;//左2
	if (g_cameray >= (float)GRID_SIZE / 2 - 1.0f&&g_cameray<10.0f &&g_camerax<10.0f && (g_camerax<-3.0f || g_camerax >= 3.0f)) g_cameray = (float)GRID_SIZE / 2 - 1.0f;//前1
	if (g_cameray >= (float)GRID_SIZE / 2 && g_cameray<11.0f &&g_camerax<10.0f && (g_camerax<-3.0f || g_camerax >= 3.0f)) g_cameray = (float)GRID_SIZE / 2 + 1.0f;//後6
	if (g_camerax >= 29.0f&&g_camerax < 30.0f &&g_cameray<10.0f && (g_cameray<2.0f || g_cameray>7.0f))g_camerax = (float)GRID_SIZE*1.5 - 1;//右2
	if (g_camerax >= 30.0f&&g_camerax<31.0f && g_cameray<10.0f && (g_cameray<2.0f || g_cameray>7.0f))g_camerax = (float)GRID_SIZE*1.5 + 1;//左3
	if (g_cameray >= 9.0f&&g_cameray < 10.0f && g_camerax>=10&&g_camerax<30/*&&(g_camerax<14.0f || g_camerax>20.0f)*/)g_cameray = (float)GRID_SIZE / 2 - 1.0f;//前2
	if (g_cameray >= 10.0f&&g_cameray < 11.0f && g_camerax >= 10 && g_camerax<30 /*&& (g_camerax<14.0f || g_camerax>20.0f)*/)g_cameray = (float)GRID_SIZE / 2 + 1.0f;//後5
	if (g_cameray >= 9.0f&&g_cameray < 10.0f &&g_camerax>30.0f&& (g_camerax<43.0f || g_camerax>49.0f))g_cameray = (float)GRID_SIZE / 2 - 1.0f;//前3
	if (g_cameray >= 10.0f&&g_cameray < 11.0f && g_camerax>30.0f && (g_camerax<43.0f || g_camerax>49.0f))g_cameray = (float)GRID_SIZE / 2 + 1.0f;//後4
	if (g_camerax >= (float)GRID_SIZE / 2 - 1.0f&&g_camerax<10.0f &&g_cameray>=10.0f && (g_cameray<19.0f || g_cameray >= 25.0f)) g_camerax = (float)GRID_SIZE / 2 - 1.0f;//右6
	if (g_camerax >= 10.0f &&g_camerax < 11.0f &&g_cameray >= 10.0f && (g_cameray<19.0f || g_cameray >= 25.0f)) g_camerax = (float)GRID_SIZE / 2 + 1.0f;//左5
	if (g_camerax >= 29.0f&&g_camerax < 30.0f && g_cameray >= 10.0f && (g_cameray<22.0f || g_cameray>27.0f))g_camerax = (float)GRID_SIZE*1.5 - 1;//右5
	if (g_camerax >= 30.0f&&g_camerax<31.0f && g_cameray >= 10.0f && (g_cameray<22.0f || g_cameray>27.0f))g_camerax = (float)GRID_SIZE*1.5 + 1;//左4

	point4  at(g_fRadius*sin(g_fTheta)*sin(g_fPhi) + g_camerax, g_fRadius*cos(g_fTheta)+5.0f, g_fRadius*sin(g_fTheta)*cos(g_fPhi) + g_cameray, 1.0f);
	point4  eye(g_camerax, 5.0f, g_cameray, 1.0f);//高度
	auto camera = CCamera::getInstance();
	camera->updateViewLookAt(eye, at);
}
void onFrameMove(float delta)
{

	if( g_bAutoRotating ) { // Part 2 : 重新計算 Light 的位置
		UpdateLightPosition(delta);
	}

	mat4 mvx;	// view matrix & projection matrix
	bool bVDirty;	// view 與 projection matrix 是否需要更新給物件
	auto camera = CCamera::getInstance();
	mvx = camera->getViewMatrix(bVDirty);
	if (bVDirty) {
		//Animal SetviewMa------------
		g_pAimal[0]->SetViewMatrix(mvx);
		//door1
		for (int i = 0; i < 12; i++)
		{
			g_pDoor[i]->SetViewMatrix(mvx);
		}
		
		//2DButton
		//g_p2DBtn[3]->SetViewMatrix(mvx);
		//---------------------room1
		g_pFloor->SetViewMatrix(mvx);
		g_LeftWall->SetViewMatrix(mvx);
		g_RightWall->SetViewMatrix(mvx);
		g_FrontWall->SetViewMatrix(mvx);
		g_BackWall->SetViewMatrix(mvx);
		g_CellingWall->SetViewMatrix(mvx);//天花板
		g_pCube->SetViewMatrix(mvx);
		g_pSphere->SetViewMatrix(mvx);
		g_pSphere->SetViewPosition(camera->getViewPosition());
		g_pLight->SetViewMatrix(mvx);
		g_pObj->SetViewMatrix(mvx);//obj reader
		//--------------room2
		g_pLightR2->SetViewMatrix(mvx);
		g_LeftWall2->SetViewMatrix(mvx);
		g_RightWall2->SetViewMatrix(mvx);
		g_FrontWall2->SetViewMatrix(mvx);
		g_BackWall2->SetViewMatrix(mvx);
		g_CellingWall2->SetViewMatrix(mvx);//天花板
		g_Floor2->SetViewMatrix(mvx);
		g_pCube2->SetViewMatrix(mvx);//cube2
		g_pSphere2->SetViewMatrix(mvx);
		//g_pSphere2->SetViewPosition(camera->getViewPosition());
		//----------------room3
		g_pLightR3->SetViewMatrix(mvx);
		g_LeftWall3->SetViewMatrix(mvx);
		g_RightWall3->SetViewMatrix(mvx);
		g_FrontWall3->SetViewMatrix(mvx);
		g_BackWall3->SetViewMatrix(mvx);
		g_CellingWall3->SetViewMatrix(mvx);//天花板
		g_Floor3->SetViewMatrix(mvx);
		g_pCube3->SetViewMatrix(mvx);//cube3
		g_pSphere3->SetViewMatrix(mvx);
		g_pSphere3->SetViewPosition(camera->getViewPosition());
		//------------------room4
		g_pLightR4->SetViewMatrix(mvx);
		//g_LeftWall4->SetViewMatrix(mvx);
		g_RightWall4->SetViewMatrix(mvx);
		g_FrontWall4->SetViewMatrix(mvx);
		g_BackWall4->SetViewMatrix(mvx);
		g_CellingWall4->SetViewMatrix(mvx);//天花板
		g_Floor4->SetViewMatrix(mvx);
		g_pCube4->SetViewMatrix(mvx);//cube4
		g_pSphere4->SetViewMatrix(mvx);
		g_pSphere4->SetViewPosition(camera->getViewPosition());
		//----------------room5
		g_pLightR5->SetViewMatrix(mvx);
		g_LeftWall5->SetViewMatrix(mvx);
		g_RightWall5->SetViewMatrix(mvx);
		g_FrontWall5->SetViewMatrix(mvx);
		g_BackWall5->SetViewMatrix(mvx);
		g_CellingWall5->SetViewMatrix(mvx);//天花板
		g_Floor5->SetViewMatrix(mvx);
		g_pCube5->SetViewMatrix(mvx);//cube5
		g_pSphere5->SetViewMatrix(mvx);
		g_pSphere5->SetViewPosition(camera->getViewPosition());
		//-----------------room6
		g_pLightR6->SetViewMatrix(mvx);
		g_LeftWall6->SetViewMatrix(mvx);
		g_RightWall6->SetViewMatrix(mvx);
		g_FrontWall6->SetViewMatrix(mvx);
		g_BackWall6->SetViewMatrix(mvx);
		g_CellingWall6->SetViewMatrix(mvx);//天花板
		g_Floor6->SetViewMatrix(mvx);
		g_pCube6->SetViewMatrix(mvx);//cube6
		g_pSphere6->SetViewMatrix(mvx);
		g_pSphere6->SetViewPosition(camera->getViewPosition());
		g_pObj2->SetViewMatrix(mvx);//obj2 reader
	}

	//Animal update
	g_pAimal[0]->Update(delta, g_Light1);
	//door 1
	g_pDoor[0]->Update(delta, g_Light1);
	g_pDoor[11]->Update(delta, g_Light1);
	// 如果需要重新計算時，在這邊計算每一個物件的顏色
	g_pFloor->Update(delta, g_Light1);
	//------------------------room1
	g_LeftWall->Update(delta, g_Light1);
	g_RightWall->Update(delta, g_Light1);
	g_FrontWall->Update(delta, g_Light1);
	g_BackWall->Update(delta, g_Light1);
	g_CellingWall->Update(delta, g_Light1);//天花板
	g_pCube->Update(delta, g_Light1);
	g_pSphere->Update(delta, g_Light1);
	g_pLight->Update(delta);
	
	//----------------room2 to room 6---------------
	//room2
	g_LeftWall2->UpdateTwoLights(delta, g_LightRoom2,g_Light1);
	g_RightWall2->Update(delta, g_LightRoom2);
	g_FrontWall2->Update(delta, g_LightRoom2);
	g_BackWall2->Update(delta, g_LightRoom2);
	g_CellingWall2->Update(delta, g_LightRoom2);//天花板
	g_Floor2->Update(delta, g_LightRoom2);
	g_pLightR2->Update(delta);
	g_pCube2->Update(delta, g_LightRoom2);
	g_pSphere2->Update(delta, g_LightRoom2);
	g_pObj->Update(delta, g_LightRoom2);//obj reader
	g_pDoor[1]->Update(delta, g_LightRoom2);
	g_pDoor[2]->Update(delta, g_LightRoom2);
	//room3--------------
	g_LeftWall3->Update(delta, g_LightRoom3);
	g_RightWall3->Update(delta, g_LightRoom3);
	g_FrontWall3->Update(delta, g_LightRoom3);
	g_BackWall3->Update(delta, g_LightRoom3);
	g_CellingWall3->Update(delta, g_LightRoom3);//天花板
	g_Floor3->Update(delta, g_LightRoom3);
	g_pLightR3->Update(delta);
	g_pCube3->Update(delta, g_LightRoom3);
	g_pSphere3->Update(delta, g_LightRoom3);
	g_pDoor[3]->Update(delta, g_LightRoom3);
	g_pDoor[4]->Update(delta, g_LightRoom3);
	//room4
	g_LeftWall4->Update(delta, g_LightRoom4);
	g_RightWall4->Update(delta, g_LightRoom4);
	g_FrontWall4->Update(delta, g_LightRoom4);
	g_BackWall4->Update(delta, g_LightRoom4);
	g_CellingWall4->Update(delta, g_LightRoom4);//天花板
	g_Floor4->Update(delta, g_LightRoom4);
	g_pLightR4->Update(delta);
	g_pCube4->Update(delta, g_LightRoom4);
	g_pSphere4->Update(delta, g_LightRoom4);
	g_pDoor[5]->Update(delta, g_LightRoom4);
	g_pDoor[6]->Update(delta, g_LightRoom4);
	//room5
	g_LeftWall5->Update(delta, g_LightRoom5);
	g_RightWall5->Update(delta, g_LightRoom5);
	g_FrontWall5->Update(delta, g_LightRoom5);
	g_BackWall5->Update(delta, g_LightRoom5);
	g_CellingWall5->Update(delta, g_LightRoom5);//天花板
	g_Floor5->Update(delta, g_LightRoom5);
	g_pLightR5->Update(delta);
	g_pCube5->Update(delta, g_LightRoom5);
	g_pSphere5->Update(delta, g_LightRoom5);
	g_pDoor[7]->Update(delta, g_LightRoom5);
	g_pDoor[8]->Update(delta, g_LightRoom5);
	//room6
	g_LeftWall6->Update(delta, g_LightRoom6);
	g_RightWall6->Update(delta, g_LightRoom6);
	g_FrontWall6->Update(delta, g_LightRoom6);
	g_BackWall6->Update(delta, g_LightRoom6);
	g_CellingWall6->Update(delta, g_LightRoom6);//天花板
	g_Floor6->Update(delta, g_LightRoom6);
	g_pLightR6->Update(delta);
	g_pCube6->Update(delta, g_LightRoom6);
	g_pSphere6->Update(delta, g_LightRoom6);//ball 6
	g_pObj2->Update(delta, g_LightRoom6);//obj2 reader  car
	g_pDoor[9]->Update(delta, g_LightRoom6);
	g_pDoor[10]->Update(delta, g_LightRoom6);
	//2D
	g_p2DBtn[3]->Update(delta, g_Light1);
	GL_Display();
}

//----------------------------------------------------------------------------

void Win_Keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
	case  SPACE_KEY:

		break;
//----------------------------------------------------------------------------
// Part 2 : for single light source
	case 65: // A key
	case 97: // a key
		g_bAutoRotating = !g_bAutoRotating;
		break;
	case 82: // R key
		if( g_fLightR <= 0.95f ) g_fLightR += 0.05f;
		g_Light1.diffuse.x = g_fLightR;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
	case 114: // r key
		if( g_fLightR >= 0.05f ) g_fLightR -= 0.05f;
		g_Light1.diffuse.x = g_fLightR;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
	case 71: // G key
		if( g_fLightG <= 0.95f ) g_fLightG += 0.05f;
		g_Light1.diffuse.y = g_fLightG;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
	case 103: // g key
		if( g_fLightG >= 0.05f ) g_fLightG -= 0.05f;
		g_Light1.diffuse.y = g_fLightG;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
	case 66: // B key
		if( g_fLightB <= 0.95f ) g_fLightB += 0.05f;
		g_Light1.diffuse.z = g_fLightB;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
	case 98: // b key
		if( g_fLightB >= 0.05f ) g_fLightB -= 0.05f;
		g_Light1.diffuse.z = g_fLightB;
		g_pLight->SetColor(g_Light1.diffuse);
		break;
//----------------------------------------------------------------------------
    case 033:
		glutIdleFunc( NULL );
		delete g_pCube;
		delete g_pFloor;
		delete g_pLight;
		delete g_LeftWall, g_BackWall, g_FrontWall, g_CellingWall, g_RightWall;//牆壁
		delete g_pObj; //obj reader
		delete g_pObj2; //obj2 reader car
		CCamera::getInstance()->destroyInstance();
		CTexturePool::getInstance()->destroyInstance();
        exit( EXIT_SUCCESS );
        break;
    }
}
//2DUI
inline void ScreenToUICoordinate(int x, int y, vec2 &pt)
{
	pt.x = 2.0f*(float)x / SCREEN_SIZE - 1.0f;
	pt.y = 2.0f*(float)(SCREEN_SIZE - y) / SCREEN_SIZE - 1.0f;
}
//----------------------------------------------------------------------------
void Win_Mouse(int button, int state, int x, int y) {
	vec2 pt;
	switch(button) {
		
		case GLUT_LEFT_BUTTON:   // 目前按下的是滑鼠左鍵
			if (state == GLUT_DOWN){
				ScreenToUICoordinate(x, y, pt);
				
				if (g_p2DBtn[3]->OnTouches(pt)) {
					g_bAutoRotating = !g_bAutoRotating;
				}
			}
			break;
		case GLUT_MIDDLE_BUTTON:  // 目前按下的是滑鼠中鍵 ，換成 Y 軸
			//if ( state == GLUT_DOWN ) ; 
			break;
		case GLUT_RIGHT_BUTTON:   // 目前按下的是滑鼠右鍵
			//if ( state == GLUT_DOWN ) ;
			break;
		default:
			break;
	} 
}
//----------------------------------------------------------------------------
void Win_SpecialKeyboard(int key, int x, int y) {

	switch(key) {
	case GLUT_KEY_UP:		// 目前按下的是向上方向鍵
		Camera_Move(2.1f, 2.1f, 0.0f, 0.0f);
		break;
	case GLUT_KEY_DOWN:		// 目前按下的是向下方向鍵
		Camera_Move(-2.1f, -2.1f, 0.0f, 0.0f);
		break;
	case GLUT_KEY_LEFT:		// 目前按下的是向左方向鍵
		Camera_Move(0.0f, 0.0f, -2.1f, 2.1f);
		break;
	case GLUT_KEY_RIGHT:	// 目前按下的是向右方向鍵
		Camera_Move(0.0f, 0.0f, 2.1f, -2.1f);
		break;
		default:
			break;
	}
}

//----------------------------------------------------------------------------
// The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.
void Win_PassiveMotion(int x, int y) {

	g_fPhi = (float)-M_PI*(x - HALF_SIZE)/(HALF_SIZE); // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	g_fTheta = (float)M_PI*(float)y/SCREEN_SIZE;
	point4  at(g_fRadius*sin(g_fTheta)*sin(g_fPhi) + g_camerax, g_fRadius*cos(g_fTheta)+5.0f, g_fRadius*sin(g_fTheta)*cos(g_fPhi) + g_cameray, 1.0f);
	point4  eye(g_camerax, 5.0f, g_cameray, 1.0f);
	//point4  at(0.0f, 0.0f, 0.0f, 1.0f);
	CCamera::getInstance()->updateViewLookAt(eye, at);
}

// The motion callback for a window is called when the mouse moves within the window while one or more mouse buttons are pressed.
void Win_MouseMotion(int x, int y) {
	g_fPhi = (float)-M_PI*(x - HALF_SIZE)/(HALF_SIZE);  // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	g_fTheta = (float)M_PI*(float)y/SCREEN_SIZE;;
	point4  eye(g_fRadius*sin(g_fTheta)*sin(g_fPhi) + g_camerax, g_fRadius*cos(g_fTheta)+5.0f, g_fRadius*sin(g_fTheta)*cos(g_fPhi)+g_cameray, 1.0f);
	point4  at(g_camerax, 5.0f, g_cameray, 1.0f);
	//point4  at(0.0f, 0.0f, 0.0f, 1.0f);
	CCamera::getInstance()->updateViewLookAt(eye, at);
}
//----------------------------------------------------------------------------
void GL_Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background
	glEnable(GL_DEPTH_TEST);
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
    
	glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( SCREEN_SIZE, SCREEN_SIZE );

	// If you use freeglut the two lines of code can be added to your application 
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow("Compositing and Blending");

	// The glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), 
	// which ensures that all extensions with valid entry points will be exposed.
	glewExperimental = GL_TRUE; 
    glewInit();  

    init();

	glutMouseFunc(Win_Mouse);
	glutMotionFunc(Win_MouseMotion);
	glutPassiveMotionFunc(Win_PassiveMotion);  
    glutKeyboardFunc( Win_Keyboard );	// 處理 ASCI 按鍵如 A、a、ESC 鍵...等等
	glutSpecialFunc( Win_SpecialKeyboard);	// 處理 NON-ASCI 按鍵如 F1、Home、方向鍵...等等
    glutDisplayFunc( GL_Display );
	glutReshapeFunc( GL_Reshape );
	glutIdleFunc( IdleProcess );
	
    glutMainLoop();
    return 0;
}