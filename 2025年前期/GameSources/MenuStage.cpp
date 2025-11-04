#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void MenuStage::CreateViewLight() {
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
	void MenuStage::CreateResorce() {
		auto& app = App::GetApp();

		wstring path = app->GetDataDirWString();
		wstring uiPath = path + L"UI/";
		wstring texPath = path + L"Texture/";
	
		app->RegisterTexture(L"TEMP_COLOR_PALETTE", uiPath + L"tempColorPalette.png");

	}

	void MenuStage::OnCreate() {
		try {
			CreateViewLight();
			CreateResorce();
			AddGameObject<ButtonManager>();

			auto viewport = App::GetApp()->GetScene<Scene>()->GetViewport();
			m_Menu = AddGameObject<GameMenu>(L"TEMP_COLOR_PALETTE", L"MENU", viewport);
		}
		catch (...) {
			throw;
		}
	}
	void MenuStage::OnUpdate() {

	}
}