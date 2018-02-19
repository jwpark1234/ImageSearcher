
// ImageSearchDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ImageSearch.h"
#include "ImageSearchDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageSearchDlg ��ȭ ����


CImageSearchDlg::CImageSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageSearchDlg::IDD, pParent)
	, m_penPrev(0)
	, undo(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PATH, m_editPath);
	DDX_Control(pDX, IDC_LIST, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CImageSearchDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(ID_CLEAR, &CImageSearchDlg::OnBnClickedClear)
	ON_BN_CLICKED(ID_UNDO, &CImageSearchDlg::OnBnClickedUndo)
	ON_BN_CLICKED(ID_SEARCH, &CImageSearchDlg::OnBnClickedSearch)
	ON_BN_CLICKED(ID_PATH, &CImageSearchDlg::OnBnClickedPath)
END_MESSAGE_MAP()


// CImageSearchDlg �޽��� ó����

BOOL CImageSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&m_rectPic);
	ScreenToClient(&m_rectPic);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CImageSearchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CImageSearchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{	
		GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&m_rectPic);    // Picture Control�� ũ�⸦ ����� m_rectPic���� �޾ƿ´�.
		ScreenToClient(&m_rectPic);                               // ��üȭ������ �����ִ� ���� ��ǥ�� m_rectPic���� �޾ƿ´�.
		                                                          
		CPaintDC dc(this);                                 
		CBrush NewBrush, *oldBrush;
		NewBrush.CreateSolidBrush(RGB(255,255,255));       // ���( RGB(255,255,255) ) �귯���� ĥ�Ѵ�
		oldBrush = dc.SelectObject(&NewBrush);             // ȭ��dc�� �귯���� �����Ѵ�.
		dc.Rectangle(&m_rectPic);                          // Picture Control�� ĥ�Ѵ�.
		dc.SelectObject(oldBrush);
		NewBrush.DeleteObject();

		// ����ġ�� ���� ����� ���
		if(undo) {
			for(int i = 0; i < m_penIndex.GetSize() - 1; i++) {      // ����ġ�ߴ� ȹ �� ��ŭ �ݺ� 
				CClientDC dc(this);
				CPen m_CurrentPen(PS_INSIDEFRAME, 10, RGB(0,0,0));         // ������ ���� ����, ���� 10, ������ ���� �����Ѵ�.
				CPen* pOldPen = dc.SelectObject(&m_CurrentPen);      // ȭ�� dc�� ���� �����Ѵ�.

				// �� ȹ�� ���� ��ǥ�� �ε������� ������ ��ǥ�� �ε�������
				for(int j = m_penIndex.GetAt(i); j < m_penIndex.GetAt(i + 1) - 1; j++) {   
					dc.MoveTo(m_penArray.GetAt(j));
					dc.LineTo(m_penArray.GetAt(j+1));              // ���� �����Ѵ�.
					m_penPrev = m_penArray.GetAt(j+1);             // �� ȹ�� ������ ��ǥ�� �����Ѵ�.
				}
			}
			m_penIndex.RemoveAt(m_penIndex.GetSize() - 1);         // ����ڰ� ����� ȹ�� ���� ��ǥ�� �ε����� ������.
			undo = false;                                          // ���� ��� ����!!
		}

		CDialogEx::OnPaint();
	}
}

// ���� Ŭ�� �Լ�
// : ����ڰ� ���콺 ���� ��ư�� ������ �� ��ǥ�� ���´�.
//   Picture Control ���� ���̶�� ��ǥ���� ���������� �迭�� �����Ѵ�.
void CImageSearchDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	// ����ġ ���� ��
	if(point.x >= m_rectPic.TopLeft().x + 5 && point.y >= m_rectPic.TopLeft().y + 5) {
		if(point.x <= m_rectPic.BottomRight().x - 5 && point.y <= m_rectPic.BottomRight().y - 5) {
			SetCapture();              // ���콺 ĸ�� ����
			m_penPrev = point;         // ����ġ ���� ��ǥ ����

			m_penArray.Add(point);      // ����ġ�� ���� ��ǥ���� �����Ѵ�.
			m_penIndex.Add(m_penArray.GetSize() - 1);  // �� ȹ�� ���� ��ǥ�� �ε����� �����Ѵ�
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

// ���콺 �̵� �Լ�
// : ����ڰ� ���콺�� Ŭ���� �� �������� �� ó���� �����Ѵ�.
//   ���콺�� ��ġ�� Picture Control ���� ���̶��
//   ���� ��ǥ�� ���� ��ǥ�� �����Ѵ�.
void CImageSearchDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	// Picture Control�� DC�� �����Ѵ�.
	GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&m_rectPic);
	ScreenToClient(&m_rectPic);
	
	// ����ġ ���� ��
	if(point.x >= m_rectPic.TopLeft().x + 5 && point.y >= m_rectPic.TopLeft().y + 5) {
		if(point.x <= m_rectPic.BottomRight().x - 5 && point.y <= m_rectPic.BottomRight().y - 5) {

			if(GetCapture() != this)   // ���콺�� Ŭ���� ���°� �ƴ϶��
				return;

			CClientDC dc(this);
			CPen m_CurrentPen(PS_INSIDEFRAME, 10, RGB(0,0,0));    // ������ ���� ����, ���� 10, ������ ���� �����Ѵ�.
			CPen* pOldPen = dc.SelectObject(&m_CurrentPen);
			dc.MoveTo(m_penPrev);                                 // ���� ��ǥ��
			dc.LineTo(point);                                     // ���� ��ǥ�� �����Ѵ�.
			m_penPrev = point;                                    // ���� ��ǥ�� ���� ��ǥ�� �ȴ�.

			m_penArray.Add(point);                                // ����ġ�� ��ǥ���� ����ȴ�.
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

// ���� Ŭ�� ���� �Լ�
// : ����ڰ� ���콺 Ŭ���� �� ��츦 ó���Ѵ�.
//   �ܼ��� ���콺 ĸ�ĸ� �����Ѵ�.
//   ���� �������� ������ ���콺�� �� �Ŀ��� ���콺 �޽����� �信�� ��� ��ƿͼ�
//   ���콺�� �ٸ� �۾��� ������ �� ���� �ȴ�.
void CImageSearchDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	ReleaseCapture();             // ���콺 ĸ�ĸ� �����Ѵ�.

	CDialogEx::OnLButtonUp(nFlags, point);
}

// DC->IplImage ��ȯ �Լ�
// : ����ڰ� DC ������ �׸� ����ġ�� ����ó���� ���� IplImage ���·� �ٲ��ش�.
//   ���� ȭ�� DC�� ���� Picture Control�� �׷��� ����ġ�� �޾ƿ� ��
//   �޸� DC�� �ְ� hBITMAP ��ü�� �����Ѵ�.
//   �� �� hBITMAP ��ü�� �ִ� ���� ������ IplImage ��ü�� �����Ѵ�.
IplImage* CImageSearchDlg::DCtoIplImage(CDC* pDC)
{
	/***************** ȭ��DC -> hBitmap ************************/

    pDC->GetWindow()->GetClientRect(&m_rectPic);     // ��ȯ�� DC������ Picture Control ��ü!!

    int cx = m_rectPic.right - m_rectPic.left;       // ���̸� ���Ѵ�.
    int cy = m_rectPic.bottom - m_rectPic.top;       // �ʺ� ���Ѵ�.

    if(cx <= 0 || cy <= 0) return NULL;              // ������ ������ ���� 

    HDC hMemDC;                                      

    hMemDC = CreateCompatibleDC(pDC->m_hDC);         // �޸� DC �����ϰ� ȭ�� DC�� �����Ѵ�.

    BITMAPINFO    bmi;                                  // ��Ʈ�� ����� �����.
    BITMAPINFOHEADER* bmih = &(bmi.bmiHeader);
    LPVOID        pBits;
    HBITMAP        hBitmap;

    ZeroMemory(bmih, sizeof(BITMAPINFOHEADER));

    bmih->biSize        = sizeof(BITMAPINFOHEADER);
    bmih->biBitCount    = 24; 
    bmih->biWidth        = cx+440;                      // ������ �߷��� �ʺ� �÷��� �ذ��Ͽ���.
    bmih->biHeight        = cy - 1;                     // ������ y�������� 1�ȼ��� ©�� ���������.
    bmih->biPlanes        = 1;

    hBitmap = CreateDIBSection(pDC->m_hDC, &bmi, DIB_RGB_COLORS, (LPVOID*)&pBits, NULL, 0); // �޸�DC�� hBitmap ��ü�� �����Ѵ�.

    SelectObject(hMemDC, hBitmap);

	// ȭ��DC�� ������ ��Ʈ�����·� ����� �޸�DC�� �ѷ��ش�. -> hBitmap���� ������ ������.
    BitBlt(hMemDC, 0, 0, cx, cy, pDC->m_hDC, m_rectPic.left+1, m_rectPic.top, SRCCOPY);  

    DeleteDC(hMemDC);
    

	/***************** hBitmap -> IplImage ************************/

    hMemDC = NULL;
    bool bMustRelease = false;

    const int nDepth = IPL_DEPTH_8U;       
    const int nChannels = 3;
    
    if(hMemDC == NULL)
    {
        hMemDC = ::GetDC(NULL);
        bMustRelease = true;
    }

    IplImage* img = NULL;    // IplImage ��ü ����

	// IplImage ����ü�� �°� �������ش�.
    if(GetDIBits(hMemDC, hBitmap, 0, 0, NULL, &bmi, DIB_RGB_COLORS))       
    {
        int nHeight = (bmih->biHeight > 0) ? bmih->biHeight : -bmih->biHeight;
        img = cvCreateImage(cvSize(bmih->biWidth, nHeight), nDepth, nChannels);
        img->origin = (bmih->biHeight > 0);
        bmih->biBitCount = (WORD)(img->nChannels * 8);
        bmih->biCompression = BI_RGB;
        GetDIBits(hMemDC, hBitmap, 0, nHeight, img->imageData, &bmi, DIB_RGB_COLORS);
    }

    if(bMustRelease) ::ReleaseDC(NULL, hMemDC);


    DeleteObject(hBitmap);

    return img;
}

// �˻� ��ư �Լ�
// : �˻� ��ư�� ������ ���� ó���� �ٷ��.
//   ���� ���� ���ο� ���� ���� ���� ������ Ȯ���Ѵ�.
//   �� �� ����ġ ����� �о�� ���� ����(�����)���� �����Ͽ� ���� ���絵�� �����Ѵ�.
//   ���� ���絵 �̻��� �Ǵ� ������ ��� ���絵�� ���� ������ �����ش�.
void CImageSearchDlg::OnBnClickedSearch()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	m_listCtrl.DeleteAllItems();            // �˻���� ����Ʈ ��Ʈ���� �ʱ�ȭ�Ѵ�. 

	CString path;
	m_editPath.GetWindowText(path);         // ����ڰ� ������ ���� ��� ���� �����´�.

	// ���� ������ ���� ������ ���
	if(path == _T("")) {
		AfxMessageBox(_T("�˻��� ������ ���� �����ϼ���."));
		return;
	}
	// ������ ������ �ϳ��� ���� ���
	if(m_pathArray.GetSize() == 0) {
		AfxMessageBox(_T("�˻��� �̹����� �����ϴ�. �ٸ� ������ �����ϼ���."));
		return;
	}

	MessageDlg *dlg = new MessageDlg(this);               // �ȳ� �޼��� ���̾�α� ��ü ����
	dlg->Create(IDD_MESSAGE,this);
	dlg->SetMessage(_T("�˻� ���Դϴ�."));                 // �޽��� ����
	dlg->ShowWindow(SW_SHOW);                             // ���̾�α׸� ����.

	CArray<CPoint> rank;             // �˻������ ��Ȯ�� ������ �����ϱ� ���� �迭
	
	/********************** ����ġ ���� ó�� ******************************/

	CClientDC dc(GetDlgItem(IDC_STATIC_PIC));        // Picture Control�� DC�� �����Ѵ�.

	// ����ġ �̹��� ��ü�� ũ��� Picture Control ũ��� �Ѵ�.
	IplImage *sketch = cvCreateImage(cvSize(m_rectPic.Width(), m_rectPic.Height()), IPL_DEPTH_8U, 1);

	sketch = DCtoIplImage(&dc);     // Picture Control�� ����ڰ� �׸� ����ġ�� IplImage ��ü�� ��ȯ�Ѵ�.
	sketch->nChannels = 1;          // IplImage ��ü�� ��ȯ�ϸ� 3ä���� �ǰ� ������ 1ä�η� �ٲ۴�.

	IplImage *temp = cvCreateImage(cvSize(128,128), IPL_DEPTH_8U, 1);    // ����ó���� ���� 128x128ũ���� IplImage ��ü
	
	cvResize(sketch, temp, CV_INTER_NN );           // ����ġ ������ 128x128 ũ��� �������Ѵ�. 
	cvFlip(temp, temp, 0);                          // ��Ʈ�� �����̱� ������ ���Ʒ��� �����´�.


	/******************** �˻���� ���� ó�� ******************************/

	IplImage *target;

	for(int k = 0; k < m_pathArray.GetSize(); k++) {               // ������ ���� ���� ��� ���� ���� ���� ���� �ǽ�

		CString filePath = _T("");
		filePath.Format(_T("thumbnail_%d.jpg"),k);         // ����� ����� ���� �ǽ�

		CStringA strA(filePath);
		LPCSTR path = strA;                   // CString -> const char* ���� ��ȯ

		target = cvLoadImage(path, IPL_DEPTH_8U);          // ������ �����´�.
		cvSmooth( target, target, CV_GAUSSIAN, 7, 7, 3);   // �˻���Ȯ���� ���̱� ���� �������� �ǽ��Ͽ� ū ������ �����.
		cvCanny(target,target,50,100);                     // ������ �����Ѵ�.

		double similarity = 0.3;              // �ּ� ���絵�� 30%
		int total = 0, count = 0;                // total : �� ���� �ȼ� ��, conut : ������ �ȼ� ��

		for(int i = 1; i < 127; i++) {
			for(int j = 1; j < 127; j++) {
				if((unsigned char)temp->imageData[j+128*i] == 0) {   // ����ġ �� �ȼ��� ��� 
					total++;
					int sum = 0;

					for(int x = -1; x <= 1; x++) {    // ����ġ�� �ȼ��� ���� ��ǥ�� �ش��ϴ� �˻� ��� ������ �ȼ����� 
						for(int y = -1; y <= 1; y++)  // �� �ȼ��� �߽����� 9�ȼ��� ���� �ȼ� ���� ���� ���Ѵ�.
							sum += (unsigned char)target->imageData[(j+y)+128*(i+x)];
					}

					if( sum > 128 * 5)         // 9���� �ȼ� �߿� 5�ȼ� �̻��� ������ ���
						count++;               // ������ �ȼ��̶�� �Ǵ��Ѵ�.
				}			
			}
		}
		//cvNamedWindow("ske", 1);
		//cvShowImage("ske", temp);

		//cvNamedWindow("tar", 1);
		//cvShowImage("tar", target);

		//cvWaitKey(0);

		// ��ġ���� �ּ� ��ġ�� �̻� AND ����ڰ� ����ġ�� ���� ���
		if( (double) count / total > similarity && total != 0)  
			rank.Add(CPoint(k,count));       // �˻� ��� ������ �ε����� ������ �ȼ��� ���� �迭�� �����Ѵ�.

		cvReleaseImage(&target);
	}

	/*********************** �˻� ��� ó�� ***************************/

	int  size = rank.GetSize();
	for(int i = 0; i < size; i++) {        // �˻� �����ŭ �ݺ�
		int max_index = 0;                 // max_index : ���絵�� ���� ������ �ε���
		int max = 0;                       // max : ���絵 �ִ밪
		int index = 0;                     // index : �˻���� �迭�� �ε���

		for(int j = 0; j < rank.GetSize(); j++) {  // ���� �˻� �����ŭ �ݺ�
			if(rank.GetAt(j).y > max) {            // ���絵 �ִ밪 ���� Ŭ ���
				max_index = rank.GetAt(j).x;       
				max = rank.GetAt(j).y;
				index = j;
			}
		}
		rank.RemoveAt(index);                     // ���絵�� ���� ���� ���� ��(�ε���, ���絵) ������

		// ��ΰ��� ������ ���� ����� Ȯ���ڸ� ǥ��
		CString title = _T("");
		int n = m_pathArray.GetAt(max_index).ReverseFind('\\') + 1;
		title = m_pathArray.GetAt(max_index).Mid(n, m_pathArray.GetAt(max_index).GetLength() - n);
			
		m_listCtrl.SetImageList(&m_imageList, LVSIL_NORMAL);      // ��� ��� ����Ʈ ���� ���� ����Ʈ�� ���� ���
		m_listCtrl.InsertItem(max_index, title, max_index);		  // �˻� ��� ����Ʈ�� ���� ����� ���� ���
	}

	cvReleaseImage(&sketch);
	cvReleaseImage(&temp);

	dlg->DestroyWindow();           // �˸� �޽��� ���̾�α� ����
	delete dlg;
}

// ����� ��ư �Լ�
// : ����ڰ� ����� ��ư�� ���� ��츦 ó���Ѵ�.
//   ���̾�α׸� ���� �׸��� ���� Invalidate()�Լ��� ȣ���ϰ�
//   ����ġ ������ �˻������ �ʱ�ȭ�Ѵ�.
void CImageSearchDlg::OnBnClickedClear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	Invalidate();                        // OnPaint() �Լ� ��ȣ��

	m_penArray.RemoveAll();              // ����ġ ����� ���� ������.
	m_penIndex.RemoveAll();
	m_listCtrl.DeleteAllItems();         // �˻������ ������.
}

// ��� ��ư �Լ�
// : ����ڰ� ��� ��ư�� ���� ��츦 ó���Ѵ�.
//   ������ ȹ�� ������ ù ȹ���� ���� ȹ������ �ٽ� �׷��ش�.
void CImageSearchDlg::OnBnClickedUndo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	if(m_penIndex.GetSize() == 0)        // �ƿ� ����ġ�� ���� ���
		return;

	int end = m_penArray.GetSize() - 1;    // ������ ȹ�� ������ ��ǥ�� �ε���
	int start = m_penIndex.GetAt(m_penIndex.GetSize() - 1);    // ������ ȹ�� ���� ��ǥ�� �ε���
	
	for(int i = end; i >= start; i--)       
		m_penArray.RemoveAt(i);            // ������ ȹ�� ������.

	undo = true;                          // ���� ��� ����!!

	Invalidate();                         // OnPaint() �Լ� ȣ��. ������ ȹ�� ���� ����� �ٽ� �׷���� �Ѵ�.
}

// ���� ���� �Լ�
// : ����ڰ� ���� ���� ��ư�� Ŭ���� ����� ó���� �Ѵ�.
//   ���� ���� ���̾�α׸� ���� ����ڰ� ������ �����ϸ�
//   �ش� ���� ���� ���� ������ Ž���Ѵ�. �� �� ��� ���� ���Ͽ�
//   ���� ������� ����� �˻���� ���� ����Ʈ(DB ����)�� �־� �д�.
void CImageSearchDlg::OnBnClickedPath()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	/************************* ���� ���� ���̾�α� ���� *****************************/

	BROWSEINFO Brinfo;             // ���õ� ������ ������ �޾� ���̴� ��ü
	TCHAR szBuffer[512];
	::ZeroMemory(&Brinfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	Brinfo.hwndOwner = GetSafeHwnd();                 // ���� ������ ����
	Brinfo.lpszTitle = _T("������ �������ּ���.");

	// �÷��� ���� : ���ο� ���̾�α� ��Ÿ�� | ����Ʈ �ڽ� ǥ�� | ���� �ý��� ���丮�� ���� ����
	Brinfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;    
	LPITEMIDLIST pItemdList = ::SHBrowseForFolder(&Brinfo);

	::SHGetPathFromIDList(pItemdList, szBuffer);          // ���õ� ���� ��θ� szBuffer�� �޾ƿ´�.

	MessageDlg *dlg = new MessageDlg(this);          // �˸� �޽��� ���̾�α� ����
	dlg->Create(IDD_MESSAGE,this);
	dlg->SetMessage(_T("������ �а� �ֽ��ϴ�."));     // �˸� �޽��� ����
	dlg->ShowWindow(SW_SHOW);                        // ���̾�α� ȣ��
	
	m_editPath.SetWindowText(szBuffer);                  // ���� ���̾�α� �ϴܿ� ������ ���� ��� ǥ��
	FileSearch(szBuffer, true);                              // ���õ� ���� ���� ���� ���� Ž�� ����!!

	/*********************** ����� ����� *****************************/

 
	m_imageList.DeleteImageList();            // �̹��� ����Ʈ �ʱ�ȭ
	m_imageList.Create(128, 128, ILC_MASK|ILC_COLORDDB, 10, 10);   // �� ������ 128x128 ������� ����

	int num = 0;

	// ã�Ƴ� ���� ���� ���� ��ŭ �ݺ�
	for(int i = 0; i < m_pathArray.GetSize(); i++) { 
 
 		CStringA strA(m_pathArray.GetAt(i));
		LPCSTR path = strA;

		IplImage *img = cvLoadImage(path);              // ���� �����ͼ�
		IplImage *icon = cvCreateImage(cvSize(128,128),img->depth,img->nChannels);    
		cvResize(img,icon, CV_INTER_NN);            // 128x128�� ���

		char filename[50];
		sprintf(filename, "thumbnail_%d.jpg", num);      

		cvSaveImage(filename, icon);    // jpg �������� ����� ����. �˻� �� ���絵 ������ ���� ����.
		
		CString filePath = _T("");
		filePath.Format(_T("thumbnail_%d.jpg"),i);

		CImage image;

		HRESULT hResult = image.Load(filePath);       // ������� �ٽ� �о� �´�.
		num++;

		m_imageList.Add(CBitmap::FromHandle(image), RGB(0,128,128));  // �̹��� ����Ʈ�� �߰��Ѵ�. �����¿� ����.
 
	}

	dlg->DestroyWindow();           // �˸� �޽��� ���̾�α� ����
	delete dlg;
}

// ���� Ž�� �Լ�
// : ���ȣ���Ͽ� ���� ������ ã�� ������ Ž���Ѵ�.
//   ����ڰ� ������ ������ ��θ� �޾ƿ� �� bmp�� jpg������ ã�� ������ Ž���Ѵ�. 
bool CImageSearchDlg::FileSearch(CString PathName/* ��� */, bool bOption/* ���� ���� Ž�� �ɼ� */)
{
	CString TempPathName;
	CFileFind cfile;

	BOOL bFind = false;

	TempPathName = PathName + _T("\\*.*");      // ��Ʈ ��η� ����
	bFind = cfile.FindFile(TempPathName);


	while(bFind)                               // ã�� ������ �ݺ�
	{
		bFind = cfile.FindNextFile();          // ���� ���� Ž��

		
		if(cfile.IsDots()) // .�̳� .. �� ���
			continue;
	
		if(cfile.IsDirectory() && bOption) //���� ���� üũ 
		{ 

			TempPathName = PathName;
			TempPathName = TempPathName + _T("\\") + cfile.GetFileName();
			FileSearch(TempPathName, true); // ��� ȣ�� 

			continue;
		}

		CString FileName = PathName + _T("\\") + cfile.GetFileName();

		// bmp ���� �̰ų� jpg ������ ���
		if(FileName.Find(_T(".bmp")) > -1 || FileName.Find(_T(".jpg")) > -1 ) {
			int n = 0;
			CString FileName2 = _T("");
			for(int i = 0; i < FileName.GetLength(); i++) {
				if(FileName.GetAt(i) == '\\') {
					FileName2 += FileName.Mid(n,i-n) + _T("\\");      // \�� �ϳ��� �� ���δ�.
					n = i;
				}
			}
			FileName2 += FileName.Mid(n);

			m_pathArray.Add(FileName2);          // ���� ���� ��� �迭�� ����
		}
	}

	cfile.Close();

	return true;
}

