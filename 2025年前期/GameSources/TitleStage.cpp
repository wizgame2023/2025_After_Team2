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
		auto titleSp = AddGameObject<Sprite>(L"TitleLogoUI", Vec3(0.0f), Vec2(1000, 500), Anchor::Bottom);
		m_StartSprite = AddGameObject<Sprite>(L"StartUI", Vec3(0.0f,-100.0f,0.0f), Vec2(512, 128), Anchor::Center);

	}

	void TitleStage::OnCreate()
	{
		try {
			//ビューとライトの作成
			CreateViewLight();
			SpriteCreate();

			/*auto sprite = AddGameObject<Sprite>(L"StartUI", Vec3(0.0f, -200.0f, 0.0f), Vec2(512, 128), Anchor::Center);
			auto sprite = AddGameObject<Sprite>(L"StartUI", Vec3(0.0f, -20.0f, 0.0f), Vec2(102.4, 128), Anchor::Center);
			sprite->SetAnimationActive(true);
			sprite->CreateAnimationUV(Vec2(5, 1));
			sprite->AddAnimation(L"anim1", 0, 4, 0.1f,true);
			sprite->AddAnimation(L"anim2", { 0,1,2,3,4,5 }, 0.1f, true);

			sprite->SetCurrentAnimation(L"anim2");*/

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


		m_Fade += 0.06f;
		Col4 col = m_StartSprite->GetDiffuse();
		col.w = (sinf(m_Fade) + 1.0f) / 2.0f;
		m_StartSprite->SetDiffuse(col);

	}
}
//end basecross
