/*	Mission 01
	매트릭스 A[2x2](배열 2차원)는 백터 b[1x2](배열 1차원)를 곱해서 백터[1x2](배열 1차원) 결과를 
	나타내는 C파일 작성해보세요.
*/
#include<stdio.h>
int main() {
	int i,j,sum;
	int A[2][2] =	{	{1,2},
						{3,4}
					},
		b[2] =		{5,6},
		result[2];

	//계산
	printf("A x b =\n");
	for(i = 0; i < 2; i++) {
		sum = 0;
		for(j = 0; j < 2; j++)
			sum += (A[i][j] * b[j]);
		printf("| %2d |\n", result[i] = sum);
	}
	system("pause");
	return 0;
}