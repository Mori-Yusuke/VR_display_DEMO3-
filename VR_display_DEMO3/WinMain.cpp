
//-------------------------------------------------------------
// DirectX�T���v���v���O����
//-------------------------------------------------------------
#include "GameWindow.h"
#include "SampleCamera.h"

//-------------------------------------------------------------
// openCV�T���v���v���O����
//-------------------------------------------------------------
#include "openCVCamera.h"

//-------------------------------------------------------------
// ���֐��v���g�^�C�v
//-------------------------------------------------------------
int WINAPI			WinMain( HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR cmd_line, int cmd_show );
LRESULT CALLBACK	WindowProc(HWND h_window, UINT msg, WPARAM wParam, LPARAM lParam);


//-------------------------------------------------------------
// �A�v���P�[�V�����̃G���g���|�C���g
//-------------------------------------------------------------
// ����
//		h_instance		: ���݂̃C���X�^���X�̃n���h��
//		h_prev_instance	: �ȑO�̃C���X�^���X�̃n���h��
//		cmd_line		: �R�}���h���C���p�����[�^
//		cmd_show		: �E�B���h�E�̕\�����
// �߂�l
//		����������0
//-------------------------------------------------------------
int WINAPI	WinMain( HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR cmd_line, int cmd_show )
{
	//---------------------------------------------------------
	// �e�평���ݒ�
	//---------------------------------------------------------
	CoInitializeEx( NULL, COINIT_APARTMENTTHREADED );	// DirectX�̏�����
	timeBeginPeriod( 1 );								// ���Ԃ̃J�E���g�Ԋu��1ms�ɂ���
	srand( (DWORD)time( NULL ) );						// �����̏����l��ϓ�
	rand();rand();rand();rand();rand();					// �����͉��x����ǂ݂���Ɨǂ�

	// �Q�[���p�E�B���h�E�̐���
	GameWindow	game_window( SCREEN_WIDTH, SCREEN_HEIGHT, h_instance, cmd_line, cmd_show );

	// �T���v���̐���
	SampleCamera sample;
	sample.Create( game_window.GetDevice() );

	// openCV �J�����N���X
	openCVCamera cameracv;


	//---------------------------------------------------------
	// ���b�Z�[�W��������ѕ`�惋�[�v
	//---------------------------------------------------------
	DWORD	time_render	= timeGetTime();
	while( true )
	{
		DWORD	time_now	= timeGetTime();

		// ���`�揈��
		if( timeGetTime() > time_render )
		{
			// �E�B���h�E�������Ă��鎞�����`��
			if( game_window.IsVisible() ) 
			{
				cameracv.Tracking();
				sample.SetDxDy(cameracv.GetDx(), cameracv.GetDy());

				// �`�揈���̎��s
				game_window.ClearScene( D3DCOLOR_XRGB(0,0,0) );		// ��ʂ�������
				game_window.BeginScene();							// �`��̊J�n

				sample.Draw( game_window.GetDevice() );

				game_window.EndScene();								// �`����I�����A��ʂɓ]��

#if DEBUG
																	// FPS�̕\��
				TCHAR buff[80];
				wsprintf(buff, L"%d DX\n", cameracv.GetDx());
				OutputDebugString(buff);
				wsprintf(buff, L"%d DY\n", cameracv.GetDy());
				OutputDebugString(buff);
#endif // !NDEBUG
			}

			// ���̕`��\�莞����2ms�と�b�Ԗ�60��`�悷��
			time_render	+= 2;
		}

		// ���E�B���h�E���b�Z�[�W����
		if( !game_window.ProcMessage() )	break;

		// OS�ɐ���𖾂��n�����߂̃X���[�v�i�R�����Q�Ɓj
		Sleep(1);
    }


	//---------------------------------------------------------
	// �v���O�����I������
	//---------------------------------------------------------
	// DirectX�̏I������
	CoUninitialize();

	// �v���O�����I��
	return	0;
}

