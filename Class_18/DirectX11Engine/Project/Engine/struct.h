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


// Particle
struct tParticle
{
	Vec4	vColor;
	Vec3	vLocalPos;
	Vec3	vWorldPos;
	Vec3	vWorldScale;
	Vec3	vWorldRotation;
	
	Vec3	vForce;
	Vec3	vVelocity;
		

	float	Mass;
	float	Life;
	float	Age;
	float	NomarlizedAge;
	int		Active;

	float	padding;
};

// Particle SpawnCount
struct tSpawnCount
{
	int  SpawnCount;
	UINT padding[3];
};

// Particle Module
struct tParticleModule
{
	// Spawn
	UINT	SpawnRate;				// 초당 파티클 생성개수 (Spawn Per Second)
	Vec4	vSpawnColor;			// 생성 시점 색상
	Vec4	vSpawnMinScale;			// 생성 시 최소 크기
	Vec4	vSpawnMaxScale;			// 생성 시 최대 크기
		
	float	MinLife;				// 최소 수명
	float	MaxLife;				// 최대 수명

	UINT	SpawnShape;				// 0 : Box,  1: Sphere
	Vec3	SpawnShapeScale;		// SpawnShapeScale.x == Radius

	UINT	BlockSpawnShape;		// 0 : Box,  1: Sphere
	Vec3	BlockSpawnShapeScale;	// SpawnShapeScale.x == Radius

	// Spawn Burst
	UINT	SpawnBurstCount;		// 한번에 발생시키는 Particle 수
	UINT	SpawnBurstRepeat;
	float	SpawnBurstRepeatTime;

	// Add Velocity
	UINT	AddVelocityType;		// 0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed 
	Vec3	AddVelocityFixedDir;
	float	AddSpeed;

	// Module On / Off
	int		Module[(UINT)PARTICLE_MODULE::END];
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
	UINT	btex[16];
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
