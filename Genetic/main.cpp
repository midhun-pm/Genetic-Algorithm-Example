#include<iostream>
#include<vector>
#include<map>
#include<string>
#include <ctime>
#include <cmath>
#include<algorithm>


#include"../Snippets/snippets.h"

using namespace std;

map<string,int> situationActionMap;

const int CAN_PICKED_REWARD = 5;
const int NO_CAN_PENALTY = -1;
const int HIT_WALL_PENALTY = -10;

int BOT_C;
int BOT_R;

const int ROWS = 10;
const int COLS = 10;

const int MAX_BOT_MOVES = 100;


const int EMPTY_SQUARE = 0;
const int CAN_SQUARE = 1;
const int WALL_SQUARE= 2;

const int INIT_POPULATION = 100;


const int POSSIBILITY_DIRECTION_COUNT = 3;

enum actions { moveNorth, moveSouth, moveEast, moveWest, moveRandom, pickCan, stay, ActionsCount};

enum direction { northSquare, southSquare, eastSquare, westSquare, DirectionCount };

enum fuseType {MidSplit};

int grid[10][10];

map < int , vector<int> > variantDNAMap;
map < int , vector<int> > newDNAVariants;

multimap<int,int> sortedDNAIdsByPoints;





void PlaceBot(int row=-1, int col=-1)
{
   BOT_R = row;
   BOT_C = col;

    if(row == -1)
     BOT_R = rand()%10;
    
    if(col == -1)
     BOT_C = rand()%10;

//cout<<"BOT AT: R:"<<BOT_R<<" C:"<<BOT_C;

}

bool isWall(int r, int c)
{
	if(r<0 || c<0 || r>=ROWS || c>=COLS)
		return true;
	else
		return false;
}

int ActuateBot(int action)
{
	switch(action)
	{
		case moveNorth:
			{
				if(isWall(BOT_R-1,BOT_C))
				{
					return HIT_WALL_PENALTY;
				}
				else
				{
					BOT_R = BOT_R - 1;
				}
			}
		break;

		case moveSouth:
			{
				if(isWall(BOT_R+1,BOT_C))
				{
					return HIT_WALL_PENALTY;
				}
				else
				{
					BOT_R = BOT_R + 1;
				}
			}
		break;

		case moveWest:
			{
				if(isWall(BOT_R,BOT_C - 1))
				{
					return HIT_WALL_PENALTY;
				}
				else
				{
					BOT_C = BOT_C - 1;
				}
			}
		break;

		case moveEast:
			{
				if(isWall(BOT_R,BOT_C + 1))
				{
					return HIT_WALL_PENALTY;
				}
				else
				{
					BOT_C = BOT_C + 1;
				}
			}
		break;

		case pickCan:
			{
				if(grid[BOT_R][BOT_C] == 1)
				{
					grid[BOT_R][BOT_C] = 0;
					cout<<"Picked"<<"\n";
					return CAN_PICKED_REWARD;
				}
				else
				{
					return NO_CAN_PENALTY;
				}
			}
		break;

		case moveRandom:
		{
			int coin[] = { -1, 0, 1};
			int r_inc = coin[rand()%3];
			int c_inc = coin[rand()%3];

			if(isWall(BOT_R + r_inc, BOT_C + c_inc))
			{
			}
			else
			{
				BOT_R = BOT_R + r_inc;
				BOT_C = BOT_C + c_inc;
			}
		}
		break;

		case stay:
		default:
			return 0;
		break;
	}
	return -1;
}



string GetSituationString()
{

int spots[4];


if(BOT_R - 1 < 0)
{
	spots[northSquare] = WALL_SQUARE;
}
else
{


   if(grid[BOT_R - 1][BOT_C] == 0)
	spots[northSquare] = EMPTY_SQUARE;

   else if(grid[BOT_R - 1][BOT_C] == 1)
	spots[northSquare] = CAN_SQUARE;

}

if(BOT_R + 1 >= ROWS)
{
	spots[southSquare] = WALL_SQUARE;
}
else
{

   if(grid[BOT_R + 1][BOT_C] == 0)
	spots[southSquare] = EMPTY_SQUARE;

   else if(grid[BOT_R + 1][BOT_C] == 1)
	spots[southSquare] = CAN_SQUARE;

}


if(BOT_C - 1 < 0)
{
	spots[westSquare] = WALL_SQUARE;
}
else
{

   if(grid[BOT_R][BOT_C - 1] == 0)
	spots[westSquare] = EMPTY_SQUARE;

   else if(grid[BOT_R][BOT_C - 1] == 1)
	spots[westSquare] = CAN_SQUARE;

}

if(BOT_C + 1 >= COLS)
{
	spots[eastSquare] = WALL_SQUARE;
}
else
{

   if(grid[BOT_R][BOT_C + 1] == 0)
	spots[eastSquare] = EMPTY_SQUARE;

   else if(grid[BOT_R][BOT_C + 1] == 1)
	spots[eastSquare] = CAN_SQUARE;

}

stringstream ss;
for(int k=0;k<sizeof(spots)/sizeof(spots[0]);k++)
ss<<spots[k];

return ss.str();

	
	
}


void PopulateGrid(int cans)
{
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			grid[i][j] = 0;
		}
	}


	for(int c = 0 ; c < cans; c++)
	{
		int canSpot = rand()%(ROWS*COLS);
		int row = canSpot/ROWS;
		int col = canSpot%COLS;
		grid[row][col] = 1;
	}
}

void PrintGrid()
{
for(int i=0;i<ROWS;i++)
{
	for(int j=0;j<COLS;j++)
	{
		cout<<grid[i][j];
	}
	cout<<"\n";

}
}


int GetSituationCount()
{
	int situations = pow((double)DirectionCount,(double)POSSIBILITY_DIRECTION_COUNT);
	return situations;
}


vector< vector<int> > GenerateSituations()
{

	vector< vector<int> > vecAry;
	int rowCount = GetSituationCount();

	int alt = DirectionCount;
	int variables = POSSIBILITY_DIRECTION_COUNT;

	

	for(int i=0;i<rowCount;i++)
	{
		vector<int> row = change_base(i,variables,alt);
		vecAry.push_back(row);

	}
	return vecAry;

}

int GetRandomAction()
{
	return rand()%ActionsCount;
}


void GenerateSituationActionsMap(vector<int> dna)
{
	situationActionMap.clear();
	vector<vector<int> > situations = GenerateSituations();
	int rows = situations.size();

	for(int i=0;i<rows;i++)
	{
		vector<int> situation = situations[i];
		string situationStr = vec_to_string(situation);
		int action = dna[i];
		situationActionMap.insert(pair<string,int>(situationStr,action));
	}
}

void PrintSituationActionMap()
{
	for(map<string,int>::iterator itr=situationActionMap.begin(); itr != situationActionMap.end(); itr++)
		cout<<itr->first<<" "<<itr->second <<"\n";
}

void PrintVariantDNA(int id=-1)
{
	if(id == -1)
	{
		for(map<int,vector<int> >::iterator itr=variantDNAMap.begin(); itr != variantDNAMap.end(); itr++)
		{
			string dnaStr = vec_to_string(itr->second);
			cout<<itr->first<<": "<<dnaStr <<"\n";
		}
	}
	else if(variantDNAMap.find(id) != variantDNAMap.end())
	{
			string dnaStr = vec_to_string(variantDNAMap.find(id)->second);
			cout<<variantDNAMap.find(id)->first<<": "<<dnaStr <<"\n";
	}
}

void SimulatePlaceBot(int times)
{
	int i = times;
	while(i-- > 0)
	{
		PlaceBot();
		cout<< " "<<GetSituationString() <<"\n";
	}

}

void GenerateRandomDNA()
{
	for(int pop = 0 ; pop < INIT_POPULATION ; pop ++)
	{
		vector<int> dnastrand;
		for(int i = 0 ; i < GetSituationCount(); i++)
		{
			dnastrand.push_back(GetRandomAction());
		}
		variantDNAMap.insert(pair<int,vector<int> >(pop,dnastrand));
	}
}

int GetAction(string situation)
{
	if( situationActionMap.find(situation) != situationActionMap.end())
	{
		int action =  situationActionMap.find(situation) -> second;
		return action;
	}
	return -1;
}


int RunSimulation(int dnaID)
{
	if(variantDNAMap.find(dnaID) == variantDNAMap.end())
	{ cout<< "Stopping... ERROR: Could not find DNA_ID: "<<dnaID <<"\n"; exit(0); }

	else
	{
		vector<int> dna = variantDNAMap.find(dnaID)->second;
		GenerateSituationActionsMap(dna);

		int moves = 0;

		int points = 0;

		while ( moves++ < MAX_BOT_MOVES )
		{
			string situation = GetSituationString();	
			int action = GetAction(situation);
			int actionPoints = ActuateBot(action);
			points = points + actionPoints;
//			cout<<"Situation : "<<situation<<" Action : "<<action<<" Points: "<<actionPoints<<"\n";
		}
		return points;
	}

}

void FuseDNA(int dnaID1, int dnaID2, vector<int>& offspring1, vector<int>& offspring2, int fuseType = MidSplit)
{

	if(variantDNAMap.find(dnaID1) == variantDNAMap.end() || variantDNAMap.find(dnaID2) == variantDNAMap.end())
	{
		cout<<"ERROR: FuseDNA Failed for IDs: "<<dnaID1<<" "<<dnaID2;
	}
	else
	{
		vector<int> dna1 = variantDNAMap.find(dnaID1)->second;
		vector<int> dna2 = variantDNAMap.find(dnaID2)->second;


		switch(fuseType)
		{
			case MidSplit:
			{
				int i=0;
				while(i < dna1.size()/2)
				{
					offspring1.push_back(dna1[i]);
					i++;
				}

				while(i < dna2.size())
				{
					offspring1.push_back(dna2[i]);
					i++;
				}

				i=0;

				while(i < dna2.size()/2)
				{
					offspring2.push_back(dna1[i]);
					i++;
				}

				while(i < dna1.size())
				{
					offspring2.push_back(dna2[i]);
					i++;
				}

			}
			break;
		}
	}
}

void MutateRandom(vector<int>& dna, int maxBits)
{
	int bitsToFlip = rand()%maxBits + 1;
	
	while( bitsToFlip--  > 0 )
	{
		int bitLocation = rand() % dna.size();
		dna[bitLocation] = ( dna[bitLocation] + 1 ) % ActionsCount;
	}
}

void MateFirstN(int n)
{
	int count =0;
	vector<int> dnaIndexes;
	for(multimap<int,int>::reverse_iterator itr = sortedDNAIdsByPoints.rbegin() ; itr != sortedDNAIdsByPoints.rend() && count < n; itr++, count++)
	{
		dnaIndexes.push_back(itr->second);
	}
	random_shuffle(dnaIndexes.begin(),dnaIndexes.end());

	int k = 0;
	int p = 0;
	newDNAVariants.clear();
	while( k < n/2)
	{ 
	  vector<int> off1, off2;
	  FuseDNA( dnaIndexes[k] , dnaIndexes[n-1-k],off1,off2);
	  MutateRandom(off1,3);
	  MutateRandom(off2,3);
	  newDNAVariants.insert(pair<int,vector<int> >(p++,off1));
	  newDNAVariants.insert(pair<int,vector<int> >(p++,off2));
	  k++;
	}

	variantDNAMap.clear();

	int r=0;

	for(int i=0; i<newDNAVariants.size(); i++)
	{
	   variantDNAMap.insert(pair<int,vector<int> >(r++,newDNAVariants.find(i)->second));
	}
}

void PrintSortedDNAIds(int max)
{
	int c = 0;
	for(multimap<int,int>::reverse_iterator itr = sortedDNAIdsByPoints.rbegin(); itr != sortedDNAIdsByPoints.rend()&&c<max; itr++,c++)
	{
		cout<<itr->first<<" :"<<itr->second<<"\n";
	}
}


int main()
{
	srand((int)time(0));
	PopulateGrid(7);
	GenerateRandomDNA();

	/*map<int,int> a;
	a.insert(pair<int,int>(2,3));
	a.remove(2);
	cout<<"MAP"<<a.find(2)->second;*/
	grid[4][4] = 1;

	PrintGrid();


for(int gen=0; gen < 1000; gen++)
{	
sortedDNAIdsByPoints.clear();

	for(int i=0;i<INIT_POPULATION;i++)
	{
		PlaceBot(4,4);
		GenerateSituationActionsMap(variantDNAMap.find(i)->second);
		int points = RunSimulation(i);
		sortedDNAIdsByPoints.insert(pair<int,int>(points,i));
	}

	cout<<"Generation : "<<gen<<"\n";
	//PrintSituationActionMap();
	//PrintVariantDNA();
	PrintSortedDNAIds(1);
	MateFirstN(INIT_POPULATION);
}

/*
	GenerateRandomDNA();
	RunSimulation(0);
	PrintSituationActionMap();

	int simtimes;
	cout<<"Times:";
	cin>>simtimes;

	SimulatePlaceBot(simtimes);
	GenerateSituationActionsMap();
	PrintSituationActionMap();
	PopulateGrid(7);
	PrintGrid();
	cout<<"\n";
	PlaceBot(5,5);
	cout<<"Situation: "<<GetSituationString();
	cout<<"\n";
*/
}
