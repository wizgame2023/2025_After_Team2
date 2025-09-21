/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void GameManager::Update() {
		if (m_Hand->IsEmpty()) {
			for (auto& sphere : m_Spheres) {
				//プレイヤーを稼働開始
				sphere->SetUpdateActive(true);
			}
		}
		auto device = App::GetApp()->GetInputDevice().GetControlerVec()[0];

		if (device.bConnected) {
			if (device.wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
				m_Hand->Back();
			}
			if (device.wPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
				m_Hand->Next();
			}
			if (device.wPressedButtons & XINPUT_GAMEPAD_A) {
				m_Map->PutGimmick(m_Hand->Use());
			}
			if (device.wPressedButtons & XINPUT_GAMEPAD_B) {
				m_Hand->Add(m_Map->RecoverGimmick());
			}
		}
	}
}
//end basecross
