#include "stdafx.h" 
#include <iostream>
#include <string>
#include <random>

using namespace std;

const string WRONG = "Wrong input!";
const string VICTORY = "Victory! Nothing special in outplaying this machine though...";
const string DRAW = "Draw. You came to the typical final of this game.";
const string DEFEAT = "Defeat. I know what you did it intentionally.";

char player;
char bot;
bool gameOver;

// Получение случайных чисел для бота
random_device rd;
mt19937 rng(rd());

void show_field(char cells[3][3])
{
	cout << "Current state:" << endl;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			cout << cells[i][j] << ' ';

		cout << endl;
	}
}

bool victory(char cells[3][3])
{
	if ((cells[0][0] != '.' && ((cells[0][0] == cells[1][0] && cells[1][0] == cells[2][0]) || (cells[0][0] == cells[0][1] && cells[0][1] == cells[0][2]) ||
		(cells[0][0] == cells[1][1] && cells[1][1] == cells[2][2]))) || (cells[1][1] != '.' && ((cells[0][1] == cells[1][1] && cells[1][1] == cells[2][1]) ||
		(cells[1][0] == cells[1][1] && cells[1][1] == cells[1][2]) || (cells[0][2] == cells[1][1] && cells[1][1] == cells[2][0]))) ||
		(cells[2][2] != '.' && ((cells[0][2] == cells[1][2] && cells[1][2] == cells[2][2]) || (cells[2][0] == cells[2][1] && cells[2][1] == cells[2][2]))))
			return true;

	return false;
}

void game_over(char res)
{
	if (res == player)
		cout << VICTORY << endl;
	else if (res == '.')
		cout << DRAW << endl;
	else if (res == bot)
		cout << DEFEAT << endl;
	else if (res == 'M')
		cout << "Player " << player << " won the game!" << endl;

	gameOver = true;
}

int read_coordinate(string message)
{
	int n;

	bool firstTry = true;
	do 
	{
		if (!firstTry)
		{
			cin.clear();
			while (cin.peek() != '\n')
				cin.get();
			cout << WRONG << endl;
		}

		cout << message;
		cin >> n;

		firstTry = false;
	} while (!cin || cin.peek() != '\n' || n < 1 || n > 3);

	return n - 1;
}

void third_in_a_row(char symbol, int &row, int &column, char cells[3][3])
{
	for (int i = 0; i < 3; i++)
		if (row == -1)
			for (int j = 0; j < 3; j++)
			{
				if (cells[i][j] == '.')
				{
					int inRow = 0;

					// Проверка по вертикали
					for (int k = 0; k < 3; k++)
						if (k != i && cells[k][j] == symbol)
							inRow++;

					if (inRow == 2)
					{
						row = i;
						column = j;
						break;
					}

					inRow = 0;

					// Проверка по горизонтали
					for (int k = 0; k < 3; k++)
						if (k != j && cells[i][k] == symbol)
							inRow++;

					if (inRow == 2)
					{
						row = i;
						column = j;
						break;
					}

					inRow = 0;

					// Проверка по главной диагонали
					if (i == j)
						for (int k = 0; k < 3; k++)
							if (k != i && cells[k][k] == symbol)
								inRow++;

					if (inRow == 2)
					{
						row = i;
						column = j;
						break;
					}

					inRow = 0;

					// Проверка по побочной диагонали
					if (i + j == 2)
						for (int k = 0; k < 3; k++)
							if (k != i && cells[k][2 - k] == symbol)
								inRow++;

					if (inRow == 2)
					{
						row = i;
						column = j;
						break;
					}
				}
			}
}

// Бот, если это возможно, выиграет игру в этот ход или помешает сделать это игроку
bool win_or_prevent(char (&cells)[3][3]) 
{
	int row = -1;
	int column = -1;

	third_in_a_row(bot, row, column, cells);

	bool botWin = false;
	if (row == -1)
		third_in_a_row(player, row, column, cells);
	else
		botWin = true;

	if (row != -1)
	{
		cout << "Bot sets '" << bot << "' at the position (" << row + 1 << ", " << column + 1 << ")" << endl;
		cells[row][column] = bot;
		show_field(cells);
		if(botWin)
			game_over(bot);

		return true;
	}

	return false;
}

void random_turn(int empty, char(&cells)[3][3])
{
	bool set = false;
	uniform_int_distribution<int> uni(1, empty);

	while (!set)
	{
		for(int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (!set && cells[i][j] == '.' && rand() % uni(rng) == 0)
				{
					cout << "Bot sets '" << bot << "' at the position (" << i + 1 << ", " << j + 1 << ")" << endl;
					cells[i][j] = bot;
					show_field(cells);
					set = true;
				}
	}
}

bool yes_or_no(string message)
{
	char reply;

	bool firstTry = true;
	do
	{
		if (!firstTry)
		{
			while (cin.peek() != '\n')
				cin.get();
			cout << WRONG << endl;
		}

		cout << message;
		cin >> reply;

		firstTry = false;
	} while (cin.peek() != '\n' || (reply != 'n' && reply != 'N' && reply != 'y' && reply != 'Y'));

	if (reply == 'y' || reply == 'Y')
		return true;

	return false;
}

int main()
{
	cout << "___ Tic-tac-toe ___" << endl; 

	do
	{
		gameOver = false;
		bool multiplayer = false;

		char reply;
		bool firstTry = true;
		do
		{
			if (!firstTry)
			{
				while (cin.peek() != '\n')
					cin.get();
				cout << WRONG << endl;
			}
			
			cout << "Enable multiplayer? (Y/N): ";
			cin >> reply;

			firstTry = false;
		} while (cin.peek() != '\n' || (reply != 'n' && reply != 'N' && reply != 'y' && reply != 'Y'));

		if (reply == 'y' || reply == 'Y')
			multiplayer = true;

		if (!multiplayer)
		{
			bool firstTry = true;
			do
			{
				if (!firstTry)
				{
					while (cin.peek() != '\n')
						cin.get();
					cout << WRONG << endl;
				}

				cout << "Choose your sign (X/O): ";
				cin >> player;

				firstTry = false;
			} while (cin.peek() != '\n' || (player != 'x' && player != 'X' && player != 'o' && player != 'O' && player != '0'));

			if (player == 'x' || player == 'X')
			{
				player = 'X';
				bot = 'O';
			}
			else if (player == 'o' || player == 'O' || player == '0')
			{
				player = 'O';
				bot = 'X';
			}
		}

		char cells[3][3] = { { '.', '.', '.' }, { '.', '.', '.' }, { '.', '.', '.' } };
		show_field(cells);

		int turn = 1;
		int row = 0;
		int column = 0;
		while (!gameOver)
		{
			if (multiplayer)
			{
				turn % 2 == 1 ? player = 'X' : player = 'O';

				cout << player << " player's turn" << endl;

				do
				{
					row = read_coordinate("Input the number of the row: ");
					column = read_coordinate("Input the number of the column: ");

					if (cells[row][column] != '.')
						cout << "This cell is already setted as " << cells[row][column] << ", choose another position!" << endl;
				} while (cells[row][column] != '.');

				cells[row][column] = player;
				show_field(cells);

				if (victory(cells))
					game_over('M');
			}
			else if (!multiplayer)
			{
				if ((turn % 2 == 1 && player == 'X') || (turn % 2 == 0 && player == 'O')) // Ход игрока
				{
					cout << "Your turn" << endl;

					do
					{
						row = read_coordinate("Input the number of the row: ");
						column = read_coordinate("Input the number of the column: ");

						if (cells[row][column] != '.')
							cout << "This cell is already setted as " << cells[row][column] << ", choose another position!" << endl;
					} while (cells[row][column] != '.');

					cells[row][column] = player;
					show_field(cells);

					if (victory(cells))
						game_over(player);
				}
				else if (!win_or_prevent(cells)) // Ход бота
					random_turn(10 - turn, cells);
			}

			if (turn == 9 && !gameOver) // Последний ход, ничья
				game_over('.');

			turn++;
		}
	} while (yes_or_no("Play again? (Y/N): "));

	return 0;
}