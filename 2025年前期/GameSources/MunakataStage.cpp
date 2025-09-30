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
		app->RegisterTexture(L"TEMP_GIMMICK_GOAL", uiPath + L"testGoal.png");
		app->RegisterTexture(L"TEMP_GIMMICK_PLAYER", uiPath + L"testSetPl.png");
		app->RegisterTexture(L"TEMP_GIMMICK_UPPER", uiPath + L"testUpper.png");
		app->RegisterTexture(L"TEMP_GIMMICK_COURSE", uiPath + L"testCoruse.png");
		app->RegisterTexture(L"TEMP_GOAL_SPRITE", uiPath + L"testGoalTxt.png");
	}
	void MStage::OnCreate() {
		try {
			GameManager::GetInstance().Reset();
			GameManager::GetInstance().SetGameStage(GetThis<GameStage>());
			GameStage::OnCreate();

			CreateViewLight();
			CreateResorce();
			GameManager::GetInstance().DrawGoalEffect();

			auto player = AddGameObject<MoveCube>();
			player->SetPosition(Vec3(0.0f,0.0f,0.0f));
			player->SetVelocity(Vec3(0.0f, 0.0f, -1.0f));
			player->SetMoveSec(1.0f);
			
			auto stageMap = AddGameObject<Map>();
			stageMap->Load();

			Vec3 mapSize = stageMap->GetMapSize();

			player->SetMoveArea(AABB(Vec3(-1.0f, -100.0f, 0.0f), Vec3(mapSize.x - 1, 5.0f, mapSize.y)));
			//player->SetUpdateActive(false);

			auto camera = static_pointer_cast<MainCamera>(GetView()->GetTargetCamera());
			camera->SetFixedPoint(stageMap);

			auto hand = AddGameObject<GimmickHand>();
			hand->SetCardSize(Vec2(200, 300));
			hand->Add(GimmickObjects::Goal);
			hand->Add(GimmickObjects::SetPlayer);

			GameManager::GetInstance().SetHand(hand);
			GameManager::GetInstance().SetMap(stageMap);
		}
		catch (...) {
			throw;
		}
	}

	void MStage::OnUpdate() {
		GameManager::GetInstance().Update();

		if (GameManager::GetInstance().IsClear()) {
			auto device = App::GetApp()->GetInputDevice().GetControlerVec()[0];

			if (device.bConnected) {

				if (device.wPressedButtons & XINPUT_GAMEPAD_X) {
					PostEvent(0.0f, nullptr, App::GetApp()->GetScene<Scene>(), L"ToGameStage");
				}
			}
		}
	}

}
//end basecross
