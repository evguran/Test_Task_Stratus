#include "MovementControls.h"
#include <UnigineControls.h>
#include <UnigineEditor.h>
#include <UnigineMathLib.h>
#include <UnigineGame.h>
#include <UnigineVisualizer.h>

REGISTER_COMPONENT(MovementControls);

using namespace Unigine;
using namespace Math;

void MovementControls::init() {
	
	// enable visualizer
	Visualizer::setEnabled(1);

	// check if the key is pressed and update the state of the specified control
	
	ControlsApp::setStateKey(Controls::STATE_AUX_0, Input::KEY_P);
	
}

void MovementControls::update() {
	
	// get the frame duration
	float ifps = Game::getIFps();

	// get the current world transformation matrix of the node this property is assigned to
	Mat4 transform = node->getWorldTransform();

	// get the direction vector of the node from the second column of the transformation matrix
	Vec3 direction = transform.getColumn3(1);

	// render the direction vector for visual clarity
	Visualizer::renderDirection(node->getWorldPosition(), vec3(direction), vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.1f, 0);

	// check if the control key is pressed
	if (ControlsApp::getState(Controls::STATE_AUX_0)) {

		// calculate the delta of movement
		Vec3 delta_movement = direction * movement_speed * ifps;

		// set a new position to the node
		node->setWorldPosition(node->getWorldPosition() + delta_movement);
	}
}