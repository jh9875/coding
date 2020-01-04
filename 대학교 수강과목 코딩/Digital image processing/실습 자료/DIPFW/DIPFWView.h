// DIPFWView.h : CDIPFWView Ŭ������ �������̽�
//


#pragma once


class CDIPFWView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CDIPFWView();
	DECLARE_DYNCREATE(CDIPFWView)

// Ư���Դϴ�.
public:
	CDIPFWDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
protected:

// �����Դϴ�.
public:
	virtual ~CDIPFWView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void InitializeOutput();

protected:
	BITMAPINFO* m_pBmInfo;
	int		m_nHeight;
	int		m_nWidth;

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DIPFWView.cpp�� ����� ����
inline CDIPFWDoc* CDIPFWView::GetDocument() const
   { return reinterpret_cast<CDIPFWDoc*>(m_pDocument); }
#endif

