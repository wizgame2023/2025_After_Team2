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


			auto& gameManager = GameManager::GetInstance();
			gameManager.Reset();
			gameManager.SetGameStage(GetThis<GameStage>());
			auto& levelManager = gameManager.GetLevelManager();
			levelManager->Load(m_StageFilename);

			//auto mapJson = levelManager->GetJson();
			//auto mapJson = GameManager::GetInstance().LoadStage(L"TestMap");


			/*auto stageMap = AddGameObject<Map>();
			stageMap->Load();*/

			auto camera = static_pointer_cast<MainCamera>(GetView()->GetTargetCamera());
			camera->SetFixedPoint(levelManager->GetMap());

			/*auto hand = AddGameObject<GimmickHand>();
			hand->SetCardSize(Vec2(200, 300));*/

			//hand->LoadHands(mapJson.At<JsonArray>(L"items"));

			//GameManager::GetInstance().SetHand(hand);
			//GameManager::GetInstance().SetMap(stageMap);
			auto view = dynamic_pointer_cast<SingleView>(GetView());
			Viewport viewport = view->GetTargetViewport();
			viewport.Height;
			viewport.Width /= 1.5f;
			viewport.TopLeftY = 0;
			view->SetViewport(viewport);

			App::GetApp()->GetScene<Scene>()->SetViewport(viewport);

			auto menuStage = AddChileStage<MenuStage>();

			GameManager::GetInstance().SetMenuStage(menuStage);

			SoundManager::GetInstance().PlayBGM(L"GameBGM");

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
