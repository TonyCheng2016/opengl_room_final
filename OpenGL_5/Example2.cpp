// 
// OpenGL Texture Mapping 
//
// 1. �򥻳]�w Texture object
// 2. �����Ѽƪ��]�w
// 3. Mipmapping ���ϥλP�ѼƳ]�w
// 
// ����e���ǳƤu�@ 
//  CShape.h �� 17 �� 25 ��A �ȥ��}�� 22 ��(�p�U��)�A��L���]�w������
//  #define MULTITEXTURE  DIFFUSE_MAP  
//

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

#define SPACE_KEY 32
#define SCREEN_SIZE 800
#define HALF_SIZE SCREEN_SIZE /2 
#define VP_HALFWIDTH  20.0f
#define VP_HALFHEIGHT 20.0f
#define GRID_SIZE 20 // must be an even number

// For Objects
CQuad		  *g_pFloor;
CSolidCube    *g_pCube;
CSolidSphere  *g_pSphere;

// For View Point
GLfloat g_fRadius = 12.0;  // ��l�� 12.0
GLfloat g_fTheta = 60.0f*DegreesToRadians;
GLfloat g_fPhi = 45.0f*DegreesToRadians;

//----------------------------------------------------------------------------
// Part 2 : for single light source
bool g_bAutoRotating = false;
float g_fElapsedTime = 0;
float g_fLightRadius = 6;	// ��l�� 6.0
float g_fLightTheta = 0;

float g_fLightR = 0.95f;
float g_fLightG = 0.95f;
float g_fLightB = 0.95f;

LightSource g_Light1 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(6.0f, 6.0f, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	2.0f,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot ���ө��d��
	0.707f,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), �ө���V�P�Q�ө��I���������ר� cos ��, cut off ����
	1,	// constantAttenuation	(a + bd + cd^2)^-1 ���� a, d ��������Q�ө��I���Z��
	0,	// linearAttenuation	    (a + bd + cd^2)^-1 ���� b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 ���� c
};

CWireSphere *g_pLight;
//----------------------------------------------------------------------------

// Texture ���ե�
GLuint g_uiFTexID[3]; // �T�Ӫ�����O�����P���K��
int g_iTexWidth,  g_iTexHeight;

//----------------------------------------------------------------------------
// �禡���쫬�ŧi
extern void IdleProcess();

void init( void )
{
	mat4 mxT;
	vec4 vT, vColor;
	// ���ͩһݤ� Model View �P Projection Matrix
	// ���ͩһݤ� Model View �P Projection Matrix
	point4  eye(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);
	point4  at(0.0f, 0.0f, 0.0f, 1.0f);
	auto camera = CCamera::create();
	camera->updateViewLookAt(eye, at);
	camera->updatePerspective(60.0, (GLfloat)SCREEN_SIZE / (GLfloat)SCREEN_SIZE, 1.0, 1000.0);

	auto texturepool = CTexturePool::create();
	g_uiFTexID[0] = texturepool->AddTexture("texture/checker.png");
	g_uiFTexID[1] = texturepool->AddTexture("texture/Masonry.Brick.png");
	g_uiFTexID[2] = texturepool->AddTexture("texture/earth.png");

	// ���ͪ��󪺹���
	g_pFloor = new CQuad;
	g_pFloor->SetShader();
	g_pFloor->SetTRSMatrix(Scale(25,1,25)); // ��l�� (15,1,15)
	g_pFloor->SetShadingMode(GOURAUD_SHADING);
	g_pFloor->SetTiling(40, 40); // ��l�� (10, 10)


	// �]�w�K��
	g_pFloor->SetMaterials(vec4(0), vec4(1.25f, 1.25f, 1.25f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pFloor->SetKaKdKsShini(0, 0.8f, 0.5f, 1);

	g_pCube = new CSolidCube;
	g_pCube->SetShader();
	// �]�w Cube
	vT.x = 1.5f; vT.y = 1.01f; vT.z = -1.5f;
	mxT = Translate(vT);
	mxT._m[0][0] = 2.0f; mxT._m[1][1] = 2.0f; mxT._m[2][2] = 2.0f;	// ��j�⭿
	g_pCube->SetTRSMatrix(mxT);
	g_pCube->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pCube->SetMaterials(vec4(0.35f, 0.35f, 0.35f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pCube->SetKaKdKsShini(0.15f, 1.0f, 0, 2);

	g_pSphere = new CSolidSphere(1, 16, 16);
	g_pSphere->SetShader();
	// �]�w Sphere
	vT.x = -3.5; vT.y = 2.0; vT.z = 1.5;
	mxT = Translate(vT)* RotateX(-90) * Scale(2,2,2);
	g_pSphere->SetTRSMatrix(mxT);
	g_pSphere->SetShadingMode(GOURAUD_SHADING);
	// materials
	g_pSphere->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pSphere->SetKaKdKsShini(0.15f, 0.9f, 0.1f, 2);

	// �]�w �N�� Light �� WireSphere
	g_pLight = new CWireSphere(0.25f, 6, 3);
	g_pLight->SetTextureLayer(0);		// �S���K��
	g_pLight->SetLightingDisable();		// ���ݭn�p������ө�
	g_pLight->SetShader();
	mxT = Translate(g_Light1.position);
	g_pLight->SetTRSMatrix(mxT);
	g_pLight->SetColor(g_Light1.diffuse);

	// �]�����d�Ҥ��|�ʨ� Projection Matrix �ҥH�b�o�̳]�w�@���Y�i
	// �N���g�b OnFrameMove ���C���� Check
	bool bPDirty;
	mat4 mpx = camera->getProjectionMatrix(bPDirty);
	g_pFloor->SetProjectionMatrix(mpx);
	g_pCube->SetProjectionMatrix(mpx);
	g_pSphere->SetProjectionMatrix(mpx);
	g_pLight->SetProjectionMatrix(mpx);
}

//----------------------------------------------------------------------------
void GL_Display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear the window
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[0]); 
	g_pFloor->Draw();
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[1]);
	g_pCube->Draw();
	glBindTexture(GL_TEXTURE_2D, g_uiFTexID[2]);
	g_pSphere->Draw();
	glBindTexture(GL_TEXTURE_2D, 0);
	g_pLight->Draw();
	glutSwapBuffers();	// �洫 Frame Buffer
}

//----------------------------------------------------------------------------
// Part 2 : for single light source
void UpdateLightPosition(float dt)
{
	mat4 mxT;
	// �C��¶ Y �b�� 90 ��
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

void onFrameMove(float delta)
{
	if( g_bAutoRotating ) { // Part 2 : ���s�p�� Light ����m
		UpdateLightPosition(delta);
	}

	mat4 mvx;	// view matrix & projection matrix
	bool bVDirty;	// view �P projection matrix �O�_�ݭn��s������
	auto camera = CCamera::getInstance();
	mvx = camera->getViewMatrix(bVDirty);
	if (bVDirty) {
		g_pFloor->SetViewMatrix(mvx);
		g_pCube->SetViewMatrix(mvx);
		g_pSphere->SetViewMatrix(mvx);
		g_pLight->SetViewMatrix(mvx);
	}

	// �p�G�ݭn���s�p��ɡA�b�o��p��C�@�Ӫ����C��
	g_pFloor->Update(delta, g_Light1);
	g_pCube->Update(delta, g_Light1);
	g_pSphere->Update(delta, g_Light1);
	g_pLight->Update(delta);
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
		delete g_pSphere;
		delete g_pFloor;
		delete g_pLight;
		CCamera::getInstance()->destroyInstance();
		CTexturePool::getInstance()->destroyInstance();
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------
void Win_Mouse(int button, int state, int x, int y) {
	switch(button) {
		case GLUT_LEFT_BUTTON:   // �ثe���U���O�ƹ�����
			//if ( state == GLUT_DOWN ) ; 
			break;
		case GLUT_MIDDLE_BUTTON:  // �ثe���U���O�ƹ����� �A���� Y �b
			//if ( state == GLUT_DOWN ) ; 
			break;
		case GLUT_RIGHT_BUTTON:   // �ثe���U���O�ƹ��k��
			//if ( state == GLUT_DOWN ) ;
			break;
		default:
			break;
	} 
}
//----------------------------------------------------------------------------
void Win_SpecialKeyboard(int key, int x, int y) {

	switch(key) {
		case GLUT_KEY_LEFT:		// �ثe���U���O�V����V��

			break;
		case GLUT_KEY_RIGHT:	// �ثe���U���O�V�k��V��

			break;
		default:
			break;
	}
}

//----------------------------------------------------------------------------
// The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.
void Win_PassiveMotion(int x, int y) {

	g_fPhi = (float)-M_PI*(x - HALF_SIZE)/(HALF_SIZE); // �ഫ�� g_fPhi ���� -PI �� PI ���� (-180 ~ 180 ����)
	g_fTheta = (float)M_PI*(float)y/SCREEN_SIZE;
	point4  eye(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);
	point4  at(0.0f, 0.0f, 0.0f, 1.0f);
	CCamera::getInstance()->updateViewLookAt(eye, at);
}

// The motion callback for a window is called when the mouse moves within the window while one or more mouse buttons are pressed.
void Win_MouseMotion(int x, int y) {
	g_fPhi = (float)-M_PI*(x - HALF_SIZE)/(HALF_SIZE);  // �ഫ�� g_fPhi ���� -PI �� PI ���� (-180 ~ 180 ����)
	g_fTheta = (float)M_PI*(float)y/SCREEN_SIZE;;
	point4  eye(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);
	point4  at(0.0f, 0.0f, 0.0f, 1.0f);
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
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DOUBLE );
    glutInitWindowSize( SCREEN_SIZE, SCREEN_SIZE );

	// If you use freeglut the two lines of code can be added to your application 
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow("OpenGL Texture Mapping");

	// The glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), 
	// which ensures that all extensions with valid entry points will be exposed.
	glewExperimental = GL_TRUE; 
    glewInit();  

    init();

	glutMouseFunc(Win_Mouse);
	glutMotionFunc(Win_MouseMotion);
	glutPassiveMotionFunc(Win_PassiveMotion);  
    glutKeyboardFunc( Win_Keyboard );	// �B�z ASCI ����p A�Ba�BESC ��...����
	glutSpecialFunc( Win_SpecialKeyboard);	// �B�z NON-ASCI ����p F1�BHome�B��V��...����
    glutDisplayFunc( GL_Display );
	glutReshapeFunc( GL_Reshape );
	glutIdleFunc( IdleProcess );
	
    glutMainLoop();
    return 0;
}