/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	void GameStage::CreateViewLight() {
		const Vec3 eye(0.0f, 5.0f, -5.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<MainCamera>(XM_PI);
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	void GameStage::CreateResorce() {
		ResourceManager::Load(L"gameResource.json");
		ResourceManager::RegisterTexture(L"Texture");
		ResourceManager::RegisterTexture(L"UI");
	}

	void GameStage::OnCreate() {
		try {
			CreateViewLight();
			CreateResorce();

			//マネージャーの初期化
			auto& gameManager = GameManager::GetInstance();
			gameManager.Reset();
			gameManager.SetGameStage(GetThis<GameStage>());
			auto& levelManager = gameManager.GetLevelManager();
			levelManager->Load(m_StageData->At<JsonString>(L"file")->GetValue());
			levelManager->SetStageNumber(m_StageData->At<JsonNumber>(L"number")->GetIntValue() - 1);
			auto map = levelManager->GetMap();
			auto camera = static_pointer_cast<MainCamera>(GetView()->GetTargetCamera());
			camera->SetFixedPoint(map);

			//ゲーム本体の表示範囲を限定
			auto view = dynamic_pointer_cast<SingleView>(GetView());
			Viewport viewport = view->GetTargetViewport();
			viewport.Width /= 1.5f;
			viewport.TopLeftY = 0;
			view->SetViewport(viewport);

			App::GetApp()->GetScene<Scene>()->SetViewport(viewport);

			//メニュー用のステージを作成
			auto menuStage = AddChileStage<MenuStage>();

			GameManager::GetInstance().SetMenuStage(menuStage);

			SoundManager::GetInstance().PlayBGM(L"GameBGM");


			auto skyCube = AddGameObject<SkyCube>(L"BACKGROUND");
			skyCube->SetPosition(map->GetMapCenter());

			Vec3 mapSize = map->GetMapSize();
			Vec3 mapCenter = map->GetMapCenter();
			float boardDist = 0.75f;

			shared_ptr<Board> directionBoard[4] = {};

			directionBoard[0] = AddGameObject<Board>(L"MAP_ICON_EAST", Vec3(mapSize.x + boardDist, -0.5f, mapCenter.z), Vec3(1, 1, 1), true);// E
			directionBoard[1] = AddGameObject<Board>(L"MAP_ICON_WEST", Vec3(-1.0f - boardDist, -0.5f, mapCenter.z), Vec3(1, 1, 1), true);// W
			directionBoard[2] = AddGameObject<Board>(L"MAP_ICON_NORTH", Vec3(mapCenter.x, -0.5f, 1.0f + boardDist), Vec3(1, 1, 1), true);// N
			directionBoard[3] = AddGameObject<Board>(L"MAP_ICON_SOUTH", Vec3(mapCenter.x, -0.5f, -mapSize.z - boardDist), Vec3(1, 1, 1), true);// S

			shared_ptr<Board> floorBoard = AddGameObject<Board>(L"FLOOR_WIRE", Vec3(mapCenter.x, -0.5f, mapCenter.z), Vec3(mapSize.z, mapSize.x, 1.0f), false);// S

			floorBoard->RotateVector(Vec3(0, 1, 0));
			StartFadeIn(0.75f);

		}
		catch (...) {
			throw;
		}
	}

	void GameStage::OnUpdate() {
		GameManager::GetInstance().Update();
		TutorialManager::GetInstance().Update();
	}
	void GameStage::OnDestroy() {
		SoundManager::GetInstance().StopAll();
	}

	void GameStage::StartFadeIn(float time) {
		if (!m_StageFade) {
			auto sprite = AddGameObject<Sprite>(L"MENU", Vec3(), Vec2(), Anchor::Center);
			sprite->MatchToScreenSize();
			sprite->SetDiffuse(Col4(0, 0, 0, 1));
			sprite->SetLayer(11);
			m_StageFade = sprite->AddComponent<SpriteFade>(time);
		}
		m_StageFade->StartFade(FadeState::In);
	}
	void GameStage::StartFadeOut(float time) {
		if (!m_StageFade) {
			auto sprite = AddGameObject<Sprite>(L"MENU", Vec3(), Vec2(), Anchor::Center);
			sprite->MatchToScreenSize();
			sprite->SetDiffuse(Col4(0, 0, 0, 1));
			sprite->SetLayer(11);
			m_StageFade = sprite->AddComponent<SpriteFade>(time);
		}
		m_StageFade->StartFade(FadeState::Out);

	}
}
//end basecross
