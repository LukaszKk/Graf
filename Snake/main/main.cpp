// main.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

void PrintGame( int width, int height, int fruitX, int fruitY );
int CheckFruit( int width, int height, int x, int y, int* fruitX, int* fruitY );
bool Move(  int width, int height, int* x, int* y, int* LastMove );
enum Move{ UP, DOWN, RIGHT, LEFT };
int score;
vector < int > TailX;
vector < int > TailY;
int length = 1;
void HideCursor();
void ShowCursor();

int main()
{
	const int width = 10;
	const int height = 10;
labAgain:
	score = 0;
	int x = width / 2;
	int y = height / 2;
	int LastMove = UP;
	int fruitX = rand() % width;
	int fruitY = rand() % height;
	char diff = 0;
	TailX.push_back( x );
	TailY.push_back( y );

	system( "cls" );
	/*do
	{
		cout << "Pick difficulty:\n1.Easy (Press 1)\n2.Normal (Press 2)\n3.Hard (Press 3)\n";
		diff = _getch() - '0';
	} while( diff != 1 && diff != 2 && diff != 3 );*/
	HideCursor();
	while( 1 )
	{
		PrintGame( width, height, fruitX, fruitY );
		if( !CheckFruit( width, height, x, y, &fruitX, &fruitY ) )
			if( Move( width, height, &x, &y, &LastMove ) )
					break;

		if( score >= 10 )
			Sleep( 10 );
		else if( score >= 6 )
			Sleep( 50 );
		else if( score >= 3 )
			Sleep( 100 );
		else
			Sleep( 150 );
		/*if( diff == 1)
			Sleep( 150 );
		else if( diff == 2 )
			Sleep( 75 );
		else if( diff == 3 )
			Sleep( 10 );*/
	}
	TailX.clear();
	TailY.clear();
	cout << "GAME OVER\nDo you want to play again?[Y/N]\n";
	ShowCursor();
	do
	{
		diff = _getch();
	} while( diff != 'y' && diff != 'n' );
	if( diff == 'y' )
		goto labAgain;

    return 0;
}

void PrintGame( int width, int height, int fruitX, int fruitY )
{
	system( "cls" );
	cout << "Gora: W   Dol: S   Prawo: D   Lewo: A   Zakoncz: X\n";
	for( int i = 0; i < width + 2; i++ )
		cout << "#";
	cout << "\n";
	for( int i = 0; i < height; i++ )
	{
		cout << "#";
		for( int j = 0; j < width; j++ )
		{
			int brik = 0;
			for( size_t k = 0; k < TailX.size(); k++ )
				if( i == TailY[k] && j == TailX[k] )
				{
					brik = 1;
					cout << "O";
					break;
				}
			if( !brik && i == fruitY && j == fruitX )
				cout << "F";
			else if( !brik )
				cout << " ";
		}
		cout << "#\n";
	}
	for( int i = 0; i < width + 2; i++ )
		cout << "#";
	cout << "\n";
	cout << "Score: " << score << endl;
}

bool Move(  int width, int height, int* x, int* y, int* LastMove )
{
	if( _kbhit() )
	{
		int temp = *LastMove;
		switch( _getch() )
		{
		case 'w' :
		{
			(*y)--; 
			*LastMove = UP;
			break;
		}
		case 's' :
		{
			(*y)++; 
			*LastMove = DOWN;
			break;
		}
		case 'd' : 
		{
			(*x)++; 
			*LastMove = RIGHT;
			break;
		}
		case 'a' : 
		{
			(*x)--; 
			*LastMove = LEFT;
			break;
		}
		case 'x' : return true;
		default: goto label;
		}
	}
	else
	{
label:	if( *LastMove == UP )
			(*y)--;
		else if( *LastMove == DOWN )
			(*y)++;
		else if( *LastMove == RIGHT )
			(*x)++;
		else if( *LastMove == LEFT )
			(*x)--;
	}
	for( int i = TailX.size() - 1; i > 0; i-- )
	{
		if( *x == TailX[i] && *y == TailY[i] )
		{
			cout << "You have eaten your tail!\n";
			return true;
		}
		TailX[i] = TailX[i - 1];
		TailY[i] = TailY[i - 1];
	}
	TailX[0] = *x;
	TailY[0] = *y;

	if( *x == width || *x == -1 || *y == height || *y == -1 )
		return true;
	


	return false;
}

int CheckFruit( int width, int height, int x, int y, int* fruitX, int* fruitY )
{
	if( x == *fruitX && y == *fruitY )
	{
		score++;
		*fruitX = rand() % width;
		*fruitY = rand() % height;
		TailX.push_back( TailX[TailX.size() - 1] );
		TailY.push_back( TailY[TailY.size() - 1] );
		for( int i = TailX.size() - 2; i > 0; i-- )
		{
			TailX[i] = TailX[i - 1];
			TailY[i] = TailY[i - 1];
		}
		TailX[0] = x;
		TailY[0] = y;

		return 1;
	}
	return 0;
}

void HideCursor()
{
	::HANDLE hConsoleOut =::GetStdHandle( STD_OUTPUT_HANDLE );
	::CONSOLE_CURSOR_INFO hCCI;
	::GetConsoleCursorInfo( hConsoleOut, & hCCI );
	hCCI.bVisible = FALSE;
	::SetConsoleCursorInfo( hConsoleOut, & hCCI );
}

void ShowCursor()
{
	::HANDLE hConsoleOut =::GetStdHandle( STD_OUTPUT_HANDLE );
	::CONSOLE_CURSOR_INFO hCCI;
	::GetConsoleCursorInfo( hConsoleOut, & hCCI );
	if( hCCI.bVisible != TRUE )
	{
		hCCI.bVisible = TRUE;
		::SetConsoleCursorInfo( hConsoleOut, & hCCI );
	}
}