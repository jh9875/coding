import java.util.ArrayList;
import java.util.Collections;
import java.util.Scanner;

public class NO2606
{
	static int[][] graph;
	static ArrayList<Integer> al;

	public static void main(String[] args)
	{
		Scanner input =new Scanner(System.in);
		
		int n1 =input.nextInt();
		int n2 =input.nextInt();

		graph =new int[n1][n1];
		al =new ArrayList();
		
		for(int i =0; i <n2; i++)
		{
			int v1 =input.nextInt();
			int v2 =input.nextInt();
			
			graph[v1-1][v2-1] =1;
			graph[v2-1][v1-1] =1;
		}
		al.add(0);
		find(0);
		al.remove(0);
		
		Collections.sort(al);
		
		System.out.println(al.size());
		
		input.close();
	}
	public static void find(int n)
	{
		for(int i =0; i <graph[n].length; i++)
		{
			if(graph[n][i] ==1)
			{
				boolean check =false;
				for(int j =0; j <al.size(); j++)
				{
					int temp =al.get(j);
					if(temp ==i)
						check =true;
					
				}
				if(check !=true)
				{
					al.add(i);
					find(i);
				}
			}
		}
	}

}

//https://www.acmicpc.net/problem/2606