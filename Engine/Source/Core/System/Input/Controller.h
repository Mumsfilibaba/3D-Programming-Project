#ifndef CONTROLLERSTATE_H
#define CONTROLLERSTATE_H

#include "..\..\Utilities\Clock.h"
#include "ControllerHandle.h"

namespace Input
{
	class Controller
	{
	public:
		Controller() = delete;
		~Controller() = delete;

		static void PlatformUpdate();
	
		static bool IsConnected(CONTROLLER controller);

		static bool IsButtonDown(CONTROLLER controller, CONTROLLER_BUTTON button);
		static bool IsButtonUp(CONTROLLER controller, CONTROLLER_BUTTON button);
		static bool ButtonPressed(CONTROLLER controller, CONTROLLER_BUTTON button);
	
		static void Vibrate(CONTROLLER controller, float lMotorSpeed, float rMotorSpeed, float durationSec);
	
		static void SetConnectedIntervall(double intervall);
	
		static float GetLeftTriggerValue(CONTROLLER controller);
		static float GetRightTriggerValue(CONTROLLER controller);
	
		static const Math::Vector2& GetLeftStickValue(CONTROLLER controller);
		static const Math::Vector2& GetRightStickValue(CONTROLLER controller);
	
		static inline double GetConnectedIntervall()
		{
			return s_CheckIntervall;
		}

	private:
		static ControllerHandle s_Controllers[MAX_CONTROLLERS];
		static Clock s_Clock;
		static double s_CheckIntervall;
	};
}
#endif