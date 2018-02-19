
#include "SampleCamera.h"


//-------------------------------------------------------------
// デフォルトコンストラクタ
//-------------------------------------------------------------
SampleCamera::SampleCamera()
	: m_mesh(NULL)
	, m_mat_buf(NULL)
	, m_textures(NULL)
{
}



//-------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------
SampleCamera::~SampleCamera()
{
	Destroy();
}



//----------------------------------------------------------
// シーンの生成
//----------------------------------------------------------
HRESULT SampleCamera::Create( LPDIRECT3DDEVICE9 d3d_dev )
{
	Destroy();
	if (d3d_dev == NULL)	return	E_FAIL;

	//----------------------------------------------------------
	// ■メッシュを生成
	//----------------------------------------------------------

	// ◆Xファイル名をここに格納
	wchar_t		path_x[MAX_PATH] = L"sensi/sensi.x";

	// メッシュファイルのディレクトリ部分を抽出（path_file_partに先頭アドレス）
	wchar_t		path_full[MAX_PATH];
	wchar_t		*path_file_part;
	GetFullPathName(path_x, MAX_PATH, path_full, &path_file_part);

	//----------------------------------------------------------
	// ◆Xファイルを読み出し、メッシュ・マテリアル・テクスチャ情報を読み取る
	//----------------------------------------------------------
	if (FAILED(D3DXLoadMeshFromX(
		path_x,             // Xファイルのパス
		D3DXMESH_SYSTEMMEM,
		d3d_dev,
		NULL,
		&m_mat_buf,         // マテリアル情報を指すポインタのアドレス
		NULL,
		&m_num_mesh,        // メッシュ数が返ってくる
		&m_mesh)))			// メッシュ情報を指すポインタのアドレス
	{
		return  E_FAIL;
	}

	// マテリアルとテクスチャの設定
	D3DXMATERIAL    *materials = (D3DXMATERIAL*)m_mat_buf->GetBufferPointer();
	m_textures = new LPDIRECT3DTEXTURE9[m_num_mesh];

	// テクスチャの読み出し
	for (DWORD i = 0; i<m_num_mesh; i++)
	{
		m_textures[i] = NULL;
		if (materials[i].pTextureFilename != NULL)
		{
			// Unicode文字列へ変換
			size_t  path_converted;
			wchar_t path_tex[MAX_PATH];
			mbstowcs_s(&path_converted, path_tex, materials[i].pTextureFilename, MAX_PATH);
			if (wcslen(path_tex) > 0)
			{
				// Xファイルのディレクトリ内で指定する
				wcscpy_s(path_file_part, MAX_PATH - wcslen(path_full), path_tex);

				// テクスチャの読み込み
				D3DXCreateTextureFromFile(
					d3d_dev,
					path_full,          // テクスチャファイル名
					&m_textures[i]);   // テクスチャオブジェクト
			}
		}
	}

	// テクスチャの読み出し
	for (DWORD i = 0; i<m_num_mesh; i++)
	{
		m_textures[i] = NULL;
		if (materials[i].pTextureFilename != NULL)
		{
			// Unicode文字列へ変換
			size_t	path_converted;
			wchar_t	path_tex[MAX_PATH];
			mbstowcs_s(&path_converted, path_tex, materials[i].pTextureFilename, MAX_PATH);
			if (wcslen(path_tex) > 0)
			{
				// Xファイルのディレクトリ内で指定する
				wcscpy_s(path_file_part, MAX_PATH - wcslen(path_full), path_tex);

				// テクスチャの読み込み
				D3DXCreateTextureFromFile(
					d3d_dev,
					path_full,			// テクスチャファイル名
					&m_textures[i]);	// テクスチャオブジェクト
			}
		}
	}

	//----------------------------------------------------------
	// ■レンダリング手法の設定
	//----------------------------------------------------------
	// レンダリングステートパラメータの設定
	d3d_dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	d3d_dev->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3d_dev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return	S_OK;
}



//-------------------------------------------------------------
// シーンの破棄
//-------------------------------------------------------------
HRESULT	SampleCamera::Destroy()
{
	// 頂点バッファの解放
	SAFE_RELEASE(m_mat_buf);
	SAFE_RELEASE(m_mesh);

	if (m_textures)
	{
		for (DWORD i = 0; i<m_num_mesh; i++)	SAFE_RELEASE(m_textures[i]);
		delete[]	m_textures;
	}

	return	S_OK;
}



//-------------------------------------------------------------
// 座標変換
//-------------------------------------------------------------
HRESULT	SampleCamera::Transform( LPDIRECT3DDEVICE9 d3d_dev )
{
	if( d3d_dev == NULL )	return	E_FAIL;

	//-------------------------------------------------------------
	// ◆射影行列の初期化
	//-------------------------------------------------------------
	// ビューポートの取得
	D3DVIEWPORT9    vp;
	if (FAILED(d3d_dev->GetViewport(&vp))) {
		return  E_FAIL;
	}

	// アスペクト比の計算
	float   aspect = (float)vp.Width / (float)vp.Height;

	// 射影マトリックスの初期化
	D3DXMatrixIdentity(&m_proj);

	// 射影マトリックスの設定
	D3DXMatrixPerspectiveFovLH(&m_proj,
		D3DXToRadian(45.0f),    // 画角
		aspect,                 // アスペクト比
		1.0f,                   // Nearプレーン
		1000.0f);              // Farプレーン

	// 射影マトリックスをDirectXに設定
	d3d_dev->SetTransform(D3DTS_PROJECTION, &m_proj);

	//-------------------------------------------------------------
	// ◆ビューイング行列の初期化
	//-------------------------------------------------------------
	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_view);

	// ビューマトリックスの設定
	D3DXMatrixLookAtLH(&m_view
		, &D3DXVECTOR3(camX + dx, camY + dy, -50.0f)	// カメラの位置
		, &D3DXVECTOR3(0.0f, 10.0f, 40.0f)	// カメラの向き
		, &D3DXVECTOR3(0.0f, 10.0f, 0.0f));	// カメラの傾き

	// ビューマトリックスをDirectXに設定
	d3d_dev->SetTransform(D3DTS_VIEW, &m_view);

	return	S_OK;
}

void SampleCamera::SetDxDy(float tdx, float tdy)
{
	dx = -tdx;
	dy = -tdy;
}



//-------------------------------------------------------------
// オブジェクト等の描画
//-------------------------------------------------------------
HRESULT	SampleCamera::Draw( LPDIRECT3DDEVICE9 d3d_dev )
{
	if (d3d_dev == NULL)	return	E_FAIL;
	if (m_mesh == NULL)		return	E_FAIL;

	// 座標変換（カメラの設定など）
	Transform(d3d_dev);

	//----------------------------------------------------------
	// ■メッシュデータの描画処理
	//----------------------------------------------------------
	D3DXMATERIAL  *materials = (D3DXMATERIAL*)m_mat_buf->GetBufferPointer();

	// メッシュデータを描画パイプラインに渡す
	for (DWORD i = 0; i<m_num_mesh; i++) {
		D3DMATERIAL9  mMaterial = materials[i].MatD3D;  // マテリアルを取出
		d3d_dev->SetMaterial(&mMaterial);             // マテリアルをセット
		d3d_dev->SetTexture(0, m_textures[i]);         // テクスチャをセット
		m_mesh->DrawSubset(i);                        // メッシュを描画
	}


	return	S_OK;
}
