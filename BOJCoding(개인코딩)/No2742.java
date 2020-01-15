import java.util.Scanner;

public class No2742 
{

	public static void main(String[] args)
	{
		Scanner input =new Scanner(System.in);
	
		int n =input.nextInt();
		
		while(n >0)
		{
			System.out.println(n--);
		}
		
		input.close();
	}

}
/*
 * 자연수 N이 주어졌을 때, N부터 1까지 한 줄에 하나씩 출력하는 프로그램을 작성하시오.
 */
 