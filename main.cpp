// BARTOSZ BICZ
// ADRIAN WINIARSKI
// 4 Weze w 4 wątkach po zjedzeniu "+" zwiększają swoją długosc
//
#include "stdafx.h"
#include "include/curses.h"
#include <time.h>
#include <thread>
#include <windows.h>
#include <iostream>
#include <mutex>

using namespace std;

int max_ogon = 2;
mutex mx;
class Waz
{
		static int a;
public: bool gameOver = false;
		int pierwszyLos = true;
		int ogon_X[10],ogon_Y[10];
		int licznosc_ogon = 1;
		int b=1;//zmienna przechowujaca ostatni ruch weza
		int amu_X, amu_Y;
		int minW, maxW, minS, maxS; //min i max wysokosc/szerokosc, narozniki pola ruchu weza
		int x;// minS + 19;// maxS - (minS / 2);
		int y;//minW + 6;//miejsce startu wonsza

		enum eDirecton { LEFT, RIGHT, UP, DOWN };
		eDirecton dir;

		Waz(int a,int b,int c,int d) 
		{ 
			minW = a;
			maxW = b;
			minS = c;
			maxS = d;
			amu_X = (rand() %38) + c;
			amu_Y = (rand() %12) + a;//polozenie amu
			x = c + 19;
			y = a + 6;
			//a = 1;
		}
		

		void snake()
		{
			

			for (int i = minW; i < maxW; i++)
			{
				for (int j = minS; j < maxS; j++)
				{

					if (i == y && j == x) 
					{
						move(i,j);
						addch('@');
						//cout << "@";//nasze czółko
					}
					else if (i == amu_Y && j == amu_X)
					{
						move(i, j);
						addch('+');
					}
					else
					{
						move(i, j);
						addch(' ');
						bool print = false;
						for (int k = 0; k < licznosc_ogon; k++)
						{
							if (ogon_X[k] == j && ogon_Y[k] == i)
							{
								move(i, j);
								addch('o');
								//cout << "o";
								print = true;
							}
						}
						//if (!print) {
							//move(i, j);
							//addch(' ');
							//cout << " ";
							//}
					}
				}
				
			}
			


		}
		void losuj_krok()
		{
			
			mx.lock();
			srand(a);
			a = rand();
			int b = rand() % 4;
			mx.unlock();
			
				if (b == 0) dir = LEFT;
				else if (b == 1) dir = RIGHT;
				else if (b == 2) dir = UP;
				else if (b == 3) dir = DOWN;
				
			
		}
		void silnik_gry()
		{
			//
			int prevX = ogon_X[0];
			int prevY = ogon_Y[0];
			int prev2X, prev2Y;
			ogon_X[0] = x;
			ogon_Y[0] = y;

			for (int i = 1; i < licznosc_ogon; i++)
			{
				prev2X = ogon_X[i];
				prev2Y = ogon_Y[i];
				ogon_X[i] = prevX;
				ogon_Y[i] = prevY;
				prevX = prev2X;
				prevY = prev2Y;
			}
			switch (dir)
			{
			case LEFT:
				x--;
				break;
			case RIGHT:
				x++;
				break;
			case UP:
				y--;
				break;
			case DOWN:
				y++;
				break;
			default:
				break;
			}

			if (x >= maxS) x = minS; else if (x < minS) x = maxS - 1;
			if (y >= maxW) y = minW; else if (y < minW) y = maxW - 1;
			if (x == amu_X && y == amu_Y)
			{
				licznosc_ogon++;
				amu_X = rand() % 38 + minS;
				amu_Y = rand() % 12 + minW;
			}
			if (licznosc_ogon > max_ogon)gameOver = true;
		}

		int graj()
		{
			snake();
			losuj_krok();
			silnik_gry();
			Sleep(50);

			return 0;
		}
};
int Waz::a = 2;

int main(void)
{
	initscr();
	int szer;
	int wys;
	curs_set(0);
	srand(time(NULL));
	noecho();
	getmaxyx(stdscr, wys, szer);
	
	
	Waz waz1(0, wys/2, 0, szer/2);
	Waz waz2(0, wys / 2, szer / 2 + 1, szer-1);
	Waz waz3(wys / 2+1, wys, 0, szer / 2);
	Waz waz4(wys / 2 + 1, wys, szer / 2 + 1, szer-1);
	
	for (int i = 0; i<szer/3-1; i++) {
		move(i, szer/2);
		addch('#');
	}

	for (int i = 0; i<szer - 1; i++) {
	move(wys / 2, i);
	addch('#');
	}
	refresh();
	
	
	while (waz1.gameOver==false && waz2.gameOver==false && waz3.gameOver==false && waz4.gameOver==false)
	{
		
		//waz1.graj();
		//waz2.graj();
		//waz3.graj();
		//waz4.graj();


		thread watek1(&Waz::graj, &waz1);
		thread watek2(&Waz::graj, &waz2);
		thread watek3(&Waz::graj, &waz3);
		thread watek4(&Waz::graj, &waz4);
		
		wrefresh(stdscr);
		watek1.join();
		watek2.join();
		watek3.join();
		watek4.join();

	}
	
	
	int x;

	cin >> x;


	return 0;
}



