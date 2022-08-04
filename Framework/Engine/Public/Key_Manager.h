#pragma once
#include "Base.h"

BEGIN(Engine)
enum class KEY
{
	LEFT = 0,
	RIGHT,
	UP,
	DOWN,
	NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, NUM0,
	F1, F2, F3, F4, F5, F6, F7, F8, F9,
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,
	ALT,
	LSHIFT,
	SPACE,
	CTRL,
	ENTER,
	ESC,
	TAB,
	LBUTTON,
	RBUTTON,
	BACKSPACE,
	DELETEKEY,
	LAST,
};

enum class KEY_STATE {
	TAP, // �� ���� ����
	HOLD,// �� ������ ����
	AWAY,// �� �� ����
	NONE,// �� �ȴ��� ����
};



class CKey_Manager final : public CBase
{
	DECLARE_SINGLETON(CKey_Manager);
private:
	CKey_Manager();
	~CKey_Manager() = default;

public:
	KEY_STATE				Get_KeyState(KEY _key);

public:
	HRESULT		Initialize(HWND hWnd);
	void		Tick();
	const POINT&	Get_MousePos() { return m_PtMouse; }
private:
	struct tKeyInfo
	{
		KEY_STATE eState;
		bool bPrevPush;
	};
	HWND				m_hWnd = 0;
	vector<tKeyInfo>	m_vecKey;
	POINT				m_PtMouse;
};

END