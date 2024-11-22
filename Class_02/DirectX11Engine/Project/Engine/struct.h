#pragma once

// 정점
// 3D 공간의 좌표를 나타내는 단위 
struct Vtx
{	
	Vec3	vPos;
	Vec4	vColor;
	Vec2	vUV;
};


// 디버그 렌더 요청 정보
struct tDebugShapeInfo
{
	DEBUG_SHAPE		Shape;
	Vec3			Position;
	Vec3			Scale;
	Vec3			Rotation;
	Matrix			matWorld;
	Vec4			Color;
	float			Duration;
	float			Age;
};

struct tTask
{
	TASK_TYPE	Type;

	DWORD_PTR	dwParam_0;
	DWORD_PTR	dwParam_1;
	DWORD_PTR	dwParam_2;
};


// 광원 관련 구조체
struct tLight
{
	Vec4	vDiffuse;		// 난반사 (빛의 색상)
	Vec4	vAmbient;		// 환경광 (빛으로 발생하는 최소한의 밝기)
	Vec4	vMaxSpecular;	// 반사광 (빛으로 발생하는 반사광의 최대 밝기)
};

struct tLightInfo
{
	tLight	Light;

	UINT	LightType;	// 0 : DirectionalLight, 1 : PointLight, 2 : SpotLight
	Vec3	WorldDir;	// WorldSpace 에서 광원의 방향
	Vec3	WorldPos;	// WorldSpace 에서 광원의 위치
	float	Range;		// SpotLight or PointLight 에 광원의 영향 범위
	float	Angle;		// Spot Light 인 경우 광원의 각도범위

	Vec3	vPadding;
};



// ===================
// 상수버퍼 관련 구조체
// ===================
struct tTransform
{
	Matrix	matWorld;
	Matrix	matView;
	Matrix  matProj;

	Matrix	matWV;
	Matrix  matWVP;
};
extern tTransform g_Trans;


struct tMtrlConst
{
	int		iArr[4];
	float	fArr[4];
	Vec2	v2Arr[4];
	Vec4	v4Arr[4];
	Matrix	matArr[4];
};

struct tAnim2DInfo
{
	Vec2	vLeftTop;
	Vec2	vSliceSize;

	Vec2	vOffset;
	Vec2	vBackground;

	int		UseAnim2D;
	int		padding[3];
};

struct tGlobalData
{
	Vec2	vResolution;	// 렌더링 해상도
	float	DeltaTime;		// DT
	float	Time;			// 게임 시작한후 지난 시간

	int		Light2DCount;
	int		Light3DCount;

	Vec2	Padding;
};

extern tGlobalData g_GlobalData;
