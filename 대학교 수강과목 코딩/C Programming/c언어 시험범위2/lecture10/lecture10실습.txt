#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct book
{
	char bookTitle[20];
	char nameOfAuthor[20];
	int volumes;
	book* next;
}book;

book* library = NULL;			//global variable

void addBook()
{
	char bookName[20];
	char author[20];

	printf("Input Book title: ");
	scanf("%s", bookName);
	printf("Input Author: ");
	scanf("%s", author);

	if (library == NULL)			//library에 book이 없는경우.
	{
		library = (book*)malloc(sizeof(book));

		book* newBook = (book*)malloc(sizeof(book));
		strcpy(newBook->bookTitle, bookName);
		strcpy(newBook->nameOfAuthor, author);
		newBook->volumes = 1;
		newBook->next =NULL;
		
		library->next = newBook;
	}
	else
	{													//library에 1권이상 있는경우.
		book* curPos = library->next;							//리스트 탐색할 변수
		int check = 0;

		while (curPos != NULL)
		{
			if (strcmp(bookName, curPos->bookTitle) ==0)			//library에 책이 있는경우.
			{		
				if (strcmp(curPos->nameOfAuthor, author) ==0)		//저자 이름도 같은경우 한권을 더해준다.
				{
					curPos->volumes += 1;
					check = 1;
					break;
				}
				
			}
			curPos = curPos->next;
		}
		if (!check)			//같은 이름의 책이 없는경우 + 같은이름의 책은 있지만 저자이름이 다른경우. 마지막에 새로운 book추가
		{
			curPos = library;

			book* newBook = (book*)malloc(sizeof(book));
			strcpy(newBook->bookTitle, bookName);
			strcpy(newBook->nameOfAuthor, author);
			newBook->volumes = 1;
			newBook->next =NULL;

			while (curPos->next != NULL)
				curPos = curPos->next;
			curPos->next = newBook;
		}
	}
	
	printf("================================================\n");
	printf("The book entitled %s is added to the library\n", bookName);
	printf("================================================\n");
}

void bookbyAuthor()
{
	if(library ==NULL)
	{
		printf("Doesn't exist author\n");
		return;
	}
	book* curPos =library->next;
	char author[20];
	int check =0;

	printf("Input Author: ");
	scanf("%s", author);

	while (curPos !=NULL)
	{
		/*
		printf("library의 저자: %s\n", curPos->nameOfAuthor);
		printf("원하는 저자: %s\n", author);
		*/
		if (strcmp(author, curPos->nameOfAuthor) ==0)			//library에 같은이름의 저자가 있으면 출력.
		{
			printf("================================================\n");
			printf("Book title: %s\n", curPos->bookTitle);
			printf("Author information: %s\n", curPos->nameOfAuthor);
			printf("Number of books in the library: %d\n", curPos->volumes);
			printf("================================================\n\n\n");
			check =1;
		}
		curPos = curPos->next;
	}
	if(check ==0)
		printf("Doesn't exist author: %s\n", author);

}
void countBooks()
{
	if (library == NULL)			//library가 NULL이면 1권도 없는경우.
	{
		printf("================================================\n");
		printf("total number of books in the library: 0\n");
		printf("================================================\n");
		return;
	}
	int sum = 0;
	book* curPos = library->next;

	while (curPos != NULL)
	{
		sum += curPos->volumes;
		curPos = curPos->next;
	}

	printf("================================================\n");
	printf("total number of books in the library: %d\n", sum);
	printf("================================================\n");
}

void borrowBook()
{
	if(library ==NULL)
	{
		printf("there isn't any books!\n");
		return;
	}
	char bookName[20];
	int check = 0;				//책이 library에 있는지 확인하는 변수 없으면0, 있으면1
	book* curPos = library->next;

	printf("Input Book title: ");
	scanf("%s", bookName);

	while (curPos !=NULL)
	{
		if (strcmp(bookName, curPos->bookTitle) ==0)	//책이 library에 등록되어 있는경우
		{
			check =1;
			if (curPos->volumes ==0)			//책이 1권이상 있는경우(0이 아닌경우)
			{
				printf("================================================\n");
				printf("Oops! Sorry. All books in the library are currently on loan.\n");
				printf("================================================\n");
			}else
			{
				printf("================================================\n");
				printf("You should return the book within next 30 days. Here is is.\n");
				printf("================================================\n");
				curPos->volumes -= 1;
			}
			break;
		}
			
		curPos = curPos->next;
	}
	if(check ==0)			//원하는 책이 아예없는경우.
	{
		printf("================================================\n");
		printf("The book that you have requested is not currently available.\n");
		printf("================================================\n");
	}
}

/*
void print()
{
	book* cur =library->next;
	while(cur !=NULL)
	{
		printf("%s\n", cur->bookTitle);
		cur = cur->next;
	}
}
*/

int main()
{
	int menu;

	while (1)
	{
		printf("1. Add a new Book\n");
		printf("2. Display all the books in the library of a particular author\n");
		printf("3. Display the total number of books in the library\n");
		printf("4. Borrow a book\n");
		printf("0. Quit program\n\n");
		printf("--> Choose a menu in the list:");
		scanf("%d", &menu);

		switch (menu)
		{
		case 0:
			return 0;
		case 1:
			addBook();
			break;
		case 2:
			bookbyAuthor();
			break;
		case 3:
			countBooks();
			break;
		case 4:
			borrowBook();
			break;
		default:
			printf("You have input a wrong number\n");
			continue;
		}
		
	}
}