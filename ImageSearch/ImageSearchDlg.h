
// ImageSearchDlg.h : ��� ����
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

// CImageSearchDlg ��ȭ ����
class CImageSearchDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CImageSearchDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_IMAGESEARCH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CRect m_rectPic;                 // ���� ��Ʈ���� ���� �簢�� ����
	CPoint m_penPrev;                // ���� ���콺�� ��ǥ
	CArray<CPoint, CPoint&> m_penArray;  // ����ġ�ϸ� ��� ���콺 ��ǥ���� �� �迭�� ����ȴ�.
	CArray<int> m_penIndex;           // m_penArray �迭���� �� ȹ�� ���� ��ǥ�� �ε������� �迭�� �����.
	bool undo;                       // ������� ���� 
	CArray<CString> m_pathArray;     // ���� �̹��� ���� ��� �迭
	CImageList m_imageList;          // 128x128 �̹��� ����Ʈ. ������ �˻��� DB
	CListCtrl m_listCtrl;            // �˻���� ����Ʈ. ����� �̹���, ���� ���ϸ����� �����ȴ�.
	CEdit m_editPath;                // ���� ���̾�α׿� ���� ��θ� ������ ����Ʈ ��Ʈ��
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
