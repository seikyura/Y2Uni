//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name	: "CTicTacToe.h"
// Description	: Implementation file for CTicTacToe
// Author		: Vivian Ngo
// Mail			: vivian.ngo7572@mediadesign.school.nz
//

#include "TicTacToe.h"
#include <conio.h>
#include <iostream>


using namespace std;

char board[3][3] =										//Initialise new board
{
	{ ' ', ' ', ' ' },
	{ ' ', ' ', ' ' },
	{ ' ', ' ', ' ' }
};

bool winnerBoard[3][3] =
{
	{ false, false, false },
	{ false, false, false },
	{ false, false, false }
};


CTicTacToe::CTicTacToe()
{
	p1Piece = 'X';
	p2Piece = 'O';
	p1Points = 0;
	p2Points = 0;
	endGame = false;
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL)
	{				 //  position	 size
		MoveWindow(hwnd, 342, 50, 700, 400, TRUE);  //Set console position to center
	}
}

CTicTacToe::~CTicTacToe()
{

}

/***********************
* StartUp: Starts up the game by introducing the player to a main menu screen
* This main menu screen will allow the player to choose to either
*	- Play a one player game against a computer AI
*		- This has an easy and hard setting
*	- Play a two player game against another human next to them
*	- Customize their board pieces
************************/
void CTicTacToe::StartUp()
{
	while (!GetHasGameEnded())
	{
		ctrl.ClearScreen(0, 0);
		cout << " =================================================" << endl;
		cout << " ||                  Tic Tac Toe                ||" << endl;
		cout << " =================================================" << endl;
		cout << " ||              Player 1 piece: " << p1Piece << "              ||" << endl;
		cout << " ||              Player 2 piece: " << p2Piece << "              ||" << endl;
		cout << " =================================================" << endl << endl;		cout << "    1) Player vs Computer" << endl;
		cout << "    2) Player vs Player" << endl;
		cout << "    3) Customize" << endl;
		cout << "    4) Exit" << endl << endl; 
		cout << " Please select an option: ";
		
		//Select an option from the title screen
		switch (ChooseOption(" Pick a valid option (1, 2, 3, 4): ", 4)) 
		{
			case '1':	//Player vs Computer
			{
				ctrl.ClearScreen(0, 0);
				ctrl.SetColour(6);
				cout << " =================================================" << endl;
				cout << "             Player vs Computer Selected." << endl;
				cout << " =================================================" << endl << endl;
				ctrl.SetColour(7);

				// Prompt player if they would like to play easy or hard mode
				cout << " Would you like to play Easy or Hard Mode? " << endl << endl;
				cout << "    1) Easy" << endl;
				cout << "    2) Hard" << endl << endl;
				cout << " Please select an option: ";

				char easyOrHard = ChooseOption(" Pick a valid option (1, 2): ", 2);
				
				// Inform player of their difficulty choice
				ctrl.SetColour(6);
				if (easyOrHard == '1')
				{
					cout << " =================================================" << endl;
					cout << "               Easy mode selected." << endl;
					cout << " =================================================" << endl << endl;
				}
				else
				{
					cout << " =================================================" << endl;
					cout << "               Hard mode selected." << endl;
					cout << " =================================================" << endl << endl;
				}
				ctrl.SetColour(7);

				//Prompt user whether they would like to be player 1 or player 2
				cout << " Would you like to be player 1 (" << p1Piece << ") or player 2 (" << p2Piece << ")?: ";
				char p1Or2 = ChooseOption(" Pick a valid option (1, 2): ", 2);
				
				// Inform player of their turn choice
				ctrl.SetColour(6);
				if (p1Or2 == '1')
				{
					ctrl.ClearScreen(0, 0);
					cout << " =================================================" << endl;
					cout << "       You will play as player 1 with the '" << p1Piece << "' piece." << endl;
					cout << " =================================================" << endl << endl;
				}
				else
				{
					ctrl.ClearScreen(0, 0);
					cout << " =================================================" << endl;
					cout << "       You will play as player 2 with the '" << p2Piece << "' piece." <<  endl;
					cout << " =================================================" << endl << endl;
				}
				ctrl.SetColour(7);

				if (easyOrHard == '1')
				{
					PlayGame(p1Or2, true, false);
					//PvCEasy(p1Or2);	//If player picks option one, start easy mode
				}
				else
				{
					PlayGame(p1Or2, false, false);
					//PvCHard(p1Or2); //If player picks option two, start hard mode
				}
				break;
			}
			case '2':				//Player vs Player
			{
				ctrl.ClearScreen(0, 0);
				ctrl.SetColour(6);
				cout << " =================================================" << endl;
				cout << "           Player vs Player Selected." << endl;
				cout << " =================================================" << endl << endl;
				ctrl.SetColour(7);

				PlayGame('1', false, true);	//Start up Player vs Player game
				break;
			}
			case '3':
			{
				ctrl.ClearScreen(0, 0);
				Customize();		//Customize player pieces
				break;
			}
			case '4':
			{
				SetGameEnd();		//Set the endGame value to true
			}
		}
		if (GetHasGameEnded())		//If game has ended, break while loop and close application
		{
			break;
		}
	}
}

void CTicTacToe::PlayGame(char p1Or2, bool easyMode, bool pvp)
{
	p1Points = 0;
	p2Points = 0;
	while (true) //While loop used to reset values if players want to play again
	{
		bool isPlayerTurn = ((p1Or2 == '1') ? true : false);	//Set true if player chose to go first
		char currentPlayer = p1Piece;							//Set the current player's piece to p1Piece 

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				board[i][j] = ' ';
			}
		}

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				winnerBoard[i][j] = false;
			}
		}

		while (true)			//While loop for a single game session
		{
			ctrl.ClearScreen(0, 0);
			PrintBoard();	//Print current board status

			row = 0;			//row to be picked
			col = 0;			//column to be picked

			if (pvp || isPlayerTurn)	//If players turn, allow player to set their position
			{
				ctrl.SetColour(6);
				cout << " =================================================" << endl;
				cout << "                 Player: " << currentPlayer << "'s turn" << endl;
				cout << " =================================================" << endl << endl;
				ctrl.SetColour(7);

				while (true)
				{
					//Allow player to pick a row and column
					cout << endl << "     Pick a row: ";
					row = ChooseOption(" Pick a valid row (1, 2, 3): ", 3);

					cout << "     Pick a column: ";
					col = ChooseOption("     Pick a valid column (1, 2, 3): ", 3);

					//If position is valid, end loop
					if (board[row - 49][col - 49] == ' ')
					{
						break;
					}
					//Otherwise, repeat while loop
					cout << "     Please pick a valid position: " << endl;
				}
			}	
			else	//If players turn, allow player to set their position
			{
				ctrl.SetColour(6);
				cout << " =================================================" << endl;
				cout << "                Computer: " << currentPlayer << "'s turn" << endl;
				cout << " =================================================" << endl << endl;
				ctrl.SetColour(7);

				//Insert minimax here================================++++++++++++++++++++++++++++++++++++++++++++++++++++++
				if (easyMode)
				{
					while (true)
					{
						//Easy random computer position generator
						int compRow = (rand() % 3 + 1);
						row = '0' + compRow;

						int compCol = (rand() % 3 + 1);
						col = '0' + compCol;

						if (board[row - 49][col - 49] == ' ') //If the position is valid, proceed with sleep
						{
							cout << "   Computer " << currentPlayer << " is making their move: ";
							Sleep(500);
							cout << ". ";
							Sleep(500);
							cout << ". ";
							Sleep(500);
							cout << ". ";
							Sleep(500);

							while (_kbhit()) //Prevents user input while program is sleeping
								_getch();

							cout << endl << endl;

							break;
						}
					}
				}
				else
				{
					//Minimax
				}
				
			}
			//Set player piece on board
			board[row - 49][col - 49] = currentPlayer;

			//Check if there is a winner. If there is, break while loop and end round
			if (CheckWinner(currentPlayer, isPlayerTurn, pvp, p1Or2))
			{
				break;
			}

			//Switch players after a turn is up
			if (currentPlayer == p1Piece)
			{
				currentPlayer = p2Piece;
			}
			else
			{
				currentPlayer = p1Piece;
			}
			isPlayerTurn = !isPlayerTurn;
		}
		//Ask players if they want to play another round
		cout << " Play again?" << endl << endl;
		cout << "    1) Yes" << endl;
		cout << "    2) No" << endl << endl;
		cout << " Please select an option: ";

		//Allows player to choose whether to play again or not
		if (ChooseOption("Pick a valid option (1, 2): ", 2) == '1')
		{
			ctrl.ClearScreen(0, 0);
			ctrl.SetColour(6);
			cout << " =================================================" << endl;
			cout << "                 STARTING NEW GAME " << endl;
			cout << " =================================================" << endl << endl;
			ctrl.SetColour(7);
		}
		else
		{
			//Return to main menu by breaking the while loop
			break;
		}
	}
}

/***********************
* Customize: Allows player to customize the TicTacTie game pieces
* Player can change either Player1's piece, Player2's piece or swap them with each other
************************/
void CTicTacToe::Customize()
{
	bool custExit = false;
	while (true)
	{
		cout << " =================================================" << endl;
		cout << " ||                 CUSTOMIZATION               ||" << endl;
		cout << " =================================================" << endl;
		cout << " ||              Player 1 piece: " << p1Piece << "              ||" << endl;
		cout << " ||              Player 2 piece: " << p2Piece << "              ||" << endl;
		cout << " =================================================" << endl << endl;
		cout << " Customization options:" << endl;
		cout << "    1) Change Player 1 piece" << endl;
		cout << "    2) Change Player 2 piece" << endl;
		cout << "    3) Switch pieces" << endl;
		cout << "    4) Return to main menu" << endl << endl;
		cout << " Please select an option: ";

		switch (ChooseOption(" Pick a valid option (1, 2, 3, 4): ", 4))
		{
		case '1': //Allow player to change p1 piece
		{
			cout << " Choose a new Player 1 piece: ";
			p1Piece = ChangePiece(p1Piece);
			ctrl.ClearScreen(0, 0);
			ctrl.SetColour(6);
			cout << " =================================================" << endl;
			cout << "               Player 1 piece changed." << endl;
			cout << " =================================================" << endl;
			ctrl.SetColour(7);
			break;
		}
		case '2': //Allow player to change p2 piece
		{
			cout << " Choose a new Player 2 piece: ";
			p2Piece = ChangePiece(p2Piece);
			ctrl.ClearScreen(0, 0);
			ctrl.SetColour(6);
			cout << " =================================================" << endl;
			cout << "               Player 2 piece changed." << endl;
			cout << " =================================================" << endl;
			ctrl.SetColour(7);
			break;
		}
		case '3': //Allow player to swap p1 and p2 pieces
		{
			ctrl.ClearScreen(0, 0);
			ctrl.SetColour(6);
			cout << " =================================================" << endl;
			cout << "               P1 & P2 pieces Switched." << endl;
			cout << " =================================================" << endl;
			ctrl.SetColour(7);

			char tempPiece = p1Piece;
			p1Piece = p2Piece;
			p2Piece = tempPiece;

			break;
		}
		case '4':
		{
			//Return to main menu
			custExit = true;
			break;
		}

		}
		if (custExit) //Exit customization menu if custExit is true
		{
			break;
		}
	}
}

/***********************
* ChooseOption: Allows player to choose an option and prompts a question if a wrong input is entered
* @parameter: question - string containing the question to ask player if a wrong input is entered
* @parameter: numOptions - the number of options to select from
************************/
char CTicTacToe::ChooseOption(std::string question, int numOptions)
{
	char option = _getch();
	while (option < 49 || option >(49 + numOptions - 1))
	{
		cout << endl;
		cout << question;
		option = _getch();
	}
	cout << option << endl << endl;
	return option;
}

/***********************
* ChooseCharacter: Takes the user's input and checks if it is a valid board game piece
* @return: newPiece - The valid new piece the user inputed
************************/
char CTicTacToe::ChangePiece(char checkWithPlayer)
{
	char newPiece = 'n';
	while (true)
	{
		newPiece = _getch();
		cout << newPiece << endl;
		//If the new piece is not equals to player two's piece or a space character, return.
		if (newPiece != checkWithPlayer && newPiece != ' ')
		{
			break;
		}
		//prompt statement if input is invalid
		else
		{
			cout << " Please choose a different character: ";
		}
	}
	return newPiece;
}


/***********************
* PrintBoard: Prints the CTicTacToe board
* @parameter: board[3][3] - the tic tac toe board and it's current values
* @parameter: winnerBoard[3][3] - The boolean board that determines which pieces to 
*								  print in colour when the game has been won
************************/
void CTicTacToe::PrintBoard()
{
	cout << endl;
	ctrl.Spc(' ', 5); cout << "                 COLUMN" << endl;
	ctrl.Spc(' ', 5); cout << "       " << endl;
	ctrl.Spc(' ', 5); cout << "            1       2       3" << endl;
	ctrl.Spc(' ', 5); cout << "        ________________________" << endl;
	ctrl.Spc(' ', 5); cout << "       |        |       |       |" << endl;
	ctrl.Spc(' ', 5); cout << "    1  |    "; PrintWinningPiece(0, 0); 
						   cout << "   |   "; PrintWinningPiece(0, 1);
						   cout << "   |   "; PrintWinningPiece(0, 2); cout << "   |" << endl;
	ctrl.Spc(' ', 5); cout << "       |        |       |       |" << endl;
	ctrl.Spc(' ', 5); cout << "       |------------------------|" << endl;
	ctrl.Spc(' ', 5); cout << " R     |        |       |       |" << endl;
	ctrl.Spc(' ', 5); cout << " O  2  |    "; PrintWinningPiece(1, 0);
						   cout << "   |   "; PrintWinningPiece(1, 1);
						   cout << "   |   "; PrintWinningPiece(1, 2); cout << "   |" << endl;
	ctrl.Spc(' ', 5); cout << " W     |        |       |       |" << endl;
	ctrl.Spc(' ', 5); cout << "       |------------------------|" << endl;
	ctrl.Spc(' ', 5); cout << "       |        |       |       |" << endl;
	ctrl.Spc(' ', 5); cout << "    3  |    "; PrintWinningPiece(2, 0);
						   cout << "   |   "; PrintWinningPiece(2, 1);
						   cout << "   |   "; PrintWinningPiece(2, 2); cout << "   |" << endl;
	ctrl.Spc(' ', 5); cout << "       |________|_______|_______|" << endl << endl;
}

void CTicTacToe::PrintWinningPiece(int x, int y)
{
	if (winnerBoard[x][y])
	{
		ctrl.PrintInColour(board[x][y]);
	}
	else
	{
		cout << board[x][y];
	}
}

bool CTicTacToe::CheckWinner(char player, bool isPlayerTurn, bool isPvP, char p1Or2)
{
	bool winner = false;


	if (board[0][0] != ' ' && board[0][1] != ' ' && board[0][2] != ' ' &&
		board[1][0] != ' ' && board[1][1] != ' ' && board[1][2] != ' ' &&
		board[2][0] != ' ' && board[2][1] != ' ' && board[2][2] != ' ')
	{
		cout << endl;
		ctrl.ClearScreen(0, 0);
		PrintBoard();
		winner = true;

		ctrl.SetColour(6);
		cout << " =================================================" << endl;
		cout << "                      DRAW!!" << endl;
		PrintScores(p1Or2, isPvP, isPlayerTurn, true);
		ctrl.SetColour(7);
	}
	else //Check for a 3 in a row and set winnerBoard values to true to change the colour of the winning pieces
	{
		if (board[0][0] == board[0][1] && board[0][1] == board[0][2] && board[0][0] == board[0][2] && board[0][0] == player)
		{
			winnerBoard[0][0] = winnerBoard[0][1] = winnerBoard[0][2] = winner = true;
		}
		else if (board[1][0] == board[1][1] && board[1][1] == board[1][2] && board[1][0] == board[1][1] && board[1][0] == player)
		{
			winnerBoard[1][0] = winnerBoard[1][1] = winnerBoard[1][2] = winner = true;
		}
		else if (board[2][0] == board[2][1] && board[2][1] == board[2][2] && board[2][0] == board[2][2] && board[2][0] == player)
		{
			winnerBoard[2][0] = winnerBoard[2][1] = winnerBoard[2][2] = winner = true;
		}
		else if (board[0][0] == board[1][0] && board[1][0] == board[2][0] && board[1][0] == board[2][0] && board[0][0] == player)
		{
			winnerBoard[0][0] = winnerBoard[1][0] = winnerBoard[2][0] = winner = true;
		}
		else if (board[0][1] == board[1][1] && board[1][1] == board[2][1] && board[1][1] == board[2][1] && board[0][1] == player)
		{
			winnerBoard[0][1] = winnerBoard[1][1] = winnerBoard[2][1] = winner = true;
		}
		else if (board[0][2] == board[1][2] && board[1][2] == board[2][2] && board[1][2] == board[2][2] && board[0][2] == player)
		{
			winnerBoard[0][2] = winnerBoard[1][2] = winnerBoard[2][2] = winner = true;
		}
		else if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[1][1] == board[2][2] && board[0][0] == player)
		{
			winnerBoard[0][0] = winnerBoard[1][1] = winnerBoard[2][2] = winner = true;
		}
		else if (board[2][0] == board[1][1] && board[1][1] == board[0][2] && board[1][1] == board[0][2] && board[2][0] == player)
		{
			winnerBoard[2][0] = winnerBoard[1][1] = winnerBoard[0][2] = winner = true;
		}

		if (winner)
		{
			ctrl.ClearScreen(0, 0);
			PrintBoard();

			ctrl.SetColour(6);
			PrintScores(p1Or2, isPvP, isPlayerTurn, false);
			ctrl.SetColour(7);
		}
	}
	return winner;
}

/***********************
* PrintScores: Prints the scores of the current game session
* @parameter: p1Or2 - Checks if the player is player 1 or player 2 (PvC scores)
* @parameter: isPvP - Checks if the game is a PvP (Player vs Player) or PvC (Player vs Computer) game
************************/
void CTicTacToe::PrintScores(char p1Or2, bool isPvP, bool isPlayerTurn, bool isDraw)
{
	if (isPvP)
	{
		if (isDraw)
		{
			cout << "     Player 1 Points: " << p1Points 
				<< "      Player 2 Points: " << p2Points << endl;
			cout << " =================================================" << endl << endl;
		}
		else
		{
			if (isPlayerTurn)
				++p1Points;
			else
				++p2Points;

			cout << " =================================================" << endl;
			cout << "          PLAYER " << ((isPlayerTurn) ? '1' : '2') << ": " << ((isPlayerTurn) ? p1Piece : p2Piece)
										<< " HAS WON THE GAME!   " << endl;
			cout << "     Player 1 Points: " << p1Points 
				<< "      Player 2 Points: " << p2Points << endl;
			cout << " =================================================" << endl << endl;
		}
	}
	else //Player vs Computer game
	{
		if (isDraw)
		{
			cout << "     Player Points: " << ((p1Or2 == '1') ? p1Points : p2Points)
				<< "      Computer Points: " << ((p1Or2 == '1') ? p2Points : p1Points) << endl;
			cout << " =================================================" << endl << endl;
		}
		else
		{
			if (isPlayerTurn)
			{
				if (p1Or2 == '1')
					++p1Points;
				else
					++p2Points;

				cout << " =================================================" << endl;
				cout << "          PLAYER: " << ((p1Or2 == '1') ? p1Piece : p2Piece) 
											 << " HAS WON THE GAME!   " << endl;
				cout << "     Player Points: " << ((p1Or2 == '1') ? p1Points : p2Points)
					<< "      Computer Points: " << ((p1Or2 == '1') ? p2Points : p1Points) << endl;
				cout << " =================================================" << endl << endl;
			}
			else
			{
				if (p1Or2 != '1')
					++p1Points;
				else
					++p2Points;

				cout << " =================================================" << endl;
				cout << "          COMPUTER: " << ((p1Or2 != '1') ? p1Piece : p2Piece) 
											   << " HAS WON THE GAME!   " << endl;
				cout << "     Player Points: " << ((p1Or2 == '1') ? p1Points : p2Points) 
					<< "      Computer Points: " << ((p1Or2 == '1') ? p2Points : p1Points) << endl;
				cout << " =================================================" << endl << endl;
			}
		}
	}
}

//Getters and Setters--------------------------------------------------------------------------------

/***********************
* SetP1Piece: Set the char of p1's piece
* @parameter: p1 - char value that the player wants to change to
************************/
void CTicTacToe::SetP1Piece(char p1)
{
	p1Piece = p1;
}

/***********************
* SetP2Piece: Set the char of p2's piece
* @parameter: p1 - char value that the player wants to change to
************************/
void CTicTacToe::SetP2Piece(char p2)
{
	p2Piece = p2;
}

/***********************
* GetP1Piece: Get the char of p1's piece
************************/
char CTicTacToe::GetP1Piece()
{
	return p1Piece;
}

/***********************
* GetP1Piece: Get the char of p2's piece
************************/
char CTicTacToe::GetP2Piece()
{
	return p2Piece;
}

/***********************
* SetGameEnd: Set the endGame value to true
************************/
void CTicTacToe::SetGameEnd()
{
	endGame = true;
}

/***********************
* GetHasGameEnded: return the endGame value
************************/
bool CTicTacToe::GetHasGameEnded()
{
	return endGame;
}
