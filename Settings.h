#ifndef SETTINGS_H
#define SETTINGS_H


#include "Box2D/Box2D.h"
#include "glui/glui.h"
#include "freeglut/freeglut.h"
#include <iostream>

#include <stdio.h>


/// Organism Settings.  Length, width, height.  Relative area of joints.
/// Some of these might be controlled by Genes in future version.
struct OrgSettings
{

    OrgSettings()
    {

    }

    /// Rear Leg dimension, Upper, Lower, Foot
    double RUlength = 31;
    double RUwidth = 11;
	double RLlength = 26;
	double RLwidth = 5;
    double RFtlength = 15;
	double RFtwidth = 5;

	/// Front Leg dimensions, Upper, Lower, Foot
	double FUlength = 29;
	double FUwidth = 9;
	double FLlength = 20;
	double FLwidth = 4;
	double FFtlength = 15;
	double FFtwidth = 5 ;

	/// Body dimensions
	double Bwidth = 30;
    double Blength = 90;

    /// Scale for Organism Dimensions
	double scale = 6;
};


/// Test settings. Some can be controlled in the GUI.
struct Settings
{
        Settings()
        {
                viewCenter.Set(0.0f, 20.0f);
                hz = 60.0f;
                velocityIterations = 8;
                positionIterations = 3;
                drawShapes = 1;
                drawJoints = 1;
                drawAABBs = 0;
                drawContactPoints = 0;
                drawContactNormals = 0;
                drawContactImpulse = 0;
                drawFrictionImpulse = 0;
                drawCOMs = 0;
                drawStats = 0;
                drawProfile = 0;
                enableWarmStarting = 1;
                enableContinuous = 1;
                enableSubStepping = 0;
                enableSleep = 1;
                pause = 0;
                singleStep = 0;
        }

        b2Vec2 viewCenter;
        float32 hz;
        int32 velocityIterations;
        int32 positionIterations;
        int32 drawShapes;
        int32 drawJoints;
        int32 drawAABBs;
        int32 drawContactPoints;
        int32 drawContactNormals;
        int32 drawContactImpulse;
        int32 drawFrictionImpulse;
        int32 drawCOMs;
        int32 drawStats;
        int32 drawProfile;
        int32 enableWarmStarting;
        int32 enableContinuous;
        int32 enableSubStepping;
        int32 enableSleep;
        int32 pause;
        int32 singleStep;
};

#endif
