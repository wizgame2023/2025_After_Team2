/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void GameManager::DrawTempGimmicks() {
		
	}

	void GameManager::Start() {
		for (auto& mapVec : m_Map->GetMapData()) {
			for (auto& map : mapVec) {
				if (map.m_TempGimmick) {
					map.m_TempGimmick->Begin();
				}
			}
		}
		for (auto& sphere : m_Cubes) {
			//プレイヤーを稼働開始
			sphere->SetUpdateActive(true);
		}
		m_GameState = GameState::Game;
	}
	void GameManager::Update() {
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
				if (m_Hand->IsEmpty()) {
					GameManager::GetInstance().Start();
				}
			}
			if (device.wPressedButtons & XINPUT_GAMEPAD_B) {
				m_Hand->Add(m_Map->RecoverGimmick());
			}			
		}



		//ここから下はゲーム進行中の処理
		if (!CompareState(GameState::Game)) return;

		m_Tick += App::GetApp()->GetElapsedTime();
		if (m_Tick <= m_UpdateTicks) return;

		m_Tick = 0;
		for (auto& mapVec : m_Map->GetMapData()) {
			for (auto& map : mapVec) {
				if (map.m_TempGimmick) {
					map.m_TempGimmick->Update();
				}
			}
		}
		//ゲームオーバー判定
		bool isOver = true;
		for (auto& cube : m_Cubes) {
			//一つでも動けるキューブがいたらfalse
			if (cube->CheckArea()) isOver = false;
			cube->Move();
		}

		if (isOver) DrawOverEffect();
	}

	void GameManager::DrawGoalEffect() {
		if (!CompareState(GameState::Game)) return;
		DeleteEffectSprite();
		auto sprite = m_Stage->AddGameObject<Sprite>(L"TEMP_GOAL_SPRITE",Vec3(), Vec2(500, 250), Anchor::Center);
		m_EffectSprite.push_back(sprite);
		m_GameState = GameState::Clear;

		for (auto& cube : m_Cubes) {
			cube->SetUpdateActive(false);
		}
	}
	void GameManager::DrawOverEffect() {
		if (!CompareState(GameState::Game)) return;
		DeleteEffectSprite();
		auto sprite = m_Stage->AddGameObject<Sprite>(L"TEMP_OVER_SPRITE", Vec3(), Vec2(500, 250), Anchor::Center);
		m_EffectSprite.push_back(sprite);
		m_GameState = GameState::Over;

		for (auto& cube : m_Cubes) {
			cube->SetUpdateActive(false);
		}
	}
}
//end basecross
