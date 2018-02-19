
//-------------------------------------------------------------
// DirectXサンプルプログラム
//-------------------------------------------------------------
#include "GameWindow.h"
#include "SampleCamera.h"

//-------------------------------------------------------------
// openCVサンプルプログラム
//-------------------------------------------------------------
#include "openCVCamera.h"

//-------------------------------------------------------------
// ◆関数プロトタイプ
//-------------------------------------------------------------
int WINAPI			WinMain( HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR cmd_line, int cmd_show );
LRESULT CALLBACK	WindowProc(HWND h_window, UINT msg, WPARAM wParam, LPARAM lParam);


//-------------------------------------------------------------
// アプリケーションのエントリポイント
//-------------------------------------------------------------
// 引数
//		h_instance		: 現在のインスタンスのハンドル
//		h_prev_instance	: 以前のインスタンスのハンドル
//		cmd_line		: コマンドラインパラメータ
//		cmd_show		: ウィンドウの表示状態
// 戻り値
//		成功したら0
//-------------------------------------------------------------
int WINAPI	WinMain( HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR cmd_line, int cmd_show )
{
	//---------------------------------------------------------
	// 各種初期設定
	//---------------------------------------------------------
	CoInitializeEx( NULL, COINIT_APARTMENTTHREADED );	// DirectXの初期化
	timeBeginPeriod( 1 );								// 時間のカウント間隔を1msにする
	srand( (DWORD)time( NULL ) );						// 乱数の初期値を変動
	rand();rand();rand();rand();rand();					// 乱数は何度か空読みすると良い

	// ゲーム用ウィンドウの生成
	GameWindow	game_window( SCREEN_WIDTH, SCREEN_HEIGHT, h_instance, cmd_line, cmd_show );

	// サンプルの生成
	SampleCamera sample;
	sample.Create( game_window.GetDevice() );

	// openCV カメラクラス
	openCVCamera cameracv;


	//---------------------------------------------------------
	// メッセージ処理および描画ループ
	//---------------------------------------------------------
	DWORD	time_render	= timeGetTime();
	while( true )
	{
		DWORD	time_now	= timeGetTime();

		// ◆描画処理
		if( timeGetTime() > time_render )
		{
			// ウィンドウが見えている時だけ描画
			if( game_window.IsVisible() ) 
			{
				cameracv.Tracking();
				sample.SetDxDy(cameracv.GetDx(), cameracv.GetDy());

				// 描画処理の実行
				game_window.ClearScene( D3DCOLOR_XRGB(0,0,0) );		// 画面を初期化
				game_window.BeginScene();							// 描画の開始

				sample.Draw( game_window.GetDevice() );

				game_window.EndScene();								// 描画を終了し、画面に転写

#if DEBUG
																	// FPSの表示
				TCHAR buff[80];
				wsprintf(buff, L"%d DX\n", cameracv.GetDx());
				OutputDebugString(buff);
				wsprintf(buff, L"%d DY\n", cameracv.GetDy());
				OutputDebugString(buff);
#endif // !NDEBUG
			}

			// 次の描画予定時刻は2ms後→秒間約60回描画する
			time_render	+= 2;
		}

		// ◆ウィンドウメッセージ処理
		if( !game_window.ProcMessage() )	break;

		// OSに制御を明け渡すためのスリープ（コラム参照）
		Sleep(1);
    }


	//---------------------------------------------------------
	// プログラム終了処理
	//---------------------------------------------------------
	// DirectXの終了処理
	CoUninitialize();

	// プログラム終了
	return	0;
}

