/*!
@file InputManager.cpp
@brief 入力マネージャー
@authors Kato Yuki
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	unique_ptr<InputManager, InputManager::InputManagerDeleter> InputManager::m_InputManager;

	InputManager::InputManager()
	{
	}

	// シングルトン構築
	unique_ptr<InputManager, InputManager::InputManagerDeleter>& InputManager::CreateInputManager()
	{
		try
		{
			if (m_InputManager.get() == 0)
			{
				//自分自身の構築
				m_InputManager.reset(new InputManager());
				// 初期化
				m_InputManager->Initialize();
			}
			return m_InputManager;
		}
		catch (...)
		{
			throw;
		}
	}

	// シングルトンアクセサ
	unique_ptr<InputManager, InputManager::InputManagerDeleter>& InputManager::GetInputManager()
	{
		try
		{
			if (m_InputManager.get() == 0)
			{
				throw BaseException(
					L"InputManagerがまだ作成されてません",
					L"if (m_InputManager.get() == 0)",
					L"InputManager::GetInputManager()"
				);
			}
			return m_InputManager;
		}
		catch (...)
		{
			throw;
		}
	}

	// 強制破棄
	void InputManager::DeleteInputManager()
	{
		if (m_InputManager.get())
		{
			// 削除処理
			m_InputManager->Discard();
			// 自分自身の破棄
			m_InputManager.reset();
		}
	}

	// 初期化
	void InputManager::Initialize()
	{
		auto& app = App::GetApp();
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];

		m_LStick = Vec2(pad.fThumbLX, pad.fThumbLY);
		m_RStick = Vec2(pad.fThumbRX, pad.fThumbRY);

		// Button
		{
			m_Buttons[L"DUp"] = pad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_Buttons[L"DDown"] = pad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_Buttons[L"DLeft"] = pad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_Buttons[L"DRight"] = pad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_Buttons[L"Start"] = pad.wButtons & XINPUT_GAMEPAD_START;
			m_Buttons[L"Back"] = pad.wButtons & XINPUT_GAMEPAD_BACK;

			m_Buttons[L"LStick"] = pad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_Buttons[L"RStick"] = pad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_Buttons[L"L"] = pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_Buttons[L"R"] = pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_Buttons[L"A"] = pad.wButtons & XINPUT_GAMEPAD_A;
			m_Buttons[L"B"] = pad.wButtons & XINPUT_GAMEPAD_B;
			m_Buttons[L"X"] = pad.wButtons & XINPUT_GAMEPAD_X;
			m_Buttons[L"Y"] = pad.wButtons & XINPUT_GAMEPAD_Y;
		}

		// DownButton
		{
			m_DownButtons[L"DUp"] = pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_DownButtons[L"DDown"] = pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_DownButtons[L"DLeft"] = pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_DownButtons[L"DRight"] = pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_DownButtons[L"Start"] = pad.wPressedButtons & XINPUT_GAMEPAD_START;
			m_DownButtons[L"Back"] = pad.wPressedButtons & XINPUT_GAMEPAD_BACK;

			m_DownButtons[L"LStick"] = pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_DownButtons[L"RStick"] = pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_DownButtons[L"L"] = pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_DownButtons[L"R"] = pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_DownButtons[L"A"] = pad.wPressedButtons & XINPUT_GAMEPAD_A;
			m_DownButtons[L"B"] = pad.wPressedButtons & XINPUT_GAMEPAD_B;
			m_DownButtons[L"X"] = pad.wPressedButtons & XINPUT_GAMEPAD_X;
			m_DownButtons[L"Y"] = pad.wPressedButtons & XINPUT_GAMEPAD_Y;
		}

		// UpButton
		{
			m_UpButtons[L"DUp"] = pad.wReleasedButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_UpButtons[L"DDown"] = pad.wReleasedButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_UpButtons[L"DLeft"] = pad.wReleasedButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_UpButtons[L"DRight"] = pad.wReleasedButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_UpButtons[L"Start"] = pad.wReleasedButtons & XINPUT_GAMEPAD_START;
			m_UpButtons[L"Back"] = pad.wReleasedButtons & XINPUT_GAMEPAD_BACK;

			m_UpButtons[L"LStick"] = pad.wReleasedButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_UpButtons[L"RStick"] = pad.wReleasedButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_UpButtons[L"L"] = pad.wReleasedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_UpButtons[L"R"] = pad.wReleasedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_UpButtons[L"A"] = pad.wReleasedButtons & XINPUT_GAMEPAD_A;
			m_UpButtons[L"B"] = pad.wReleasedButtons & XINPUT_GAMEPAD_B;
			m_UpButtons[L"X"] = pad.wReleasedButtons & XINPUT_GAMEPAD_X;
			m_UpButtons[L"Y"] = pad.wReleasedButtons & XINPUT_GAMEPAD_Y;
		}

		// NowUpdateButton
		{
			m_NowUpdateButtons[L"DUp"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_NowUpdateButtons[L"DDown"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_NowUpdateButtons[L"DLeft"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_NowUpdateButtons[L"DRight"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_NowUpdateButtons[L"Start"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_START;
			m_NowUpdateButtons[L"Back"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_BACK;

			m_NowUpdateButtons[L"LStick"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_NowUpdateButtons[L"RStick"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_NowUpdateButtons[L"L"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_NowUpdateButtons[L"R"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_NowUpdateButtons[L"A"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_A;
			m_NowUpdateButtons[L"B"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_B;
			m_NowUpdateButtons[L"X"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_X;
			m_NowUpdateButtons[L"Y"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_Y;
		}

		// LastButton
		{
			m_LastButtons[L"DUp"] = pad.wLastButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_LastButtons[L"DDown"] = pad.wLastButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_LastButtons[L"DLeft"] = pad.wLastButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_LastButtons[L"DRight"] = pad.wLastButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_LastButtons[L"Start"] = pad.wLastButtons & XINPUT_GAMEPAD_START;
			m_LastButtons[L"Back"] = pad.wLastButtons & XINPUT_GAMEPAD_BACK;

			m_LastButtons[L"LStick"] = pad.wLastButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_LastButtons[L"RStick"] = pad.wLastButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_LastButtons[L"L"] = pad.wLastButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_LastButtons[L"R"] = pad.wLastButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_LastButtons[L"A"] = pad.wLastButtons & XINPUT_GAMEPAD_A;
			m_LastButtons[L"B"] = pad.wLastButtons & XINPUT_GAMEPAD_B;
			m_LastButtons[L"X"] = pad.wLastButtons & XINPUT_GAMEPAD_X;
			m_LastButtons[L"Y"] = pad.wLastButtons & XINPUT_GAMEPAD_Y;
		}
	}

	// 更新
	void InputManager::Update()
	{
		auto& app = App::GetApp();
		auto input = app->GetInputDevice();
		auto pad = input.GetControlerVec()[0];
		
		m_LStick = Vec2(pad.fThumbLX, pad.fThumbLY);
		m_RStick = Vec2(pad.fThumbRX, pad.fThumbRY);

		// Button
		{
			m_Buttons[L"DUp"] = pad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_Buttons[L"DDown"] = pad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_Buttons[L"DLeft"] = pad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_Buttons[L"DRight"] = pad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_Buttons[L"Start"] = pad.wButtons & XINPUT_GAMEPAD_START;
			m_Buttons[L"Back"] = pad.wButtons & XINPUT_GAMEPAD_BACK;

			m_Buttons[L"LStick"] = pad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_Buttons[L"RStick"] = pad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_Buttons[L"L"] = pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_Buttons[L"R"] = pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_Buttons[L"A"] = pad.wButtons & XINPUT_GAMEPAD_A;
			m_Buttons[L"B"] = pad.wButtons & XINPUT_GAMEPAD_B;
			m_Buttons[L"X"] = pad.wButtons & XINPUT_GAMEPAD_X;
			m_Buttons[L"Y"] = pad.wButtons & XINPUT_GAMEPAD_Y;
		}

		// DownButton
		{
			m_DownButtons[L"DUp"] = pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_DownButtons[L"DDown"] = pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_DownButtons[L"DLeft"] = pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_DownButtons[L"DRight"] = pad.wPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_DownButtons[L"Start"] = pad.wPressedButtons & XINPUT_GAMEPAD_START;
			m_DownButtons[L"Back"] = pad.wPressedButtons & XINPUT_GAMEPAD_BACK;

			m_DownButtons[L"LStick"] = pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_DownButtons[L"RStick"] = pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_DownButtons[L"L"] = pad.wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_DownButtons[L"R"] = pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_DownButtons[L"A"] = pad.wPressedButtons & XINPUT_GAMEPAD_A;
			m_DownButtons[L"B"] = pad.wPressedButtons & XINPUT_GAMEPAD_B;
			m_DownButtons[L"X"] = pad.wPressedButtons & XINPUT_GAMEPAD_X;
			m_DownButtons[L"Y"] = pad.wPressedButtons & XINPUT_GAMEPAD_Y;
		}

		// UpButton
		{
			m_UpButtons[L"DUp"] = pad.wReleasedButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_UpButtons[L"DDown"] = pad.wReleasedButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_UpButtons[L"DLeft"] = pad.wReleasedButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_UpButtons[L"DRight"] = pad.wReleasedButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_UpButtons[L"Start"] = pad.wReleasedButtons & XINPUT_GAMEPAD_START;
			m_UpButtons[L"Back"] = pad.wReleasedButtons & XINPUT_GAMEPAD_BACK;

			m_UpButtons[L"LStick"] = pad.wReleasedButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_UpButtons[L"RStick"] = pad.wReleasedButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_UpButtons[L"L"] = pad.wReleasedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_UpButtons[L"R"] = pad.wReleasedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_UpButtons[L"A"] = pad.wReleasedButtons & XINPUT_GAMEPAD_A;
			m_UpButtons[L"B"] = pad.wReleasedButtons & XINPUT_GAMEPAD_B;
			m_UpButtons[L"X"] = pad.wReleasedButtons & XINPUT_GAMEPAD_X;
			m_UpButtons[L"Y"] = pad.wReleasedButtons & XINPUT_GAMEPAD_Y;
		}

		// NowUpdateButton
		{
			m_NowUpdateButtons[L"DUp"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_NowUpdateButtons[L"DDown"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_NowUpdateButtons[L"DLeft"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_NowUpdateButtons[L"DRight"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_NowUpdateButtons[L"Start"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_START;
			m_NowUpdateButtons[L"Back"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_BACK;

			m_NowUpdateButtons[L"LStick"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_NowUpdateButtons[L"RStick"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_NowUpdateButtons[L"L"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_NowUpdateButtons[L"R"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_NowUpdateButtons[L"A"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_A;
			m_NowUpdateButtons[L"B"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_B;
			m_NowUpdateButtons[L"X"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_X;
			m_NowUpdateButtons[L"Y"] = pad.wNowUpdateButtons & XINPUT_GAMEPAD_Y;
		}

		// LastButton
		{
			m_LastButtons[L"DUp"] = pad.wLastButtons & XINPUT_GAMEPAD_DPAD_UP;
			m_LastButtons[L"DDown"] = pad.wLastButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			m_LastButtons[L"DLeft"] = pad.wLastButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			m_LastButtons[L"DRight"] = pad.wLastButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

			m_LastButtons[L"Start"] = pad.wLastButtons & XINPUT_GAMEPAD_START;
			m_LastButtons[L"Back"] = pad.wLastButtons & XINPUT_GAMEPAD_BACK;

			m_LastButtons[L"LStick"] = pad.wLastButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			m_LastButtons[L"RStick"] = pad.wLastButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

			m_LastButtons[L"L"] = pad.wLastButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			m_LastButtons[L"R"] = pad.wLastButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

			m_LastButtons[L"A"] = pad.wLastButtons & XINPUT_GAMEPAD_A;
			m_LastButtons[L"B"] = pad.wLastButtons & XINPUT_GAMEPAD_B;
			m_LastButtons[L"X"] = pad.wLastButtons & XINPUT_GAMEPAD_X;
			m_LastButtons[L"Y"] = pad.wLastButtons & XINPUT_GAMEPAD_Y;
		}
	}

	// 破棄
	void InputManager::Discard()
	{

	}
}
//end basecross
