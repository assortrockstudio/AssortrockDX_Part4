#pragma once

// ����
// 3D ������ ��ǥ�� ��Ÿ���� ���� 
struct Vtx
{	
	Vec3	vPos;
	Vec4	vColor;
	Vec2	vUV;
};


// ����� ���� ��û ����
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


// ���� ���� ����ü
struct tLight
{
	Vec4	vDiffuse;		// ���ݻ� (���� ����)
	Vec4	vAmbient;		// ȯ�汤 (������ �߻��ϴ� �ּ����� ���)
	Vec4	vMaxSpecular;	// �ݻ籤 (������ �߻��ϴ� �ݻ籤�� �ִ� ���)
};

struct tLightInfo
{
	tLight	Light;

	UINT	LightType;	// 0 : DirectionalLight, 1 : PointLight, 2 : SpotLight
	Vec3	WorldDir;	// WorldSpace ���� ������ ����
	Vec3	WorldPos;	// WorldSpace ���� ������ ��ġ
	float	Range;		// SpotLight or PointLight �� ������ ���� ����
	float	Angle;		// Spot Light �� ��� ������ ��������

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
	UINT	SpawnRate;				// �ʴ� ��ƼŬ �������� (Spawn Per Second)
	Vec4	vSpawnColor;			// ���� ���� ����
	Vec4	vSpawnMinScale;			// ���� �� �ּ� ũ��
	Vec4	vSpawnMaxScale;			// ���� �� �ִ� ũ��
		
	float	MinLife;				// �ּ� ����
	float	MaxLife;				// �ִ� ����

	UINT	SpawnShape;				// 0 : Box,  1: Sphere
	Vec3	SpawnShapeScale;		// SpawnShapeScale.x == Radius

	UINT	BlockSpawnShape;		// 0 : Box,  1: Sphere
	Vec3	BlockSpawnShapeScale;	// SpawnShapeScale.x == Radius

	// Spawn Burst
	UINT	SpawnBurstCount;		// �ѹ��� �߻���Ű�� Particle ��
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
// ������� ���� ����ü
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
	Vec2	vResolution;	// ������ �ػ�
	float	DeltaTime;		// DT
	float	Time;			// ���� �������� ���� �ð�

	int		Light2DCount;
	int		Light3DCount;

	Vec2	Padding;
};

extern tGlobalData g_GlobalData;