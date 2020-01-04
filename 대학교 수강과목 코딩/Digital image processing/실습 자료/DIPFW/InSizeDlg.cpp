// InSizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DIPFW.h"
#include "InSizeDlg.h"


// CInSizeDlg dialog

IMPLEMENT_DYNAMIC(CInSizeDlg, CDialog)

CInSizeDlg::CInSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInSizeDlg::IDD, pParent)
{
	m_nWidth  = 256;
	m_nHeight = 256;
}

CInSizeDlg::~CInSizeDlg()
{
}

void CInSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WIDTH,  m_nWidth);
	DDX_Text(pDX, IDC_HEIGHT, m_nHeight);
}


BEGIN_MESSAGE_MAP(CInSizeDlg, CDialog)
END_MESSAGE_MAP()


// CInSizeDlg message handlers

int CInSizeDlg::getWidth()
{
	return m_nWidth;
}

int CInSizeDlg::getHeight()
{
	return m_nHeight;
}
