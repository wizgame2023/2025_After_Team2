/*!
@file InputManager.h
@brief ���̓}�l�[�W���[
@authors Kato Yuki
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class InputManager
	{
	private:
		struct InputManagerDeleter
		{
			void operator()(InputManager* p) { delete p; }
		};
		static unique_ptr<InputManager, InputManagerDeleter> m_InputManager;

		InputManager();
		virtual ~InputManager() {}

		/*
			L"DUp"    XINPUT_GAMEPAD_DPAD_UP
			L"DDown"  XINPUT_GAMEPAD_DPAD_DOWN
			L"DLeft"  XINPUT_GAMEPAD_DPAD_LEFT
			L"DRight" XINPUT_GAMEPAD_DPAD_RIGHT

			L"Start"  XINPUT_GAMEPAD_START
			L"Back"   XINPUT_GAMEPAD_BACK

			L"LStick" XINPUT_GAMEPAD_LEFT_THUMB
			L"RStick" XINPUT_GAMEPAD_RIGHT_THUMB

			L"L"      XINPUT_GAMEPAD_LEFT_SHOULDER
			L"R"      XINPUT_GAMEPAD_RIGHT_SHOULDER

			L"A"      XINPUT_GAMEPAD_A
			L"B"      XINPUT_GAMEPAD_B
			L"X"      XINPUT_GAMEPAD_X
			L"Y"      XINPUT_GAMEPAD_Y
		*/

		Vec2 m_LStick;
		Vec2 m_RStick;
		map<wstring, bool> m_Buttons;
		map<wstring, bool> m_DownButtons;
		map<wstring, bool> m_UpButtons;
		map<wstring, bool> m_NowUpdateButtons;
		map<wstring, bool> m_LastButtons;

	public:
		/*!
		@brief ���̓}�l�[�W���[���쐬
		@return �쐬�������̓}�l�[�W���[
		*/
		static unique_ptr<InputManager, InputManagerDeleter>& CreateInputManager();

		/*!
		@brief ���̓}�l�[�W���[���擾
		@return ���̓}�l�[�W���[
		*/
		static unique_ptr<InputManager, InputManagerDeleter>& GetInputManager();

		/*!
		@brief ���̓}�l�[�W���[���폜
		@return �Ȃ�
		*/
		static void DeleteInputManager();

		/*!
		@brief ���̓}�l�[�W���[��������
		@return �Ȃ�
		*/
		void Initialize();

		/*!
		@brief ���̓}�l�[�W���[���X�V
		@return �Ȃ�
		*/
		void Update();

		/*!
		@brief ���̓}�l�[�W���[��j��
		@return �Ȃ�
		*/
		void Discard();

		/*!
		@brief L�X�e�B�b�N�̌X�����擾
		@return L�X�e�B�b�N�̌X��
		*/
		Vec2 GetLStick()
		{
			return m_LStick;
		}

		/*!
		@brief R�X�e�B�b�N�̌X�����擾
		@return R�X�e�B�b�N�̌X��
		*/
		Vec2 GetRStick()
		{
			return m_RStick;
		}

		/*!
		@brief �{�^���̏�Ԃ��擾
		@param[in] button �擾�������{�^���̖��O
		@return �{�^���̏��
		*/
		bool GetButton(wstring button)
		{
			auto it = m_Buttons.find(button);
			if (it != m_Buttons.end())
			{
				return it->second;
			}

			return false;
		}

		/*!
		@brief �{�^���̉�����Ԃ��擾
		@param[in] button �擾�������{�^���̖��O
		@return �{�^���̉������
		*/
		bool GetDownButton(wstring button)
		{
			auto it = m_DownButtons.find(button);
			if (it != m_DownButtons.end())
			{
				return it->second;
			}

			return false;
		}

		/*!
		@brief �{�^���̗��E��Ԃ��擾
		@param[in] button �擾�������{�^���̖��O
		@return �{�^���̗��E���
		*/
		bool GetUpButton(wstring button)
		{
			auto it = m_UpButtons.find(button);
			if (it != m_UpButtons.end())
			{
				return it->second;
			}

			return false;
		}

		/*!
		@brief �{�^���̕ύX��Ԃ��擾
		@param[in] button �擾�������{�^���̖��O
		@return �{�^���̕ύX���
		*/
		bool GetNowUpdateButton(wstring button)
		{
			auto it = m_NowUpdateButtons.find(button);
			if (it != m_NowUpdateButtons.end())
			{
				return it->second;
			}

			return false;
		}

		/*!
		@brief �O�t���[���̃{�^���̏�Ԃ��擾
		@param[in] button �擾�������{�^���̖��O
		@return �O�t���[���̃{�^���̏��
		*/
		bool GetLastButton(wstring button)
		{
			auto it = m_LastButtons.find(button);
			if (it != m_LastButtons.end())
			{
				return it->second;
			}

			return false;
		}

	private:
		//�R�s�[�֎~
		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;
		//���[�u�֎~
		InputManager(const InputManager&&) = delete;
		InputManager& operator=(const InputManager&&) = delete;
	};
}
//end basecross
