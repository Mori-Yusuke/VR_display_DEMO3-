#include "openCVCamera.h"

//-------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//-------------------------------------------------------------
openCVCamera::openCVCamera() 
	: cap(CAMERA)
{
	if (!nested_cascade_f.load(XML_FACE))
	{
		cv::waitKey(0);
		return;
	}
	if (!nested_cascade_e.load(XML_EYE))
	{
		cv::waitKey(0);
		return;
	}

	cam_width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	cam_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	// �E�B���h�̃X�^�C���ݒ�
	cv::namedWindow(OPENCVWNAME, CV_WINDOW_AUTOSIZE);
	wchar_t* WindName = new wchar_t[strlen(OPENCVWNAME) + 1];
	setlocale(LC_ALL, "japanese");
	size_t tmp;
	mbstowcs_s(&tmp, WindName, strlen(OPENCVWNAME) + 1, (const char*)(OPENCVWNAME), _TRUNCATE);
	HWND hWnd = FindWindow(0, WindName);
	LONG hStyle = GetWindowLong(hWnd, GWL_STYLE);
	hStyle &= ~WS_SYSMENU;
	SetWindowLong(hWnd, GWL_STYLE, hStyle);

	SAFE_DELETE_ARRAY(WindName);
}

//-------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------
openCVCamera::~openCVCamera()
{
}

//-------------------------------------------------------------
// ��ԑ�����`���擾����z��
// ����
//		rects	: ��`�̔z��
// �߂�l
//		��ԑ傫���z��
//-------------------------------------------------------------
cv::Rect openCVCamera::faceMaxRect(std::vector<cv::Rect> rects)
{
	cv::Rect max_rect(10, 10, 10, 10);
	int max;

	if (rects[0].x <= 0 || rects[0].y <= 0 || rects[0].width <= 0 || rects[0].height <= 0) {
		max = 0;
	}
	else {
		max = (rects[0].width)*(rects[0].height);
		max_rect = rects[0];
	}

	for (int i = 1; i < rects.size(); i++)
	{
		if ((max < rects[i].width)*(rects[i].height))
		{
			max = (rects[i].width)*(rects[i].height);
			max_rect = rects[i];
		}
	}

	return max_rect;
}

//-------------------------------------------------------------
// �^����ꂽ�����𑫂��ē�Ŋ���
// ����
//		XorY	:	����
//		WorH	:	����
// �߂�l
//		�v�Z��̒l��Ԃ�
//-------------------------------------------------------------
float openCVCamera::CenterPoint(int XorY, int WorH)
{
	float center;
	center = (float)(XorY + (WorH / 2));

	return center;
}

//-------------------------------------------------------------
// �J���������̈ʒu���擾
//-------------------------------------------------------------
void openCVCamera::Tracking(void)
{
	cap >> frame;

	// �ڂ̌��o
	// �摜�C�o�͋�`�C�k���X�P�[���C�Œ��`���C�i�t���O�j�C�ŏ���`
	cv::cvtColor(frame, frameGray, CV_RGB2GRAY);
	cv::resize(frameGray, resize_f, cv::Size(), 1.0 / F_RESIZE, 1.0 / F_RESIZE, cv::INTER_LINEAR);
	nested_cascade_f.detectMultiScale(
		resize_f,
		nestedObjects_f,
		1.1f, 2,
		CV_HAAR_SCALE_IMAGE,
		cv::Size(10, 10),
		cv::Size(1200, 1200)
	);
	if (!nestedObjects_f.empty()) {
		max_face = faceMaxRect(nestedObjects_f);
		camDx = CenterPoint(max_face.x, max_face.width);
		camDy = CenterPoint(max_face.y, max_face.height);
	}
#ifdef DEBUG
	for (std::vector<cv::Rect>::const_iterator nr = nestedObjects_f.begin(); nr != nestedObjects_f.end(); ++nr) {
		cv::rectangle(
			frame,
			cv::Point(nr->x * F_RESIZE, nr->y * F_RESIZE),
			cv::Point((nr->x + nr->width) * F_RESIZE, (nr->y + nr->height) * F_RESIZE),
			cv::Scalar(255, 0, 0), 3, 4);
	}
	cv::imshow(OPENCVWNAME, frame);				// �r�f�I�����1�t���[���\��
#endif // DEBUG

	return;
}

//-------------------------------------------------------------
// x���̈ړ��ʂ�Ԃ�
// �߂�l
//		�l��Ԃ�
//-------------------------------------------------------------
float openCVCamera::GetDx()
{
	return (40.0f *(camDx * F_RESIZE) / cam_width) - 20.0f;
}

//-------------------------------------------------------------
// y���̈ړ��ʂ�Ԃ�
// �߂�l
//		�l��Ԃ�
//-------------------------------------------------------------
float openCVCamera::GetDy()
{
	return (40.0f * (camDy * F_RESIZE) / cam_height) - 20.0;
}