// DIPFWView.h : CDIPFWView 클래스의 인터페이스
//


#pragma once


class CDIPFWView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CDIPFWView();
	DECLARE_DYNCREATE(CDIPFWView)

// 특성입니다.
public:
	CDIPFWDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
protected:

// 구현입니다.
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

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DIPFWView.cpp의 디버그 버전
inline CDIPFWDoc* CDIPFWView::GetDocument() const
   { return reinterpret_cast<CDIPFWDoc*>(m_pDocument); }
#endif

