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

		if (isOver) {
			m_GameState = GameState::Over;
			RestartGame();
		}
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


	void GameManager::StartFade() {
		if (!m_SpriteFade) {
			auto sprite = m_MenuStage->AddGameObject<Sprite>(L"TEMP_OVER_SPRITE",Vec3(),Vec2(),Anchor::Center);
			sprite->MatchToScreenSize();
			sprite->SetDiffuse(Col4(0, 0, 0, 1));
			m_SpriteFade = sprite->AddComponent<SpriteFade>(1.0f);
		}
		m_SpriteFade->StartFade(FadeState::OutToIn);
		m_IsFading = true;
	}
	void GameManager::RestartGame(bool isAll) {
		StartFade();
		//m_GameState = GameState::Put;
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
		ResultUpdate();
		MapUpdate();

		if (m_IsFading) {
			if (m_SpriteFade->IsFinish()) {
				m_IsFading = false;
				m_GameState = GameState::Put;
			}
		}
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

				evaluationUI->SetPosition(Vec3(0.0f, -80.0f - 60.0f * i, 0.0f));
				auto uv = evaluationUI->GetUv(i);
				evaluationUI->UpdateUV(uv);
				m_EvaluationSp.push_back(evaluationUI);
				m_EffectSprite.push_back(evaluationUI);

			}


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
