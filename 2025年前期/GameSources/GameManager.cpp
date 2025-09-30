/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void GameManager::Update() {
		for (auto& sphere : m_Balls) {
			//プレイヤーを稼働開始
			sphere->SetUpdateActive(m_Hand->IsEmpty());
			sphere->GetComponent<PNTStaticDraw>()->SetDrawActive(m_Hand->IsEmpty());
		}
		auto device = App::GetApp()->GetInputDevice().GetControlerVec()[0];

		if (device.bConnected) {
			if (device.wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
				m_Hand->Back();
			}
			if (device.wPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
				m_Hand->Next();
			}
			if (device.wPressedButtons & XINPUT_GAMEPAD_A && !m_Map->CheckPutGimmick()) {
				m_Map->PutGimmick(m_Hand->Use());
			}
			if (device.wPressedButtons & XINPUT_GAMEPAD_B) {
				m_Hand->Add(m_Map->RecoverGimmick());
			}			
		}

		m_Tick += App::GetApp()->GetElapsedTime();
		if (m_Tick <= m_UpdateTicks) return;

		m_Tick = 0;
		for (auto& cube : m_Balls) {
			cube->Move();
		}
	}

	void GameManager::DrawGoalEffect() {
		auto sprite = m_Stage->AddGameObject<Sprite>(L"TEMP_GOAL_SPRITE",Vec3(), Vec2(500, 250), Anchor::Center);
		m_EffectSprite.push_back(sprite);
		m_IsGameClear = true;

		for (auto& cube : m_Balls) {
			cube->SetUpdateActive(false);
		}
	}
}
//end basecross
