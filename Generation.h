#ifndef GENERATION_H
#define GENERATION_H

#include <iostream>
#include <string>
#include <sstream>
#include "Gene.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define PI 3.14159

int championCount = 5;
double mutateChance1 = 0.35;
double mutateChance2 = 0.05;
double mutateStrength = 1;

class Generation
{
	public:
		Generation(std::string fileName);
		void geneMate();
		void geneMutate();
		std::string getGeneString(int indx);
        std::string createRandomString();
        std::string toString(int n);
	private:
		int geneLength;
		std::vector<double> scoreList;
		std::vector<std::string> geneList;
		std::vector<std::string> geneListNew;
};

Generation::Generation(std::string fileName)
{
	std::ifstream fileIn(fileName.c_str());

	std::string lineIn;
	if(fileIn.is_open())
	{
		while(getline(fileIn,lineIn))
		{
			std::string geneTemp, scoreTemp;
			double dScoreTemp;
			std::istringstream iss(lineIn);
			iss>>geneTemp;
			iss>>scoreTemp;
			dScoreTemp=std::atof(scoreTemp.c_str());

			scoreList.push_back(dScoreTemp);
			geneList.push_back(geneTemp);
		}
	}
	geneLength = geneList.size();

}

void Generation::geneMate()
{
	std::cout<<"Creating new gene list with top "<<championCount<<" champions from previous generation.\n\n";
	for(int i = 0; i < geneLength; i++)
	{
		geneListNew.push_back(geneList[i%championCount]);
	}
}

void Generation::geneMutate()
{
	std::cout<<"Mutating new gene list with a chance of "<<mutateChance2<<" and a strength of "<<mutateStrength<<".\n\n";

	//From 1 to length-5, keep 2 champion(s) unmodified from previous generation.
	//Also create 5 new completely random organisms
	for(int i = 2; i < geneListNew.size()-5; i++)
	{
        for(int h = 0; h < 1; h++)  //Mutate MAX times
        {
		for(int j = 0; j < geneListNew[i].length(); j++)
		{
			int rnd = rand() % 100;
			//In Even values (10's digits), use mutateChance2, in Odd values (1's digit) use mutateChance1
			if((j%2==0&&rnd < (mutateChance2 * 100))||(j%2==1&&rnd < (mutateChance1 * 100)))
			{
				int oldInt, newInt;
				std::string sNewInt;

				oldInt = std::atoi(geneListNew[i].substr(j,1).c_str());
				newInt = (rand()%2 ==1) ? oldInt + 1 : oldInt - 1;
				if(newInt == 10) newInt = 0;
				else if(newInt == -1) newInt = 9;
				sNewInt = static_cast<std::ostringstream*>(&(std::ostringstream()<<newInt))->str();
				geneListNew[i].replace(j,1,sNewInt);

			}
		}
		}
	}
    for(int i = geneListNew.size()-5; i < geneListNew.size(); i++)
	{
        geneListNew[i] = createRandomString();
	}
}

std::string Generation::getGeneString(int indx)
{
	return geneListNew[indx];
}

std::string Generation::createRandomString()
{
	std::string strVal = "";

	int rndX = 0;
	for(int i = 0; i < 18; i++)
	{
		rndX = rand() % 100;
		if(rndX==0)
			strVal = strVal + "00";
		else if(rndX>0&&rndX<10)
			strVal = strVal + "0" + this->toString(rndX);
		else
			strVal = strVal + this->toString(rndX);
	}

	return strVal;
}

std::string Generation::toString(int n)
{
	std::string cvtStr;
	std::ostringstream convert;
	convert<<n;
	cvtStr = convert.str();

	return cvtStr;
}
#endif

