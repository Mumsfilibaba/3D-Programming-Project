#include "Controller.h"

namespace Input
{
	ControllerHandle Controller::s_Controllers[];
	Clock Controller::s_Clock = Clock();
	double Controller::s_CheckIntervall = 2.5;



	void Controller::SetConnectedIntervall(double intervall)
	{
		s_CheckIntervall = intervall;
	}



	bool Controller::IsConnected(CONTROLLER controller)
	{
		if (controller < 4)
			return s_Controllers[controller].isConnected;

		return false;
	}



	bool Controller::IsButtonDown(CONTROLLER controller, CONTROLLER_BUTTON button)
	{
		if (controller > CONTROLLER_4)
			return false;
		else
			return s_Controllers[controller].buttonStates & button;
	}

	bool Controller::IsButtonUp(CONTROLLER controller, CONTROLLER_BUTTON button)
	{
		if (controller > CONTROLLER_4)
			return false;
		else
			return !(s_Controllers[controller].buttonStates & button);
	}

	bool Controller::ButtonPressed(CONTROLLER controller, CONTROLLER_BUTTON button)
	{
		if (controller > CONTROLLER_4)
			return false;

		return (s_Controllers[controller].buttonStates & button) &&
			!(s_Controllers[controller].lastButtonStates & button);
	}



	float Controller::GetLeftTriggerValue(CONTROLLER controller)
	{
		if (controller > CONTROLLER_4)
			return 0.0f;
		else
			return s_Controllers[controller].leftTrigger;
	}

	float Controller::GetRightTriggerValue(CONTROLLER controller)
	{
		if (controller > CONTROLLER_4)
			return 0.0f;
		else
			return s_Controllers[controller].rightTrigger;
	}



	const Math::Vector2& Controller::GetLeftStickValue(CONTROLLER controller)
	{
		assert(controller <= CONTROLLER_4);
		return s_Controllers[controller].leftStick;
	}

	const Math::Vector2& Controller::GetRightStickValue(CONTROLLER controller)
	{
		assert(controller <= CONTROLLER_4);
		return s_Controllers[controller].rightStick;
	}
}