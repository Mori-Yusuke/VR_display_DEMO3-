
#pragma once


// include
#include "Common.h"


// define
#define WINDOW_NAME				L"DEMO Sample"			// ウィンドウ名
#define WINDOW_CLASS_NAME		L"DEMO_Sample_Class"	// ウィンドウ名


class	GameWindow
{
protected:
	HINSTANCE	m_h_instance;
	HWND		m_h_window;
	bool		m_is_full_screen;
	WORD		m_width;
	WORD		m_height;

	LPDIRECT3D9				m_d3d;		// IDirect3D9インターフェイスへのポインタ
	LPDIRECT3DDEVICE9		m_d3d_dev;	// IDirect3DDevice9インターフェイスへのポインタ
	D3DPRESENT_PARAMETERS	m_d3d_pp;	// デバイスのプレゼンテーションパラメータ

public:
	GameWindow();
	GameWindow( WORD width, WORD height, HINSTANCE h_instance, LPSTR cmd_line, int cmd_show );
	~GameWindow();

// 関数群
public:
	bool	RegisterGameWindow( HINSTANCE h_instance );
	HWND	CreateGameWindow( WORD width, WORD height, HINSTANCE h_instance, int cmd_show );
	bool	ProcMessage( void );

	bool		IsValidWindow( void ) const		{ return m_h_window && m_width && m_height; };
	bool		IsVisible( void ) const;
	bool		IsFullScreen( void ) const		{ return m_is_full_screen; };
	HWND		GetHWND( void ) const			{ return m_h_window; };
	HINSTANCE	GetHINSTANCE( void ) const		{ return m_h_instance; };

	void	SetFullScreenFromCommand( LPSTR cmd_line );

public:
	HRESULT	InitializeDirect3D( void );
	void	FinalizeDirect3D( void );

	LPDIRECT3DDEVICE9	GetDevice() const				{ return m_d3d_dev; };
	bool				IsValidDirect3D( void ) const	{ return m_d3d && m_d3d_dev; };

	bool	ClearScene( D3DXCOLOR color );
	bool	BeginScene( void );
	bool	EndScene( void );
};
