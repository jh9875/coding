// DIPFWDoc.cpp : CDIPFWDoc 클래스의 구현
//
#include <stdlib.h>
#include <math.h>

#include "stdafx.h"
#include "DIPFW.h"

#include "DIPFWDoc.h"
#include "InSizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.141592
#define FILTER_SIZE 9
#define N 8

// CDIPFWDoc

IMPLEMENT_DYNCREATE(CDIPFWDoc, CDocument)

BEGIN_MESSAGE_MAP(CDIPFWDoc, CDocument)	
	ON_COMMAND(ID_POINTPROCESSING_INVERSE, &CDIPFWDoc::OnInverse)
	ON_COMMAND(ID_POINTPROCESSING_TEST, &CDIPFWDoc::test)
	ON_COMMAND(ID_FILTERING_MAKEANDPEPPER, &CDIPFWDoc::OnFilteringMakeandpepper)
	ON_COMMAND(ID_FILTERING_LPF, &CDIPFWDoc::OnFilteringLpf)
	ON_COMMAND(ID_FILTERING_HPF, &CDIPFWDoc::OnFilteringHpf)
	ON_COMMAND(ID_FILTERING_MEDIAN, &CDIPFWDoc::OnFilteringMedian)
	ON_COMMAND(ID_POINTPROCESSING_DILATION, &CDIPFWDoc::OnPointprocessingDilation)
	ON_COMMAND(ID_POINTPROCESSING_EROSION, &CDIPFWDoc::OnPointprocessingErosion)
	ON_COMMAND(ID_POINTPROCESSING_OPENING, &CDIPFWDoc::OnPointprocessingOpening)
	ON_COMMAND(ID_POINTPROCESSING_CLOSING, &CDIPFWDoc::OnPointprocessingClosing)
	ON_COMMAND(ID_IMAGETRANSFORMS_DCT, &CDIPFWDoc::OnImagetransformsDct)
	ON_COMMAND(ID_IMAGETRANSFORMS_IDCT, &CDIPFWDoc::OnImagetransformsIdct)
	ON_COMMAND(ID_TRUNCATIONCODING_N, &CDIPFWDoc::OnTruncationcodingN)
	ON_COMMAND(ID_TRUNCATIONCODING_RESTORENEARESTNEIGHBOR, &CDIPFWDoc::OnTruncationcodingRestorenearestneighbor)
	ON_COMMAND(ID_TRUNCATIONCODING_AVERAGE, &CDIPFWDoc::OnTruncationcodingAverage)
	ON_COMMAND(ID_TRUNCATIONCODING_RESTOREAVERAGE, &CDIPFWDoc::OnTruncationcodingRestoreaverage)
	ON_COMMAND(ID_ADCT2_S1, &CDIPFWDoc::OnAdct2S1)
	ON_COMMAND(ID_ADCT2_S2, &CDIPFWDoc::OnAdct2S2)
END_MESSAGE_MAP()


// CDIPFWDoc 생성/소멸

CDIPFWDoc::CDIPFWDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_pImgOpen1D = NULL;
	m_pImgOpen   = NULL;
	m_pImgResult1D = NULL;
	m_pImgResult = NULL;
	m_pImgRGB    = NULL;
}

CDIPFWDoc::~CDIPFWDoc()
{
}

BOOL CDIPFWDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.
	::OpenClipboard(NULL);
	if (!IsClipboardFormatAvailable(CF_DIB)) return FALSE;
	HGLOBAL m_hImage = ::GetClipboardData(CF_DIB);
	::EmptyClipboard();
	::CloseClipboard();

	LPSTR pDIB = (LPSTR) ::GlobalLock((HGLOBAL)m_hImage);
	memcpy( &m_nHeight, pDIB,               sizeof(int) );
	memcpy( &m_nWidth,  pDIB+(sizeof(int)), sizeof(int) );

	InitImage();
	memcpy( m_pImgOpen1D, pDIB+(2*sizeof(int)), m_nHeight*m_nWidth );
	
	ReadyDisplayImage();
	return TRUE;
}

void CDIPFWDoc::CopyClipboard(BYTE* m_pImgSrc, int nWidth, int nHeight)
{
	DWORD dwBitsSize = nHeight * nWidth * sizeof(char) + (2*sizeof(int));
	HGLOBAL m_hImage = (HGLOBAL)::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	LPSTR pDIB = (LPSTR)::GlobalLock((HGLOBAL)m_hImage);

	memcpy( pDIB,                 &nHeight, sizeof(int) );
	memcpy( pDIB+(1*sizeof(int)), &nWidth,  sizeof(int) );
	memcpy( pDIB+(2*sizeof(int)), m_pImgSrc, nHeight*nWidth );

	::OpenClipboard(NULL);
	::SetClipboardData(CF_DIB, m_hImage);
	::CloseClipboard();

	::GlobalUnlock((HGLOBAL)m_hImage);
	GlobalFree(m_hImage);
}

// CDIPFWDoc serialization

void CDIPFWDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
		//File* fp = ar.GetFile();
		ar.Write(m_pImgOpen1D, m_nWidth*m_nHeight);
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.		
		CInSizeDlg InsizeDlg;						// Size Dialog instance.

		if (InsizeDlg.DoModal() == IDOK)			// Dialog getting Image Size
		{
			BeginWaitCursor();						// Cursor waiting

			m_nWidth  = InsizeDlg.getWidth();
			m_nHeight = InsizeDlg.getHeight();

 			InitImage();							// Memory Allocation Images(Open,Result)
 			ar.Read(m_pImgOpen1D, m_nWidth*m_nHeight);
 			ReadyDisplayImage();

			UpdateAllViews(NULL);					// Update Document data to View
			EndWaitCursor();						// Cursor return
		}
	}
}

void CDIPFWDoc::DeleteContents()
{
	if (m_pImgOpen)
		delete [] m_pImgOpen;
	if (m_pImgOpen1D)
		delete [] m_pImgOpen1D;
	if (m_pImgResult)
		delete [] m_pImgResult;
	if (m_pImgResult1D)
		delete [] m_pImgResult1D;
	if (m_pImgRGB)
	{
		if (m_pImgRGB[0])
			delete [] (m_pImgRGB[0]);
		delete [] m_pImgRGB;
	}

	CDocument::DeleteContents();
}


// CDIPFWDoc 진단

#ifdef _DEBUG
void CDIPFWDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDIPFWDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDIPFWDoc 명령

int	CDIPFWDoc::getWidth()
{
	return m_nWidth;
}

int	CDIPFWDoc::getHeight()
{
	return m_nHeight;
}

BYTE** CDIPFWDoc::getRGBImg()
{
	return m_pImgRGB;
}

BYTE* CDIPFWDoc::getOpenImg()
{
	return m_pImgOpen1D;
}

void CDIPFWDoc::InitImage()
{
	int iY;
	if (!m_pImgOpen1D)
		m_pImgOpen1D = new BYTE [m_nWidth*m_nHeight];
	
	if (m_pImgOpen)
	{
		if (m_pImgOpen[0])
			delete [] (m_pImgOpen[0]);
		delete [] m_pImgOpen;
	}
	m_pImgOpen = new BYTE* [m_nHeight];
	for(iY = 0; iY < m_nHeight; iY++)
		m_pImgOpen[iY] = m_pImgOpen1D + (m_nWidth * iY);

	if (!m_pImgRGB)
	{
		m_pImgRGB    = new BYTE* [m_nHeight];
		m_pImgRGB[0] = new BYTE  [3*m_nWidth*m_nHeight];
	}	
	for(iY = 1; iY < m_nHeight; iY++)
		m_pImgRGB[iY] = *m_pImgRGB + (3 * m_nWidth * iY);
}

void CDIPFWDoc::ReadyDisplayImage()
{
	int	iY, iX;
	for(iY = 0; iY < m_nHeight; iY++) 
	{
		for(iX = 0; iX < m_nWidth; iX++)
		{
			m_pImgRGB[iY][3*iX+2] = m_pImgRGB[iY][3*iX+1] = m_pImgRGB[iY][3*iX] = m_pImgOpen1D[iY*m_nWidth+iX];
		}
	}
}

BYTE** CDIPFWDoc::CreateResultImage(int nWidth, int nHeight)
{
	if (m_pImgResult1D)
		delete [] m_pImgResult1D;
	if (m_pImgResult)
		delete [] m_pImgResult;

	m_pImgResult1D = new BYTE [nWidth * nHeight];
	m_pImgResult = new BYTE* [nHeight];
	for(int iY = 0; iY < nHeight; iY++)
		m_pImgResult[iY] = m_pImgResult1D + (nWidth * iY);

	return m_pImgResult;
}

void CDIPFWDoc::CreateNewImage(BYTE*** pImg, int nWidth, int nHeight)
{
	(*pImg)    = new BYTE* [nHeight];
	(*pImg)[0] = new BYTE  [nWidth * nHeight];
	for(int iY = 1; iY < nHeight; iY++)
		(*pImg)[iY] = (*pImg)[0] + (nWidth * iY);
}

void CDIPFWDoc::ReleaseImage(BYTE** pImg)
{
	if (pImg[0])
		delete [] (pImg[0]);
	if (pImg)
		delete [] pImg;
}


void CDIPFWDoc::MakeNewWindow(BYTE* m_pImgResult, int nWidth, int nHeight)
{
	CopyClipboard(m_pImgResult, nWidth, nHeight);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}

void CDIPFWDoc::MakeNewWindow(BYTE** m_pImgResult, int nWidth, int nHeight)
{
	BYTE* pImg = new BYTE [nWidth * nHeight];
	for(int iY = 0; iY < nHeight; iY++)
		memcpy((pImg + iY*nWidth), m_pImgResult[iY], sizeof(BYTE)*nWidth);
	CopyClipboard(pImg, nWidth, nHeight);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
	delete [] pImg;
}

// CDIPFWDoc 알고리즘 구현
//

// 이미지반전 예제
void CDIPFWDoc::OnInverse()
{
	/*
	 *	1) input image 의 width 와 height 는 각각 m_nWidth, m_nHeight
	 *	2) input image 는 m_pImgOpen 에 2차원 배열로 존재
	 *	3) CreateResultImage(TargetWidth, TargetHeight)
	 *	4) 마지막 종료시 MakeNewWindow(TargetImage, TargetWidth, TargetHeight);
	 *	5) 알고리즘 구현에 메모리가 필요하다면, CreateResultImage()이외에 new를 사용한 동적할당도 사용 가능
	 */
	
	int i, j;
	
	/*
	 *	원본 영상에 대해서 처리한 후 결과는 pImg 를 이용해서 출력
	 *	pImg 를 만들어준 이유는 원본 영상 자체에 접근하는것은 좋지 못한 방법이기 때문
	 */
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);

	for(i = 0; i < m_nHeight; i++)
	{
		for(j = 0; j < m_nWidth; j++)
		{
			pImg[i][j] = 255 - m_pImgOpen[i][j];
		}
	}

	// 새로운 창을 띄워서 결과 영상 출력
	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}


void CDIPFWDoc::test()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j;
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);

	for (i = 0; i < m_nHeight; i++)
	{
		for (j = 0; j < m_nWidth; j++)
		{
			pImg[i][j] = 255;
		}
	}

	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}


void CDIPFWDoc::OnFilteringMakeandpepper()
{
	int i, j;
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);

	for (i = 0; i < m_nHeight; i++)
	{
		for (j = 0; j < m_nWidth; j++)
		{
			if (i == 0 || j == 0 || i == m_nHeight - 1 || j == m_nWidth - 1)	//모서리부분
			{
				pImg[i][j] = m_pImgOpen[i][j];
			}
			else
			{
				if (rand() % 50 < 1)			//확률 2/50
					pImg[i][j] = 0;
				else if (rand() % 50 > 48)
					pImg[i][j] = 255;
				else
					pImg[i][j] = m_pImgOpen[i][j];
			}
		}
	}

	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}


void CDIPFWDoc::OnFilteringLpf()
{
	int i, j;
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);

	BYTE M[3][3] = { {1, 1, 1},
					{1, 1, 1},
					{1, 1, 1} };
	double maskSize = (double)sizeof(M) / sizeof(BYTE);

	for (i = 0; i < m_nHeight; i++)
	{
		for (j = 0; j < m_nWidth; j++)
		{
			if (i == 0 || j == 0 || i == m_nHeight - 1 || j == m_nWidth-1)	//모서리부분
			{
				pImg[i][j] = m_pImgOpen[i][j];
			}
			else
			{
				pImg[i][j] = (m_pImgOpen[i - 1][j - 1] * M[0][0] + m_pImgOpen[i - 1][j] * M[0][1]
					+ m_pImgOpen[i - 1][j + 1] * M[0][2] + m_pImgOpen[i][j - 1] * M[1][0]
					+ m_pImgOpen[i][j] * M[1][1] + m_pImgOpen[i][j + 1] * M[1][2]
					+ m_pImgOpen[i + 1][j - 1] * M[2][0] + m_pImgOpen[i + 1][j] * M[2][1]
					+ m_pImgOpen[i + 1][j + 1] * M[2][2]) / maskSize;
			}
		}
	}

	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}


void CDIPFWDoc::OnFilteringHpf()
{
	int i, j;
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);

	int M[3][3] = { {-1, -1, -1},
					{-1, 8, -1},
					{-1, -1, -1} };

	for (i = 0; i < m_nHeight; i++)
	{
		for (j = 0; j < m_nWidth; j++)
		{
			if (i == 0 || j == 0 || i == m_nHeight - 1 || j == m_nWidth - 1)
			{
				pImg[i][j] = m_pImgOpen[i][j];
			}
			else
			{
				int n = (m_pImgOpen[i - 1][j - 1] * M[0][0] + m_pImgOpen[i - 1][j] * M[0][1]
					+ m_pImgOpen[i - 1][j + 1] * M[0][2] + m_pImgOpen[i][j - 1] * M[1][0]
					+ m_pImgOpen[i][j] * M[1][1] + m_pImgOpen[i][j + 1] * M[1][2]
					+ m_pImgOpen[i + 1][j - 1] * M[2][0] + m_pImgOpen[i + 1][j] * M[2][1]
					+ m_pImgOpen[i + 1][j + 1] * M[2][2]) ;
				
				n = abs(n);
				pImg[i][j] = n;
			}
		}
	}

	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}

int compare(const void* first, const void* second)
{
	if (*(int*)first > *(int*)second)
		return 1;
	else if (*(int*)first < *(int*)second)
		return -1;
	else
		return 0;
}

void CDIPFWDoc::OnFilteringMedian()
{
	int i, j;
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);

	int M[9];
	int maskSize = sizeof(M) / sizeof(int);

	for (i = 0; i < m_nHeight; i++)
	{
		for (j = 0; j < m_nWidth; j++)
		{
			if (i == 0 || j == 0 || i == m_nHeight - 1 || j == m_nWidth - 1)
			{
				pImg[i][j] = m_pImgOpen[i][j];
			}
			else
			{
				int k = 0;
				M[k++] = m_pImgOpen[i - 1][j - 1];
				M[k++] = m_pImgOpen[i - 1][j];
				M[k++] = m_pImgOpen[i - 1][j + 1];
				M[k++] = m_pImgOpen[i][j - 1];
				M[k++] = m_pImgOpen[i][j];
				M[k++] = m_pImgOpen[i][j + 1];
				M[k++] = m_pImgOpen[i + 1][j - 1];
				M[k++] = m_pImgOpen[i + 1][j];
				M[k++] = m_pImgOpen[i + 1][j + 1];

				qsort(M, maskSize, sizeof(int), compare);

				pImg[i][j] = M[maskSize / 2];
			}
		}
	}

	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}


void CDIPFWDoc::OnPointprocessingDilation()
{
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);
	int M[9];

	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			if (i == 0 || j == 0 || i == m_nHeight - 1 || j == m_nWidth - 1)
			{
				pImg[i][j] = m_pImgOpen[i][j];
			}
			else
			{
				int idx = 0;

				for (int k = -1; k < 2; k++)
				{
					for (int l = -1; l < 2; l++)
					{
						M[idx++] = m_pImgOpen[i+k][j+l];
					}
				}
				qsort(M, FILTER_SIZE, sizeof(int), compare);
				pImg[i][j] = M[FILTER_SIZE - 1];
			}
		}
	}
	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}


void CDIPFWDoc::OnPointprocessingErosion()
{
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);
	int M[9];

	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			if (i == 0 || j == 0 || i == m_nHeight - 1 || j == m_nWidth - 1)
			{
				pImg[i][j] = m_pImgOpen[i][j];
			}
			else
			{
				int idx = 0;

				for (int k = -1; k < 2; k++)
				{
					for (int l = -1; l < 2; l++)
					{
						M[idx++] = m_pImgOpen[i + k][j + l];
					}
				}
				qsort(M, FILTER_SIZE, sizeof(int), compare);
				pImg[i][j] = M[0];
			}
		}
	}
	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}
/*
BYTE** maxValue(BYTE** temp, int m_nWidth, int m_nHeight)
{
	
	BYTE** pImg = (BYTE**)malloc(sizeof(BYTE**)*sizeof(m_nWidth)*sizeof(m_nHeight));

	int M[9];

	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			if (i == 0 || j == 0 || i == m_nHeight - 1 || j == m_nWidth - 1)
			{
				pImg[i][j] = temp[i][j];
			}
			else
			{
				int idx = 0;

				for (int k = -1; k < 2; k++)
				{
					for (int l = -1; l < 2; l++)
					{
						M[idx++] = temp[i + k][j + l];
					}
				}
				qsort(M, FILTER_SIZE, sizeof(int), compare);
				pImg[i][j] = M[FILTER_SIZE -1];
			}
		}
	}
	return pImg;
	
}
BYTE** minValue(BYTE** temp, int m_nWidth, int m_nHeight)
{
	BYTE** pImg = (BYTE**)malloc(sizeof(BYTE**) * sizeof(m_nWidth) * sizeof(m_nHeight));

	int M[9];

	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			if (i == 0 || j == 0 || i == m_nHeight - 1 || j == m_nWidth - 1)
			{
				pImg[i][j] = temp[i][j];
			}
			else
			{
				int idx = 0;

				for (int k = -1; k < 2; k++)
				{
					for (int l = -1; l < 2; l++)
					{
						M[idx++] = temp[i + k][j + l];
					}
				}
				qsort(M, FILTER_SIZE, sizeof(int), compare);
				pImg[i][j] = M[0];
			}
		}
	}
	return pImg;
}
*/
void CDIPFWDoc::OnPointprocessingOpening()
{
	/*
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);
	pImg = minValue(m_pImgOpen, m_nWidth, m_nHeight);
	pImg = maxValue(pImg, m_nWidth, m_nHeight);
	MakeNewWindow(pImg, m_nWidth, m_nHeight);
	*/
}


void CDIPFWDoc::OnPointprocessingClosing()
{
	/*
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);
	pImg = maxValue(m_pImgOpen, m_nWidth, m_nHeight);
	pImg = minValue(pImg, m_nWidth, m_nHeight);
	MakeNewWindow(pImg, m_nWidth, m_nHeight);
	*/
}

double** getDCT(int height, int width, BYTE** openImg)
{
	//반환할 이미지 할당.
	double** result = (double**)malloc(sizeof(double*) * height);
	double B[N][N];			//N ==8
	double Bt[N][N];
	double part[N][N];
	double temp[N][N];
	double k;

	for (int i = 0; i < height; i++)
	{
		result[i] = (double*)malloc(sizeof(double) * width);
	}

	//기저함수 초기화
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (i == 0)
			{
				k = sqrt(1 / (double)N);
			}
			else
			{
				k = sqrt(2 / (double)N);
			}

			B[i][j] = k * cos((2 * j + 1)*i*PI / (2 * (double)N));
		}
	}
	//역 기저함수 초기화
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			Bt[i][j] = B[j][i];
		}
	}

	//DCT 연산
	for (int y = 0; y < height; y += N)
	{
		for (int x = 0; x < width; x += N)
		{
			//연산할 8*8부분 만들기
			for (int a = y; a < y + N; a++)
			{
				for (int b = x; b < x + N; b++)
				{
					part[a - y][b - x] = openImg[a][b];
				}
			}

			//F=BfBt에서 1,2 행렬 계산.
			for (int a = 0; a < N; a++)
			{
				for (int b = 0; b < N; b++)
				{
					double sum = 0;
					for (int c = 0; c < N; c++)
					{
						sum += B[a][c] * part[c][b];
					}
					temp[a][b] = sum;
				}
			}

			//F=BfB~에서 결과,3 행렬 계산
			for (int a = 0; a < N; a++)
			{
				for (int b = 0; b < N; b++)
				{
					double sum = 0;
					for (int c = 0; c < N; c++)
					{
						sum += temp[a][c] * Bt[c][b];
					}
					part[a][b] = sum;
				}
			}

			//결과에 쓰기
			for (int a = y; a < y + N; a++)
			{
				for (int b = x; b < x + N; b++)
				{
					/*
					if (part[a - y][b - x] > 255)
						part[a - y][b - x] = 255;
					else if (part[a - y][b - x] < 0)
						part[a - y][b - x] = 0;
					*/
					result[a][b] = part[a - y][b - x];
				}
			}
		}
	}

	return result;
}

void CDIPFWDoc::OnImagetransformsDct()
{
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);
	double** receive;
	receive = getDCT(m_nHeight, m_nWidth, m_pImgOpen);

	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			if (receive[y][x] < 0)
				receive[y][x] = 0;
			if (receive[y][x] > 255)
				receive[y][x] = 255;
			pImg[y][x] = receive[y][x];
		}
	}

	// 새로운 창을 띄워서 결과 영상 출력
	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}

double** getIDCT(int height, int width, double** openImg)
{
	//반환할 이미지 할당.
	double** result = (double**)malloc(sizeof(double*) * height);
	double B[N][N];			//N ==8
	double Bt[N][N];
	double part[N][N];
	double temp[N][N];
	double k;

	for (int i = 0; i < height; i++)
	{
		result[i] = (double*)malloc(sizeof(double) * width);
	}

	//기저함수 초기화
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (i == 0)
			{
				k = sqrt(1 / (double)N);
			}
			else
			{
				k = sqrt(2 / (double)N);
			}

			B[i][j] = k * cos((2 * j + 1)*i*PI / (2 * (double)N));
		}
	}
	//역 기저함수 초기화
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			Bt[i][j] = B[j][i];
		}
	}

	//DCT 연산
	for (int y = 0; y < height; y += N)
	{
		for (int x = 0; x < width; x += N)
		{
			//연산할 8*8부분 만들기
			for (int a = y; a < y + N; a++)
			{
				for (int b = x; b < x + N; b++)
				{
					part[a - y][b - x] = openImg[a][b];
				}
			}

			//F=BfBt에서 1,2 행렬 계산.
			for (int a = 0; a < N; a++)
			{
				for (int b = 0; b < N; b++)
				{
					double sum = 0;
					for (int c = 0; c < N; c++)
					{
						sum += Bt[a][c] * part[c][b];
					}
					temp[a][b] = sum;
				}
			}

			//F=BfB~에서 결과,3 행렬 계산
			for (int a = 0; a < N; a++)
			{
				for (int b = 0; b < N; b++)
				{
					double sum = 0;
					for (int c = 0; c < N; c++)
					{
						sum += temp[a][c] * B[c][b];
					}
					part[a][b] = sum;
				}
			}

			//결과에 쓰기
			for (int a = y; a < y + N; a++)
			{
				for (int b = x; b < x + N; b++)
				{
					result[a][b] = part[a - y][b - x];
				}
			}
		}
	}

	return result;
}

void CDIPFWDoc::OnImagetransformsIdct()
{
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);
	double** receive1;
	double** receive2;
	receive1 = getDCT(m_nHeight, m_nWidth, m_pImgOpen);
	receive2 = getIDCT(m_nHeight, m_nWidth, receive1);

	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			if (receive2[y][x] < 0)
				receive2[y][x] = 0;
			if (receive2[y][x] > 255)
				receive2[y][x] = 255;
			pImg[y][x] = receive2[y][x];
		}
	}
	
	// 새로운 창을 띄워서 결과 영상 출력
	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}


void CDIPFWDoc::OnTruncationcodingN()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	BYTE** pImg = CreateResultImage(m_nWidth / 2, m_nHeight / 2);

	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			if (y % 2 == 0 && x % 2 == 0)
			{
				pImg[y / 2][x / 2] = m_pImgOpen[y][x];
			}

		}
	}

	MakeNewWindow(pImg, m_nWidth / 2, m_nHeight / 2);
}


void CDIPFWDoc::OnTruncationcodingRestorenearestneighbor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	BYTE** pImg = CreateResultImage(m_nWidth*2, m_nHeight*2);
	
	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			pImg[y * 2][x * 2] = m_pImgOpen[y][x];
			pImg[y * 2 + 1][x * 2] = m_pImgOpen[y][x];
			pImg[y * 2][x * 2 + 1] = m_pImgOpen[y][x];
			pImg[y * 2 + 1][x * 2 + 1] = m_pImgOpen[y][x];		
		}
	}

	MakeNewWindow(pImg, m_nWidth*2, m_nHeight*2);
}


void CDIPFWDoc::OnTruncationcodingAverage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	BYTE** pImg = CreateResultImage(m_nWidth / 2, m_nHeight / 2);

	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			if (y % 2 == 0 && x % 2 == 0)
			{
				int sum = 0;
				sum += m_pImgOpen[y][x];
				sum += m_pImgOpen[y + 1][x];
				sum += m_pImgOpen[y][x + 1];
				sum += m_pImgOpen[y + 1][x + 1];
				sum /= 4;
				if (sum > 255)
					sum = 255;
				pImg[y / 2][x / 2] = sum;
			}

		}
	}

	MakeNewWindow(pImg, m_nWidth / 2, m_nHeight / 2);
}


void CDIPFWDoc::OnTruncationcodingRestoreaverage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	BYTE** pImg = CreateResultImage(m_nWidth * 2, m_nHeight * 2);

	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			pImg[y * 2][x * 2] = m_pImgOpen[y][x];
			pImg[y * 2 + 1][x * 2] = m_pImgOpen[y][x];
			pImg[y * 2][x * 2 + 1] = m_pImgOpen[y][x];
			pImg[y * 2 + 1][x * 2 + 1] = m_pImgOpen[y][x];
		}
	}

	MakeNewWindow(pImg, m_nWidth * 2, m_nHeight * 2);
}

double** getDctQuantizser(int height, int width, BYTE** openImg)
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	double** result = (double**)malloc(sizeof(double*) * height);
	for (int i = 0; i < height; i++)
		result[i] = (double*)malloc(sizeof(double) * width);

	double** dct;
	double QTable[N][N] = {			//N ==8
		{16, 11, 10, 16, 24, 40, 51, 61},
		{12, 12, 14, 19, 26, 58, 60, 66},
		{14, 13, 16, 24, 40, 57, 69, 57},
		{14, 17, 22, 29, 51, 87, 80, 62},
		{18, 22, 37, 56, 68, 109,103, 77},
		{24, 36, 55, 64, 81, 104, 113, 92},
		{49, 64, 78, 87, 103, 121, 120, 101},
		{72, 92,95, 98, 112, 100, 103, 99},
	};

	dct = getDCT(height, width, openImg);

	//이미지 전체에 8*8씩 양자화
	for (int y = 0; y < height; y += N)
	{
		for (int x = 0; x < width; x += N)
		{
			//8*8양자화
			for (int i = y; i < y + N; i++)
			{
				for (int j = x; j < x + N; j++)
				{
					result[i][j] = dct[i][j] / QTable[i - y][j - x];
				}
			}
		}
	}

	return result;
}

void CDIPFWDoc::OnAdct2S1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);
	double** receive;
	receive = getDctQuantizser(m_nHeight, m_nWidth, m_pImgOpen);

	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			if (receive[y][x] < 0)
				receive[y][x] = 0;
			if (receive[y][x] > 255)
				receive[y][x] = 255;
			pImg[y][x] = receive[y][x];
		}
	}

	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}

double** getIDctIQuantizser(int height, int width, double** openImg)
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	double** result = (double**)malloc(sizeof(double*) * height);
	for (int i = 0; i < height; i++)
		result[i] = (double*)malloc(sizeof(double) * width);
	double** temp = (double**)malloc(sizeof(double*) * height);
	for (int i = 0; i < height; i++)
		temp[i] = (double*)malloc(sizeof(double) * width);
	double QTable[N][N] = {
		{16, 11, 10, 16, 24, 40, 51, 61},
		{12, 12, 14, 19, 26, 58, 60, 66},
		{14, 13, 16, 24, 40, 57, 69, 57},
		{14, 17, 22, 29, 51, 87, 80, 62},
		{18, 22, 37, 56, 68, 109,103, 77},
		{24, 36, 55, 64, 81, 104, 113, 92},
		{49, 64, 78, 87, 103, 121, 120, 101},
		{72, 92,95, 98, 112, 100, 103, 99},
	};

	//이미지 전체에 8*8씩 역양자화
	for (int y = 0; y < height; y += N)
	{
		for (int x = 0; x < width; x += N)
		{
			//8*8역양자화
			for (int i = y; i < y + N; i++)
			{
				for (int j = x; j < x + N; j++)
				{
					temp[i][j] = openImg[i][j] * QTable[i - y][j - x];
				}
			}
		}
	}
	//IDCT
	result = getIDCT(height, width, temp);

	return result;
}

void CDIPFWDoc::OnAdct2S2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	BYTE** pImg = CreateResultImage(m_nWidth, m_nHeight);
	double** receive1;
	double** receive2;
	receive1 = getDctQuantizser(m_nHeight, m_nWidth, m_pImgOpen);		//DCT,Quantizser
	receive2 = getIDctIQuantizser(m_nHeight, m_nWidth, receive1);

	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			if (receive2[y][x] < 0)
				receive2[y][x] = 0;
			if (receive2[y][x] > 255)
				receive2[y][x] = 255;
			pImg[y][x] = receive2[y][x];
		}
	}

	MakeNewWindow(pImg, m_nWidth, m_nHeight);
}
