
//
// GameWindow.cpp
// �A�v���P�[�V����(�E�B���h�E)�̎���
//

#include "GameWindow.h"



GameWindow::GameWindow()
	:	m_h_instance( NULL )
	,	m_h_window( NULL )
	,	m_is_full_screen( false )
	,	m_width( 0 )
	,	m_height( 0 )
	,	m_d3d( NULL )
	,	m_d3d_dev( NULL )
{
}

GameWindow::GameWindow( WORD width, WORD height, HINSTANCE h_instance, LPSTR cmd_line, int cmd_show )
	:	m_h_instance( h_instance )
	,	m_h_window( NULL )
	,	m_is_full_screen( false )
	,	m_width( width )
	,	m_height( height )
	,	m_d3d( NULL )
	,	m_d3d_dev( NULL )
{
	SetFullScreenFromCommand( cmd_line );
	CreateGameWindow( width, height, h_instance, cmd_show );
	InitializeDirect3D();
}

GameWindow::~GameWindow()
{
}



//-------------------------------------------------------------
// ���b�Z�[�W�����p�R�[���o�b�N�֐�
// ����
//		h_window	: �E�B���h�E�n���h��
//		msg		: ���b�Z�[�W
//		wParam	: ���b�Z�[�W�̍ŏ��̃p�����[�^
//		lParam	: ���b�Z�[�W��2�Ԗڂ̃p�����[�^
// �߂�l
//		���b�Z�[�W��������
//-------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND h_window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
	{
		case	WM_CLOSE:
		{
			// ���E�C���h�E������ꂽ
			PostQuitMessage( 0 );			// �A�v���P�[�V�������I������
			break;
		}
		case	WM_KEYDOWN:
		{
			// ���L�[�������ꂽ
			if (wParam == VK_ESCAPE)
			{
				// ESC�L�[
				PostQuitMessage( 0 );		// �A�v���P�[�V�������I������
			}
			break;
		}
		default:
		{
			return	DefWindowProc(h_window, msg, wParam, lParam);
		}
	}

    return	0;
}



//---------------------------------------------------------
// �Q�[���p�E�B���h�E�̂��߂̃E�B���h�E�N���X�̐���
//---------------------------------------------------------
bool	GameWindow::RegisterGameWindow( HINSTANCE h_instance )
{
	// �E�B���h�E�N���X�̏�����
	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),				// ���̍\���̂̃T�C�Y
		NULL,							// �E�C���h�E�̃X�^�C��(default)
		WindowProc,						// ���b�Z�[�W�����֐��̓o�^
		0,								// �ʏ�͎g��Ȃ��̂ŏ��0
		0,								// �ʏ�͎g��Ȃ��̂ŏ��0
		h_instance,						// �C���X�^���X�ւ̃n���h��
		NULL,							// �A�C�R���i�Ȃ��j
		LoadCursor(NULL, IDC_ARROW),	// �J�[�\���̌`
		NULL, NULL,						// �w�i�Ȃ��A���j���[�Ȃ�
		WINDOW_CLASS_NAME,				// �N���X���̎w��
		NULL							// ���A�C�R���i�Ȃ��j
	};

	// �E�B���h�E�N���X�̓o�^
	if( RegisterClassEx( &wcex ) == 0 )
	{
		return	false;	// �o�^���s
	}

	return	true;
}



//---------------------------------------------------------
// �Q�[���p�E�B���h�E�̐���
//---------------------------------------------------------
HWND	GameWindow::CreateGameWindow( WORD width, WORD height, HINSTANCE h_instance, int cmd_show )
{
	// �E�B���h�E�N���X�̓o�^
	if( !RegisterGameWindow( h_instance ) )
	{
		return	NULL;
	}

	// �e����̓o�^
	m_h_instance	= h_instance;
	m_width			= width;
	m_height		= height;

	// �E�B���h�E�̍쐬
	if( m_is_full_screen ) 
	{
		// �t���X�N���[��
		int		sw, sh;

		// ��ʑS�̂̕��ƍ������擾
		sw	= GetSystemMetrics(SM_CXSCREEN);
		sh	= GetSystemMetrics(SM_CYSCREEN);

		m_h_window = CreateWindow( 
					WINDOW_CLASS_NAME, 		// �o�^����Ă���N���X��
					WINDOW_NAME, 			// �E�C���h�E��
					WS_POPUP,				// �E�C���h�E�X�^�C���i�|�b�v�A�b�v�E�C���h�E���쐬�j
					0, 						// �E�C���h�E�̉������̈ʒu
					0, 						// �E�C���h�E�̏c�����̈ʒu
					width, 					// �E�C���h�E�̕�
					height,					// �E�C���h�E�̍���
					NULL,					// �e�E�C���h�E�̃n���h���i�ȗ��j
					NULL,					// ���j���[��q�E�C���h�E�̃n���h��
					h_instance, 				// �A�v���P�[�V�����C���X�^���X�̃n���h��
					NULL					// �E�C���h�E�̍쐬�f�[�^
				);
	}
	else
	{
		// �ʏ�E�B���h�E
		m_h_window = CreateWindow(WINDOW_CLASS_NAME, 
							WINDOW_NAME, 
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, CW_USEDEFAULT, 
							CW_USEDEFAULT, CW_USEDEFAULT,
							NULL, NULL, h_instance, NULL);

		// �E�B���h�E�T�C�Y���Đݒ肷��
		RECT	rect;
		int		ww, wh, cw, ch;

		// �N���C�A���g�̈�̊O�̕����v�Z
		GetClientRect( m_h_window, &rect );	// �N���C�A���g�����̃T�C�Y�̎擾
		cw	= rect.right - rect.left;		// �N���C�A���g�̈�O�̉������v�Z
		ch	= rect.bottom - rect.top;		// �N���C�A���g�̈�O�̏c�����v�Z

		// �E�C���h�E�S�̂̉����̕����v�Z
		GetWindowRect( m_h_window, &rect );	// �E�C���h�E�S�̂̃T�C�Y�擾
		ww	= rect.right - rect.left;		// �E�C���h�E�S�̂̕��̉������v�Z
		wh	= rect.bottom - rect.top;		// �E�C���h�E�S�̂̕��̏c�����v�Z
		ww	= ww - cw;						// �N���C�A���g�̈�ȊO�ɕK�v�ȕ�
		wh	= wh - ch;						// �N���C�A���g�̈�ȊO�ɕK�v�ȍ���

		// �E�B���h�E�T�C�Y�̍Čv�Z
		ww	= width + ww;					// �K�v�ȃE�C���h�E�̕�
		wh	= height + wh;					// �K�v�ȃE�C���h�E�̍���

		// �E�C���h�E�T�C�Y�̍Đݒ�
		SetWindowPos( m_h_window, HWND_TOP, 0, 0, ww, wh, SWP_NOMOVE );
	}

	if( m_h_window )
	{
		// �E�B���h�E�̕\��
		ShowWindow( m_h_window, cmd_show );

		// WM_PAINT���Ă΂�Ȃ��悤�ɂ���
		ValidateRect( m_h_window, 0 );
	}

	return	m_h_window;
}



bool			GameWindow::ProcMessage( void )
{
	MSG		msg;
	if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
	{
		if( msg.message == WM_QUIT ) 
		{	
			// PostQuitMessage()���Ă΂ꂽ
			return	false;		// �v���O�����̃��C�����[�v�𔲂��A�I������
		}
		else 
		{
			// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
    }

	return	true;
}



bool			GameWindow::IsVisible( void ) const 
{
	WINDOWPLACEMENT	wndpl;
	GetWindowPlacement( m_h_window, &wndpl );

	// �E�B���h�E�������Ă��鎞�����`�悷�邽�߂̏���
	return( (wndpl.showCmd != SW_HIDE) && 
		(wndpl.showCmd != SW_MINIMIZE) &&
		(wndpl.showCmd != SW_SHOWMINIMIZED) &&
		(wndpl.showCmd != SW_SHOWMINNOACTIVE) );
}



void			GameWindow::SetFullScreenFromCommand( LPSTR cmd_line )
{
	// �t���X�N���[���ɂ��邩�ǂ����̔���
	// �R�}���h���C����/f��/F���ݒ肳��Ă�����t���X�N���[���ɂ���
	m_is_full_screen	= false;
	if( strlen(cmd_line) < 2 )	return;

	for( DWORD i = 0; i < strlen(cmd_line)-2; i++ )
	{
		if(_stricmp((char*)&cmd_line[i], "/f") == 0)
		{
			// �R�}���h���C����/f�𔭌�
			m_is_full_screen	= true;	 // �t���O��TRUE�ɐݒ�
			break;
		}
    }
}



HRESULT			GameWindow::InitializeDirect3D( void )
{
	if( !IsValidWindow() )		return	E_FAIL;

	// Direct3D9�I�u�W�F�N�g�̍쐬
	if( (m_d3d = ::Direct3DCreate9(D3D_SDK_VERSION)) == 0 )		return	E_FAIL;	// �擾���s

	// ���݂̃f�B�X�v���C���[�h���擾
	D3DDISPLAYMODE	d3ddm;
    if( FAILED( m_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm ) ) )	return	E_FAIL;

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^��������
	ZeroMemory( &m_d3d_pp, sizeof(D3DPRESENT_PARAMETERS) );
	m_d3d_pp.BackBufferCount		= 1;
	if(m_is_full_screen) { // �t���X�N���[���̏ꍇ
		m_d3d_pp.Windowed			= FALSE;		// �t���X�N���[���\���̎w��
		m_d3d_pp.BackBufferWidth	= m_width;		// �t���X�N���[�����̉���
		m_d3d_pp.BackBufferHeight	= m_height;		// �t���X�N���[�����̏c��
	}
	else {
		m_d3d_pp.Windowed			= TRUE;			// �E�C���h�E���\���̎w��
	}
	m_d3d_pp.BackBufferFormat		= d3ddm.Format;				// �J���[���[�h�̎w��
	m_d3d_pp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 
	m_d3d_pp.EnableAutoDepthStencil	= TRUE;						// �G���[�΍�
	m_d3d_pp.AutoDepthStencilFormat	= D3DFMT_D16;				// �G���[�΍�
	
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if( FAILED( m_d3d->CreateDevice( D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									m_h_window, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, 
									&m_d3d_pp, &m_d3d_dev) ) ) {
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if( FAILED( m_d3d->CreateDevice( D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										m_h_window, 
										D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, 
										&m_d3d_pp, &m_d3d_dev) ) ) {
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if( FAILED( m_d3d->CreateDevice( D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF, m_h_window, 
											D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, 
											&m_d3d_pp, &m_d3d_dev) ) ) {
				// ���������s
				return E_FAIL;
			}
		}
	}

	return S_OK;
}



void			GameWindow::FinalizeDirect3D( void )
{
	// �f�o�C�X�I�u�W�F�N�g�̉��
	SAFE_RELEASE( m_d3d_dev );

	// DirectXGraphics�̉��
	SAFE_RELEASE( m_d3d );
}



bool			GameWindow::ClearScene( D3DXCOLOR color )
{
	// �r���[�|�[�g�Ɛ[�x�o�b�t�@�̃N���A�ƃX�e���V���o�b�t�@�̍폜
	if( FAILED( m_d3d_dev->Clear( 0,NULL,		// �N���A����̈�͑S��
					D3DCLEAR_TARGET |			// �o�b�N�o�b�t�@���w��
					D3DCLEAR_ZBUFFER, 			// �[�x�o�b�t�@�iZ�o�b�t�@�j���w��
					color,						// �N���A����F
					1.0f, 						// �N���A����[�x�o�b�t�@�iZ�o�b�t�@�j�̒l
					0 ) ) ) {					// �N���A����X�e���V���o�b�t�@�̒l
		return	false;
	}

	return	true;
}



bool			GameWindow::BeginScene( void )
{
	return	SUCCEEDED( m_d3d_dev->BeginScene() );
}



bool			GameWindow::EndScene( void )
{
	bool	succeeded	= true;

	// �`��I���錾
	succeeded	&= SUCCEEDED( m_d3d_dev->EndScene() );

	// �`�挋�ʂ̓]��
	succeeded	&= SUCCEEDED( m_d3d_dev->Present( 0, 0, 0, 0 ) );
	if( !succeeded )
	{
		// �f�o�C�X�������畜�A
		m_d3d_dev->Reset(&m_d3d_pp);
	}

	return	succeeded;
}

