#include "PlayerController.h"

using namespace Unigine;
using namespace Math;

REGISTER_COMPONENT(PlayerController);

void PlayerController::init()
{
	player = Game::getPlayer();

	if (node)
	{
		rigid = node->getObjectBodyRigid();
		if (rigid)
		{
			rigid->setAngularScale(vec3(0.0f, 0.0f, 0.0f)); // restricting the rotation
			rigid->setLinearScale(vec3(1.0f, 1.0f, 0.0f)); // restricting Z movement
			rigid->setMaxLinearVelocity(8.0f); // clamping the max linear velocity
		}

	}
}

void PlayerController::updatePhysics()
{
	if (!Console::isActive())  // do not process input if the console is shown
	{
		// check if W key is pressed
		if (Input::isKeyPressed(Input::KEY::KEY_W))
			move(player->getWorldDirection(MathLib::AXIS::AXIS_Y)); // move forward

		// check if S key is pressed
		if (Input::isKeyPressed(Input::KEY::KEY_S))
			move(player->getWorldDirection(MathLib::AXIS::AXIS_NY)); // move backward

		// check if A key is pressed
		if (Input::isKeyPressed(Input::KEY::KEY_A))
			move(player->getWorldDirection(MathLib::AXIS::AXIS_NX)); // move left

		// check if D key is pressed
		if (Input::isKeyPressed(Input::KEY::KEY_D))
			move(player->getWorldDirection(MathLib::AXIS::AXIS_X)); // move right

		// finding the positions of the cursor and the point moved 100 units away in the camera forward direction 
		ivec2 mouse = Input::getMousePosition();
		Vec3 p0 = player->getWorldPosition();
		Vec3 p1 = p0 + Vec3(player->getDirectionFromMainWindow(mouse.x, mouse.y)) * 100;

		// casting a ray from p0 to p1 to find the first intersected object
		ObjectPtr obj = World::getIntersection(p0, p1, 1, intersection); // the first bit of the intersection mask is set to 1, the rest are 0s

		// finding the intersection position, creating a transformation matrix to face this position and setting the transform matrix for the body preserving current angular and linear velocities
		if (obj && rigid)
		{
			pos = intersection->getPoint();
			pos.z = rigid->getTransform().getTranslate().z; // project the position vector to the Body Rigid pivot plane
			Mat4 transform = Math::setTo(rigid->getTransform().getTranslate(), pos, vec3_up, AXIS::AXIS_Y);
			rigid->setPreserveTransform(transform); // turn the character's body
		}
	}
}

// moving the rigid body with linear impulse in the specified direction
void PlayerController::move(const Unigine::Math::vec3& direction)
{
	// direction is a normalized camera axis vector 
	if (rigid)
		// direction is a normalized camera axis vector 
		rigid->addLinearImpulse(direction);
}