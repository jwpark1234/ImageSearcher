// MessageDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageSearch.h"
#include "MessageDlg.h"
#include "afxdialogex.h"


// MessageDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(MessageDlg, CDialogEx)

MessageDlg::MessageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(MessageDlg::IDD, pParent)
{

}

MessageDlg::~MessageDlg()
{
}

void MessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_static);
}


BEGIN_MESSAGE_MAP(MessageDlg, CDialogEx)
END_MESSAGE_MAP()


// MessageDlg 메시지 처리기입니다.

// 알림 메시지 설정 함수 : 다이얼로그에 메시지를 표시한다.
void MessageDlg::SetMessage(CString msg)
{
	m_static.SetWindowTextW(msg);     // static text에 메시지를 표시한다.
}
