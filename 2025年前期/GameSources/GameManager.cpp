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
	void GameManager::MapUpdate() {
		//前のフレームから消えた物を削除
		for (int i = 0; i < m_BeforeGimmickColorPairs.size(); i++) {
			if (find(
				m_GimmickColorPairs.begin(), m_GimmickColorPairs.end(),
				m_BeforeGimmickColorPairs[i]) == m_GimmickColorPairs.end()) {

				m_Map->RecoverGimmick(m_BeforeGimmickColorPairs[i].first);
			}
		}
		//前のフレームから消えたものを追加
		for (int i = 0; i < m_GimmickColorPairs.size(); i++) {
			if (find(
				m_BeforeGimmickColorPairs.begin(), m_BeforeGimmickColorPairs.end(),
				m_GimmickColorPairs[i]) == m_BeforeGimmickColorPairs.end()) {

				m_Map->PutGimmick(m_GimmickColorPairs[i].first, m_Hand->Get(m_GimmickColorPairs[i].second));
			}
		}
		m_BeforeGimmickColorPairs = m_GimmickColorPairs;
	}
	void GameManager::GimmickUpdate() {
		for (auto& gimmick : m_Map->GetGimmicks()) {
			gimmick->Update();
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

	void GameManager::Start() {
		for (auto& gimmick : m_Map->GetGimmicks()) {
			auto color = gimmick->GetComponent<SmBaseDraw>()->GetDiffuse();
			color.w = 1.0f;
			gimmick->GetComponent<SmBaseDraw>()->SetDiffuse(color);
			gimmick->Begin();
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
		MapUpdate();

		//ここから下はゲーム進行中の処理
		if (!CompareState(GameState::Game))return;

		m_Tick += App::GetApp()->GetElapsedTime();
		float updateTick = m_UpdateTicks / m_TickRate;
		if (m_Tick <= updateTick) return;

		if (!IsUpdate()) return;
		m_Tick = 0;
		
		GimmickUpdate();
		CubeUpdate();
	}

	void GameManager::InputUpdate() {

		auto& input = InputManager::GetInputManager();
		if (CompareState(GameState::Clear) || CompareState(GameState::Over)) {
			if (input->GetDownButton(GetKeyConfig(L"restart"))) {
				m_Stage->PostEvent(0.0f, nullptr, App::GetApp()->GetScene<Scene>(), L"ToGameStage");
			}
		}
		if (input->GetDownButton(GetKeyConfig(L"start"))) {
			if (CompareState(GameState::Put)) {
				if (m_GimmickColorPairs.size() == m_Map->GetColorTable().size()) {
					Start();
				}
			}
		}

		if (!CompareState(GameState::Game)) return;

		if (input->GetButton(GetKeyConfig(L"fastMove"))) {
			m_TickRate = 3.0f;
		}
		else {
			m_TickRate = 1.0f;
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
