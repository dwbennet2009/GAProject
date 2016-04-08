#ifndef WALKERGAS_H
#define WALKERGAS_H
#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "SegmentHex.h"
#include "WalkerOrganism.h"
#include "Generation.h"
#include "Settings.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define PI 3.14159

std::string fileName = "./gaFile.txt";
std::string fileName2 = "./gaHighScores.txt";


class WalkerGAs : public Test
{

	public:

	struct timespec tv;
	double time_init, time_now;

	int doneFlag = 0;

	std::vector<WalkerOrganism*> walkerList;
	std::vector<Gene*> geneList;
	std::vector<double> finalScores;
	std::vector<Gene*> finalGenes;

	b2Body* staticBody1;
    b2Body* staticBody2;

    /// Destructor
	~WalkerGAs();

    /// Constructor
	WalkerGAs() {

        /// Gravity
        m_world->SetGravity(b2Vec2(0, -25));

        /// Call organism settings
        OrgSettings orgSettings;

        /// Create new generation from file, then mate and mutate it
        Generation genNow(fileName);
        genNow.geneMate();
        genNow.geneMutate();

        /// Fill gene list with strings from Generation
        for(int i = 0; i < 50; i++)
        {
            Gene* t1 = new Gene();

            t1->setGeneString(genNow.getGeneString(i));
            geneList.push_back(t1);
        }

        int indx = 0;

        /// Create all organism classes into vector list.
        for(int i = 0; i < geneList.size(); i++)
        {
            WalkerOrganism* orgOne = new WalkerOrganism();

                   /// Create body and 4 legs from orgSettings dimensions for length and width and scale.
            (orgOne)->setBody( orgSettings.Blength / orgSettings.scale,  orgSettings.Bwidth / orgSettings.scale );

            (orgOne)->setLeg( 0, orgSettings.FUlength / orgSettings.scale, orgSettings.FUwidth /orgSettings.scale,
				  orgSettings.FLlength / orgSettings.scale, orgSettings.FLwidth /orgSettings.scale,
				  orgSettings.FFtlength / orgSettings.scale, orgSettings.FFtwidth /orgSettings.scale );

            (orgOne)->setLeg( 1, orgSettings.FUlength /orgSettings.scale, orgSettings.FUwidth /orgSettings.scale,
				  orgSettings.FLlength / orgSettings.scale, orgSettings.FLwidth /orgSettings.scale,
				  orgSettings.FFtlength / orgSettings.scale, orgSettings.FFtwidth /orgSettings.scale );

            (orgOne)->setLeg( 2, orgSettings.RUlength /orgSettings.scale, orgSettings.RUwidth /orgSettings.scale,
				  orgSettings.RLlength / orgSettings.scale, orgSettings.RLwidth /orgSettings.scale,
				  orgSettings.RFtlength / orgSettings.scale, orgSettings.RFtwidth /orgSettings.scale );

            (orgOne)->setLeg( 3, orgSettings.RUlength /orgSettings.scale, orgSettings.RUwidth /orgSettings.scale,
				  orgSettings.RLlength / orgSettings.scale, orgSettings.RLwidth /orgSettings.scale,
				  orgSettings.RFtlength / orgSettings.scale, orgSettings.RFtwidth /orgSettings.scale );

            (orgOne)->createOrg(m_world);
            (orgOne)->setJoints(m_world);
            walkerList.push_back(orgOne);
        }


	for(int i = 0; i < walkerList.size(); i++)
	{
		walkerList[i]->setMotors(geneList[i]);
	}

	b2FixtureDef myFixDef;
	b2BodyDef myBodyDef;

	//void SegmentHex::initialize(double posX, double posY, b2PolygonShape shape, int catBits, int maskBits)



	//World Edges

	myBodyDef.type = b2_staticBody;
	myFixDef.filter.categoryBits = EDGE; 			//Is a RIGHT
	myFixDef.filter.maskBits = LEFT | RIGHT | EDGE | BODY; 	//Collide with LEFT, RIGHT, EDGE, BODY

	myFixDef.friction = 0.8;

	b2EdgeShape edgeShape;
	myFixDef.shape = &edgeShape;
	myBodyDef.position.Set(0, 0);
	edgeShape.Set( b2Vec2(-40, 0), b2Vec2(400, 0) );
	staticBody1 = m_world->CreateBody(&myBodyDef);
	staticBody1->CreateFixture(&myFixDef);

	myFixDef.shape = &edgeShape;
	myBodyDef.position.Set(-40, 0);
	edgeShape.Set( b2Vec2(0, 40), b2Vec2(0, -40) );
	staticBody2 = m_world->CreateBody(&myBodyDef);
	staticBody2->CreateFixture(&myFixDef);


	}

	void Step(Settings *settings)
	{

		Test::Step(settings);

		b2Vec2 position = walkerList[0]->getBodyPos();
		b2Vec2 edgePosition = staticBody2->GetPosition();

        double time = (m_stepCount * 1. / 60.);

		for(int i = 0; i < walkerList.size(); i++)
		{
                walkerList[i]->stepMotors(time);
                walkerList[i]->score = walkerList[i]->getBodyPos().x - edgePosition.x;
		}

		position.y = position.y - 15;
		Test::ShiftOrigin(position);

		if(m_stepCount>=100)
		{
			int imax = walkerList.size();
			for(int i = 0; i < imax; i++)
			{
				if(walkerList[i]->getBodyAngle()>90||walkerList[i]->getBodyAngle()<-90||m_stepCount>=1200)
				{
					finalScores.push_back(walkerList[i]->score);
					finalGenes.push_back(walkerList[i]->g1);

					delete walkerList[i];
					walkerList.erase(walkerList.begin()+i);

					i = i - 1;
					imax = imax - 1;
				}
			}
		}

		if(walkerList.size()==0&&doneFlag==0)
		{
			int i, j, flag = 1;
			int temp;
			Gene* tempGene;
			int numLength = finalGenes.size();
			for(i = 1; i < numLength && flag; i++)
			{
				flag = 0;
				for( j = 0; j < (numLength - 1); j++)
				{
					if(finalScores[j+1] > finalScores[j])
					{
						temp = finalScores[j];
						finalScores[j] = finalScores[j+1];
						finalScores[j+1] = temp;

						tempGene = finalGenes[j];
						finalGenes[j] = finalGenes[j+1];
						finalGenes[j+1] = tempGene;

						flag = 1;
					}
				}
			}


			for(i = 0; i < 5; i++)
			{
				std::cout<<finalGenes[i]->getGeneString()<<" | "<<finalScores[i]<<"\n";
			}
			std::cout<<"\n";

			std::ofstream gaFile;
			gaFile.open(fileName.c_str());
			for(i = 0; i < finalGenes.size(); i++)
			{
				gaFile<<finalGenes[i]->getGeneString()<<" "<<finalScores[i]<<"\n";
			}
			gaFile.close();

            std::ofstream gaFile2(fileName2.c_str(), std::ios::out | std::ios::app);
			for(i = 0; i < finalGenes.size(); i++)
			{
                if(finalScores[i]>100)
				gaFile2<<finalGenes[i]->getGeneString()<<" "<<finalScores[i]<<"\n";
			}
			gaFile2.close();

            doneFlag=1;
		}

	}

	uint32_t getOrganismsRemaining()
      	{
      	   return walkerList.size();
      	}

	static Test* Create()
	{
		return new WalkerGAs;
	}


};

WalkerGAs::~WalkerGAs()
{

        walkerList.clear();
        geneList.clear();

}

#endif
