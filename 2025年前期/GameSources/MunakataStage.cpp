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

	void MStage::OnCreate() {
		try {
			GameStage::OnCreate();

			CreateViewLight();
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
			//GetView()->GetTargetCamera()->SetEye(Vec3(2.5f, 2.5f, -10.0f));
			//GetView()->GetTargetCamera()->SetAt(Vec3(2.5f, -5.0f, 2.5f));

			auto a = AddGameObject<TempBox>(m_StageMap->GetMapCenter(), Col4(1, 1, 1, 1));
			a->SetScale(Vec3(0.1f));
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
	}

}
//end basecross
