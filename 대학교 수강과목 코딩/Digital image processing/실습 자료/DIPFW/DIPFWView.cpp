// DIPFWView.cpp : CDIPFWView 클래스의 구현
//

#include "stdafx.h"
#include "DIPFW.h"

#include "DIPFWDoc.h"
#include "DIPFWView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDIPFWView

IMPLEMENT_DYNCREATE(CDIPFWView, CScrollView)

BEGIN_MESSAGE_MAP(CDIPFWView, CScrollView)
END_MESSAGE_MAP()

// CDIPFWView 생성/소멸

CDIPFWView::CDIPFWView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_pBmInfo = NULL;
}

CDIPFWView::~CDIPFWView()
{
	if (m_pBmInfo)
		free(m_pBmInfo);
}

BOOL CDIPFWView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CDIPFWView 그리기

void CDIPFWView::OnDraw(CDC* pDC)
{
	CDIPFWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	InitializeOutput();
	// Display
	SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, m_nWidth, m_nHeight, 0, 0, 0, m_nHeight, *(pDoc->getRGBImg()), m_pBmInfo, DIB_RGB_COLORS);
}

void CDIPFWView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal(500,500);
	CDIPFWDoc* pDoc = GetDocument();
	if (pDoc->getOpenImg())
		sizeTotal = CSize(pDoc->getWidth(), pDoc->getHeight());
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
}

// CDIPFWView 진단

#ifdef _DEBUG
void CDIPFWView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDIPFWView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDIPFWDoc* CDIPFWView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDIPFWDoc)));
	return (CDIPFWDoc*)m_pDocument;
}
#endif //_DEBUG


// CDIPFWView 메시지 처리기

void CDIPFWView::InitializeOutput()					// Fast Output Function
{
	CDIPFWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_nHeight = pDoc->getHeight();
	m_nWidth  = pDoc->getWidth();
	//int rwsize = (((m_nWidth)+31)/32*4);			// make image width to multiple number of 4 bytes 

	m_pBmInfo = (LPBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER));
	m_pBmInfo->bmiHeader.biBitCount = 24;
	m_pBmInfo->bmiHeader.biClrImportant = 0;
	m_pBmInfo->bmiHeader.biClrUsed = 0;
	m_pBmInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmInfo->bmiHeader.biHeight = -m_nHeight;
	m_pBmInfo->bmiHeader.biWidth = m_nWidth;
	m_pBmInfo->bmiHeader.biPlanes = 1;
	m_pBmInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmInfo->bmiHeader.biSizeImage = 3*m_nWidth*m_nHeight;
	m_pBmInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmInfo->bmiHeader.biYPelsPerMeter = 0;
}