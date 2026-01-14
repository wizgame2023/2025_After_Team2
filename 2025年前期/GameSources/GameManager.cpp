/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	void EntityManager::Update() {
		auto& levelManager = GameManager::GetInstance().GetLevelManager();

		auto& map = levelManager->GetMap();
		for (auto& gimmick : map->GetGimmicks()) {
			gimmick->Update();
		}

		bool isOver = true;
		for (auto& player : m_Players) {
			//一つでも動けるキューブがいたらfalse
			if (player->CheckArea()) isOver = false;
			player->Move();
		}
		if (isOver) {
			auto stage = GameManager::GetInstance().GetGameStage();
			for (auto& player : m_Players) {
				stage->RemoveGameObject<MoveCube>(player);
			}
			m_Players.clear();
			GameManager::GetInstance().GetFlowManager()->GameOver();
		}
	}
	bool EntityManager::IsUpdate() {
		for (auto& player : m_Players) {
			if (player->IsEffecting()) return false;
		}
		return true;
	}
	void EntityManager::StartPlayer() {
		for (auto& player : m_Players) {
			player->SetUpdateActive(true);
		}
	}
	void EntityManager::StopPlayer() {
		for (auto& player : m_Players) {
			player->SetUpdateActive(false);
		}
	}
	void EntityManager::DestroyPlayer(shared_ptr<MoveCube>& player) {
		auto it = find(m_Players.begin(), m_Players.end(), player);
		if (it != m_Players.end()) {
			m_Players.erase(it);
			auto stage = GameManager::GetInstance().GetGameStage();
			stage->RemoveGameObject<MoveCube>(player);
		}
	}
	void GameFlowManager::Update() {
		m_Tick += App::GetApp()->GetElapsedTime();
		float updateTick = m_UpdateTicks / m_Rate;
		if (m_Tick <= updateTick) return;

		auto& entityManager = GameManager::GetInstance().GetEntityManager();

		if (!entityManager->IsUpdate()) return;
		m_Tick = 0;

		entityManager->Update();
	}
	void GameFlowManager::GameClear() {
		m_GameState = GameState::Clear;
	}
	void GameFlowManager::GameOver() {
		m_GameState = GameState::Over;

		GameManager::GetInstance().RestartGame();
	}
	void GameFlowManager::GameRestart() {
		m_GameState = GameState::Put;
	}
	void GameFlowManager::GameStart() {
		m_GameState = GameState::Game;
	}

	void LevelManager::Update() {
		//前のフレームから消えた物を削除
		for (int i = 0; i < m_BeforePairs.size(); i++) {
			if (find(
				m_CurrentPairs.begin(), m_CurrentPairs.end(),
				m_BeforePairs[i]) == m_CurrentPairs.end()) {

				m_Map->RecoverGimmick(m_BeforePairs[i].first);
			}
		}
		//前のフレームから消えたものを追加
		for (int i = 0; i < m_CurrentPairs.size(); i++) {
			if (find(
				m_BeforePairs.begin(), m_BeforePairs.end(),
				m_CurrentPairs[i]) == m_BeforePairs.end()) {

				m_Map->PutGimmick(m_CurrentPairs[i].first, m_Hand->Get(m_CurrentPairs[i].second));
			}
		}
		m_BeforePairs = m_CurrentPairs;
	}
	void LevelManager::Load(const wstring& key) {
		m_MapFile.Load(L"Level/" + key + L".json");

		auto gameStage = GameManager::GetInstance().GetGameStage();
		auto map = gameStage->AddGameObject<Map>();
		map->Load(m_MapFile);

		m_Map = map;

		auto hand = gameStage->AddGameObject<GimmickHand>();

		hand->LoadHands(m_MapFile.At<JsonArray>(L"items"));

		m_Hand = hand;
	}

	Vec3 LevelManager::GetMapSize() { return m_Map->GetMapSize(); }
	bool LevelManager::IsStart(){ return m_CurrentPairs.size() == m_Map->GetColorTable().size(); }

	void LevelManager::AddPair(pair<int, int>& gimmickPair) {
		RemovePair(gimmickPair.first, gimmickPair.second);
		m_CurrentPairs.push_back(gimmickPair);
	}
	void LevelManager::RemovePair(pair<int, int>& gimmickPair) {
		stack<vector<pair<int, int>>::iterator> eraseIterators;
		for (auto& it = m_CurrentPairs.begin(); it != m_CurrentPairs.end(); it++) {
			if (*it == gimmickPair) {
				eraseIterators.push(it);
			}
		}
		while (!eraseIterators.empty()) {
			m_CurrentPairs.erase(eraseIterators.top());
			eraseIterators.pop();
		}
	}
	void LevelManager::RemovePair(int color, int gimmick) {
		stack<vector<pair<int, int>>::iterator> eraseIterators;
		for (auto& it = m_CurrentPairs.begin(); it != m_CurrentPairs.end(); it++) {
			auto& pair = (*it);
			if (pair.first == color || pair.second == gimmick) {
				eraseIterators.push(it);
			}
		}
		while (!eraseIterators.empty()) {
			m_CurrentPairs.erase(eraseIterators.top());
			eraseIterators.pop();
		}
	}


	void GameManager::Reset() {
		m_LevelManager = make_shared<LevelManager>();
		m_GameFlowManager = make_shared<GameFlowManager>();
		m_EntityManager = make_shared<EntityManager>();

		m_GameFlowManager->SetTickRate(1.0f);
		m_GameFlowManager->SetUpdateTick(0.5f);

		m_Stage = nullptr;
		m_IsFading = false;

		m_DirectionMap[L"south"] = Vec3(0, 0, -1);
		m_DirectionMap[L"north"] = Vec3(0, 0, 1);
		m_DirectionMap[L"east"] = Vec3(1, 0, 0);
		m_DirectionMap[L"west"] = Vec3(-1, 0, 0);

		m_KeyConfigFile.Load(L"Json/keyconfig.json");

		m_StarSp.clear();
		m_EvaluationSp.clear();

		m_GameEvaluation = 3;
		m_CurrentStarIndex = 0;
	}

	void GameManager::ResultUpdate()
	{
		if (!m_GameFlowManager->IsClear()) return;

		m_ResultTime += App::GetApp()->GetElapsedTime();

		if (m_ResultTime >= 0.05f)
		{
			ResultCreate();
			m_ResultTime = 0.0f;
		}
	}


	void GameManager::StartFade() {
		if (!m_SpriteFade) {
			auto sprite = m_MenuStage->AddGameObject<Sprite>(L"MENU",Vec3(),Vec2(),Anchor::Center);
			sprite->MatchToScreenSize();
			sprite->SetDiffuse(Col4(0, 0, 0, 1));
			sprite->SetLayer(10);
			m_SpriteFade = sprite->AddComponent<SpriteFade>(1.0f);
		}
		m_SpriteFade->StartFade(FadeState::OutToIn);
		m_IsFading = true;
	}
	void GameManager::RestartGame(bool isAll) {
		StartFade();

		auto& map = m_LevelManager->GetMap();
		for (auto& gimmick : map->GetGimmicks()) {
			gimmick->Reset();
		}
	}
	void GameManager::Start() {
		auto& map = m_LevelManager->GetMap();
		for (auto& gimmick : map->GetGimmicks()) {
			auto draw = gimmick->GetComponent<SmBaseDraw>();
			auto color = draw->GetDiffuse();
			color.w = 1.0f;
			draw->SetDiffuse(color);
			gimmick->Begin();
		}
		m_EntityManager->StartPlayer();
		m_GameFlowManager->GameStart();
	}
	void GameManager::Update() {
		m_KeyConfigFile.Load(L"Json/keyconfig.json");
		InputUpdate();
		ResultUpdate();
		m_LevelManager->Update();

		if (m_IsFading) {
			if (m_SpriteFade->IsFinish()) {
				m_IsFading = false;
				m_GameFlowManager->GameRestart();
			}
		}
		//ここから下はゲーム進行中の処理
		if (!m_GameFlowManager->IsGame())return;

		m_GameFlowManager->Update();
	}

	void GameManager::InputUpdate() {

		auto& input = InputManager::GetInputManager();
		if (m_GameFlowManager->IsFinished()) {
			if (input->GetDownButton(GetKeyConfig(L"restart"))) {
				int number = m_LevelManager->GetStageNumber();
				m_Stage->PostEvent(0.0f, nullptr, App::GetApp()->GetScene<Scene>(), L"ToGameStage", make_shared<int>(number));
			}
		}
		if (input->GetDownButton(GetKeyConfig(L"start"))) {
			if (m_GameFlowManager->IsPut()) {
				if (m_LevelManager->IsStart()) {
					Start();
				}
			}
		}

		if (!m_GameFlowManager->IsGame()) return;

		if (input->GetButton(GetKeyConfig(L"fastMove"))) {
			m_GameFlowManager->SetTickRate(3.0f);
		}
		else {
			m_GameFlowManager->SetTickRate(1.0f);
		}

	}

	void GameManager::DrawGoalEffect() {
		if (!m_GameFlowManager->IsGame()) return;

		m_EntityManager->StopPlayer();

		auto gameObjectVec = m_Stage->GetGameObjectVec();

		for (auto& obj : gameObjectVec)
		{
			auto effect = dynamic_pointer_cast<Effect>(obj);

			if (!effect) continue;

			if (effect->GetEffectName() == L"GoalGimmickEffect.efk")
			{

				if (effect->EffectEnd())
				{
					auto menu = m_Stage->GetChileStageVec()[0];
					auto backBoardUI = menu->AddGameObject<Sprite>(L"ResultBackBoardUI", Vec3(0.0f, 0.0f, 0.0f), Vec2(600, 600), Anchor::Center);
					backBoardUI->SetLayer(10);
					auto starCoverUI = menu->AddGameObject<Sprite>(L"StarCoverUI", Vec3(0.0f, 0.0f, 0.0f), Vec2(600, 180), Anchor::Center);
					starCoverUI->SetLayer(10);

					m_EffectSprite.push_back(backBoardUI);
					m_EffectSprite.push_back(starCoverUI);
					effect->EffectDelete();
					m_GameFlowManager->GameClear();
					break;
				}
			}
		}
		//SoundManager::GetInstance().PlaySE(L"Clear");
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
				starUI->SetLayer(11);

				auto evaluationUI = menu->AddGameObject<Sprite>(L"EvaluationUI", Vec3(0.0f, 0.0f, 0.0f), Vec2(500.0f, 78.0f), Anchor::Top);
				evaluationUI->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
				evaluationUI->CreateAnimationUV(Vec2(1, 3));
				evaluationUI->SetLayer(11);

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
