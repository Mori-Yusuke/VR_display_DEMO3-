
#include "SampleCamera.h"


//-------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//-------------------------------------------------------------
SampleCamera::SampleCamera()
	: m_mesh(NULL)
	, m_mat_buf(NULL)
	, m_textures(NULL)
{
}



//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
SampleCamera::~SampleCamera()
{
	Destroy();
}



//----------------------------------------------------------
// �V�[���̐���
//----------------------------------------------------------
HRESULT SampleCamera::Create( LPDIRECT3DDEVICE9 d3d_dev )
{
	Destroy();
	if (d3d_dev == NULL)	return	E_FAIL;

	//----------------------------------------------------------
	// �����b�V���𐶐�
	//----------------------------------------------------------

	// ��X�t�@�C�����������Ɋi�[
	wchar_t		path_x[MAX_PATH] = L"sensi/sensi.x";

	// ���b�V���t�@�C���̃f�B���N�g�������𒊏o�ipath_file_part�ɐ擪�A�h���X�j
	wchar_t		path_full[MAX_PATH];
	wchar_t		*path_file_part;
	GetFullPathName(path_x, MAX_PATH, path_full, &path_file_part);

	//----------------------------------------------------------
	// ��X�t�@�C����ǂݏo���A���b�V���E�}�e���A���E�e�N�X�`������ǂݎ��
	//----------------------------------------------------------
	if (FAILED(D3DXLoadMeshFromX(
		path_x,             // X�t�@�C���̃p�X
		D3DXMESH_SYSTEMMEM,
		d3d_dev,
		NULL,
		&m_mat_buf,         // �}�e���A�������w���|�C���^�̃A�h���X
		NULL,
		&m_num_mesh,        // ���b�V�������Ԃ��Ă���
		&m_mesh)))			// ���b�V�������w���|�C���^�̃A�h���X
	{
		return  E_FAIL;
	}

	// �}�e���A���ƃe�N�X�`���̐ݒ�
	D3DXMATERIAL    *materials = (D3DXMATERIAL*)m_mat_buf->GetBufferPointer();
	m_textures = new LPDIRECT3DTEXTURE9[m_num_mesh];

	// �e�N�X�`���̓ǂݏo��
	for (DWORD i = 0; i<m_num_mesh; i++)
	{
		m_textures[i] = NULL;
		if (materials[i].pTextureFilename != NULL)
		{
			// Unicode������֕ϊ�
			size_t  path_converted;
			wchar_t path_tex[MAX_PATH];
			mbstowcs_s(&path_converted, path_tex, materials[i].pTextureFilename, MAX_PATH);
			if (wcslen(path_tex) > 0)
			{
				// X�t�@�C���̃f�B���N�g�����Ŏw�肷��
				wcscpy_s(path_file_part, MAX_PATH - wcslen(path_full), path_tex);

				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(
					d3d_dev,
					path_full,          // �e�N�X�`���t�@�C����
					&m_textures[i]);   // �e�N�X�`���I�u�W�F�N�g
			}
		}
	}

	// �e�N�X�`���̓ǂݏo��
	for (DWORD i = 0; i<m_num_mesh; i++)
	{
		m_textures[i] = NULL;
		if (materials[i].pTextureFilename != NULL)
		{
			// Unicode������֕ϊ�
			size_t	path_converted;
			wchar_t	path_tex[MAX_PATH];
			mbstowcs_s(&path_converted, path_tex, materials[i].pTextureFilename, MAX_PATH);
			if (wcslen(path_tex) > 0)
			{
				// X�t�@�C���̃f�B���N�g�����Ŏw�肷��
				wcscpy_s(path_file_part, MAX_PATH - wcslen(path_full), path_tex);

				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(
					d3d_dev,
					path_full,			// �e�N�X�`���t�@�C����
					&m_textures[i]);	// �e�N�X�`���I�u�W�F�N�g
			}
		}
	}

	//----------------------------------------------------------
	// �������_�����O��@�̐ݒ�
	//----------------------------------------------------------
	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	d3d_dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	d3d_dev->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3d_dev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return	S_OK;
}



//-------------------------------------------------------------
// �V�[���̔j��
//-------------------------------------------------------------
HRESULT	SampleCamera::Destroy()
{
	// ���_�o�b�t�@�̉��
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
// ���W�ϊ�
//-------------------------------------------------------------
HRESULT	SampleCamera::Transform( LPDIRECT3DDEVICE9 d3d_dev )
{
	if( d3d_dev == NULL )	return	E_FAIL;

	//-------------------------------------------------------------
	// ���ˉe�s��̏�����
	//-------------------------------------------------------------
	// �r���[�|�[�g�̎擾
	D3DVIEWPORT9    vp;
	if (FAILED(d3d_dev->GetViewport(&vp))) {
		return  E_FAIL;
	}

	// �A�X�y�N�g��̌v�Z
	float   aspect = (float)vp.Width / (float)vp.Height;

	// �ˉe�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_proj);

	// �ˉe�}�g���b�N�X�̐ݒ�
	D3DXMatrixPerspectiveFovLH(&m_proj,
		D3DXToRadian(45.0f),    // ��p
		aspect,                 // �A�X�y�N�g��
		1.0f,                   // Near�v���[��
		1000.0f);              // Far�v���[��

	// �ˉe�}�g���b�N�X��DirectX�ɐݒ�
	d3d_dev->SetTransform(D3DTS_PROJECTION, &m_proj);

	//-------------------------------------------------------------
	// ���r���[�C���O�s��̏�����
	//-------------------------------------------------------------
	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_view);

	// �r���[�}�g���b�N�X�̐ݒ�
	D3DXMatrixLookAtLH(&m_view
		, &D3DXVECTOR3(camX + dx, camY + dy, -50.0f)	// �J�����̈ʒu
		, &D3DXVECTOR3(0.0f, 10.0f, 40.0f)	// �J�����̌���
		, &D3DXVECTOR3(0.0f, 10.0f, 0.0f));	// �J�����̌X��

	// �r���[�}�g���b�N�X��DirectX�ɐݒ�
	d3d_dev->SetTransform(D3DTS_VIEW, &m_view);

	return	S_OK;
}

void SampleCamera::SetDxDy(float tdx, float tdy)
{
	dx = -tdx;
	dy = -tdy;
}



//-------------------------------------------------------------
// �I�u�W�F�N�g���̕`��
//-------------------------------------------------------------
HRESULT	SampleCamera::Draw( LPDIRECT3DDEVICE9 d3d_dev )
{
	if (d3d_dev == NULL)	return	E_FAIL;
	if (m_mesh == NULL)		return	E_FAIL;

	// ���W�ϊ��i�J�����̐ݒ�Ȃǁj
	Transform(d3d_dev);

	//----------------------------------------------------------
	// �����b�V���f�[�^�̕`�揈��
	//----------------------------------------------------------
	D3DXMATERIAL  *materials = (D3DXMATERIAL*)m_mat_buf->GetBufferPointer();

	// ���b�V���f�[�^��`��p�C�v���C���ɓn��
	for (DWORD i = 0; i<m_num_mesh; i++) {
		D3DMATERIAL9  mMaterial = materials[i].MatD3D;  // �}�e���A������o
		d3d_dev->SetMaterial(&mMaterial);             // �}�e���A�����Z�b�g
		d3d_dev->SetTexture(0, m_textures[i]);         // �e�N�X�`�����Z�b�g
		m_mesh->DrawSubset(i);                        // ���b�V����`��
	}


	return	S_OK;
}
