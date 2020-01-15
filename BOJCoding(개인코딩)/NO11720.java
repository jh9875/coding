import java.util.Scanner;

public class NO11720 
{

	public static void main(String[] args)
	{
		Scanner input =new Scanner(System.in);
		int sum =0;
		
		int n1 =input.nextInt();
		String n2 =input.next();
		
		for(int i =0; i <n2.length(); i++)
		{
			char temp =n2.charAt(i);
			sum +=(int)temp -48;
		}
		
		System.out.println(sum);
		
		input.close();
	}

}
