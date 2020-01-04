// DIPFWView.cpp : CDIPFWView Ŭ������ ����
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

// CDIPFWView ����/�Ҹ�

CDIPFWView::CDIPFWView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	m_pBmInfo = NULL;
}

CDIPFWView::~CDIPFWView()
{
	if (m_pBmInfo)
		free(m_pBmInfo);
}

BOOL CDIPFWView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CDIPFWView �׸���

void CDIPFWView::OnDraw(CDC* pDC)
{
	CDIPFWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
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

// CDIPFWView ����

#ifdef _DEBUG
void CDIPFWView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDIPFWView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDIPFWDoc* CDIPFWView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDIPFWDoc)));
	return (CDIPFWDoc*)m_pDocument;
}
#endif //_DEBUG


// CDIPFWView �޽��� ó����

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