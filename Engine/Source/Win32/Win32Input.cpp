#include "..\Defines.h"

#ifdef PLATFORM_WINDOWS
#include "..\Core\System\Input\Controller.h"
#include "..\Core\System\Input\Mouse.h"
#include "..\Core\System\Input\Keyboard.h"
#include "..\Core\System\Window.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>

#include "Win32Keycode.h"

#pragma comment(lib, "XInput.lib")

static void update_controller(ControllerHandle& controller, XINPUT_STATE& state,
	XINPUT_VIBRATION& vibration, float dt, uint8 index);
static void check_sticks(ControllerHandle& controller, const XINPUT_GAMEPAD& gamepad);
static void check_buttons(ControllerHandle& controller, const XINPUT_GAMEPAD& gamepad);
static void check_triggers(ControllerHandle& controller, const XINPUT_GAMEPAD& gamepad);


namespace Input
{
	void Mouse::SetGlobalPosition(const Math::Point& position)
	{
		SetCursorPos(position.x, position.y);
	}

	void Mouse::SetPosition(const Math::Point& position, const Window& relativeWindow)
	{
		if (&relativeWindow == nullptr)
			return;

		POINT mouse;
		mouse.x = position.x;
		mouse.y = position.y;

		ClientToScreen((HWND)relativeWindow.GetHandle(), &mouse);
		SetCursorPos(mouse.x, mouse.y);
	}

	const Math::Point& Mouse::GetGlobalPosition()
	{
		POINT mouse;
		GetCursorPos(&mouse);

		s_GlobalPositon.x = mouse.x;
		s_GlobalPositon.y = mouse.y;
		return s_GlobalPositon;
	}



	void Keyboard::PlatformUpdate()
	{
		if (GetKeyState(WIN32KEYCODE_NUMLOCK) & 1)
			Keyboard::s_Toggle |= TOGGLEKEY_NUMLOCK;
		else
			Keyboard::s_Toggle &= ~TOGGLEKEY_NUMLOCK;

		if (GetKeyState(WIN32KEYCODE_CAPSLOCK) & 1)
			Keyboard::s_Toggle |= TOGGLEKEY_CAPSLOCK;
		else
			Keyboard::s_Toggle &= ~TOGGLEKEY_CAPSLOCK;

		if (GetKeyState(WIN32KEYCODE_SCROLLOCK) & 1)
			Keyboard::s_Toggle |= TOGGLEKEY_SCROLLOCK;
		else
			Keyboard::s_Toggle &= ~TOGGLEKEY_SCROLLOCK;
	}



	void Controller::PlatformUpdate()
	{
		s_Clock.Tick();

		XINPUT_STATE state;
		XINPUT_VIBRATION vibration;

		if ((s_Clock.TotalTime().AsSeconds() >= s_CheckIntervall))
		{
			for (int i = 0; i < MAX_CONTROLLERS; i++)
				update_controller(s_Controllers[i], state, vibration, (float)s_Clock.DeltaTime().AsSeconds(), i);

			s_Clock.Reset();
		}
		else
		{
			for (int i = 0; i < MAX_CONTROLLERS; i++)
			{
				if (s_Controllers[i].isConnected)
					update_controller(s_Controllers[i], state, vibration, (float)s_Clock.DeltaTime().AsSeconds(), i);
			}
		}
	}

	void Controller::Vibrate(CONTROLLER controller, float lMotorSpeed, float rMotorSpeed, float durationSec)
	{
		if (controller > CONTROLLER_4)
			return;
		if (!s_Controllers[controller].isConnected)
			return;

		if (s_Controllers[controller].vibrationDelay <= 0.0f)
		{
			XINPUT_VIBRATION vibration;
			vibration.wLeftMotorSpeed = (WORD)(lMotorSpeed * UINT16_MAX);
			vibration.wRightMotorSpeed = (WORD)(rMotorSpeed * UINT16_MAX);
			XInputSetState(controller, &vibration);
			s_Controllers[controller].vibrationDelay = durationSec;
		}
	}
}



void update_controller(ControllerHandle& controller, XINPUT_STATE& state, XINPUT_VIBRATION& vibration, float dt, uint8 index)
{
	if (XInputGetState(index, &state) == ERROR_SUCCESS)
	{
		controller.vibrationDelay -= dt;

		if (controller.vibrationDelay == 0.0f)
		{
			controller.vibrationDelay = 0.0f;

			vibration.wLeftMotorSpeed = 0;
			vibration.wRightMotorSpeed = 0;

			XInputSetState(index, &vibration);
		}


		controller.lastButtonStates = controller.buttonStates;
		controller.isConnected = true;


		check_sticks(controller, state.Gamepad);
		check_buttons(controller, state.Gamepad);
		check_triggers(controller, state.Gamepad);
	}
	else
	{
		controller.Disconnect();
	}
}



void check_sticks(ControllerHandle& controller, const XINPUT_GAMEPAD& gamepad)
{
	using namespace Math;

	Vector2 leftStick(gamepad.sThumbLX, gamepad.sThumbLY);
	Vector2 rightStick(gamepad.sThumbRX, gamepad.sThumbRY);

	//LEFT STICK
	float magnitude = leftStick.Length();
	if (magnitude > LEFT_STICK_DEADZONE)
		controller.leftStick = leftStick / magnitude;
	else
		controller.leftStick = Vector2();

	//RIGHT STICK
	magnitude = rightStick.Length();
	if (magnitude > RIGHT_STICK_DEADZONE)
		controller.rightStick = rightStick / magnitude;
	else
		controller.rightStick = Vector2();
}

void check_buttons(ControllerHandle& controller, const XINPUT_GAMEPAD& gamepad)
{
	//CONTROLLER PAD
	controller.buttonStates = 0;
	if (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		controller.buttonStates |= CONTROLLER_BUTTON_PAD_UP;
	if (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		controller.buttonStates |= CONTROLLER_BUTTON_PAD_DOWN;
	if (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		controller.buttonStates |= CONTROLLER_BUTTON_PAD_RIGHT;
	if (gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		controller.buttonStates |= CONTROLLER_BUTTON_PAD_LEFT;

	//CONTROLLER FACE
	if (gamepad.wButtons & XINPUT_GAMEPAD_Y)
		controller.buttonStates |= CONTROLLER_BUTTON_FACE_UP;
	if (gamepad.wButtons & XINPUT_GAMEPAD_A)
		controller.buttonStates |= CONTROLLER_BUTTON_FACE_DOWN;
	if (gamepad.wButtons & XINPUT_GAMEPAD_B)
		controller.buttonStates |= CONTROLLER_BUTTON_FACE_RIGHT;
	if (gamepad.wButtons & XINPUT_GAMEPAD_X)
		controller.buttonStates |= CONTROLLER_BUTTON_FACE_LEFT;

	//START - SELECT
	if (gamepad.wButtons & XINPUT_GAMEPAD_START)
		controller.buttonStates |= CONTROLLER_BUTTON_START;
	if (gamepad.wButtons & XINPUT_GAMEPAD_BACK)
		controller.buttonStates |= CONTROLLER_BUTTON_SELECT;

	//TRIGGER
	if (gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		controller.buttonStates |= CONTROLLER_BUTTON_LEFT_TRIGGER;
	if (gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		controller.buttonStates |= CONTROLLER_BUTTON_RIGHT_TRIGGER;

	//STICK
	if (gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
		controller.buttonStates |= CONTROLLER_BUTTON_LEFT_STICK;
	if (gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
		controller.buttonStates |= CONTROLLER_BUTTON_RIGHT_STICK;

}

void check_triggers(ControllerHandle& controller, const XINPUT_GAMEPAD& gamepad)
{
	//LEFT TRIGGER
	if (gamepad.bLeftTrigger > TRIGGER_DEADZONE)
		controller.leftTrigger = (float)(gamepad.bLeftTrigger - TRIGGER_DEADZONE) / (TRIGGER_MAXVALUE - TRIGGER_DEADZONE);
	else
		controller.leftTrigger = 0;

	//RIGHT TRIGGER
	if (gamepad.bRightTrigger > TRIGGER_DEADZONE)
		controller.rightTrigger = (float)(gamepad.bRightTrigger - TRIGGER_DEADZONE) / (TRIGGER_MAXVALUE - TRIGGER_DEADZONE);
	else
		controller.rightTrigger = 0;
}

#endif