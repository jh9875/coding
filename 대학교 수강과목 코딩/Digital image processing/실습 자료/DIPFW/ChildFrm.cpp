// ChildFrm.cpp : CChildFrame Ŭ������ ����
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


// CChildFrame ����/�Ҹ�

CChildFrame::CChildFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡�� Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame ����

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


// CChildFrame �޽��� ó����

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
