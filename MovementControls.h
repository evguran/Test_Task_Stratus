#pragma once

#include <UnigineComponentSystem.h>
#include <UnigineLogic.h>
#include <UnigineStreams.h>

class MovementControls : public Unigine::ComponentBase
{
public:
	COMPONENT_DEFINE(MovementControls, Unigine::ComponentBase);

	// define the property parameters that can be adjusted via the Editor as well
	PROP_PARAM(Node, moving_node);

	// define the movement speed
	PROP_PARAM(Float, movement_speed, 5.0f);

	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update);

	void init();
	void update();
}; 