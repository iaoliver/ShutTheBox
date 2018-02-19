/************************************************************************************************************************************
*	Author:			Ian OLiver																										*
*	Program:		ShutTheBox Pt1,2,3,4																							*
*	Current Date:	1/22/2018																										*
*	Due Date:		1/24/2018																										*
*	Discription:	A random numbers game where when a number is "rolled" the coorasponding box is closed. When all boxes are shut	*
*					 then the game is over and winner is declared.																	*
*																																	*
*************************************************************************************************************************************/

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <Windows.h>

using namespace std;

void menu(HANDLE hConsole, char &prompt, int &players, int &winner, int numBoard[]);
void readInstr(HANDLE hConsole);
void rollDice(int &die1, int &die2, bool &doubles);
bool displayBoard(int numBoard[], HANDLE hConsole);
bool allShut(int numBoard[]);
int play1(int numBoard[], HANDLE hConsole);
int play2(int numBoard[], HANDLE hConsole);
int playAI();


int main()
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	//handler for the console

	char prompt;								//instruction prompt vairable
	int players;								//int for keeping track players
	int winner;									//winner comp:0 firstPlayer:1 secondPlayer:2
	int numBoard[9]{ 1,2,3,4,5,6,7,8,9 };		//making the board

	SetConsoleTextAttribute(hConsole, 0x07);	//set console text colors to fun

	srand(time(NULL));							//seed for the rng dice roll

	menu(hConsole, prompt, players, winner, numBoard);

	switch (winner)
	{
		case 0:
			cout << "Player One Wins!" << endl;
			break;
		case 1:
			cout << "Player Two Wins!" << endl;
			break;
		case 2:
			cout << "Computer Players Wins!" << endl;
			break;
	}


	cout << endl;
	system("pause");
	return 0;
}

/****************************************************************************************\
*	menu: initial game display and settings selectors									*
*	Pass-In: HANDLE hConsole, int numBoard[]											*
*	Pass-by-Reference: char prompt, int players, int winner								*
*	Return: Void																		*
\****************************************************************************************/
void menu(HANDLE hConsole, char &prompt, int &players, int &winner, int numBoard[])
{

	SetConsoleTextAttribute(hConsole, 0x06);
	cout << "  ___  _   _  __  __  ____     ____  _   _  _____     ____  _____  _  _ " << endl;
	cout << " / __)( )_( )(  )(  )(_  _)   (_  _)( )_( )( ____)   (  _ \\(  _  )( \\/ )" << endl;
	cout << " \\__ \\ ) _ (  )(__)(   )(       )(   ) _ (  ) _)      ) _ < )(_)(  )  ( " << endl;
	cout << " (___/(_) (_)(______) (__)     (__) (_) (_)(_____)   (____/(_____)(_/\\_)" << endl << endl;
	SetConsoleTextAttribute(hConsole, 0x07);
	
	cout << "Do you want to read the instructions for Shut The Box? (Y/N:) " << endl;
	cin >> prompt;
	cout << endl << endl;

	if (tolower(prompt) == 'y') //this block checks for file open and no errors
	{
		readInstr(hConsole);
	}

	cout << endl << "Game Setting: \n1. Single Player\n2. Two Player\n3. Player vs Comp" << endl;
	cin >> players;

	switch (players)
	{
	case 1: 
		winner = play1(numBoard, hConsole);
		break;
	case 2:
		winner = play2(numBoard, hConsole);
		break;
	case 3:
		winner = playAI();
		break;
	}
  
}

/****************************************************************************************\
 *	readInstr: reads the instructions for the shut the box game from .txt file			*
 *	Pass-In: HANDLE hConsole															*
 *	Return: Void																		*
\****************************************************************************************/
void readInstr(HANDLE hConsole)
{
	ifstream fin;
	string rules;

	fin.open("shutTheBoxInstructions.txt");

	if (fin.is_open())
	{
		SetConsoleTextAttribute(hConsole, 0x06);	//changing console text colors for flair
		while (getline(fin, rules))
		{
			cout << rules << endl;
		}
		fin.close();
		SetConsoleTextAttribute(hConsole, 0x07);	//changing console text colors back to program defualt

	}
	else
	{
		SetConsoleTextAttribute(hConsole, 0x0c);	//console is red for error
		cout << "Error occured while trying to open file..." << endl;
	}
	SetConsoleTextAttribute(hConsole, 0x07);		//console back to program default
}

/*******************************************************************************\
 *	rollDice: simulates 2D6 rolling and checks for valid use for game	       *
 *	Pass by Reference: die1, die2
 *	Return: Void
\*******************************************************************************/
void rollDice(int &die1, int &die2, bool &doubles)
{
	char prompt;
	doubles = false;
	die1 = rand() % 6 + 1;
	die2 = rand() % 6 + 1;

	if (die1 == die2)
	{
		doubles = true;
	}
	cout << "Rolled " << die1 << " and " << die2 << "." << endl;

	//this block to check the sum range for random roll, cannot exceed 2-9 range
	if ((die1 + die2) > 9)
	{
		cout << "Dice sum is greater than board, can only use individual rolls." << endl;
	}
	else
	{
		cout << "Use dice sum or individual rolls? " << endl << "A. Dice Sum\nB. Individual Rolls" << endl;
		cin >> prompt;
		if (tolower(prompt) == 'a')
		{
			die1 = die1 + die2;		//sum is placed in die1
			die2 = -1;				//die2 is set to null
		}
		else if (tolower(prompt) == 'b')
		{

		}
	}
}

/****************************************************************************************\
 *	displayBoard: displays the board array on the console screen						*
 * 	Pass-In: int numBoard[]																*
 *	Return: Void																		*
\****************************************************************************************/
bool displayBoard(int numBoard[], HANDLE hConsole)
{
	SetConsoleTextAttribute(hConsole, 0x8f);

	for (int i = 0; i < 9; i++)
	{		//block for top parts of open box lids and open air
		if ((i <= 0) && (numBoard[i] > 0)) { cout << "   __________"; }
		if ((i > 0) && (numBoard[i] > 0)) { cout << " __________"; }
		if ((i <= 0) && (numBoard[i] < 1)) { cout << "             "; }
		if ((i > 0) && (numBoard[i] < 1)) { cout << "           "; }
	}
	cout << endl;

	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 9; i++)
		{		//block for middle parts of open box lids and open air
			if ((i <= 0) && (numBoard[i] > 0)) { cout << "   |        |"; }
			if ((i > 0) && (numBoard[i] > 0)) { cout << " |        |"; }
			if ((i <= 0) && (numBoard[i] < 1)) { cout << "             "; }
			if ((i > 0) && (numBoard[i] < 1)) { cout << "           "; }
		}
		cout << endl;
	}

	for (int i = 0; i < 9; i++)
	{		//block for bottom part of open box lids and hinges
		if ((i <= 0) && (numBoard[i] > 0)) { cout << "   |________|"; }
		if ((i > 0) && (numBoard[i] > 0)) { cout << " |________|"; }
		if ((i <= 0) && (numBoard[i] < 1)) { cout << "   _________ "; }
		if ((i > 0) && (numBoard[i] < 1)) { cout << " _________ "; }
	}
	cout << endl;
	cout << "  /        /|/        /|/        /|/        /|/        /|/        /|/        /|/        /|/        /|" << endl;
	cout << " /        / /        / /        / /        / /        / /        / /        / /        / /        / |" << endl;
	cout << "/________/ /________/ /________/ /________/ /________/ /________/ /________/ /________/ /________/  |" << endl;
	cout << "|        | |        | |        | |        | |        | |        | |        | |        | |        |  /" << endl;
//	cout << "|   1    | |   2    | |   3    | |   4    | |   5    | |   6    | |   7    | |   8    | |   9    | / " << endl;

	for (int i = 0; i < 9; i++)
	{
		if (numBoard[i] > 0)
		{
			cout << "|   " << numBoard[i] << "    | ";
		}
		else
		{		//block sets color scheme of box numbers to silver-black, use i instead of array numbers
			cout << "|   ";
			SetConsoleTextAttribute(hConsole, 0x80);
			cout << i + 1 ;
			SetConsoleTextAttribute(hConsole, 0x8f);//reset default for board colors
			cout << "    | ";
		}
	}
	cout <<"/ " << endl;
	cout << "|________|/|________|/|________|/|________|/|________|/|________|/|________|/|________|/|________|/  " << endl;
	cout << endl;
	SetConsoleTextAttribute(hConsole, 0x07);//reset console colors for menu

	return allShut(numBoard);
}

/****************************************************************************************\
*	allShut: function used in displayBoard to check if all boxes are shut				*
* 	Pass-In: int numBoard[]																*
*	Return: bool shut																	*
\****************************************************************************************/
bool allShut(int numBoard[])
{
	bool shut = true;

	for (int i = 0; i < 9; i++)
	{
		if (numBoard[i] > 0)
		{
			shut = false;
		}
	}
	return shut;
}

/****************************************************************************************\
*	play1: single player game function for single player use 							*
* 	Pass-In: int numBoard[], HANDLE hConsole											*
*	Return: int winner																	*
\****************************************************************************************/
int play1(int numBoard[], HANDLE hConsole) 
{
	int die1, die2, totalRolls;
	bool doubles = false;								//vairable for finding doubles in dice roll
	bool gameOver = false;
	cout << endl;										//flush the io buffer
	system("CLS");										//clears the console for redraw

	SetConsoleTextAttribute(hConsole, 0x06);
	cout << "  ___  _   _  __  __  ____     ____  _   _  _____     ____  _____  _  _ " << endl;
	cout << " / __)( )_( )(  )(  )(_  _)   (_  _)( )_( )( ____)   (  _ \\(  _  )( \\/ )" << endl;
	cout << " \\__ \\ ) _ (  )(__)(   )(       )(   ) _ (  ) _)      ) _ < )(_)(  )  ( " << endl;
	cout << " (___/(_) (_)(______) (__)     (__) (_) (_)(_____)   (____/(_____)(_/\\_)" << endl << endl;
	SetConsoleTextAttribute(hConsole, 0x07);
	cout << "                   <<<Single Player Game STARTED!>>>                    " << endl << endl;

	totalRolls = 0;
	gameOver = displayBoard(numBoard, hConsole);
	while (gameOver == false)
	{
		rollDice(die1, die2, doubles);
		totalRolls++;									//counter for dice rolls
		if (numBoard[die1 - 1] > 0)						//checks for open or shut boxes
		{
			numBoard[die1 - 1] = 0;
		}
		else
		{
			numBoard[die1 - 1] = (die1);
		}
		if ((die2 > 0)&& (numBoard[die2 -1] > 0))		//checks for open or shut boxes
		{ 
			numBoard[die2 - 1] = 0;
		}
		else
		{
			numBoard[die2 - 1] = (die2);
		}
		gameOver = displayBoard(numBoard, hConsole);
	}
	cout << "Total number of dice rolled: " << totalRolls * 2 << endl;
	return 0;
}

/****************************************************************************************\
*	play2: two player game function for two player sharing keyboard						*
* 	Pass-In: int numBoard[], HANDLE hConsole											*
*	Return: int winner																	*
\****************************************************************************************/
int play2(int numBoard[], HANDLE hConsole)
{
	int die1, die2, player, totalRolls;
	bool doubles = false;								//vairable for finding doubles in dice roll
	bool gameOver = false;								
	cout << endl;										//flush the io buffer
	system("CLS");										//clears the console for redraw

	SetConsoleTextAttribute(hConsole, 0x06);
	cout << "  ___  _   _  __  __  ____     ____  _   _  _____     ____  _____  _  _ " << endl;
	cout << " / __)( )_( )(  )(  )(_  _)   (_  _)( )_( )( ____)   (  _ \\(  _  )( \\/ )" << endl;
	cout << " \\__ \\ ) _ (  )(__)(   )(       )(   ) _ (  ) _)      ) _ < )(_)(  )  ( " << endl;
	cout << " (___/(_) (_)(______) (__)     (__) (_) (_)(_____)   (____/(_____)(_/\\_)" << endl << endl;
	SetConsoleTextAttribute(hConsole, 0x07);
	cout << "                      <<<Two Player Game STARTED!>>>                    " << endl << endl;

	player = 0;											//player set to first player by defualt
	totalRolls = 0;										//counter for rollDice function being called in function
	gameOver = displayBoard(numBoard, hConsole);
	while (gameOver == false)
	{
		cout << "-=Player" << player + 1 << "'s turn=-" << endl;
		rollDice(die1, die2, doubles);
		totalRolls++;									
		if (numBoard[die1 - 1] > 0)						//checks for open or shut box
		{
			numBoard[die1 - 1] = 0;
		}
		else
		{
			numBoard[die1 - 1] = (die1);
		}
		if ((die2 > 0) && (numBoard[die2 - 1] > 0))		//check for open or shut boxes
		{
			numBoard[die2 - 1] = 0;
		}
		else
		{
			numBoard[die2 - 1] = (die2);
		}
		gameOver = displayBoard(numBoard, hConsole);
		if (doubles == true)
		{
			player = player;							//player doesn't change, current player takes and extra turn
		}
		else
		{
			player = (player + 1) % 2;					//switching players from 0 to 1 to 0
		}
	}
	cout << "Total number of dice rolled: " << totalRolls * 2 << endl;
	return (player + 1) % 2;							//forces a player switch again for return for actual player winner
}

//unfinished AI player, still needs to have the algorithm injected
int playAI()
{
	cout << "<<<Player vs AI>>>" << endl;
	return 2;
}

/*
	Issues: Ran into the error "unresolved external conflict..." turns out I forgot to update the prototype parameters to match the function definition.

	No other errors were found.
*/