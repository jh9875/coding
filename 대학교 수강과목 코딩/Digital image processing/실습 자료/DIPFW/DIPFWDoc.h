// DIPFWDoc.h : CDIPFWDoc 클래스의 인터페이스
//


#pragma once

typedef unsigned char BYTE;

class CDIPFWDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CDIPFWDoc();
	DECLARE_DYNCREATE(CDIPFWDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();

// 구현입니다.
public:
	virtual ~CDIPFWDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	int	   getWidth();
	int	   getHeight();
	BYTE*  getOpenImg();
	BYTE** getRGBImg();

	void InitImage();
	void ReadyDisplayImage();
	void CopyClipboard(BYTE* m_pImgSrc, int nWidth, int nHeight);
	BYTE** CreateResultImage(int nWidth, int nHeight);
	void CreateNewImage(BYTE*** pImg, int nWidth, int nHeight);
	void ReleaseImage(BYTE** pImg);
	void MakeNewWindow(BYTE* m_pImgResult, int nWidth, int nHeight);
	void MakeNewWindow(BYTE** m_pImgResult, int nWidth, int nHeight);

protected:
	BYTE*	m_pImgOpen1D;	// 1D open image
	BYTE**  m_pImgOpen;		// 2D open image
	BYTE*	m_pImgResult1D;	// for temporary storing
	BYTE**	m_pImgResult;	// for temporary storing	
	BYTE**	m_pImgRGB;		// for display

	int		m_nWidth;		// image width
	int		m_nHeight;		// image height

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnInverse();
	afx_msg void test();
	afx_msg void OnFilteringMakeandpepper();
	afx_msg void OnFilteringLpf();
	afx_msg void OnFilteringHpf();
	afx_msg void OnFilteringMedian();
	afx_msg void OnPointprocessingDilation();
	afx_msg void OnPointprocessingErosion();
	afx_msg void OnPointprocessingOpening();
	afx_msg void OnPointprocessingClosing();
	afx_msg void OnImagetransformsDct();
	afx_msg void OnImagetransformsIdct();
	afx_msg void OnTruncationcodingN();
	afx_msg void OnTruncationcodingRestorenearestneighbor();
	afx_msg void OnTruncationcodingAverage();
	afx_msg void OnTruncationcodingRestoreaverage();
	afx_msg void OnAdctStep1();
	afx_msg void OnAdctStep2();
	afx_msg void OnAdct2S1();
	afx_msg void OnAdct2S2();
};


