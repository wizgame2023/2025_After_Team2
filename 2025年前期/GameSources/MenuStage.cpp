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

			auto step = TutorialStep(GetThis<Stage>(), { L"TUTORIAL_INFO_1",L"TUTORIAL_INFO_2" }, Vec3(-100.0f, 200.0f, 0.0f), 600, 300);
			TutorialManager::GetInstance().RegisterStep(L"GAME_START",make_shared<TutorialStep>(step));


			TutorialManager::GetInstance().Start(L"GAME_START");
			StartFadeIn(0.75f);
		}
		catch (...) {
			throw;
		}
	}
	void MenuStage::OnUpdate() {

	}

	void MenuStage::StartFadeIn(float time) {
		if (!m_StageFade) {
			auto sprite = AddGameObject<Sprite>(L"MENU", Vec3(), Vec2(), Anchor::Center);
			sprite->MatchToScreenSize();
			sprite->SetDiffuse(Col4(0, 0, 0, 1));
			sprite->SetLayer(11);
			m_StageFade = sprite->AddComponent<SpriteFade>(time);
		}
		m_StageFade->StartFade(FadeState::In);
	}
	void MenuStage::StartFadeOut(float time) {
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