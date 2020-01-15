import java.util.Scanner;

public class No2440
{

	public static void main(String[] args)
	{
		Scanner input =new Scanner(System.in);
		
		int n =input.nextInt();
		draw(n);
		
		input.close();
	}

	public static void draw(int n)
	{
		for(int i =n; i >0; i--)
		{
			for(int j =i; j >0; j--)
				System.out.print("*");
			System.out.println();
		}
	}
}
