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


#ifndef __APP_WORLD_LOGIC_H__
#define __APP_WORLD_LOGIC_H__

#include <UnigineLogic.h>
#include <UnigineStreams.h>
#include <UnigineObjects.h>
#include <UniginePlayers.h>
#include <UnigineWorld.h>

#include <ProcJson.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_spline.h>

using namespace Unigine;

class AppWorldLogic : public Unigine::WorldLogic
{

public:
	AppWorldLogic();
	~AppWorldLogic() override;

	int init() override;

	int update() override;
	int postUpdate() override;
	int updatePhysics() override;

	int shutdown() override;

	int save(const Unigine::StreamPtr &stream) override;
	int restore(const Unigine::StreamPtr &stream) override;


private:
	// define the ObjectMeshStatic instance
	// so that it will be deleted with the AppWorldLogic instance
	Unigine::ObjectMeshStaticPtr mesh;

	//define the player camera
	Unigine::PlayerPtr player;

	//define the node to move
	Unigine::NodePtr node;

	//JSON object
	ProcJson pj;

	//interpolation things
	gsl_interp_accel *acc;
	gsl_spline *spline_cubic_x;
	gsl_spline *spline_cubic_y;
	gsl_spline *spline_cubic_z;

};

#endif // __APP_WORLD_LOGIC_H__
