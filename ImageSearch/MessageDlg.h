#pragma once
#include "afxwin.h"


// MessageDlg ��ȭ �����Դϴ�.

class MessageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MessageDlg)

public:
	MessageDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~MessageDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_static;
	void SetMessage(CString msg);
};
