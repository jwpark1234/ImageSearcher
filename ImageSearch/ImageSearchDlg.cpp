
// ImageSearchDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ImageSearch.h"
#include "ImageSearchDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageSearchDlg 대화 상자


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


// CImageSearchDlg 메시지 처리기

BOOL CImageSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&m_rectPic);
	ScreenToClient(&m_rectPic);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CImageSearchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CImageSearchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{	
		GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&m_rectPic);    // Picture Control의 크기를 사격형 m_rectPic으로 받아온다.
		ScreenToClient(&m_rectPic);                               // 전체화면으로 잡혀있는 영역 좌표를 m_rectPic으로 받아온다.
		                                                          
		CPaintDC dc(this);                                 
		CBrush NewBrush, *oldBrush;
		NewBrush.CreateSolidBrush(RGB(255,255,255));       // 흰색( RGB(255,255,255) ) 브러쉬로 칠한다
		oldBrush = dc.SelectObject(&NewBrush);             // 화면dc에 브러쉬를 연결한다.
		dc.Rectangle(&m_rectPic);                          // Picture Control에 칠한다.
		dc.SelectObject(oldBrush);
		NewBrush.DeleteObject();

		// 스케치를 실행 취소할 경우
		if(undo) {
			for(int i = 0; i < m_penIndex.GetSize() - 1; i++) {      // 스케치했던 획 수 만큼 반복 
				CClientDC dc(this);
				CPen m_CurrentPen(PS_INSIDEFRAME, 10, RGB(0,0,0));         // 프레임 안쪽 직선, 굵기 10, 검은색 펜을 생성한다.
				CPen* pOldPen = dc.SelectObject(&m_CurrentPen);      // 화면 dc에 펜을 연결한다.

				// 한 획의 시작 좌표의 인덱스에서 마지막 좌표의 인덱스까지
				for(int j = m_penIndex.GetAt(i); j < m_penIndex.GetAt(i + 1) - 1; j++) {   
					dc.MoveTo(m_penArray.GetAt(j));
					dc.LineTo(m_penArray.GetAt(j+1));              // 줄을 연결한다.
					m_penPrev = m_penArray.GetAt(j+1);             // 한 획의 마지막 좌표를 저장한다.
				}
			}
			m_penIndex.RemoveAt(m_penIndex.GetSize() - 1);         // 사용자가 취소한 획의 시작 좌표의 인덱스는 버린다.
			undo = false;                                          // 실행 취소 해제!!
		}

		CDialogEx::OnPaint();
	}
}

// 왼쪽 클릭 함수
// : 사용자가 마우스 왼쪽 버튼을 누르면 그 좌표를 얻어온다.
//   Picture Control 영역 안이라면 좌표값을 전역변수와 배열에 저장한다.
void CImageSearchDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	// 스케치 영역 안
	if(point.x >= m_rectPic.TopLeft().x + 5 && point.y >= m_rectPic.TopLeft().y + 5) {
		if(point.x <= m_rectPic.BottomRight().x - 5 && point.y <= m_rectPic.BottomRight().y - 5) {
			SetCapture();              // 마우스 캡쳐 시작
			m_penPrev = point;         // 스케치 시작 좌표 설정

			m_penArray.Add(point);      // 스케치에 대한 좌표값을 저장한다.
			m_penIndex.Add(m_penArray.GetSize() - 1);  // 한 획의 시작 좌표의 인덱스를 저장한다
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

// 마우스 이동 함수
// : 사용자가 마우스를 클릭한 후 움직였을 때 처리를 수행한다.
//   마우스의 위치가 Picture Control 영역 안이라면
//   이전 좌표와 현재 좌표를 연결한다.
void CImageSearchDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// Picture Control을 DC와 연결한다.
	GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&m_rectPic);
	ScreenToClient(&m_rectPic);
	
	// 스케치 영역 안
	if(point.x >= m_rectPic.TopLeft().x + 5 && point.y >= m_rectPic.TopLeft().y + 5) {
		if(point.x <= m_rectPic.BottomRight().x - 5 && point.y <= m_rectPic.BottomRight().y - 5) {

			if(GetCapture() != this)   // 마우스를 클릭한 상태가 아니라면
				return;

			CClientDC dc(this);
			CPen m_CurrentPen(PS_INSIDEFRAME, 10, RGB(0,0,0));    // 프레임 안쪽 직선, 굵기 10, 검은색 펜을 생성한다.
			CPen* pOldPen = dc.SelectObject(&m_CurrentPen);
			dc.MoveTo(m_penPrev);                                 // 이전 좌표와
			dc.LineTo(point);                                     // 현재 좌표를 연결한다.
			m_penPrev = point;                                    // 현재 좌표가 이전 좌표가 된다.

			m_penArray.Add(point);                                // 스케치한 좌표들은 저장된다.
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

// 왼쪽 클릭 해제 함수
// : 사용자가 마우스 클릭을 뗄 경우를 처리한다.
//   단순히 마우스 캡쳐를 해제한다.
//   만약 해제하지 않으면 마우스를 뗀 후에도 마우스 메시지를 뷰에서 계속 잡아와서
//   마우스로 다른 작업을 수행할 수 없게 된다.
void CImageSearchDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	ReleaseCapture();             // 마우스 캡쳐를 해제한다.

	CDialogEx::OnLButtonUp(nFlags, point);
}

// DC->IplImage 변환 함수
// : 사용자가 DC 영역에 그린 스케치를 영상처리를 위해 IplImage 형태로 바꿔준다.
//   먼저 화면 DC를 통해 Picture Control에 그려진 스케치를 받아온 후
//   메모리 DC에 넣고 hBITMAP 객체로 전달한다.
//   그 후 hBITMAP 객체에 있는 영상 정보를 IplImage 객체로 전달한다.
IplImage* CImageSearchDlg::DCtoIplImage(CDC* pDC)
{
	/***************** 화면DC -> hBitmap ************************/

    pDC->GetWindow()->GetClientRect(&m_rectPic);     // 변환할 DC영역은 Picture Control 객체!!

    int cx = m_rectPic.right - m_rectPic.left;       // 높이를 구한다.
    int cy = m_rectPic.bottom - m_rectPic.top;       // 너비를 구한다.

    if(cx <= 0 || cy <= 0) return NULL;              // 영상이 없으면 리턴 

    HDC hMemDC;                                      

    hMemDC = CreateCompatibleDC(pDC->m_hDC);         // 메모리 DC 생성하고 화면 DC와 연결한다.

    BITMAPINFO    bmi;                                  // 비트맵 헤더를 만든다.
    BITMAPINFOHEADER* bmih = &(bmi.bmiHeader);
    LPVOID        pBits;
    HBITMAP        hBitmap;

    ZeroMemory(bmih, sizeof(BITMAPINFOHEADER));

    bmih->biSize        = sizeof(BITMAPINFOHEADER);
    bmih->biBitCount    = 24; 
    bmih->biWidth        = cx+440;                      // 영상이 잘려서 너비를 늘려서 해결하였다.
    bmih->biHeight        = cy - 1;                     // 영상이 y방향으로 1픽셀씩 짤라서 보정해줬다.
    bmih->biPlanes        = 1;

    hBitmap = CreateDIBSection(pDC->m_hDC, &bmi, DIB_RGB_COLORS, (LPVOID*)&pBits, NULL, 0); // 메모리DC와 hBitmap 객체를 연결한다.

    SelectObject(hMemDC, hBitmap);

	// 화면DC의 영상을 비트맵형태로 만들어 메모리DC에 뿌려준다. -> hBitmap에도 영상이 생성됨.
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

    IplImage* img = NULL;    // IplImage 객체 생성

	// IplImage 구조체에 맞게 설정해준다.
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

// 검색 버튼 함수
// : 검색 버튼을 눌렀을 때의 처리를 다룬다.
//   폴더 선택 여부와 영상 파일 존재 유무를 확인한다.
//   그 후 스케치 영상과 읽어온 영상 파일(썸네일)들을 대조하여 각각 유사도를 측정한다.
//   일정 유사도 이상이 되는 영상일 경우 유사도가 높은 순으로 보여준다.
void CImageSearchDlg::OnBnClickedSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_listCtrl.DeleteAllItems();            // 검색결과 리스트 컨트롤을 초기화한다. 

	CString path;
	m_editPath.GetWindowText(path);         // 사용자가 선택한 폴더 경로 값을 가져온다.

	// 폴더 선택을 먼저 안했을 경우
	if(path == _T("")) {
		AfxMessageBox(_T("검색할 폴더를 먼저 선택하세요."));
		return;
	}
	// 폴더에 영상이 하나도 없을 경우
	if(m_pathArray.GetSize() == 0) {
		AfxMessageBox(_T("검색할 이미지가 없습니다. 다른 폴더를 선택하세요."));
		return;
	}

	MessageDlg *dlg = new MessageDlg(this);               // 안내 메세지 다이얼로그 객체 생성
	dlg->Create(IDD_MESSAGE,this);
	dlg->SetMessage(_T("검색 중입니다."));                 // 메시지 설정
	dlg->ShowWindow(SW_SHOW);                             // 다이얼로그를 띄운다.

	CArray<CPoint> rank;             // 검색결과를 정확도 순으로 정렬하기 위한 배열
	
	/********************** 스케치 영상 처리 ******************************/

	CClientDC dc(GetDlgItem(IDC_STATIC_PIC));        // Picture Control을 DC와 연결한다.

	// 스케치 이미지 객체의 크기는 Picture Control 크기로 한다.
	IplImage *sketch = cvCreateImage(cvSize(m_rectPic.Width(), m_rectPic.Height()), IPL_DEPTH_8U, 1);

	sketch = DCtoIplImage(&dc);     // Picture Control에 사용자가 그린 스케치를 IplImage 객체로 변환한다.
	sketch->nChannels = 1;          // IplImage 객체로 변환하면 3채널이 되게 때문에 1채널로 바꾼다.

	IplImage *temp = cvCreateImage(cvSize(128,128), IPL_DEPTH_8U, 1);    // 영상처리를 위한 128x128크기의 IplImage 객체
	
	cvResize(sketch, temp, CV_INTER_NN );           // 스케치 영상을 128x128 크기로 재조정한다. 
	cvFlip(temp, temp, 0);                          // 비트맵 영상이기 때문에 위아래로 뒤집는다.


	/******************** 검색대상 영상 처리 ******************************/

	IplImage *target;

	for(int k = 0; k < m_pathArray.GetSize(); k++) {               // 선택한 폴더 내의 모든 영상에 대해 영상 대조 실시

		CString filePath = _T("");
		filePath.Format(_T("thumbnail_%d.jpg"),k);         // 썸네일 영상과 대조 실시

		CStringA strA(filePath);
		LPCSTR path = strA;                   // CString -> const char* 으로 변환

		target = cvLoadImage(path, IPL_DEPTH_8U);          // 영상을 가져온다.
		cvSmooth( target, target, CV_GAUSSIAN, 7, 7, 3);   // 검색정확도를 높이기 위해 스무딩을 실시하여 큰 엣지만 남긴다.
		cvCanny(target,target,50,100);                     // 엣지를 추출한다.

		double similarity = 0.3;              // 최소 유사도는 30%
		int total = 0, count = 0;                // total : 총 대조 픽셀 수, conut : 유사한 픽셀 수

		for(int i = 1; i < 127; i++) {
			for(int j = 1; j < 127; j++) {
				if((unsigned char)temp->imageData[j+128*i] == 0) {   // 스케치 된 픽셀일 경우 
					total++;
					int sum = 0;

					for(int x = -1; x <= 1; x++) {    // 스케치된 픽셀과 같은 좌표에 해당하는 검색 대상 영상의 픽셀에서 
						for(int y = -1; y <= 1; y++)  // 그 픽셀을 중심으로 9픽셀에 대해 픽셀 값의 합을 구한다.
							sum += (unsigned char)target->imageData[(j+y)+128*(i+x)];
					}

					if( sum > 128 * 5)         // 9개의 픽셀 중에 5픽셀 이상이 엣지인 경우
						count++;               // 유사한 픽셀이라고 판단한다.
				}			
			}
		}
		//cvNamedWindow("ske", 1);
		//cvShowImage("ske", temp);

		//cvNamedWindow("tar", 1);
		//cvShowImage("tar", target);

		//cvWaitKey(0);

		// 일치율이 최소 일치율 이상 AND 사용자가 스케치를 했을 경우
		if( (double) count / total > similarity && total != 0)  
			rank.Add(CPoint(k,count));       // 검색 대상 영상의 인덱스와 유사한 픽셀의 수를 배열에 저장한다.

		cvReleaseImage(&target);
	}

	/*********************** 검색 결과 처리 ***************************/

	int  size = rank.GetSize();
	for(int i = 0; i < size; i++) {        // 검색 결과만큼 반복
		int max_index = 0;                 // max_index : 유사도가 높은 영상의 인덱스
		int max = 0;                       // max : 유사도 최대값
		int index = 0;                     // index : 검색결과 배열의 인덱스

		for(int j = 0; j < rank.GetSize(); j++) {  // 남은 검색 결과만큼 반복
			if(rank.GetAt(j).y > max) {            // 유사도 최대값 보다 클 경우
				max_index = rank.GetAt(j).x;       
				max = rank.GetAt(j).y;
				index = j;
			}
		}
		rank.RemoveAt(index);                     // 유사도가 높은 영상에 대한 값(인덱스, 유사도) 버리기

		// 경로값을 날리고 영상 제목과 확장자만 표시
		CString title = _T("");
		int n = m_pathArray.GetAt(max_index).ReverseFind('\\') + 1;
		title = m_pathArray.GetAt(max_index).Mid(n, m_pathArray.GetAt(max_index).GetLength() - n);
			
		m_listCtrl.SetImageList(&m_imageList, LVSIL_NORMAL);      // 결과 결과 리스트 내의 영상 리스트에 영상 등록
		m_listCtrl.InsertItem(max_index, title, max_index);		  // 검색 결과 리스트에 영상 제목과 영상 등록
	}

	cvReleaseImage(&sketch);
	cvReleaseImage(&temp);

	dlg->DestroyWindow();           // 알림 메시지 다이얼로그 종료
	delete dlg;
}

// 지우기 버튼 함수
// : 사용자가 지우기 버튼을 누를 경우를 처리한다.
//   다이얼로그를 새로 그리기 위해 Invalidate()함수를 호출하고
//   스케치 정보와 검색결과를 초기화한다.
void CImageSearchDlg::OnBnClickedClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	Invalidate();                        // OnPaint() 함수 재호출

	m_penArray.RemoveAll();              // 스케치 기록을 전부 버린다.
	m_penIndex.RemoveAll();
	m_listCtrl.DeleteAllItems();         // 검색결과도 버린다.
}

// 취소 버튼 함수
// : 사용자가 취소 버튼을 누를 경우를 처리한다.
//   마지막 획을 버리고 첫 획부터 이전 획까지를 다시 그려준다.
void CImageSearchDlg::OnBnClickedUndo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if(m_penIndex.GetSize() == 0)        // 아예 스케치를 안한 경우
		return;

	int end = m_penArray.GetSize() - 1;    // 마지막 획의 마지막 좌표의 인덱스
	int start = m_penIndex.GetAt(m_penIndex.GetSize() - 1);    // 마지막 획의 시작 좌표의 인덱스
	
	for(int i = end; i >= start; i--)       
		m_penArray.RemoveAt(i);            // 마지막 획을 버린다.

	undo = true;                          // 실행 취소 선택!!

	Invalidate();                         // OnPaint() 함수 호출. 마지막 획을 지운 결과를 다시 그려줘야 한다.
}

// 폴더 선택 함수
// : 사용자가 폴더 선택 버튼을 클릭할 경우의 처리를 한다.
//   폴더 선택 다이얼로그를 띄우고 사용자가 폴더를 선택하면
//   해당 폴더 내의 영상 파일을 탐색한다. 그 후 모든 영상 파일에
//   대해 썸네일을 만들어 검색결과 영상 리스트(DB 개념)에 넣어 둔다.
void CImageSearchDlg::OnBnClickedPath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	/************************* 폴더 선택 다이얼로그 띄우기 *****************************/

	BROWSEINFO Brinfo;             // 선택된 폴더의 정보를 받아 들이는 객체
	TCHAR szBuffer[512];
	::ZeroMemory(&Brinfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	Brinfo.hwndOwner = GetSafeHwnd();                 // 오너 윈도우 지정
	Brinfo.lpszTitle = _T("폴더를 선택해주세요.");

	// 플래그 설정 : 새로운 다이얼로그 스타일 | 에디트 박스 표시 | 파일 시스템 디렉토리만 선택 가능
	Brinfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;    
	LPITEMIDLIST pItemdList = ::SHBrowseForFolder(&Brinfo);

	::SHGetPathFromIDList(pItemdList, szBuffer);          // 선택된 폴더 경로를 szBuffer에 받아온다.

	MessageDlg *dlg = new MessageDlg(this);          // 알림 메시지 다이얼로그 생성
	dlg->Create(IDD_MESSAGE,this);
	dlg->SetMessage(_T("폴더를 읽고 있습니다."));     // 알림 메시지 설정
	dlg->ShowWindow(SW_SHOW);                        // 다이얼로그 호출
	
	m_editPath.SetWindowText(szBuffer);                  // 메인 다이얼로그 하단에 선택한 폴더 경로 표시
	FileSearch(szBuffer, true);                              // 선택된 폴더 내의 영상 파일 탐색 시작!!

	/*********************** 썸네일 만들기 *****************************/

 
	m_imageList.DeleteImageList();            // 이미지 리스트 초기화
	m_imageList.Create(128, 128, ILC_MASK|ILC_COLORDDB, 10, 10);   // 각 영상은 128x128 사이즈로 설정

	int num = 0;

	// 찾아낸 영상 파일 갯수 만큼 반복
	for(int i = 0; i < m_pathArray.GetSize(); i++) { 
 
 		CStringA strA(m_pathArray.GetAt(i));
		LPCSTR path = strA;

		IplImage *img = cvLoadImage(path);              // 영상 가져와서
		IplImage *icon = cvCreateImage(cvSize(128,128),img->depth,img->nChannels);    
		cvResize(img,icon, CV_INTER_NN);            // 128x128로 축소

		char filename[50];
		sprintf(filename, "thumbnail_%d.jpg", num);      

		cvSaveImage(filename, icon);    // jpg 포멧으로 썸네일 저장. 검색 시 유사도 측정을 위한 영상.
		
		CString filePath = _T("");
		filePath.Format(_T("thumbnail_%d.jpg"),i);

		CImage image;

		HRESULT hResult = image.Load(filePath);       // 썸네일을 다시 읽어 온다.
		num++;

		m_imageList.Add(CBitmap::FromHandle(image), RGB(0,128,128));  // 이미지 리스트에 추가한다. 결과출력용 영상.
 
	}

	dlg->DestroyWindow();           // 알림 메시지 다이얼로그 삭제
	delete dlg;
}

// 폴더 탐색 함수
// : 재귀호출하여 영상 파일을 찾을 때까지 탐색한다.
//   사용자가 선택한 폴더의 경로를 받아온 후 bmp나 jpg파일을 찾을 때까지 탐색한다. 
bool CImageSearchDlg::FileSearch(CString PathName/* 경로 */, bool bOption/* 하위 폴더 탐색 옵션 */)
{
	CString TempPathName;
	CFileFind cfile;

	BOOL bFind = false;

	TempPathName = PathName + _T("\\*.*");      // 루트 경로로 설정
	bFind = cfile.FindFile(TempPathName);


	while(bFind)                               // 찾을 때까지 반복
	{
		bFind = cfile.FindNextFile();          // 다음 파일 탐색

		
		if(cfile.IsDots()) // .이나 .. 일 경우
			continue;
	
		if(cfile.IsDirectory() && bOption) //하위 폴더 체크 
		{ 

			TempPathName = PathName;
			TempPathName = TempPathName + _T("\\") + cfile.GetFileName();
			FileSearch(TempPathName, true); // 재귀 호출 

			continue;
		}

		CString FileName = PathName + _T("\\") + cfile.GetFileName();

		// bmp 파일 이거나 jpg 파일일 경우
		if(FileName.Find(_T(".bmp")) > -1 || FileName.Find(_T(".jpg")) > -1 ) {
			int n = 0;
			CString FileName2 = _T("");
			for(int i = 0; i < FileName.GetLength(); i++) {
				if(FileName.GetAt(i) == '\\') {
					FileName2 += FileName.Mid(n,i-n) + _T("\\");      // \를 하나씩 덧 붙인다.
					n = i;
				}
			}
			FileName2 += FileName.Mid(n);

			m_pathArray.Add(FileName2);          // 영상 파일 경로 배열에 저장
		}
	}

	cfile.Close();

	return true;
}

