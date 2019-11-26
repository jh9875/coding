package level1;

import java.util.Scanner;

public class NO10828
{
	static int[] arrayStack =new int[20];
	static int size =0;
	
	public static void main(String[] args)
	{
		Scanner input = new Scanner(System.in);
		int count =input.nextInt();
		
		while(count-- >0)
		{
			
		}
		input.close();
	}
	
	static void push(int n)
	{
		arrayStack[size++] =n;
	}
	static int pop()
	{
		if(empty() ==1)
			return -1;
		else
			return arrayStack[size--];
	}
	static int size()
	{
		return size;
	}
	static int empty()
	{
		if(size ==0)
			return 1;
		else 
			return 0;
	}
	static int top()
	{
		if(empty() ==1)
			return -1;
		else
			return arrayStack[size];
		
	}
	
}
