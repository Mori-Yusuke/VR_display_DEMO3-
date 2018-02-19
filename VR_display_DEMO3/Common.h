
//
// Common.h
// ���ʂŎg�p����}�N����萔�̒�`
//

#pragma once

//-------------------------------------------------------------
// �f�o�b�O���[�h��ʒ萔
//-------------------------------------------------------------
#define DEBUG		true
#define NOTDEBUG	!DEBUG

//----------------------------------------------------------
// ���O���Q�Ɓi���ׂẴN���X�����ՓI�ɗ��p������̂͂����ŎQ�Ƃ��Ă����j
//----------------------------------------------------------
#include <tchar.h>		// UNICODE�Ή�
#include <windows.h>	// �E�B���h�E�֌W
#include <locale.h>		// UNICODE�Ή�
#include <mmsystem.h>	// timeGetTime
#include <stdio.h>		// sprintf, �t�@�C�����o��
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
// ��ʒ萔
//-------------------------------------------------------------
#define SCREEN_WIDTH	960		// �E�B���h�E�̕�
#define SCREEN_HEIGHT	540		// �E�B���h�E�̍���


//----------------------------------------------------------
// ����{�}�N��
//----------------------------------------------------------

// �������̉��
#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete [] (p);  (p)=NULL; } }

// �Q�ƃJ�E���^�̃f�N�������g
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }

// �G���[�̕񍐂ƃA�v���P�[�V�����̏I��
#define ERROR_EXIT() { int line = __LINE__; const char *file = __FILE__;\
	char msg[_MAX_FNAME + _MAX_EXT + 256];\
	char drive[_MAX_DRIVE];\
	char dir[_MAX_DIR];\
	char fname[_MAX_FNAME];\
	char ext[_MAX_EXT];\
	_splitpath(file, drive, dir, fname, ext);\
	sprintf(msg, "���炩�̃G���[�������������߃A�v���P�[�V�������I�����܂�\r\n"\
		"�t�@�C�� : %s%s\r\n"\
		"�s�ԍ� : %d", fname, ext, line);\
	MessageBox(NULL, msg, "Error", MB_OK | MB_ICONEXCLAMATION);\
	PostQuitMessage(1);\
}
