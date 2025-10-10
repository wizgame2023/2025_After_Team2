/*!
@file TitleStage.cpp
@brief タイトルステージの実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	タイトルステージ実体
	//--------------------------------------------------------------------------------------
	void TitleStage::CreateViewLight()
	{
		const Vec3 eye(0.0f, 5.0f, -5.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	void TitleStage::SpriteCreate()
	{
		auto& app = App::GetApp();

		wstring path = app->GetDataDirWString();
		wstring uiPath = path + L"UI/";

		app->RegisterTexture(L"TitleLogoUI", uiPath + L"TitleLogo.png");
		app->RegisterTexture(L"StartUI", uiPath + L"Start.png");


		auto titleSp = AddGameObject<Sprite>(L"TitleLogoUI", Vec3(0.0f), Vec2(1280, 300), Anchor::Bottom);
		auto startSp = AddGameObject<Sprite>(L"StartUI", Vec3(0.0f,-100.0f,0.0f), Vec2(256, 64), Anchor::Center);

	}

	void TitleStage::OnCreate()
	{
		try {
			//ビューとライトの作成
			CreateViewLight();
			SpriteCreate();
		}
		catch (...) {
			throw;
		}
	}

	void TitleStage::OnUpdate()
	{
		auto input = App::GetApp()->GetInputDevice().GetControlerVec()[0];

		if (input.wPressedButtons & XINPUT_GAMEPAD_A)
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToSelectStage");
		}
	}
}
//end basecross
