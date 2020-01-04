#pragma warning (disable:4996)
#include <stdio.h>

void display(char table[3][3])		//����� ���� �Լ�
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
	int turn = 0;				//���� Ȯ���ϱ� ���� ����
	int sum = 0;				//1���� �ϼ������ Ȯ���ϱ� ���� ����.
	int done = 0;				//�������� Ȯ���ϱ����� ����

	printf("\n Enter the name of Player 0: ");	//player1 �̸� �Է¹ޱ�
	scanf("%s", p[0]);
	printf("\n Enter the name of Player 1: ");
	scanf("%s", p[1]);

	
	while (1)					//���������� �����ϱ� ������ ���ѷ���.
	{
		if (turn %2)			//0�� false�̹Ƿ� 0�� 1���� �ϳ��� �����Ƿ� ���� �����ư��鼭 ����
			printf("%s's turn. input location (row, col)(0~2): ", p[1]);
		else
			printf("%s's turn. input location (row, col)(0~2): ", p[0]);

		scanf("%d %d", &row, &col);

		//inputCheck
		//�Է¹����� �ʰ��� ���
		if (!(0 <=row && row <=2) || !(0 <= col && col <= 2) || table[row][col] != '_')
		{
			printf("invalid value. please re-enter\n");
			while (row = getchar() != '\n' && row != EOF);			//�߸��� ���� �ԷµǸ� �Է¹��۸� ����ش�.
			continue;
		}
		//����� �Ͽ� ���� o�Ǵ� x�� ǥ���Ѵ�.
		if (turn%2)
			table[row][col] = 'x';
		else
			table[row][col] = 'o';
		display(table);		//���


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
		

		if(done)			//������� ���ڸ� ����� ����
		{
			if (turn%2)
				printf("%s win\n", p[1]);
			else
				printf("%s win\n", p[0]);
			break;
		}
		turn++;				//���� �ٲٱ� ���� ���� ������.
		if (turn == 9)		//9���� �Ѿ�µ� �ȳ����� �����̹Ƿ� draw����ϰ� ����
		{
			printf("Draw!\n");
			break;
		}
			
	}
	system("pause");
}
