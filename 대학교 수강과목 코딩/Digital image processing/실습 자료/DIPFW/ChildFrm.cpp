// ChildFrm.cpp : CChildFrame 클래스의 구현
//
#include "stdafx.h"
#include "DIPFW.h"
#include "DIPFWDoc.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()


// CChildFrame 생성/소멸

CChildFrame::CChildFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame 진단

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame 메시지 처리기

void CChildFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	CRect rect, rectC;
	GetWindowRect(&rect);						// Full Window Size
	GetClientRect(&rectC);						// View Window Size

	CSize sizeImg;

	sizeImg.cx = ((CDIPFWDoc*)GetActiveDocument())->getWidth();
	sizeImg.cy = ((CDIPFWDoc*)GetActiveDocument())->getHeight();
	int cx = sizeImg.cx + rect.Width()  - rectC.Width()  + 4;
	int cy = sizeImg.cy + rect.Height() - rectC.Height() + 4;
	SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_SHOWWINDOW);

	CMDIChildWnd::ActivateFrame(nCmdShow);
}
