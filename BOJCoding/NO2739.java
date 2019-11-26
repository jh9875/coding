import java.util.Scanner;

public class NO2739
{

	public static void main(String[] args)
	{
		Scanner input =new Scanner(System.in);
		
		int n =input.nextInt();
		
		for(int i =1; i <=9; i++)
			System.out.println(n + " * " + i + " = " + n*i);
		
		input.close();
	}

}
/*
 * N을 입력받은 뒤, 구구단 N단을 출력하는 프로그램을 작성하시오. 출력 형식에 맞춰서 출력하면 된다.
 */
