#pragma warning (disable:4996)
#include <stdio.h>

void display(char table[3][3])		//출력을 위한 함수
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			printf("%c", table[i][j]);
		printf("\n");
	}
	printf("\n");
}

int main()
{
	char table[3][3] = { {'_', '_', '_'}, {'_', '_', '_'}, {'_', '_', '_'}};
	char markers[2] = {'o', 'x'};
	char p[2][20];
	int made[2] = {3*'o', 3*'x'};
	int row, col;				
	int turn = 0;				//턴을 확인하기 위한 변수
	int sum = 0;				//1줄이 완성됬는지 확인하기 위한 변수.
	int done = 0;				//끝났는지 확인하기위한 변수

	printf("\n Enter the name of Player 0: ");	//player1 이름 입력받기
	scanf("%s", p[0]);
	printf("\n Enter the name of Player 1: ");
	scanf("%s", p[1]);

	
	while (1)					//종료조건을 만족하기 전까진 무한루프.
	{
		if (turn %2)			//0은 false이므로 0과 1둘중 하나가 나오므로 턴이 번갈아가면서 나옴
			printf("%s's turn. input location (row, col)(0~2): ", p[1]);
		else
			printf("%s's turn. input location (row, col)(0~2): ", p[0]);

		scanf("%d %d", &row, &col);

		//inputCheck
		//입력범위를 초과한 경우
		if (!(0 <=row && row <=2) || !(0 <= col && col <= 2) || table[row][col] != '_')
		{
			printf("invalid value. please re-enter\n");
			while (row = getchar() != '\n' && row != EOF);			//잘못된 값이 입력되면 입력버퍼를 비워준다.
			continue;
		}
		//사용자 턴에 따라서 o또는 x를 표시한다.
		if (turn%2)
			table[row][col] = 'x';
		else
			table[row][col] = 'o';
		display(table);		//출력


		//check condition
		//row
		for (int i = 0; i < 3; i++)
		{
			sum = 0;
			for (int j = 0; j < 3; j++)
			{
				if (table[i][j] == '_')
					break;
				sum += table[i][j];
			}
			if (sum == made[0] || sum == made[1])
				done = 1;
			if (done)
				break;
		}
		//col
		for (int i = 0; i < 3; i++)
		{
			sum = 0;
			for (int j = 0; j < 3; j++)
			{
				if (table[j][i] == '_')
					break;
				sum += table[j][i];
			}
			if (sum == made[0] || sum == made[1])
				done = 1;
			if (done)
				break;
		}

		//diagonal
		if (table[0][0] + table[1][1] + table[2][2] ==made[0] ||
			table[0][0] + table[1][1] + table[2][2] == made[1])
			done = 1;
		if (table[0][2] + table[1][1] + table[2][0] == made[0] ||
			table[0][2] + table[1][1] + table[2][0] == made[1])
			done = 1;
		

		if(done)			//끝난경우 승자를 출력후 종료
		{
			if (turn%2)
				printf("%s win\n", p[1]);
			else
				printf("%s win\n", p[0]);
			break;
		}
		turn++;				//턴을 바꾸기 위해 턴을 더해줌.
		if (turn == 9)		//9번이 넘어갔는데 안끝나면 비긴것이므로 draw출력하고 종료
		{
			printf("Draw!\n");
			break;
		}
			
	}
	system("pause");
}
