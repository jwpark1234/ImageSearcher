
// ImageSearchDlg.h : 헤더 파일
//

#pragma once

#include "afxwin.h"
#include "atltypes.h"
#include "afxtempl.h"
#include "afxcmn.h"
#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>
#include "MessageDlg.h"
#include <algorithm>

#include "MessageDlg.h"



#pragma comment(lib, "opencv_calib3d245d.lib")
#pragma comment(lib, "opencv_contrib245d.lib")
#pragma comment(lib, "opencv_core245d.lib")
#pragma comment(lib, "opencv_haartraining_engined.lib")
#pragma comment(lib, "opencv_gpu245d.lib")
#pragma comment(lib, "opencv_flann245d.lib")
#pragma comment(lib, "opencv_features2d245d.lib")
#pragma comment(lib, "opencv_videostab245d.lib")
#pragma comment(lib, "opencv_video245d.lib")
#pragma comment(lib, "opencv_ts245d.lib")
#pragma comment(lib, "opencv_stitching245d.lib")
#pragma comment(lib, "opencv_photo245d.lib")
#pragma comment(lib, "opencv_objdetect245d.lib")
#pragma comment(lib, "opencv_nonfree245d.lib")
#pragma comment(lib, "opencv_ml245d.lib")
#pragma comment(lib, "opencv_legacy245d.lib")
#pragma comment(lib, "opencv_imgproc245d.lib")
#pragma comment(lib, "opencv_highgui245d.lib")

// CImageSearchDlg 대화 상자
class CImageSearchDlg : public CDialogEx
{
// 생성입니다.
public:
	CImageSearchDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IMAGESEARCH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CRect m_rectPic;                 // 픽쳐 컨트롤을 담을 사각형 변수
	CPoint m_penPrev;                // 이전 마우스의 좌표
	CArray<CPoint, CPoint&> m_penArray;  // 스케치하면 모든 마우스 좌표값은 이 배열에 저장된다.
	CArray<int> m_penIndex;           // m_penArray 배열에서 각 획의 시작 좌표의 인덱스들을 배열로 만든다.
	bool undo;                       // 실행취소 여부 
	CArray<CString> m_pathArray;     // 실제 이미지 파일 경로 배열
	CImageList m_imageList;          // 128x128 이미지 리스트. 일종의 검색기 DB
	CListCtrl m_listCtrl;            // 검색결과 리스트. 썸네일 이미지, 실제 파일명으로 구성된다.
	CEdit m_editPath;                // 메인 다이얼로그에 폴더 경로를 보여줄 에디트 컨트롤
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedSearch();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedUndo();
	afx_msg void OnBnClickedPath();
	IplImage* DCtoIplImage(CDC* pDC);
	bool FileSearch(CString PathName, bool bOption);
};
