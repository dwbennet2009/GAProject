/*
* Copyright (c) 2006-2007 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/


#include "Render.h"
#include "Test.h"
#include "Box2D/Box2D.h"
#include "glui/glui.h"
#include "freeglut/freeglut.h"
#include <iostream>
#include "Settings.h"
#include "./WalkerGAs.h"

#include <stdio.h>

namespace
{
        int32 testIndex = 0;
        int32 testSelection = 0;
        int32 testCount = 0;
        Test* test;
        Settings settings;
        int32 width = 1200;
        int32 height = 750;
        int32 framePeriod = 16;
        int32 mainWindow;
        float settingsHz = 60.0;
        GLUI *glui;
        float32 viewZoom = 1.5f;
        int tx, ty, tw, th;
        bool rMouseDown;
        b2Vec2 lastp;

        int generationCount = 0;
}

static void Resize(int32 w, int32 h)
{
        width = w;
        height = h;

        GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
        glViewport(tx, ty, tw, th);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float32 ratio = float32(tw) / float32(th);

        b2Vec2 extents(ratio * 25.0f, 25.0f);
        extents *= viewZoom;

        b2Vec2 lower = settings.viewCenter - extents;
        b2Vec2 upper = settings.viewCenter + extents;

        // L/R/B/T
        gluOrtho2D(lower.x, upper.x, lower.y, upper.y);
}

static void SimulationLoop()
{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        b2Vec2 oldCenter = settings.viewCenter;
        settings.hz = settingsHz;

        //for(int i = 0; i < 5; i++)
        test->Step(&settings);

        WalkerGAs* ga = (WalkerGAs*)test;
        if (ga->getOrganismsRemaining() == 0) {
            //delete ga;
            delete test;
            test = WalkerGAs::Create();
            generationCount++;
            std::cout<<"Generation: "<<generationCount<<"\n";
        }


        if (oldCenter.x != settings.viewCenter.x || oldCenter.y != settings.viewCenter.y)
        {
                Resize(width, height);
        }

        //test->DrawTitle(entry->name);

        glutSwapBuffers();


}
// This is used to control the frame rate (60Hz).
static void Timer(int)
{
        glutSetWindow(mainWindow);
        glutPostRedisplay();
        glutTimerFunc(framePeriod, Timer, 0);
}
static void Pause(int)
{
        settings.pause = !settings.pause;
}

static void Exit(int code)
{
        // TODO: freeglut is not building on OSX
#ifdef FREEGLUT
        glutLeaveMainLoop();
#endif
        exit(code);
}

static void SingleStep(int)
{
        settings.pause = 1;
        settings.singleStep = 1;
}

// This is a simple example of building and running a simulation
// using Box2D. Here we create a large ground box and a small dynamic
// box.
// There are no graphics for this example. Box2D is meant to be used
// with your rendering engine in your game engine.
int main(int argc, char** argv)
{
	B2_NOT_USED(argc);
	B2_NOT_USED(argv);


        testIndex = b2Clamp(testIndex, 0, testCount-1);
        testSelection = testIndex;

        test = WalkerGAs::Create();


	// Define the gravity vector.
	b2Vec2 gravity(0.0f, -10.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	b2World world(gravity);

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
        glutInitWindowSize(width, height);
        char title[32];
        sprintf(title, "Box2D Version %d.%d.%d", b2_version.major, b2_version.minor, b2_version.revision);
        mainWindow = glutCreateWindow(title);

	std::cout<<"Hello World!\n";
        glutDisplayFunc(SimulationLoop);
        GLUI_Master.set_glutReshapeFunc(Resize);
//       GLUI_Master.set_glutKeyboardFunc(Keyboard);
     //   GLUI_Master.set_glutSpecialFunc(KeyboardSpecial);
      //  GLUI_Master.set_glutMouseFunc(Mouse);
#ifdef FREEGLUT
        //glutMouseWheelFunc(MouseWheel);
#endif
        //glutMotionFunc(MouseMotion);

        //glutKeyboardUpFunc(KeyboardUp);


        glui = GLUI_Master.create_glui_subwindow( mainWindow,
                GLUI_SUBWINDOW_RIGHT );

        glui->add_separator();


        glui->add_checkbox("Sleep", &settings.enableSleep);
        glui->add_checkbox("Warm Starting", &settings.enableWarmStarting);
        glui->add_checkbox("Time of Impact", &settings.enableContinuous);
        glui->add_checkbox("Sub-Stepping", &settings.enableSubStepping);

        //glui->add_separator();

        GLUI_Panel* drawPanel = glui->add_panel("Draw");
        glui->add_checkbox_to_panel(drawPanel, "Shapes", &settings.drawShapes);
        glui->add_checkbox_to_panel(drawPanel, "Joints", &settings.drawJoints);
        glui->add_checkbox_to_panel(drawPanel, "AABBs", &settings.drawAABBs);
        glui->add_checkbox_to_panel(drawPanel, "Contact Points", &settings.drawContactPoints);
        glui->add_checkbox_to_panel(drawPanel, "Contact Normals", &settings.drawContactNormals);
        glui->add_checkbox_to_panel(drawPanel, "Contact Impulses", &settings.drawContactImpulse);
        glui->add_checkbox_to_panel(drawPanel, "Friction Impulses", &settings.drawFrictionImpulse);
        glui->add_checkbox_to_panel(drawPanel, "Center of Masses", &settings.drawCOMs);
        glui->add_checkbox_to_panel(drawPanel, "Statistics", &settings.drawStats);
        glui->add_checkbox_to_panel(drawPanel, "Profile", &settings.drawProfile);
        //glui->add_button("Pause", 0, Pause);
        //glui->add_button("Single Step", 0, SingleStep);
        //glui->add_button("Restart", 0, Restart);

        //glui->add_button("Quit", 0,(GLUI_Update_CB)Exit);
        glui->set_main_gfx_window( mainWindow );

        // Use a timer to control the frame rate.
        glutTimerFunc(framePeriod, Timer, 0);

        glutMainLoop();

        return 0;

	return 0;
}
