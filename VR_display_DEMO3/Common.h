
//
// Common.h
// 共通で使用するマクロや定数の定義
//

#pragma once

//-------------------------------------------------------------
// デバッグモード画面定数
//-------------------------------------------------------------
#define DEBUG		true
#define NOTDEBUG	!DEBUG

//----------------------------------------------------------
// ◆外部参照（すべてのクラスが普遍的に利用するものはここで参照しておく）
//----------------------------------------------------------
#include <tchar.h>		// UNICODE対応
#include <windows.h>	// ウィンドウ関係
#include <locale.h>		// UNICODE対応
#include <mmsystem.h>	// timeGetTime
#include <stdio.h>		// sprintf, ファイル入出力
#include <stdlib.h>		// rand, srand
#include <time.h>		// time

#include <d3d9.h>		// Direct3D
#include <d3dx9.h>


#if DEBUG
	#pragma comment( lib, "d3dx9d.lib" )
	#pragma comment(lib, "ole32.lib")
	#pragma comment(lib, "oleaut32.lib")
#endif
#if NOTDEBUG
	#pragma comment( lib, "d3dx9.lib" )
#endif // DEBUG


#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "winmm.lib" )

//-------------------------------------------------------------
// 画面定数
//-------------------------------------------------------------
#define SCREEN_WIDTH	960		// ウィンドウの幅
#define SCREEN_HEIGHT	540		// ウィンドウの高さ


//----------------------------------------------------------
// ◆基本マクロ
//----------------------------------------------------------

// メモリの解放
#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete [] (p);  (p)=NULL; } }

// 参照カウンタのデクリメント
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }

// エラーの報告とアプリケーションの終了
#define ERROR_EXIT() { int line = __LINE__; const char *file = __FILE__;\
	char msg[_MAX_FNAME + _MAX_EXT + 256];\
	char drive[_MAX_DRIVE];\
	char dir[_MAX_DIR];\
	char fname[_MAX_FNAME];\
	char ext[_MAX_EXT];\
	_splitpath(file, drive, dir, fname, ext);\
	sprintf(msg, "何らかのエラーが発生したためアプリケーションを終了します\r\n"\
		"ファイル : %s%s\r\n"\
		"行番号 : %d", fname, ext, line);\
	MessageBox(NULL, msg, "Error", MB_OK | MB_ICONEXCLAMATION);\
	PostQuitMessage(1);\
}
