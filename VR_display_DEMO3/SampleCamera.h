
#include "Common.h"


class	SampleCamera
{
// �����o�ϐ�
private:
	// �J�������W
	float camX = 0.0f;				// �����J����X���W
	float camY = 20.0f;				// �����J����Y���W
	float dx = 0, dy = 0;			// �J�����̈ړ���

	// ���W�ϊ�
	D3DXMATRIX		m_proj;			// �ˉe�s��
	D3DXMATRIX		m_view;			// �r���[�C���O�s��

									// �����b�V���f�[�^�ϐ�
	LPD3DXMESH          m_mesh;         // ���b�V���f�[�^�̃|�C���^
	DWORD               m_num_mesh;     // ���b�V�����̑���
	LPD3DXBUFFER        m_mat_buf;      // ���b�V���̃}�e���A�����
	LPDIRECT3DTEXTURE9  *m_textures;    // �e�N�X�`���z��


// �R���X�g���N�^�E�f�X�g���N�^
public:
	SampleCamera();
	~SampleCamera();


// �����o�֐�
public:
	HRESULT	Create( LPDIRECT3DDEVICE9 d3d_dev );	// �V�[���̐���
	HRESULT	Destroy( void );						// �V�[���̔j��
	HRESULT	Transform( LPDIRECT3DDEVICE9 d3d_dev );	// ���W�ϊ�����
	void	SetDxDy(float, float);					// dx,dy ������
	HRESULT	Draw( LPDIRECT3DDEVICE9 d3d_dev );		// �V�[���̕`��
};