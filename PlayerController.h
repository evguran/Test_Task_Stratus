#pragma once
#include <UnigineComponentSystem.h>

#include <UnigineGame.h>
#include <UnigineControls.h>
#include <UnigineStreams.h>
#include <UniginePlayers.h>
#include <UnigineWorld.h>
#include <UnigineConsole.h>
#include <UnigineMathLib.h>
#include <UnigineRender.h>

class PlayerController : public Unigine::ComponentBase
{
public:
	// declare constructor and destructor for our class and define a property name. 
	COMPONENT_DEFINE(PlayerController, ComponentBase)
	// declare methods to be called at the corresponding stages of the execution sequence
	COMPONENT_INIT(init); 
	COMPONENT_UPDATE_PHYSICS(updatePhysics);

protected:
	void init();
	void updatePhysics();

private:
	void move(const Unigine::Math::vec3& direction);

	Unigine::BodyRigidPtr rigid;

	Unigine::PlayerPtr player;

	// a WorldIntersection object to store the information about the intersection
	Unigine::WorldIntersectionPtr intersection = Unigine::WorldIntersection::create();

	Unigine::Math::Vec3 pos;

};