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
	
		app->RegisterTexture(L"TEMP_MENU", uiPath + L"tempMenuBackGround.png");
		app->RegisterTexture(L"TEMP_COLOR_PALETTE", uiPath + L"tempColorPalette.png");

	}

	void MenuStage::OnCreate() {
		try {
			CreateViewLight();
			//Stage::OnCreate();
			CreateResorce();
			/*auto view = dynamic_pointer_cast<SingleView>(GetView());
			Viewport viewport = view->GetTargetViewport();
			viewport.Height;
			viewport.Width /= 1.5f;
			viewport.TopLeftY = viewport.Height / 6.0f;*/

			auto viewport = App::GetApp()->GetScene<Scene>()->GetViewport();
			m_Menu = AddGameObject<Menu>(L"TEMP_COLOR_PALETTE", L"TEMP_MENU", viewport);
		}
		catch (...) {
			throw;
		}
	}
	void MenuStage::OnUpdate() {

	}
}