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

	void GameManager::ResultUpdate()
	{
		if (!CompareState(GameState::Clear)) return;

		m_ResultTime += App::GetApp()->GetElapsedTime();

		if (m_ResultTime >= 0.05f)
		{
			ResultCreate();
			m_ResultTime = 0.0f;
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
		ResultUpdate();
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

		m_GameState = GameState::Clear;

		auto menu = m_Stage->GetChileStageVec()[0];

		auto& app = App::GetApp();

		auto backBoardUI = menu->AddGameObject<Sprite>(L"ResultBackBoardUI", Vec3(0.0f, 0.0f, 0.0f), Vec2(600, 600), Anchor::Center);
		auto starCoverUI = menu->AddGameObject<Sprite>(L"StarCoverUI", Vec3(0.0f, 0.0f, 0.0f), Vec2(600, 180), Anchor::Center);
		m_EffectSprite.push_back(backBoardUI);
		m_EffectSprite.push_back(starCoverUI);

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


	void GameManager::ResultCreate()
	{
		if (m_StarSp.size() == 0)
		{
			auto menu = m_Stage->GetChileStageVec()[0];
			for (int i = 0; i < 3; i++)
			{
				auto starUI = menu->AddGameObject<Sprite>(L"StarUI", Vec3(-200, 0.0f, 0.0f), Vec2(200, 180), Anchor::Center);
				starUI->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 0.0f));
				starUI->CreateAnimationUV(Vec2(3, 1));

				auto evaluationUI = menu->AddGameObject<Sprite>(L"EvaluationUI", Vec3(0.0f, 0.0f, 0.0f), Vec2(500.0f, 78.0f), Anchor::Top);
				evaluationUI->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
				evaluationUI->CreateAnimationUV(Vec2(1, 3));

				if (i == 1)
				{
					starUI->SetPosition(Vec3(200, 0.0f, 0.0f));
					auto uv = starUI->GetUv(2);
					starUI->UpdateUV(uv);
					m_StarSp.push_back(starUI);
					m_EffectSprite.push_back(starUI);
				}

				else if (i == 2)
				{
					starUI->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
					auto uv = starUI->GetUv(1);
					starUI->UpdateUV(uv);
					m_StarSp.push_back(starUI);
					m_EffectSprite.push_back(starUI);
				}
				else
				{
					auto uv = starUI->GetUv(i);
					starUI->UpdateUV(uv);
					m_StarSp.push_back(starUI);
					m_EffectSprite.push_back(starUI);
				}

				evaluationUI->SetPosition(Vec3(0.0f, 80.0f - 60.0f * i, 0.0f));
				auto uv = evaluationUI->GetUv(i);
				evaluationUI->UpdateUV(uv);
				m_EvaluationSp.push_back(evaluationUI);
				m_EffectSprite.push_back(evaluationUI);

			}


			m_CurrentStarIndex = 0;

		}
		if (m_CurrentStarIndex >= m_StarSp.size()) return;

		if (m_GameEvaluation <= 0) return;

		auto star = m_StarSp[m_CurrentStarIndex];
		Col4 current = star->GetDiffuse();
		float alpha = current.getW();
		Vec2 currentSize = star->GetSize();

		// フェードイン
		if (alpha < 1.0f)
		{
			alpha += 0.15f;
			if (alpha > 1.0f) alpha = 1.0f;
			star->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, alpha));

			m_EvaluationSp[m_CurrentStarIndex]->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			m_CurrentStarIndex++;
			m_GameEvaluation--;
		}
	}

}
//end basecross
