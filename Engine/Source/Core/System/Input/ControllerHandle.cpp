#include "ControllerHandle.h"

ControllerHandle::ControllerHandle()
{
	Disconnect();
}

void ControllerHandle::Disconnect()
{
	using namespace Math;

	package = 0;
	isConnected = false;
	vibrationDelay = 0.0f;
	leftStick = Vector2();
	rightStick = Vector2();
	leftTrigger = 0.0f;
	rightTrigger = 0.0f;
	buttonStates = 0;
	lastButtonStates = 0;
}