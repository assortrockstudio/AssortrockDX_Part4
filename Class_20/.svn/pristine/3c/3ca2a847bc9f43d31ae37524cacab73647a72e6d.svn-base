#include "pch.h"

#include "CDbgRenderMgr.h"
#include "CTaskMgr.h"

#include "CGameObject.h"

void SpawnObject(int _LayerIdx, CGameObject* _Object)
{
	tTask task = {};
	task.Type = TASK_TYPE::SPAWN_OBJECT;
	task.dwParam_0 = _LayerIdx;
	task.dwParam_1 = (DWORD_PTR)_Object;

	CTaskMgr::GetInst()->AddTask(task);
}

bool IsValid(CGameObject*& _Object)
{
	if (nullptr == _Object)
	{		
		return false;
	}
	else if (_Object->IsDead())
	{
		_Object = nullptr;
		return false;
	}
	else
	{
		return true;
	}
}



void DrawDebugRect(Vec3 _WorldPos, Vec3 _WorldScale, Vec3 _WorldRotation, Vec4 _vColor, float _Duration)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::RECT;

	info.Position = _WorldPos;
	info.Scale = _WorldScale;
	info.Rotation = _WorldRotation;
	info.matWorld = XMMatrixIdentity();
	info.Color = _vColor;
	info.Duration = _Duration;
	info.Age = 0.f;

	CDbgRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugRect(Matrix _matWorld, Vec4 _vColor, float _Duration)
{
	tDebugShapeInfo info = {};

	info.Shape = DEBUG_SHAPE::RECT;
	info.matWorld = _matWorld;
	info.Color = _vColor;
	info.Duration = _Duration;
	info.Age = 0.f;

	CDbgRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCircle(Vec3 _WorldPos, float _Radius, Vec4 _vColor, float _Duration)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::CIRCLE;

	info.Position = _WorldPos;
	info.Scale = Vec3(_Radius * 2.f, _Radius * 2.f, 1.f);	
	info.matWorld = XMMatrixIdentity();

	info.Color = _vColor;
	info.Duration = _Duration;
	info.Age = 0.f;

	CDbgRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void SaveWString(const wstring& _Str, FILE* _File)
{
	size_t len = _Str.length();
	fwrite(&len, sizeof(size_t), 1, _File);
	fwrite(_Str.c_str(), sizeof(wchar_t), len, _File);
}

void LoadWString(wstring& _str, FILE* _File)
{
	size_t len = 0;
	fread(&len, sizeof(size_t), 1, _File);
	_str.resize(len);
	fread((wchar_t*)_str.c_str(), sizeof(wchar_t), len, _File);
}
