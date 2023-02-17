/* Copyright (C) 2005-2022, UNIGINE. All rights reserved.
 *
 * This file is a part of the UNIGINE 2 SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE. at http://unigine.com/
 */


#include "AppWorldLogic.h"
#include <UnigineMathLib.h>
#include <UnigineGame.h>
#include <UnigineMesh.h>
#include <UnigineWorld.h>

#include <ProcJson.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_spline.h>

#include <iostream>

using namespace Math;

// World logic, it takes effect only when the world is loaded.
// These methods are called right after corresponding world script's (UnigineScript) methods.

AppWorldLogic::AppWorldLogic()
{}

AppWorldLogic::~AppWorldLogic()
{}

int AppWorldLogic::init()
{
	// Log::message("Node position: %f, %f, %f\n", node->getWorldPosition().x, node->getWorldPosition().y, node->getWorldPosition().z);
	// Log::message("Player position: %f, %f, %f\n", player->getWorldPosition().x, player->getWorldPosition().y, player->getWorldPosition().z);

	// Reading JSON file and get the first value
	pj.read_data();
	pj.get_next();

	// Defining interpolation objects
	size_t N = pj.size;
	std::cout << "N  = " << N << "\n";
	double t[N], x[N], y[N], z[N];
	for (int i = 0; i < N; i++) {
		t[i] = pj.t[i];
		x[i] = pj.x[i];
		y[i] = pj.y[i];
		z[i] = pj.z[i];
		// std::cout << t[i] << ", " << x[i] << ", " << y[i] << ", " << z[i] << "\n";
		// std::cout << pj.t[i] << ", " << pj.x[i] << ", " << pj.y[i] << ", " << pj.z[i] << "\n";
		// create a mesh instance
		MeshPtr mesh = Mesh::create();
		// add box surface with the name "box_surface" and the size of the surface is Vec3(1.0, 1.0, 1.0)
		mesh->addBoxSurface("box_surface", Math::Vec3(0.2f));

		// create the ObjectMeshDynamic from the Mesh object
		ObjectMeshDynamicPtr dynamicMesh = ObjectMeshDynamic::create(mesh);
								
		// set the position of the mesh
		dynamicMesh->setWorldPosition(Vec3(x[i], y[i], z[i]));
	}

	acc = gsl_interp_accel_alloc();
	spline_cubic_x = gsl_spline_alloc(gsl_interp_cspline, N);
	spline_cubic_y = gsl_spline_alloc(gsl_interp_cspline, N);
	spline_cubic_z = gsl_spline_alloc(gsl_interp_cspline, N);

	gsl_spline_init(spline_cubic_x, t, x, N);
	gsl_spline_init(spline_cubic_y, t, y, N);
	gsl_spline_init(spline_cubic_z, t, z, N);

	// get the current game camera
	player = Game::getPlayer();

	// set the camera position and direction to look at the object
	player->setPosition(Vec3(0.0f, 0.0f, 10.0f));
	// player->setDirection(vec3(0.0f, 1.0f, -0.4f), player->getUp());
	
	// node = World::getNodeByName("robot");
	node = World::getNodeByName("drone_costum");
	// initial positioning
	node->setWorldPosition(vec3(x[0], y[0], z[0]));
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// start of the main loop
////////////////////////////////////////////////////////////////////////////////

int AppWorldLogic::update()
{
	// Write here code to be called before updating each render frame: specify all graphics-related functions you want to be called every frame while your application executes.
	// Log::message("Players world position: %f\n", node->getWorldPosition().x);
	
	// get the frame duration
	float ifps = Game::getIFps();
	float delayStart = 2.0; // two seconds for start;
	float animTime = (Game::getTime() - delayStart) / 10;

	vec3 currPos = vec3(0.0f, 0.0f, 0.0f);
	vec3 nextPos = vec3(0.0f, 0.0f, 0.0f);
	// speed
	// float movement_speed = 2.0f;

	// vec3 direction = vec3(1.0f, 0.0f, 0.0f);

	// // calculate the delta of movement
	// Vec3 delta_movement = direction * movement_speed * ifps;

	// set a new position to the node
	// node->setWorldPosition(node->getWorldPosition() + delta_movement);
	
	if (animTime >= pj.t.front() and animTime <= pj.t.back()) {
		currPos.x = gsl_spline_eval(spline_cubic_x, animTime, acc);
		currPos.y = gsl_spline_eval(spline_cubic_y, animTime, acc);
		currPos.z = gsl_spline_eval(spline_cubic_z, animTime, acc);
		int div = 10;
		if (animTime + ifps / div >= pj.t.front() and animTime + ifps / div <= pj.t.back()) {
			nextPos.x = gsl_spline_eval(spline_cubic_x, animTime + ifps / 10, acc);
			nextPos.y = gsl_spline_eval(spline_cubic_y, animTime + ifps / 10, acc);
			nextPos.z = gsl_spline_eval(spline_cubic_z, animTime + ifps / 10, acc);
		} else {
			nextPos.x = currPos.x;
			nextPos.y = currPos.y;
			nextPos.z = currPos.z; 
			currPos.x = gsl_spline_eval(spline_cubic_x, animTime - ifps / 10, acc);
			currPos.y = gsl_spline_eval(spline_cubic_y, animTime - ifps / 10, acc);
			currPos.z = gsl_spline_eval(spline_cubic_z, animTime - ifps / 10, acc);
		}
		
		node->setWorldPosition(currPos);
		node->setWorldDirection((nextPos-currPos).normalize(), player->getUp(), Math::AXIS_Z);

	}	
	
	// camera orient
	// vec3 direc = -vec3(player->getWorldPosition().x - node->getWorldPosition().x, \ 
	// 				   player->getWorldPosition().y - node->getWorldPosition().y, \
	// 				   player->getWorldPosition().z - node->getWorldPosition().z);
	
	// player->setDirection(direc, player->getUp());
	vec3 nodeWorldPos = node->getWorldPosition();
	vec3 nodeWorldDir = node->getWorldDirection(Math::AXIS_Z); // eto xyeta izza blendera
	// std::cout << "Pos: " << nodeWorldPos.x << " " << nodeWorldPos.y << " " << nodeWorldPos.z << "\n";
	// std::cout << "Dir: " << nodeWorldDir.x << " " << nodeWorldDir.y << " " << nodeWorldDir.z << "\n";

	player->setWorldPosition(nodeWorldPos - nodeWorldDir*5.0f + vec3(0.0f, 0.0f, 2.0f));
	player->setWorldDirection(nodeWorldDir, player->getUp());

	// vec3 a = vec3(2.0f, 0.0f, 0.0f) - vec3(3.0f, 3.0f, 0.0f);
	// std::cout << a.x << " " << a.y << " " << a.z << "\n";

	return 1;
}

int AppWorldLogic::postUpdate()
{
	// The engine calls this function after updating each render frame: correct behavior after the state of the node has been updated.
	return 1;
}

int AppWorldLogic::updatePhysics()
{
	// Write here code to be called before updating each physics frame: control physics in your application and put non-rendering calculations.
	// The engine calls updatePhysics() with the fixed rate (60 times per second by default) regardless of the FPS value.
	// WARNING: do not create, delete or change transformations of nodes here, because rendering is already in progress.
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// end of the main loop
////////////////////////////////////////////////////////////////////////////////

int AppWorldLogic::shutdown()
{
	// Write here code to be called on world shutdown: delete resources that were created during world script execution to avoid memory leaks.
	
	gsl_spline_free (spline_cubic_x);
	gsl_spline_free (spline_cubic_y);
	gsl_spline_free (spline_cubic_z);
	gsl_interp_accel_free (acc);
	return 1;
}

int AppWorldLogic::save(const Unigine::StreamPtr &stream)
{
	// Write here code to be called when the world is saving its state (i.e. state_save is called): save custom user data to a file.
	UNIGINE_UNUSED(stream);
	return 1;
}

int AppWorldLogic::restore(const Unigine::StreamPtr &stream)
{
	// Write here code to be called when the world is restoring its state (i.e. state_restore is called): restore custom user data to a file here.
	UNIGINE_UNUSED(stream);
	return 1;
}
