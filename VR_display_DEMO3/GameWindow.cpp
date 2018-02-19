
//
// GameWindow.cpp
// アプリケーション(ウィンドウ)の実装
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
// メッセージ処理用コールバック関数
// 引数
//		h_window	: ウィンドウハンドル
//		msg		: メッセージ
//		wParam	: メッセージの最初のパラメータ
//		lParam	: メッセージの2番目のパラメータ
// 戻り値
//		メッセージ処理結果
//-------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND h_window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
	{
		case	WM_CLOSE:
		{
			// ◆ウインドウが閉じられた
			PostQuitMessage( 0 );			// アプリケーションを終了する
			break;
		}
		case	WM_KEYDOWN:
		{
			// ◆キーが押された
			if (wParam == VK_ESCAPE)
			{
				// ESCキー
				PostQuitMessage( 0 );		// アプリケーションを終了する
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
// ゲーム用ウィンドウのためのウィンドウクラスの生成
//---------------------------------------------------------
bool	GameWindow::RegisterGameWindow( HINSTANCE h_instance )
{
	// ウィンドウクラスの初期化
	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),				// この構造体のサイズ
		NULL,							// ウインドウのスタイル(default)
		WindowProc,						// メッセージ処理関数の登録
		0,								// 通常は使わないので常に0
		0,								// 通常は使わないので常に0
		h_instance,						// インスタンスへのハンドル
		NULL,							// アイコン（なし）
		LoadCursor(NULL, IDC_ARROW),	// カーソルの形
		NULL, NULL,						// 背景なし、メニューなし
		WINDOW_CLASS_NAME,				// クラス名の指定
		NULL							// 小アイコン（なし）
	};

	// ウィンドウクラスの登録
	if( RegisterClassEx( &wcex ) == 0 )
	{
		return	false;	// 登録失敗
	}

	return	true;
}



//---------------------------------------------------------
// ゲーム用ウィンドウの生成
//---------------------------------------------------------
HWND	GameWindow::CreateGameWindow( WORD width, WORD height, HINSTANCE h_instance, int cmd_show )
{
	// ウィンドウクラスの登録
	if( !RegisterGameWindow( h_instance ) )
	{
		return	NULL;
	}

	// 各種情報の登録
	m_h_instance	= h_instance;
	m_width			= width;
	m_height		= height;

	// ウィンドウの作成
	if( m_is_full_screen ) 
	{
		// フルスクリーン
		int		sw, sh;

		// 画面全体の幅と高さを取得
		sw	= GetSystemMetrics(SM_CXSCREEN);
		sh	= GetSystemMetrics(SM_CYSCREEN);

		m_h_window = CreateWindow( 
					WINDOW_CLASS_NAME, 		// 登録されているクラス名
					WINDOW_NAME, 			// ウインドウ名
					WS_POPUP,				// ウインドウスタイル（ポップアップウインドウを作成）
					0, 						// ウインドウの横方向の位置
					0, 						// ウインドウの縦方向の位置
					width, 					// ウインドウの幅
					height,					// ウインドウの高さ
					NULL,					// 親ウインドウのハンドル（省略）
					NULL,					// メニューや子ウインドウのハンドル
					h_instance, 				// アプリケーションインスタンスのハンドル
					NULL					// ウインドウの作成データ
				);
	}
	else
	{
		// 通常ウィンドウ
		m_h_window = CreateWindow(WINDOW_CLASS_NAME, 
							WINDOW_NAME, 
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, CW_USEDEFAULT, 
							CW_USEDEFAULT, CW_USEDEFAULT,
							NULL, NULL, h_instance, NULL);

		// ウィンドウサイズを再設定する
		RECT	rect;
		int		ww, wh, cw, ch;

		// クライアント領域の外の幅を計算
		GetClientRect( m_h_window, &rect );	// クライアント部分のサイズの取得
		cw	= rect.right - rect.left;		// クライアント領域外の横幅を計算
		ch	= rect.bottom - rect.top;		// クライアント領域外の縦幅を計算

		// ウインドウ全体の横幅の幅を計算
		GetWindowRect( m_h_window, &rect );	// ウインドウ全体のサイズ取得
		ww	= rect.right - rect.left;		// ウインドウ全体の幅の横幅を計算
		wh	= rect.bottom - rect.top;		// ウインドウ全体の幅の縦幅を計算
		ww	= ww - cw;						// クライアント領域以外に必要な幅
		wh	= wh - ch;						// クライアント領域以外に必要な高さ

		// ウィンドウサイズの再計算
		ww	= width + ww;					// 必要なウインドウの幅
		wh	= height + wh;					// 必要なウインドウの高さ

		// ウインドウサイズの再設定
		SetWindowPos( m_h_window, HWND_TOP, 0, 0, ww, wh, SWP_NOMOVE );
	}

	if( m_h_window )
	{
		// ウィンドウの表示
		ShowWindow( m_h_window, cmd_show );

		// WM_PAINTが呼ばれないようにする
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
			// PostQuitMessage()が呼ばれた
			return	false;		// プログラムのメインループを抜け、終了する
		}
		else 
		{
			// メッセージの翻訳とディスパッチ
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

	// ウィンドウが見えている時だけ描画するための処理
	return( (wndpl.showCmd != SW_HIDE) && 
		(wndpl.showCmd != SW_MINIMIZE) &&
		(wndpl.showCmd != SW_SHOWMINIMIZED) &&
		(wndpl.showCmd != SW_SHOWMINNOACTIVE) );
}



void			GameWindow::SetFullScreenFromCommand( LPSTR cmd_line )
{
	// フルスクリーンにするかどうかの判定
	// コマンドラインに/fか/Fが設定されていたらフルスクリーンにする
	m_is_full_screen	= false;
	if( strlen(cmd_line) < 2 )	return;

	for( DWORD i = 0; i < strlen(cmd_line)-2; i++ )
	{
		if(_stricmp((char*)&cmd_line[i], "/f") == 0)
		{
			// コマンドラインに/fを発見
			m_is_full_screen	= true;	 // フラグをTRUEに設定
			break;
		}
    }
}



HRESULT			GameWindow::InitializeDirect3D( void )
{
	if( !IsValidWindow() )		return	E_FAIL;

	// Direct3D9オブジェクトの作成
	if( (m_d3d = ::Direct3DCreate9(D3D_SDK_VERSION)) == 0 )		return	E_FAIL;	// 取得失敗

	// 現在のディスプレイモードを取得
	D3DDISPLAYMODE	d3ddm;
    if( FAILED( m_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm ) ) )	return	E_FAIL;

	// デバイスのプレゼンテーションパラメータを初期化
	ZeroMemory( &m_d3d_pp, sizeof(D3DPRESENT_PARAMETERS) );
	m_d3d_pp.BackBufferCount		= 1;
	if(m_is_full_screen) { // フルスクリーンの場合
		m_d3d_pp.Windowed			= FALSE;		// フルスクリーン表示の指定
		m_d3d_pp.BackBufferWidth	= m_width;		// フルスクリーン時の横幅
		m_d3d_pp.BackBufferHeight	= m_height;		// フルスクリーン時の縦幅
	}
	else {
		m_d3d_pp.Windowed			= TRUE;			// ウインドウ内表示の指定
	}
	m_d3d_pp.BackBufferFormat		= d3ddm.Format;				// カラーモードの指定
	m_d3d_pp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 
	m_d3d_pp.EnableAutoDepthStencil	= TRUE;						// エラー対策
	m_d3d_pp.AutoDepthStencilFormat	= D3DFMT_D16;				// エラー対策
	
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if( FAILED( m_d3d->CreateDevice( D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									m_h_window, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, 
									&m_d3d_pp, &m_d3d_dev) ) ) {
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if( FAILED( m_d3d->CreateDevice( D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										m_h_window, 
										D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, 
										&m_d3d_pp, &m_d3d_dev) ) ) {
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if( FAILED( m_d3d->CreateDevice( D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF, m_h_window, 
											D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, 
											&m_d3d_pp, &m_d3d_dev) ) ) {
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	return S_OK;
}



void			GameWindow::FinalizeDirect3D( void )
{
	// デバイスオブジェクトの解放
	SAFE_RELEASE( m_d3d_dev );

	// DirectXGraphicsの解放
	SAFE_RELEASE( m_d3d );
}



bool			GameWindow::ClearScene( D3DXCOLOR color )
{
	// ビューポートと深度バッファのクリアとステンシルバッファの削除
	if( FAILED( m_d3d_dev->Clear( 0,NULL,		// クリアする領域は全面
					D3DCLEAR_TARGET |			// バックバッファを指定
					D3DCLEAR_ZBUFFER, 			// 深度バッファ（Zバッファ）を指定
					color,						// クリアする色
					1.0f, 						// クリアする深度バッファ（Zバッファ）の値
					0 ) ) ) {					// クリアするステンシルバッファの値
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

	// 描画終了宣言
	succeeded	&= SUCCEEDED( m_d3d_dev->EndScene() );

	// 描画結果の転送
	succeeded	&= SUCCEEDED( m_d3d_dev->Present( 0, 0, 0, 0 ) );
	if( !succeeded )
	{
		// デバイス消失から復帰
		m_d3d_dev->Reset(&m_d3d_pp);
	}

	return	succeeded;
}

