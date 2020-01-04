/*	Mission 02
	다음과 C프로그램을 작성하시오.
	int, double 및 char 변수 또는 세 변수 각각에 대한 포인터를 선언하고 초기화한 뒤,	
	6개의 변수 각각의 메모리 크기(바이트)와 저장된 값 및 주소를 출력할 수 있게 만들어보시오.

	힌트(1): %x 형식 지정자는 16진수 주소를 출력한다. 다음과 같은 주소가 표시되어야한다: "bfe55918".
	힌트(2): 각 변수에 할당 된 메모리 크기를 알려주기 위한 sizeof 연산자를 사용하다: sizeof(a)
*/
#include<stdio.h>
int main() {
	int i = 20, *pi = &i;
	double f = 4.132, *pf = &f;
	char c = 'A', *pc = &c;

	printf("int 변수값:\t\t%d\n", i);
	printf("double 변수값:\t\t%f\n", f);
	printf("char 변수값:\t\t%c\n", c);
	printf("int의 주소:\t\t%x\n", &i);
	printf("double의 주소:\t\t%x\n", &f);
	printf("char의 주소:\t\t%x\n\n", &c);
	printf("int* 값:\t\t%x\n", pi);
	printf("double* 값:\t\t%x\n", pf);
	printf("char* 값:\t\t%x\n", pc);
	printf("int*를 통해 변수값:\t%d\n", *pi);
	printf("double*를 통해 변수값:\t%f\n", *pf);
	printf("char*를 통해 변수값:\t%c\n", *pc);
	printf("int 변수의 메모리크기:\t%d\n", sizeof(i));
	printf("double 변수의 메모리크기:%d\n", sizeof(f));
	printf("char 변수의 메모리크기:\t%d\n", sizeof(c));
	printf("int*의 메모리크기:\t%d\n", sizeof(pi));
	printf("double*의 메모리크기:\t%d\n", sizeof(pf));
	printf("char*의 메모리크기:\t%d\n", sizeof(pc));
	system("pause");
	return 0;
}