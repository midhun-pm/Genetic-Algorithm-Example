
#include<iostream>
#include<vector>
#include<map>
#include<string>


using namespace std;
extern map < int , vector<int> > variantDNAMap;

void RunSimulation(int dnaID)
{
	if(variantDNAMap.find(dnaID) != variantDNAMap.end())
	{
		vector<int> dna = variantDNAMap.find(dnaID)->second;
		GenerateSituationActionsMap(dna);
	}
}

int main()
{ 
	RunSimulation(0);
}
