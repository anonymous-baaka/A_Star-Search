#include<iostream>
#include<fstream> //for ifstream: open file
#include<string>
#include<sstream> //not used
#include<vector>
#include<math.h>
#include<algorithm>		//for sort
using namespace std;

enum class state {kempty,kobstacle};
//0=empty,1=obstacle,-1=closed,path=2...
const int delta[4][2]{ {-1, 0}, {0, -1}, {1, 0}, {0, 1} };

int Heuristic(int x1, int y1, int x2, int y2)	//calculate and return h ie dsitance from source(x1,y1) to goal(x2,y2)
{
	return abs(x1 - x2) + abs(y1 - y2);
}
bool compare(vector<int>first, vector<int>second) //compare f values of 2 nodes; true if first greater
{
	if ((first[2] + first[3]) > (second[2] + second[3]))
		return true;
	return false;
}
void AddToOpen(int x, int y, int g, int h, vector<vector<int>>& open, vector<vector<int>>& board) //add node to visited
{
	//vector<int>row;
	open.push_back(vector<int>{x, y, g, h});
	board[x][y] = -1; //closed
}
void cellsort(vector<vector<int>>*open)		//descending order
{
	sort(open->begin(), open->end(), compare);
}

bool CheckValidCell(int x, int y,vector<vector<int>>&board)
{
	int lengthOfBoard = board.size();
	int widthOfBoard = board[0].size();
	cout << "x= " << x << " y= " << y<<endl;
	//check if x,y lie in range
	if (x >= lengthOfBoard || y >= widthOfBoard || x<0 || y<0)
		return false;
	//check if x,y is visited and empty
	if (board[x][y] == 0)
		return true;
	return false;
}

void ExpandNeighbors(vector<int>& currentNode, vector<vector<int>>& open, vector<vector<int>>&board, int goal[2])
{
	int x = currentNode[0];
	int y = currentNode[1];
	int g = currentNode[2];

	for (int i = 0; i < 4; i++)
	{
		int x2 = x + delta[i][0];
		int y2 = y + delta[i][1];
		if (CheckValidCell(x2, y2, board))
		{
			int g2 = g + 1;
			int h2 = Heuristic(x2, y2, goal[0], goal[1]);
			AddToOpen(x2, y2, g2, h2, open, board);
		}
	}
}
vector<vector<int>> Search(vector<vector<int>>board,int start[2],int goal[2])
{
	vector<vector<int>>open;
	int x = start[0];	//source
	int y = start[1];	//destn
	int g = 0;
	int h = Heuristic(x, y, goal[0], goal[1]);
	vector<int > currentNode;
	AddToOpen(x, y, g, h, open, board);
	while (open.size()>0) //while open is not empty
	{
		cout << "size= " << open.size()<<endl;
		//sort
		cellsort(&open);
		//currentNode = open.back();//last row
		currentNode = open[open.size() - 1];
		//cout << currentNode[0]<<currentNode[1]<<currentNode[2]<<currentNode[3];
		open.pop_back();
		x = currentNode[0];
		y = currentNode[1];
		board[x][y] = 2;		//path
		if (x == goal[0] && y == goal[1])
			return board;
		ExpandNeighbors(currentNode, open, board, goal);

	}
	cout << "no path found!!" << endl;
}



vector<int>parseLine(string line)
{
	vector<int>temp;
	for (int i = 0; i < line.size(); i++)
		if(line[i]=='0' || line[i]=='1')
			temp.push_back(line[i]-48);
	return temp;
}

vector<vector<int>> initialise(vector<vector<int>> board,string path)
{
	int i;
	fstream myfile;
	myfile.open(path);
	string line;

	while (getline(myfile, line))
	{
		//cout << "line= " << line << endl;
		vector<int> raw;
		raw=parseLine(line);// remove '{', '}' and ','
		board.push_back(raw);
	}
	return board;
}
void display(vector<vector<int>> board)
{
	for (int i = 0; i < board.size(); i++)
	{
		for (int j = 0; j < board[i].size(); j++)
		{
			cout << board[i][j]<<"\t";
		}
		cout << endl;
	}
}
int main()
{
	string path = "abc.txt";
	vector<vector<int>> board;
	board=initialise(board,path);
	display(board);
	int start[2] = { 0,0 };
	int goal[2] = {4,5};
	vector<vector<int>> solution=Search(board,start,goal);
	display(solution);
}