#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "bengisuozdemir_ozdemir_bengisu_hw_header.h"
using namespace std;
//Bengisu Ozdemir 26683 HW1
struct path
{
	bool u,d,l,r;
	int x, y;
	bool isVisited;
	//default constructor
	path()
		:u(), d(), l(), r(), x(), y(), isVisited()
	{}
	//constructor
	path(int _x, int _y)
		:u(1), d(1), l(1), r(1), x(_x), y(_y), isVisited(false)
	{}
};
int randDirection(int possible)
{
	int randNum = rand()%(possible);
	return randNum;
}


template<class Object>
void writeMazeFiles(Stack<Stack<Object>> theMazes, int M, int N, int K)
{
	for (int j = 0; j < K; j++)
	{
		Stack<Object> theMaze = theMazes.topAndPop();
		Stack<Object> dummyMaze = theMaze;
		ofstream output;	

		string tempstr = "maze_"+to_string(K-j)+".txt"; //generating dynamic file name

		output.open(tempstr.c_str()); //open the file
		output<< M << " " << N << endl; //first write the M and N values to the file consequently
		
		//while there is a variable still exists in myStack, pop the values and overwrite them into the selected file
		for (int i = 0; i < M; i++)
		{
			for (int k = 0; k < N; k++)
			{
				while(!dummyMaze.isEmpty())
				{
					path tempPath = dummyMaze.topAndPop();
					if (tempPath.x == i && tempPath.y == k)
					{			
						output<<"x="<<tempPath.x<<" "<<"y="<<tempPath.y<<" ";
						output<<"r=" << tempPath.r << " l=" << tempPath.l <<" u=" << tempPath.u <<" d=" << tempPath.d << endl;
					}
				}
				dummyMaze = theMaze;
			}
		}
		output.close();
	}
}
template<class Object>
void writeFiles(Stack<Object> theMaze, int K, int M, int N, int mazeID, int entryX, int entryY, int exitX, int exitY)
{
	ofstream output_2;
	
	//for second file category first generate the name, open the file and overwrite the entry and the exit coordinates
	string secondstr = "maze_"+to_string(mazeID)+"_path_"+to_string(entryX)+"_"+to_string(entryY)+"_"+to_string(exitX)+"_"+to_string(exitY)+".txt";
	output_2.open(secondstr.c_str());

	//writing the path into the file
	while(!theMaze.isEmpty())
	{
		output_2 << theMaze.top().x << " " << theMaze.top().y << endl;
		theMaze.pop();
	}

	output_2.close(); //close the file again
	
}
template<class Object>
bool isPointAvailable(int x, int y, int row, int col, Stack<Object> theMaze)
{//function to check if a given coordinate is available
	if(x < 0 || x>= row || y<0 || y>= col) //if there is a single problem with this coordinate then immediately return false 
		return false;
	while(!theMaze.isEmpty()) // while the maze is not empty store the top element, check if its equal to controlled x and y
	{
		path placeholder = theMaze.topAndPop();
		if(placeholder.x == x && placeholder.y == y)
		{
			return false; //if yes return false
		}
	}
	return true; //if we reached to this line, it is safe to say this point is available
}
template<class Object>
bool isPassed(Stack<Object> theMaze, int x, int y, Stack<Object> deadEnd)
{	
	path tempPath = theMaze.topAndPop();
	while(!theMaze.isEmpty())
	{
		tempPath = theMaze.topAndPop();
		if(tempPath.x == x && tempPath.y == y)
		{
			return false;
		}
	}
	while(!deadEnd.isEmpty())
	{
		tempPath = deadEnd.topAndPop();
		if(tempPath.x == x && tempPath.y == y)
		{
			return false;
		}
	}
	return true;
}
template<class Object>
void nextCoordinates(Stack<Object> &availablePoints, Stack<Object> theMaze, Stack<Object> returning, int row, int col, bool check)
{//function to find next coordinates
	availablePoints.makeEmpty();
	
	path currentCell = theMaze.top();
	if (check)
	{
		while (!returning.isEmpty())
		{
			theMaze.push(returning.topAndPop());
		}
	}
	if(isPointAvailable(currentCell.x-1, currentCell.y, row, col, theMaze)) //means that left cell is available
	{
		availablePoints.push(path(currentCell.x-1, currentCell.y));
	}
	if(isPointAvailable(currentCell.x+1, currentCell.y, row, col, theMaze)) //means that right cell is available
	{
		availablePoints.push(path(currentCell.x+1, currentCell.y));
	}
	if(isPointAvailable(currentCell.x, currentCell.y-1, row, col, theMaze)) //means that down cell is available
	{
		availablePoints.push(path(currentCell.x, currentCell.y-1));
	}
	if(isPointAvailable(currentCell.x, currentCell.y+1, row, col, theMaze)) //means that up cell is available
	{
		availablePoints.push(path(currentCell.x, currentCell.y+1));
	}
}
template<class Object>
int getSize(Stack<Object> theMaze)
{//funciton to find given stack's size
	int size = 0;
	Stack<Object> tempStack = theMaze;
	while(!tempStack.isEmpty())
	{
		tempStack.pop();
		size++;
	}
	return size;
}
template<class Object>
void createMaze(int row, int col, Stack<Object> & theMaze)
{//funciton to create a maze
	//start with pushing 0,0 to the stack
	theMaze.push(path(0,0));
	int remainingVisit = row * col -1; //number of remaining visit 
	Stack<Object> availablePoints;
	Stack<Object> missedPoints;
	path cell;
	nextCoordinates(availablePoints, theMaze, missedPoints,row, col,false); //next coordinates are checked and generated
	int c_x, c_y; //current x and y coordinates, without initialization
	while(remainingVisit!=0)
	{
		if(!availablePoints.isEmpty())
		{
			int dir = randDirection(getSize(availablePoints));
			for (int i = 0; i <= dir; i++)
			{
				cell = availablePoints.topAndPop(); //popped random number many times and the top of remaining stack will be our chosen cell to move on 
			}
			c_x = cell.x;
			c_y = cell.y;
			//current x and y values are initialized
		
			if(theMaze.top().x < c_x) //meaning that we went to right
			{
				path dummyPath = theMaze.topAndPop();
				dummyPath.r = 0;
				dummyPath.isVisited = true;
				theMaze.push(dummyPath);
				cell.l=0;
				cell.isVisited=1;
				theMaze.push(cell);
			}
			else if(theMaze.top().x > c_x) //meaning that we went to left
			{
				
				path dummyPath = theMaze.topAndPop();
				dummyPath.l = 0;
				dummyPath.isVisited = true;
				theMaze.push(dummyPath);
				cell.r=0;
				cell.isVisited=1;
				theMaze.push(cell);
			}
			else if(theMaze.top().y < c_y) //meaning that we went to up
			{
				
				path dummyPath = theMaze.topAndPop();
				dummyPath.u = 0;
				dummyPath.isVisited = true;
				theMaze.push(dummyPath);
				cell.d=0;
				cell.isVisited=1;
				theMaze.push(cell);
			}
			else if(theMaze.top().y > c_y) //meaning that we went to down
			{
			
				path dummyPath = theMaze.topAndPop();
				dummyPath.d = 0;
				dummyPath.isVisited = true;
				theMaze.push(dummyPath);
				cell.u=0;
				cell.isVisited=1;
				theMaze.push(cell);
			}
		}
		else //no available point in the stack, we must track our path and go back
		{
			Stack<Object> tempMaze = theMaze;
			path tempPath;
			
			while(availablePoints.isEmpty()) //while there are no available point, track back and retry passed cells is there is an another wall to knock down
			{
				tempPath = theMaze.topAndPop();
				missedPoints.push(tempPath);
				nextCoordinates(availablePoints, theMaze,missedPoints, row, col,true);
				 //deadend
			}
			int n_x, n_y; //next coordinates,initalized below
			n_x = availablePoints.top().x;
			n_y = availablePoints.top().y;
			if(theMaze.top().x < n_x) //meaning that we went to right
			{
				path availableCell = availablePoints.topAndPop();
				availableCell.l=0;
				availableCell.isVisited = true;

				
				path mazeCell = theMaze.topAndPop();
				mazeCell.r = 0;
				mazeCell.isVisited = true;
				theMaze.push(mazeCell);
				while(!missedPoints.isEmpty())
				{
					theMaze.push(missedPoints.topAndPop());
				}
	
				theMaze.push(availableCell);
			}
			else if(theMaze.top().x > n_x) //meaning that we went to left
			{
				path availableCell = availablePoints.topAndPop();
				availableCell.r=0;
				availableCell.isVisited = true;

				
				path mazeCell = theMaze.topAndPop();
				mazeCell.l = 0;
				mazeCell.isVisited = true;
				theMaze.push(mazeCell);
				while(!missedPoints.isEmpty())
				{
					theMaze.push(missedPoints.topAndPop());
				}
	
				theMaze.push(availableCell);
				
			}
			else if(theMaze.top().y < n_y) //meaning that we went to up
			{
				path availableCell = availablePoints.topAndPop();
				availableCell.d=0;
				availableCell.isVisited = true;

				
				path mazeCell = theMaze.topAndPop();
				mazeCell.u = 0;
				mazeCell.isVisited = true;
				theMaze.push(mazeCell);
				while(!missedPoints.isEmpty())
				{
					theMaze.push(missedPoints.topAndPop());
				}
	
				theMaze.push(availableCell);
			}
			else if(theMaze.top().y > n_y) //meaning that we went to down
			{
				path availableCell = availablePoints.topAndPop();
				availableCell.u=0;
				availableCell.isVisited = true;

				
				path mazeCell = theMaze.topAndPop();
				mazeCell.d = 0;
				mazeCell.isVisited = true;
				theMaze.push(mazeCell);
				while(!missedPoints.isEmpty())
				{
					theMaze.push(missedPoints.topAndPop());
				}
	
				theMaze.push(availableCell);
			}
		}
		//no unvisited cell remain
		
		nextCoordinates(availablePoints, theMaze, missedPoints, row, col, false); //generate next coordinates
		remainingVisit--; //decrease remaining visit
	}
}
template<class Object>
void findPath(Stack<Stack<Object>> mazeStack, int &_mazeID, int &entryX, int &entryY, int &exitX, int &exitY, int K, int M, int N)
{
	int entry_X, entry_Y;
	Stack<Object> tempMaze, printMaze, deadEnd;
	bool check = false;
	cout << "Enter a maze ID between 1 to "<<K<<" inclusive to find a path: ";
	cin>>_mazeID;
	while(!(_mazeID>=1 && _mazeID<=K))
	{
		cout << "Invalid maze ID" << endl;
		return;
	}
	Stack<Object> choosenMaze; //as�l maze
	for (int i = 0; i < K-_mazeID; i++)
	{
		mazeStack.pop();
	}
	choosenMaze = mazeStack.top();
	int a = getSize(choosenMaze);
	Stack<Object> theMaze;
	//taking entry and exit values
	cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
	cin>>entry_X >> entry_Y;
	entryX = entry_X;
	entryY = entry_Y;
	cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
	cin>>exitX>>exitY;
	Stack<Object> copyChoosenMaze = choosenMaze;
	path exitCell;
	while(!copyChoosenMaze.isEmpty()) //the top of the stack is equal to given exit point
	{
		exitCell = copyChoosenMaze.topAndPop();
		if (exitCell.x == exitX && exitCell.y == exitY)
		{
			theMaze.push(exitCell);
		}
	}
	copyChoosenMaze = choosenMaze;
	
	while(!check)
	{
		if(theMaze.top().r == 0  && isPassed(theMaze, theMaze.top().x +1, theMaze.top().y, deadEnd))
		{
			bool shouldContinue = true;
			while (!copyChoosenMaze.isEmpty() && shouldContinue && theMaze.top().x+1 < M)
			{
				path tempPath;
				tempPath = copyChoosenMaze.topAndPop();
				if (tempPath.x == theMaze.top().x+1 && tempPath.y == theMaze.top().y)
				{	
					tempPath.isVisited = true; //changed its isVisited
					theMaze.push(tempPath); //pushed to main stack
					shouldContinue = false;
				}
			}	
		}
		else if(theMaze.top().l == 0  && isPassed(theMaze, theMaze.top().x -1, theMaze.top().y, deadEnd))
		{
			bool shouldContinue = true;
			while (!copyChoosenMaze.isEmpty() && shouldContinue )
			{
				path tempPath;
				tempPath = copyChoosenMaze.topAndPop();
				if (tempPath.x == theMaze.top().x-1 && tempPath.y == theMaze.top().y)
				{	
					tempPath.isVisited = true; //changed its isVisited
					theMaze.push(tempPath); //pushed to main stack
					shouldContinue = false;
					if (theMaze.top().x-1 < 0)
						entryX++;
				}
			}	
		}
		else if(theMaze.top().u == 0 && isPassed(theMaze, theMaze.top().x , theMaze.top().y+1, deadEnd))
		{
			bool shouldContinue = true;
			while (!copyChoosenMaze.isEmpty() && shouldContinue && theMaze.top().y+1 < N)
			{
				path tempPath;
				tempPath = copyChoosenMaze.topAndPop();
				if (tempPath.x == theMaze.top().x && tempPath.y == theMaze.top().y+1)
				{	
					tempPath.isVisited = true; //changed its isVisited
					theMaze.push(tempPath); //pushed to main stack
					shouldContinue = false;
				}
			}	
		}
		else if(theMaze.top().d == 0 && isPassed(theMaze, theMaze.top().x, theMaze.top().y-1, deadEnd))
		{
			bool shouldContinue = true;
			while (!copyChoosenMaze.isEmpty() && shouldContinue && theMaze.top().y-1 >= 0)
			{
				path tempPath;
				tempPath = copyChoosenMaze.topAndPop();
				if (tempPath.x == theMaze.top().x && tempPath.y == theMaze.top().y-1)
				{	
					tempPath.isVisited = true; //changed its isVisited
					theMaze.push(tempPath); //pushed to main stack
					shouldContinue = false;
				}
			}	
		}
		else
		{
			deadEnd.push(theMaze.topAndPop());
		}
		copyChoosenMaze = choosenMaze;
		if (theMaze.top().x == entry_X && theMaze.top().y == entry_Y)
		{
			check = true;
			cout << "bitti"<<endl;
		}
	}
	
	writeFiles(theMaze, K, M, N, _mazeID, entry_X, entry_Y, exitX, exitY);
}
int main()
{
	int K, M, N;
	int mazeID, entryX, entryY, exitX, exitY;
	cout << "Enter the number of mazes: ";
	cin>> K;
	cout << "Enter the number of rows and columns (M and N): ";
	cin>> M >> N;

	Stack<Stack<path>> mystacks;
	for (int i = 0; i < K; i++)
	{
		Stack<path> currentMaze;
		createMaze(M, N, currentMaze);
		mystacks.push(currentMaze);
	}

	writeMazeFiles(mystacks,M,N,K);

	cout << "All mazes are generated." << endl;
	findPath(mystacks, mazeID, entryX, entryY, exitX, exitY, K, M ,N); //find the path and overwrite them to files
	return 0;
}