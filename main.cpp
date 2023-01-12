#include <iostream>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <time.h>
#include <cmath>
#include <vector>
#include <chrono>
#include <thread>
#include "includes/conio.h"

class node
{
	public:
		node* parent;
		int self;
		int hscore;
		int fscore;
		int gscore;

		node(node *p, int s) : parent(p), self(s) {}
		node(int s) : self(s) {}
	        
}; 

void drawBoard(int board[], int size)
{
	//std::cout << "|--------------|\n|";
	for (int i = 0; i < size; i++)
	{
		switch (board[i])
		{
			case 0:
				std::cout <<  " .";
				break;
			case 1:
				std::cout << " ●";
				break;
			case 2:
				std::cout << " ▧";
				break;
			case 3:
				std::cout << " o";
				break;
		}

		if((i + 1) % 7 == 0)
		{
			std::cout << "\n";
		}
	}
	//std::cout << "--------------|";
}

void updatePos(int dir, std::vector<int>& snake)
{ 
	std::vector<int> temp = snake;
	for(int i = 0; i < snake.size()-1; i++)
	{
		snake[i+1] = temp[i];
	}
	snake[0] += dir;
}

void updateBoard(std::vector<int>& snake, int board[])
{
	for(int i = 0; i < snake.size(); i++)
	{
		if (i == 0)
		{
			board[snake[i]] = 3;
		}
		
		else
		{
			board[snake[i]] = 1;
		}
	}
}

void updateSnake(std::vector<int>& snake)
{
	int last = snake.size();
	snake.push_back(snake[last] - snake[last-1] - snake[last]);
}

bool inSnake(std::vector<int> & snake, int food)
{
	for (int i : snake)
	{
		if (i == food)
		{
			return (true);
		}
	}

	return (false);
}
int updateFood(std::vector<int>& snake, int food)
{
	int temp = food;
	if(snake[0] == food)
	{
		while (inSnake(snake, temp))
		{
			temp = std::rand()  % 49;
		}
		updateSnake(snake);
	}

	return (temp);
}
int moves(int c)
{
        if (c == 0)
        {
                return(-7);
        }

        if (c == 1)
        {
                return(-1);
        }


        if (c == 2)
        {
                return(7);
        }


        if (c == 3)
        {
                return(1);
        }
}

int returnMove(int c)
{
	if (c == 119)
	{
		return(-7);
	}

        if (c == 97)
        {
                return(-1);
        }


        if (c == 115)
        {
                return(7);
        }


        if (c == 100) {
                return(1);
        }
}

int heuristic(int a, int b)
{
	int horiz = 10 * ((b + 1) % 7 - (a + 1) % 7);
	int vert = 10 * ((b / 7) - (a / 7));

	return(std::sqrt(horiz * horiz + vert * vert));
}

std::vector<node> getNeighbors(int* board, int curr, node* parent)
{
	std::vector<node> nodes;
	if(curr > 6 && board[curr - 7] != 1)
	{
		nodes.push_back(node(parent, board[curr - 7]));
	}

	if(curr < 42 && board[curr+7] != 1)
	{
		nodes.push_back(node(parent, board[curr + 7]));
	}

	if(curr % 7 != 0 && board[curr - 1] != 1)
	{
		nodes.push_back(node(parent, board[curr-1]));
	}

	if((curr + 1) % 7 != 0 && board[curr + 1] != 1)
	{
		nodes.push_back(node(parent, board[curr + 1]));
	}

	return(nodes);
}

int getBest(std::vector<node> nodes)
{
	int lowest = 10000;
	int bst;
	for (int i = 0; i < nodes.size(); i++)
	{
		if(nodes[i].fscore < lowest)
		{
			bst = i;
		}
	}

	return (bst);
}

int checkMultiplicity(node curr, std::vector<node> nodes)
{
	int same;
	for (int i = 0; i < nodes.size(); i ++)
	{
		if (nodes[i].self == curr.self)
		{
			if (nodes[i].fscore <= curr.fscore)
			{
				return(0);
			}

			same = i;
		}
	}
	
	return (same);
}

node search(int h, int f, int* board)
{
	node start(h);
	node end(f);
	start.parent = &start;
	end.parent = &end;
	std::vector<node> open;
	std::vector<node> closed;
	open.push_back(start);

	//iterate thru all child nodes
	while (open.size() != 0)
	{
		int currIndex = getBest(open);
		node curr = open[currIndex];
		closed.push_back(curr);
		open.erase(open.begin() + currIndex);

		if(curr.self == end.self)
		{
			return(curr);
		}

		else
		{
			std::vector<node> aval = getNeighbors(board, curr.self,&curr);
			
			for (node i : aval)
			{
				i.hscore = heuristic(i.self, f);
				i.gscore = curr.gscore + heuristic(i.self, curr.self);
				i.fscore = i.hscore + i.gscore;
				int inOpen = checkMultiplicity(i, open);
				int inClosed = checkMultiplicity(i, closed);

				if(!inOpen || !inClosed)
				{
					break;
				}
				open.erase(open.begin() + inOpen);
				closed.erase(open.begin() + inClosed);
				open.push_back(i);
			}
		}
	}

	return(end);
}

int getSolution(int h, int f, int* board)
{

	std::cout << "hi!";
	node sol = search(h, f, board);
	node prev = sol;
	while (true)
	{
		sol = *sol.parent;
		if (sol.self == h)
		{
			return(prev.self - sol.self);
		}
	}
}


int input;

void getInput()
{
	while (true)
	{
		int mv = getch();
		input = returnMove(mv);
	}
}

bool isDead(std::vector<int>& snake)
{
	if(snake[0] < 0)
	{
		return (true);
	}

	if(snake[0] > 48)
	{
		return(true);
	}

	int end = snake.size() - 1;
	
	for (int i = 1; i < snake.size(); i ++)
	{
		if (snake[i] == snake[0])
		{
			return (true);
		}
	}

	if ( (snake[0] / 7 != snake[end] / 7) && ( (snake[0] - 1 == snake[end]) || snake[0] + 1 == snake[end]))
	{
		return(true);
	}

	return(false);
}

int main()
{
	int board[49] = {0};
	int size = sizeof(board) / 4;
	std::vector<int> snake = {21};
	int dir = 1;
	int food = 21;
	int seed;
	char mv;
	int timer;
	std::cin >> seed;
	srand(seed);

	std::thread inpp(getInput);

	while (true)
	{
		memset(board, 0, sizeof(board));
		system("clear");
		food = updateFood(snake, food);
		board[food] = 2;
		// if(timer % 2 == 0)
		// {
		// 	updatePos(dir, snake);
		// }
		updatePos(dir, snake);
		updateBoard(snake, board);
		drawBoard(board, size);
		if(isDead(snake))
		{
			break;
		}
	
		// std::cout << input;
		// dir = getSolution(snake[0],food,board);
		// int mv = getch();
		// if(mv)
		// {
		// 	dir = returnMove(mv);
		// }
		dir = input;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		timer++;
	}

	std::cout << "\n\n\n\nyou died! \nyour score was " << snake.size() << "! do better...\n \n";
}

