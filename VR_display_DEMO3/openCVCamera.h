#include <opencv2/opencv.hpp>
#include "Common.h"

#define XML_FACE	"haarcascade_frontalface_alt2.xml"
#define XML_EYE		"haarcascade_eye.xml"
#define OPENCVWNAME "CAMERA"
#define CAMERA		1
#define F_RESIZE	1.5								// �猟�o�摜���k���T�C�Y

#if NOTDEBUG
	#pragma comment(lib,"vfw32.lib")
#endif // DEBUG

class	openCVCamera
{
protected:
	

private:
	// �J�����ړ�����
	float	camDx = 0;		// �J�����ړ�����X
	float	camDy = 0;		// �J�����ړ�����Y
	int		cam_width;		// �J�����̉���
	int		cam_height;		// �J�����̏c��

	cv::VideoCapture	cap;			// �J����
	cv::Mat				frame;			// �J�����̃t���[���擾
	cv::Mat				frameGray;		// �J�����t���[�����O���[�X�P�[����
	cv::Mat				resize_f;		// ��摜�p��𑜓x
	cv::Mat				resize_e;		// �ډ摜�p��𑜓x
	cv::Rect			max_face;		// ��ԑ傫������
	cv::Rect			z_face;			// ��r�����rect
	std::vector<cv::Rect> nestedObjects_f;			// ��p���o��`���W
	cv::CascadeClassifier nested_cascade_f;			// ��p�w�K�f�[�^�̃�r�[�h
	std::vector<cv::Rect> nestedObjects_e;			// �ڗp���o��`���W
	cv::CascadeClassifier nested_cascade_e;			// �ڗp�w�K�f�[�^�̃��[�h

public:
	openCVCamera();
	~openCVCamera();

private:
	cv::Rect	faceMaxRect(std::vector<cv::Rect>); // ��`�̒���������Ƃ��傫����`�����o��
	float		CenterPoint(int, int);				// �^����ꂽ�����̐^�񒆂�Ԃ�
public:
	void	Tracking(void);			// ��̈ʒu����Dx,Dy�̈ʒu�����߂�
	float	GetDx(void);			// camDy��Ԃ�
	float	GetDy(void);			// camDy��Ԃ�

};
