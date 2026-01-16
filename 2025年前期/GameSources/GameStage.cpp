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


			auto skyCube = AddGameObject<SkyCube>(L"FLOOR");
			skyCube->SetPosition(map->GetMapCenter());

			Vec3 mapSize = map->GetMapSize();
			Vec3 mapCenter = map->GetMapCenter();
			float boardDist = 0.75f;

			shared_ptr<Board> directionBoard[4] = {};
			directionBoard[0] = AddGameObject<Board>(L"ICON_ARROW", Vec3(mapSize.x + boardDist, -0.5f, mapCenter.z), Vec3(1, 1, 1), false);// E
			directionBoard[1] = AddGameObject<Board>(L"ICON_GOAL", Vec3(-1.0f - boardDist, -0.5f, mapCenter.z), Vec3(1, 1, 1), false);// W
			directionBoard[2] = AddGameObject<Board>(L"ICON_INV", Vec3(mapCenter.x, -0.5f, 1.0f + boardDist), Vec3(1, 1, 1), false);// N
			directionBoard[3] = AddGameObject<Board>(L"ICON_PL", Vec3(mapCenter.x, -0.5f, -mapSize.z - boardDist), Vec3(1, 1, 1), false);// S

			for (auto& board : directionBoard) {
				Vec3 position = board->GetTrans()->GetPosition();
				board->RotateVector(mapCenter - position);
			}
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
}
//end basecross
