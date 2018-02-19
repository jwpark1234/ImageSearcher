#pragma once
#include "afxwin.h"


// MessageDlg 대화 상자입니다.

class MessageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MessageDlg)

public:
	MessageDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~MessageDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_static;
	void SetMessage(CString msg);
};
