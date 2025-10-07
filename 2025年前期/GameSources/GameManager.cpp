/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	bool GameManager::IsUpdate() {
		for (auto& cube : m_Cubes) {
			if (cube->IsEffecting()) {
				return false;
			}
		}
		return true;
	}
	void GameManager::GimmickUpdate() {
		for (auto& mapVec : m_Map->GetMapData()) {
			for (auto& map : mapVec) {
				if (map.m_Gimmik) {
					map.m_Gimmik->Update();
				}
			}
		}
	}
	void GameManager::CubeUpdate() {
		//ゲームオーバー判定
		bool isOver = true;
		for (auto& cube : m_Cubes) {
			//一つでも動けるキューブがいたらfalse
			if (cube->CheckArea()) isOver = false;
			cube->Move();
		}

		if (isOver) DrawOverEffect();
	}
	void GameManager::StopCube() {
		for (auto& cube : m_Cubes) {
			cube->SetUpdateActive(false);
		}
	}

	void GameManager::DrawTempGimmicks() {
		
	}

	void GameManager::Start() {
		for (auto& mapVec : m_Map->GetMapData()) {
			for (auto& map : mapVec) {
				if (map.m_Gimmik) {
					map.m_Gimmik->Begin();
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
		m_KeyConfigFile.Load(L"Json/keyconfig.json");
		InputUpdate();
		
		//ここから下はゲーム進行中の処理
		if (!CompareState(GameState::Game)) return;

		m_Tick += App::GetApp()->GetElapsedTime();
		if (m_Tick <= m_UpdateTicks) return;

		if (!IsUpdate()) return;
		m_Tick = 0;
		
		GimmickUpdate();
		CubeUpdate();
	}

	void GameManager::InputUpdate() {

		auto& input = InputManager::GetInputManager();
		if (input->GetDownButton(GetKeyConfig(L"toolBack"))) {
			m_Hand->Back();
		}
		if (input->GetDownButton(GetKeyConfig(L"toolNext"))) {
			m_Hand->Next();
		}
		if (input->GetDownButton(GetKeyConfig(L"putGimmick")) && !m_Map->CheckPutGimmick())
		{
			m_Map->PutGimmick(m_Hand->Use());
			if (m_Hand->IsEmpty()) {
				GameManager::GetInstance().Start();
			}
		}
		if (input->GetDownButton(GetKeyConfig(L"recoverGimmick")) && m_Map->CheckPutGimmick()) {
			m_Hand->Add(m_Map->RecoverGimmick());
		}
		if (CompareState(GameState::Clear) || CompareState(GameState::Over)) {
			if (input->GetDownButton(GetKeyConfig(L"restart"))) {
				m_Stage->PostEvent(0.0f, nullptr, App::GetApp()->GetScene<Scene>(), L"ToGameStage");
			}
		}
	}

	void GameManager::DrawGoalEffect() {
		if (!CompareState(GameState::Game)) return;
		DeleteEffectSprite();
		auto sprite = m_Stage->AddGameObject<Sprite>(L"TEMP_GOAL_SPRITE",Vec3(), Vec2(500, 250), Anchor::Center);
		m_EffectSprite.push_back(sprite);
		m_GameState = GameState::Clear;

		StopCube();
	}
	void GameManager::DrawOverEffect() {
		if (!CompareState(GameState::Game)) return;
		DeleteEffectSprite();
		auto sprite = m_Stage->AddGameObject<Sprite>(L"TEMP_OVER_SPRITE", Vec3(), Vec2(500, 250), Anchor::Center);
		m_EffectSprite.push_back(sprite);
		m_GameState = GameState::Over;

		StopCube();
	}
}
//end basecross
