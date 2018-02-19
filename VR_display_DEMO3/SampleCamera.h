
#include "Common.h"


class	SampleCamera
{
// メンバ変数
private:
	// カメラ座標
	float camX = 0.0f;				// 初期カメラX座標
	float camY = 20.0f;				// 初期カメラY座標
	float dx = 0, dy = 0;			// カメラの移動量

	// 座標変換
	D3DXMATRIX		m_proj;			// 射影行列
	D3DXMATRIX		m_view;			// ビューイング行列

									// ◆メッシュデータ変数
	LPD3DXMESH          m_mesh;         // メッシュデータのポインタ
	DWORD               m_num_mesh;     // メッシュ情報の総数
	LPD3DXBUFFER        m_mat_buf;      // メッシュのマテリアル情報
	LPDIRECT3DTEXTURE9  *m_textures;    // テクスチャ配列


// コンストラクタ・デストラクタ
public:
	SampleCamera();
	~SampleCamera();


// メンバ関数
public:
	HRESULT	Create( LPDIRECT3DDEVICE9 d3d_dev );	// シーンの生成
	HRESULT	Destroy( void );						// シーンの破棄
	HRESULT	Transform( LPDIRECT3DDEVICE9 d3d_dev );	// 座標変換処理
	void	SetDxDy(float, float);					// dx,dy を入れる
	HRESULT	Draw( LPDIRECT3DDEVICE9 d3d_dev );		// シーンの描画
};