#pragma once

class CEngine
	: public CSingleton<CEngine>
{
	SINGLE(CEngine)
private:
	HWND		m_hMainHwnd;
	Vec2		m_Resolution;

public:
	HWND GetMainWnd() { return m_hMainHwnd; }
	int init(HWND _hWnd, Vec2 _Resolution);
	void progress();
};

