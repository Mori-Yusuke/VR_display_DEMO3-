#include <opencv2/opencv.hpp>
#include "Common.h"

#define XML_FACE	"haarcascade_frontalface_alt2.xml"
#define XML_EYE		"haarcascade_eye.xml"
#define OPENCVWNAME "CAMERA"
#define CAMERA		1
#define F_RESIZE	1.5								// 顔検出画像を縮小サイズ

#if NOTDEBUG
	#pragma comment(lib,"vfw32.lib")
#endif // DEBUG

class	openCVCamera
{
protected:
	

private:
	// カメラ移動距離
	float	camDx = 0;		// カメラ移動距離X
	float	camDy = 0;		// カメラ移動距離Y
	int		cam_width;		// カメラの横幅
	int		cam_height;		// カメラの縦幅

	cv::VideoCapture	cap;			// カメラ
	cv::Mat				frame;			// カメラのフレーム取得
	cv::Mat				frameGray;		// カメラフレームをグレースケール化
	cv::Mat				resize_f;		// 顔画像用低解像度
	cv::Mat				resize_e;		// 目画像用低解像度
	cv::Rect			max_face;		// 一番大きい顔情報
	cv::Rect			z_face;			// 比較されるrect
	std::vector<cv::Rect> nestedObjects_f;			// 顔用検出矩形座標
	cv::CascadeClassifier nested_cascade_f;			// 顔用学習データのロrード
	std::vector<cv::Rect> nestedObjects_e;			// 目用検出矩形座標
	cv::CascadeClassifier nested_cascade_e;			// 目用学習データのロード

public:
	openCVCamera();
	~openCVCamera();

private:
	cv::Rect	faceMaxRect(std::vector<cv::Rect>); // 矩形の中からもっとも大きい矩形を取り出す
	float		CenterPoint(int, int);				// 与えられた直線の真ん中を返す
public:
	void	Tracking(void);			// 顔の位置からDx,Dyの位置を決める
	float	GetDx(void);			// camDyを返す
	float	GetDy(void);			// camDyを返す

};
