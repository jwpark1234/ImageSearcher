// MessageDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageSearch.h"
#include "MessageDlg.h"
#include "afxdialogex.h"


// MessageDlg ��ȭ �����Դϴ�.

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


// MessageDlg �޽��� ó�����Դϴ�.

// �˸� �޽��� ���� �Լ� : ���̾�α׿� �޽����� ǥ���Ѵ�.
void MessageDlg::SetMessage(CString msg)
{
	m_static.SetWindowTextW(msg);     // static text�� �޽����� ǥ���Ѵ�.
}
