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
			m_Sphere = AddGameObject<MoveSphere>(2.0f,Vec3(0.0f,0.0f,1.0f));
			m_Sphere->SetPosition(Vec3(0.0f,0.0f,0.0f));
			m_Sphere->SetVelocity(Vec3(1.0f, 0.0f, 0.0f));
			
			m_StageMap = AddGameObject<Map>();
			m_StageMap->Load();

			Vec3 mapSize = m_StageMap->GetMapSize();

			m_Sphere->SetMoveArea(AABB(Vec3(-1.0f, -100.0f, -1.0f), Vec3(mapSize.x, 5.0f, mapSize.y)));
			m_Sphere->SetUpdateActive(false);

			auto camera = static_pointer_cast<MainCamera>(GetView()->GetTargetCamera());
			camera->SetFixedPoint(m_StageMap);

			m_Hand = AddGameObject<GimmickHand>();
			/*m_Hand->Add(AddGameObject<Gimmicks>(Gimmicks::Objects::Goal));
			m_Hand->Add(AddGameObject<Gimmicks>(Gimmicks::Objects::SetPlayer));
			m_Hand->Add(AddGameObject<Gimmicks>(Gimmicks::Objects::Upper));
			m_Hand->Add(AddGameObject<Gimmicks>(Gimmicks::Objects::Upper));
			m_Hand->Add(AddGameObject<Gimmicks>(Gimmicks::Objects::Upper));*/
			//AddGameObject<Sprite>(L"TEMP_GIMMICK", Vec3(0.0f,-200.0f,0.0f), Vec2(200, 300));
		}
		catch (...) {
			throw;
		}
	}

	void MStage::OnUpdate() {
		auto device = App::GetApp()->GetInputDevice().GetControlerVec()[0];
		if (device.bConnected) {
			if (device.wPressedButtons & XINPUT_GAMEPAD_B) {
				m_Sphere->SetUpdateActive(true);
			}
		}
		if (m_Hand->IsEmpty()) {
			m_Sphere->SetUpdateActive(true);
		}
	}

}
//end basecross
