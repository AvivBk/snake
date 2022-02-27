#define	_CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define ROWS 25
#define COLS 75

typedef struct _Item {
	char data;
	int x;
	int y;
	struct _Item *next;
} Item;
typedef struct _Snake {
	Item *head;
	Item *tail;

} Snake;



void printBoard(void);
void init(void);
void gotoxy(int x, int y);
void sleep(float secs);
void moveByInput(Snake *snake, char ch);
Item *createItem(char data, Item *next, int x, int y);
Snake makeEmptySnake(void);
void addToHead(Snake *snake, int x, int y);
void printSnake(Snake *snake);
void freeSnake(Snake *snake);
Snake MakeSnake();
void expandSnake(Snake* snake, char input);
void addToTail(Snake *snake, int x, int y);
void trackTail(Snake *snake);
void bodyPosition(Snake *snake);
bool eatHimself(Snake *snake);
bool checkInput(char c);
void playGame(void);
bool NotDying(Snake *snake, char c, char prevC);
bool playOneMore();


int main()
{
	bool game = true;



	while (game) {
		init();
		printBoard();

		playGame();
		printBoard();


		gotoxy(27, 2);
		printf("FAILD!!\n");
		game = playOneMore();
		if (game == true)
			continue;
		else {
			init();
			printf("ty come agein\n");
			break;
		}
	}
	return 0;
}


bool playOneMore() {
	char c;
	printf("whould you like to start a new game ? Y/N:\n");
	c = _getch();
	if (c == 'y' || c == 'Y')
		return true;
	else return false;
}
Snake MakeSnake()
{
	Snake snake = makeEmptySnake();
	addToHead(&snake, ROWS / 2, COLS / 2);
	addToTail(&snake, ROWS / 2, COLS / 2 - 1);
	return snake;
}

Item *createItem(char data, Item *next, int x, int y)
{
	Item *newItem;
	newItem = (Item*)malloc(sizeof(Item));
	newItem->data = data;
	newItem->next = next;
	newItem->x = x;
	newItem->y = y;
	return newItem;
}

Snake makeEmptySnake(void)
{
	Snake snake = { NULL, NULL, };

	return snake;
}

void addToTail(Snake *snake, int x, int y) {
	Item *newItem = createItem(' ', NULL, x, y);

	if (snake->head == NULL)
		snake->head = snake->tail = newItem;
	else {
		snake->tail->next = newItem;
		snake->tail = newItem;
	}
}

void addToHead(Snake *snake, int x, int y) {

	Item *newItem = createItem('@', NULL, x, y);

	if (snake->head == NULL)
		snake->head = snake->tail = newItem;
	else {
		newItem->next = snake->head;
		snake->head = newItem;
	}
}

void bodyPosition(Snake *snake)
{
	Item* curr = snake->head;

	int prevX = curr->x;
	int prevY = curr->y;
	int temp1, temp2;

	curr = curr->next;

	while (curr->next != NULL)
	{
		temp1 = curr->x;
		temp2 = curr->y;
		curr->x = prevX;
		curr->y = prevY;
		prevX = temp1;
		prevY = temp2;
		curr = curr->next;
	}

	curr->x = prevX;
	curr->y = prevY;

}

void playGame(void)
{
	char c = 'd', prevC = 'd';
	int counter = 0;
	float time = 0.3;
	Snake snake = MakeSnake();

	while (NotDying(&snake, c, prevC))
	{

		printSnake(&snake);
		sleep(0.5);
		

		if (_kbhit()) {
			c = _getch();
			if (NotDying(&snake, c, prevC) == false)
				break;
			if (checkInput(c) == false)
				c = prevC;
		}
		
		if (counter % 5 == 0 && counter >= 5) {
			expandSnake(&snake, c);
			if (time > 0.03)
				time = time * 0.97;


		}
		//bodyPosition(&snake);
		trackTail(&snake);
		moveByInput(&snake, c);



		if (eatHimself(&snake) == false) {
			break;
		}
		prevC = c;
		counter++;
	}

	freeSnake(&snake);
}

void trackTail(Snake *snake)
{
	Item* curr = snake->head;

	int prevX = curr->x;
	int prevY = curr->y;
	int temp1, temp2;

	curr = curr->next;

	while (curr->next != NULL)
	{
		temp1 = curr->x;
		temp2 = curr->y;
		curr->x = prevX;
		curr->y = prevY;
		prevX = temp1;
		prevY = temp2;
		curr = curr->next;
	}

	curr->x = prevX;
	curr->y = prevY;

}


void printSnake(Snake *snake)
{
	Item *current = snake->head;

	while (current != NULL) {
		gotoxy(current->x, current->y);
		printf("%c", current->data);
		current = current->next;

	}

}

void freeSnake(Snake *snake)
{
	Item *current = snake->head;
	Item *next;

	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}

	snake->head = snake->tail = NULL;

}

void init(void) {
	system("cls");
}

void gotoxy(int x, int y) {
	printf("\x1b[%d;%df", x, y);
}

void sleep(float secs) {
	clock_t clocks_start = clock();

	while (clock() - clocks_start < secs*CLOCKS_PER_SEC)
		;
}
bool checkInput(char c)
{
	if (c != 'd' && c != 'a' && c != 'w' && c != 's')
		return false;

	else
		return true;
}

void moveByInput(Snake *snake, char ch)
{
	Item *curr = snake->head;
	switch (ch)
	{
	case 'd':
		curr->y++;
		break;
	case 'a':
		curr->y--;
		break;
	case 'w':
		curr->x--;
		break;
	case 's':
		curr->x++;
		break;
	}

}
void expandSnake(Snake* snake, char c)
{
	Item *curr = snake->head;

	int newX = curr->x, newY = curr->y;

	switch (c)
	{
	case 'd':
		newY = (curr->y) + 1;
		break;
	case 'a':
		newY = (curr->y) - 1;
		break;
	case 'w':
		newX = (curr->x) - 1;
		break;
	case 's':
		newX = (curr->x) + 1;
		break;
	}

	addToHead(snake, newX, newY);


}
bool eatHimself(Snake *snake)
{
	Item *curr = snake->head;

	if (curr->next == NULL)
		return true;


	else {
		curr = curr->next;
		while (curr != NULL) {
			if (curr->x == snake->head->x && curr->y == snake->head->y &&curr->data == '@')
				return false;
			else
				curr = curr->next;
		}
	}
}



void printBoard(void)
{
	int col;
	int row;

	for (row = 1; row <= ROWS; row++) {
		for (col = 1; col <= COLS; col++) {
			if (row == 1 || col == 1 || row == ROWS || col == COLS) {
				gotoxy(row, col);
				printf("#");
			}

		}
	}

}
bool NotDying(Snake *snake, char c, char prevC)
{
	Item *head = snake->head;

	if (head->y >= COLS || head->y == 1 || head->x >= ROWS || head->x == 1)
		return false;
	if ((c == 'd' && prevC == 'a') || (c == 'a' && prevC == 'd') || (c == 'w' && prevC == 's') || (c == 's' && prevC == 'w'))
		return false;
	else
		return true;
}


