#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "..\..\Math\Vector2.h"

#define MAX_CONTROLLERS 4
#define STICK_MAXVALUE 32767
#define LEFT_STICK_DEADZONE 7849
#define RIGHT_STICK_DEADZONE 8269
#define TRIGGER_DEADZONE 30
#define TRIGGER_MAXVALUE 255

enum CONTROLLER : uint8
{
	CONTROLLER_1 = 0,
	CONTROLLER_2 = 1,
	CONTROLLER_3 = 2,
	CONTROLLER_4 = 3,
};

enum CONTROLLER_BUTTON : uint16
{
	CONTROLLER_BUTTON_PAD_UP = (1 << 0),
	CONTROLLER_BUTTON_PAD_DOWN = (1 << 1),
	CONTROLLER_BUTTON_PAD_LEFT = (1 << 2),
	CONTROLLER_BUTTON_PAD_RIGHT = (1 << 3),
	CONTROLLER_BUTTON_START = (1 << 4),
	CONTROLLER_BUTTON_SELECT = (1 << 5),
	CONTROLLER_BUTTON_LEFT_STICK = (1 << 6),
	CONTROLLER_BUTTON_RIGHT_STICK = (1 << 7),
	CONTROLLER_BUTTON_LEFT_TRIGGER = (1 << 8),
	CONTROLLER_BUTTON_RIGHT_TRIGGER = (1 << 9),
	CONTROLLER_BUTTON_FACE_UP = (1 << 10),
	CONTROLLER_BUTTON_FACE_DOWN = (1 << 11),
	CONTROLLER_BUTTON_FACE_LEFT = (1 << 12),
	CONTROLLER_BUTTON_FACE_RIGHT = (1 << 13),
	CONTROLLER_BUTTON_UNKNOWN = (1 << 14),
};

struct ControllerHandle
{
	ControllerHandle();

	uint32 package;
	bool isConnected;
	float vibrationDelay;
	
	Math::Vector2 leftStick;
	Math::Vector2 rightStick;
	float leftTrigger;
	float rightTrigger;
	uint16 buttonStates;
	uint16 lastButtonStates;

	void Disconnect();
};
#endif