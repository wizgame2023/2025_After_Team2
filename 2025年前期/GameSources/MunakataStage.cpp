/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	void MStage::CreateViewLight() {
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
	void MStage::CreateResorce() {
		auto& app = App::GetApp();

		wstring path = app->GetDataDirWString();
		wstring uiPath = path + L"UI/";

		app->RegisterTexture(L"TEMP_GIMMICK", uiPath + L"testGimmick.png");
	}
	void MStage::OnCreate() {
		try {
			GameStage::OnCreate();

			CreateViewLight();
			CreateResorce();
			auto player = AddGameObject<MoveSphere>(2.0f,Vec3(0.0f,0.0f,1.0f));
			player->SetPosition(Vec3(0.0f,0.0f,0.0f));
			player->SetVelocity(Vec3(1.0f, 0.0f, 0.0f));
			
			auto stageMap = AddGameObject<Map>();
			stageMap->Load();

			Vec3 mapSize = stageMap->GetMapSize();

			player->SetMoveArea(AABB(Vec3(-1.0f, -100.0f, -1.0f), Vec3(mapSize.x, 5.0f, mapSize.y)));
			player->SetUpdateActive(false);

			auto camera = static_pointer_cast<MainCamera>(GetView()->GetTargetCamera());
			camera->SetFixedPoint(stageMap);

			auto hand = AddGameObject<GimmickHand>();
			hand->SetCardSize(Vec2(200, 300));
			hand->Add(AddGameObject<Gimmicks>(Gimmicks::Objects::Goal));
			hand->Add(AddGameObject<Gimmicks>(Gimmicks::Objects::SetPlayer));
			hand->Add(AddGameObject<Gimmicks>(Gimmicks::Objects::Upper));
			hand->Add(AddGameObject<Gimmicks>(Gimmicks::Objects::Upper));
			hand->Add(AddGameObject<Gimmicks>(Gimmicks::Objects::Upper));

			GameManager::GetInstance().SetHand(hand);
			GameManager::GetInstance().AddSphere(player);
			GameManager::GetInstance().SetMap(stageMap);
			//AddGameObject<Sprite>(L"TEMP_GIMMICK", Vec3(0.0f,-200.0f,0.0f), Vec2(200, 300));
		}
		catch (...) {
			throw;
		}
	}

	void MStage::OnUpdate() {

	}

}
//end basecross
