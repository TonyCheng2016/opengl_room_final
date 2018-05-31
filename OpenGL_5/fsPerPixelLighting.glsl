#version 130
// #define NPR
// iTexLayer ���]�w�P�P�_�覡�A���]���B�i�H�䴩�줻�i�K��

#define NONE_MAP    0
#define DIFFUSE_MAP 1
#define LIGHT_MAP   2
#define NORMAL_MAP  4

in vec3 v3N;
in vec3 v3L;//light1
in vec3 v3E;
in vec3 v3L2;//light2
in vec2 DiffuseMapUV;   // ��J Diffuse Map �K�Ϯy��
in vec2 LightMapUV;     // ��J Light Map �K�Ϯy��

uniform int iTexLayer;

// �H�U���s�W������
//-------------light1------------------
uniform vec4  LightInView;        // Light's position in View Space
uniform vec4  AmbientProduct;  // light's ambient  �P Object's ambient  �P ka �����n
uniform vec4  DiffuseProduct;  // light's diffuse  �P Object's diffuse  �P kd �����n
uniform vec4  SpecularProduct; // light's specular �P Object's specular �P ks �����n
uniform float fShininess;
uniform int   iLighting;
uniform vec4  vObjectColor;    // �N���󪺳�@�C��
//--------------light2---------------
uniform vec4  LightInView2;        // Light's position in View Space
uniform vec4  AmbientProduct2;  // light's ambient  �P Object's ambient  �P ka �����n
uniform vec4  DiffuseProduct2;  // light's diffuse  �P Object's diffuse  �P kd �����n
uniform vec4  SpecularProduct2; // light's specular �P Object's specular �P ks �����n
// For Texture Sampler
uniform sampler2D diffuMap; // �K�Ϫ��ѼƳ]�w
uniform sampler2D lightMap; // �K�Ϫ��ѼƳ]�w

void main()
{
	// ���ŧi diffuse �P specular
	//----------------light1-------------
    vec4 diffuse = vec4(0.0,0.0,0.0,1.0);
	vec4 specular = vec4(0.0,0.0,0.0,1.0);
	vec4 LightingColor = vec4(0.0,0.0,0.0,1.0);
	//----------------light2-------------
	vec4 diffuse2 = vec4(0.0,0.0,0.0,1.0);
	vec4 specular2 = vec4(0.0,0.0,0.0,1.0);

	if( iLighting != 1 ) {
		if( iTexLayer == NONE_MAP ) gl_FragColor = vObjectColor;
		else gl_FragColor = vObjectColor * texture2D(diffuMap, DiffuseMapUV); 
	}
	else {	
		// 1. �p�� Ambient color : Ia = AmbientProduct = Ka * Material.ambient * La = 
		vec4 ambient = AmbientProduct; //-----------light1 a---------------// m_sMaterial.ka * m_sMaterial.ambient * vLightI;
		vec4 ambient2= AmbientProduct2;//-----------light2 a---------------
		// ���ƶǤJ�� Normal Dir
		vec3 vN = normalize(v3N); 

		// 2. ���ƶǤJ�� Light Dir
		vec3 vL = normalize(v3L); // normalize light vector		//-------------light1 fL
		vec3 vL2 = normalize(v3L2); // normalize light2 vector //-------------light2 fL2
		// 5. �p�� L dot N
		float fLdotN = vL.x*vN.x + vL.y*vN.y + vL.z*vN.z;//---------light1 fLdotN2----------------
		float fLdotN2 = vL2.x*vN.x + vL2.y*vN.y + vL2.z*vN.z;//---------light2 fLdotN2----------------

		//-----------------light1----------------------
		if( fLdotN >= 0 ) { // ���I�Q�����Ө�~�ݭn�p��
#ifndef NPR
			// Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
			diffuse = fLdotN * DiffuseProduct; 
#else
			if( fLdotN >= 0.85 ) diffuse = 1.0 * DiffuseProduct;
			else if( fLdotN >= 0.55 ) diffuse = 0.55 * DiffuseProduct;
			else diffuse = 0.35 * DiffuseProduct;
#endif

			// Specular color
			// Method 1: Phone Model
			// �p�� View Vector
			// ���ƶǤJ�� v3E , View Direction
			vec3 vV = normalize(v3E);

			//�p�� Light �� �Ϯg�� vRefL = 2 * fLdotN * vN - L
			// �P�ɧQ�� normalize �ন���V�q
			vec3 vRefL = normalize(2.0f * (fLdotN) * vN - vL);

			//   �p��  vReflectedL dot View
			float RdotV = vRefL.x*vV.x + vRefL.y*vV.y + vRefL.z*vV.z;

#ifndef NPR
			// Specular Color : Is = Ks * Material.specular * Ls * (R dot V)^Shininess;
			if( RdotV > 0 ) specular = SpecularProduct * pow(RdotV, fShininess); 
#else
			specular = vec4(0.0,0.0,0.0,1.0); 
#endif
		}

				//-----------------light2---------------------------
		if( fLdotN2 >= 0 ) { // ���I�Q�����Ө�~�ݭn�p��
#ifndef NPR
			// Diffuse Color : Id = Kd * Material.diffuse * Ld * (L dot N)
			diffuse2 = fLdotN2 * DiffuseProduct2; 
#else
			if( fLdotN2 >= 0.85 ) diffuse2 = 1.0 * DiffuseProduct2;
			else if( fLdotN2 >= 0.55 ) diffuse2 = 0.55 * DiffuseProduct2;
			else diffuse2 = 0.35 * DiffuseProduct2;
#endif

			// Specular color
			// Method 1: Phone Model
			// �p�� View Vector
			// ���ƶǤJ�� v3E , View Direction
			vec3 vV = normalize(v3E);

			//�p�� Light2 �� �Ϯg�� vRefL = 2 * fLdotN * vN - L
			// �P�ɧQ�� normalize �ন���V�q
			vec3 vRefL = normalize(2.0f * (fLdotN2) * vN - vL2);

			//   �p��  vReflectedL dot View
			float RdotV = vRefL.x*vV.x + vRefL.y*vV.y + vRefL.z*vV.z;

#ifndef NPR
			// Specular Color : Is = Ks * Material.specular * Ls * (R dot V)^Shininess;
			if( RdotV > 0 ) specular2 = SpecularProduct2 * pow(RdotV, fShininess); 
#else
			specular2 = vec4(0.0,0.0,0.0,1.0); 
#endif
		}

		LightingColor = ambient + diffuse + specular+ ambient2 + diffuse2 + specular2;  // �p���C�� ambient + diffuse + specular;//light1+light2
		LightingColor.w = DiffuseProduct.w;	// �]�w���ǤJ���誺 alpha,	DiffuseProduct.w
		//LightingColor.w = 1.0;	// �]�w alpha �� 1.0

		if( iTexLayer == NONE_MAP ) gl_FragColor = LightingColor;
		else if( iTexLayer == DIFFUSE_MAP || iTexLayer == (DIFFUSE_MAP|NORMAL_MAP) ) gl_FragColor = LightingColor * texture2D(diffuMap, DiffuseMapUV);
		else if( iTexLayer == (DIFFUSE_MAP|LIGHT_MAP)) gl_FragColor = (LightingColor * texture2D(diffuMap, DiffuseMapUV) + texture2D(diffuMap, DiffuseMapUV) * texture2D(lightMap, LightMapUV)* vec4(0.35f, 0.65f, 0.55f, 1.0));
	}
}
