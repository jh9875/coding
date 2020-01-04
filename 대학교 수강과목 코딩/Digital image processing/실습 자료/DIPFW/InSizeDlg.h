#pragma once


// CInSizeDlg dialog

class CInSizeDlg : public CDialog
{
	DECLARE_DYNAMIC(CInSizeDlg)

public:
	CInSizeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInSizeDlg();

	int getWidth();
	int getHeight();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	int m_nWidth;
	int m_nHeight;

	DECLARE_MESSAGE_MAP()
};
