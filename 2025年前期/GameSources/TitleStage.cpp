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

	void TitleStage::Json()
	{
		ResourceManager::Load(L"titleResource.json");

		ResourceManager::RegisterTexture(L"UI");
	}


	void TitleStage::SpriteCreate()
	{
		Json();
		FadeStarSpriteCreate();

		auto titleSp = AddGameObject<Sprite>(L"TitleLogoUI", Vec3(0.0f,-130,0), Vec2(1000, 500), Anchor::Bottom);
		auto starBackSp = AddGameObject<Sprite>(L"StarBackGroundUI", Vec3(0.0f), Vec2(1280, 800), Anchor::Center);
		m_Cursor = AddGameObject<Sprite>(L"MOUSE_CURSOR", Vec3(0.0f, -100.0f, 0.0f), Vec2(30, 30), Anchor::Center);
		m_Cursor->SetLayer(3);
		StartSpriteCreate();

		m_RollSp = AddGameObject<Sprite>(L"RollUI", Vec3(0.0f, -650.0f, 0.0f), Vec2(900, 900), Anchor::Center);

		auto starSize = Vec2(1300, 1300);
		m_RollStarSp = AddGameObject<Sprite>(L"ShootingStarUI", Vec3(150, -40, 0.0f), starSize, Anchor::Center);
		m_RollStarSp2 = AddGameObject<Sprite>(L"ShootingStarUI", Vec3(200,  0, 0.0f), starSize - Vec2(200), Anchor::Center);
		m_RollStarSp3 = AddGameObject<Sprite>(L"ShootingStarUI", Vec3(150, -80, 0.0f), starSize - Vec2(100), Anchor::Center);

	}

	void TitleStage::StartSpriteCreate()
	{

		auto palette1 = AddGameObject<Sprite>(L"A_ButtonUI", Vec3(-200.0f, -150.0f, 0.0f), Vec2(200, 125), Anchor::Center);
		palette1->SetLayer(2);
		m_ColorPalettes.push_back(palette1);
		auto palette2 = AddGameObject<Sprite>(L"StinkUI", Vec3(200.0f, -150.0f, 0.0f), Vec2(200, 125), Anchor::Center);
		palette2->SetLayer(2);
		m_ColorPalettes.push_back(palette2);

		auto colorSize = Vec2(50, 50);
		//for (int i = 0; i < 2; i++)
		//{
		//	int xPos = (i == 0) ? -150.0f : 150.0f;
		//	int handleIndex = (i == 0) ? 0 : -1;

		//	auto colorPalette = AddGameObject<Sprite>(L"ColorPaletteUI", Vec3(xPos, -150.0f, 0.0f), colorSize, Anchor::Center);
		//	colorPalette->SetLayer(2);
		//	m_ColorPalettes.push_back(colorPalette);

		//	if (i==0)
		//	{
		//		auto handle = AddGameObject<Sprite>(
		//			L"HANDLER",
		//			static_cast<Vec3>(m_ColorPalettes[i]->GetAnchorPosition(Anchor::Right) + handleIndex),
		//			colorSize * 0.5, Anchor::Center);

		//		handle->SetLayer(1);

		//		m_Handlers.push_back(handle);
		//	}
		//	else
		//	{
		//		auto handle = AddGameObject<Sprite>(
		//			L"HANDLER",
		//			static_cast<Vec3>(m_ColorPalettes[i]->GetAnchorPosition(Anchor::Left) + handleIndex),
		//			colorSize * 0.5, Anchor::Center);

		//		handle->SetLayer(1);

		//		m_Handlers.push_back(handle);
		//	}
		//}

	}

	void TitleStage::FadeStarSpriteCreate()
	{
		m_FadeStarRedSp = AddGameObject<Sprite>(L"FadeRedStarUI", Vec3(0.0f), Vec2(1280, 800), Anchor::Center);
		m_FadeStarBlueSp = AddGameObject<Sprite>(L"FadeBlueStarUI", Vec3(0.0f), Vec2(1280, 800), Anchor::Center);
		m_FadeStarYellowSp = AddGameObject<Sprite>(L"FadeYellowStarUI", Vec3(0.0f), Vec2(1280, 800), Anchor::Center);

		m_FadeStarRedSp->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 0.0f));
		m_FadeStarBlueSp->SetDiffuse(Col4(0.0f, 0.0f, 1.0f, 0.0f));
		m_FadeStarYellowSp->SetDiffuse(Col4(1.0f, 1.0f, 0.0f, 0.0f));

	}

	void TitleStage::OnCreate()
	{
		try {
			//ビューとライトの作成
			CreateViewLight();
			SpriteCreate();

			//auto sprite = AddGameObject<Sprite>(L"StartUI", Vec3(0.0f, -200.0f, 0.0f), Vec2(512, 128), Anchor::Center);
			//sprite->SetAnimationActive(true);
			//sprite->CreateAnimationUV(Vec2(3, 2));
			//sprite->AddAnimation(L"anim1", 0, 5, 0.1f,true);
			//sprite->AddAnimation(L"anim2", { 0,1,2,3,4,5 }, 0.1f, true);

			//sprite->SetCurrentAnimation(L"anim2");

		}
		catch (...) {
			throw;
		}
	}

	void TitleStage::OnUpdate()
	{
		UpdateCursor();
		GameStartMaster();
		UpdateRollSprite();
		FadeStarSprite();

	}

	void TitleStage::UpdateRollSprite()
	{
		m_RollVelocity -= 0.5f;
		if (m_Angle160 + XMConvertToRadians(m_RollVelocity) <= 0.0f)
		{
			m_RollVelocity = 0.0f;
			m_Count++;
		}
		m_RollSp->VectorToward(Vec2(cos(XMConvertToRadians(m_RollVelocity)), sin(XMConvertToRadians(m_RollVelocity))));

		if (m_Count >= 2)
		{
			m_RollStarVelocity -= 0.8f;
			if (XM_PI * 0.5f + XMConvertToRadians(m_RollStarVelocity) <= 0.0f)
			{
				m_RollStarVelocity = 0.0f;

				m_Count = 0;
			}
		}

		m_RollStarSp->VectorToward(Vec2(cos(m_Angle270 + XMConvertToRadians(m_RollStarVelocity)), sin(m_Angle270 + XMConvertToRadians(m_RollStarVelocity))));
		m_RollStarSp2->VectorToward(Vec2(cos((m_Angle270 + XMConvertToRadians(10)) + XMConvertToRadians(m_RollStarVelocity)), sin((m_Angle270 + XMConvertToRadians(10)) + XMConvertToRadians(m_RollStarVelocity))));
		m_RollStarSp3->VectorToward(Vec2(cos((m_Angle270 - XMConvertToRadians(5)) + XMConvertToRadians(m_RollStarVelocity)), sin((m_Angle270 - XMConvertToRadians(5)) + XMConvertToRadians(m_RollStarVelocity))));
	}

	void TitleStage::GameStartMaster()
	{
		HandleAPressed();
		UpdateLine();
		HandleAReleased();
		UpdateStartSpriteBlink();
	}

	void TitleStage::UpdateCursor()
	{
		auto& input = InputManager::GetInputManager();
		Vec2 stick = input->GetLStick();
		if (stick.lengthSqr() > 0.01f)
		{
			auto pos = m_Cursor->GetPosition();
			pos.x += stick.x * 5.0f;
			pos.y += stick.y * 5.0f;
			m_Cursor->SetPosition(pos);
		}
	}

	void TitleStage::HandleAPressed()
	{
		auto& input = InputManager::GetInputManager();
		if (input->GetDownButton(L"A"))
		{
			if (!m_ColorPalettes.empty())
			{
				auto cursorPos = m_Cursor->GetPosition();
				float minDist = FLT_MAX;
				float judgeThreshold = 100.0f;

				bool found = false;

				auto palettePos = m_ColorPalettes[0]->GetPosition();

				float dist = (cursorPos - palettePos).length();

				if (dist < minDist)
				{
					minDist = dist;
					m_StartPos = Vec3(palettePos.x + 50, palettePos.y, palettePos.z);
				}
				if (dist <= judgeThreshold)
				{
					found = true;
				}


				if (found && m_Line == nullptr)
				{
					m_Line = AddGameObject<Sprite>(L"ColorPaletteUI", m_StartPos, Vec2(20, 20), Anchor::Center);
					m_Line->SetLayer(1);
					m_IsAPushed = true;
				}
			}
		}
	}

	void TitleStage::UpdateLine()
	{
		if (!m_IsAPushed || m_Line == nullptr) return;

		Vec3 currentPos = m_Cursor->GetPosition();
		Vec3 dist = currentPos - m_StartPos;

		m_Line->SetSize(Vec2(m_Line->GetSize().x, dist.length()));
		m_Line->SetAnchorPosition(m_StartPos + dist / 2.0f, Anchor::Center);
		m_Line->VectorToward(static_cast<Vec2>(dist.normalize()));

		Vec3 farPalettePos;
		if ((m_StartPos - m_ColorPalettes[0]->GetPosition()).length() <
			(m_StartPos - m_ColorPalettes[1]->GetPosition()).length())
		{
			farPalettePos = m_ColorPalettes[1]->GetPosition();
		}
		else
		{
			farPalettePos = m_ColorPalettes[0]->GetPosition();
		}

		float threshold = 30.0f;
		if ((currentPos - (farPalettePos+Vec3(-30,0,0))).length() <= threshold)
		{
			m_Line->SetSize(Vec2(m_Line->GetSize().x, (farPalettePos - m_StartPos).length()));
			m_Line->SetAnchorPosition(m_StartPos + (farPalettePos - m_StartPos) / 2.0f, Anchor::Center);
			m_Line->VectorToward(static_cast<Vec2>(farPalettePos - m_StartPos).normalize());
			m_IsConfirmed = true;

			if (m_StartSprite == nullptr)
			{
				m_StartSprite = AddGameObject<Sprite>(L"StartUI", Vec3(0.0f, -50.0f, 0.0f), Vec2(512, 128), Anchor::Center);
			}
		}
		else
		{
			RemoveGameObject<Sprite>(m_StartSprite);
			m_StartSprite = nullptr;
			m_IsConfirmed = false;

		if (m_ColorPalettes.size() >= 2)
		{
			}
		}

	}

	void TitleStage::HandleAReleased()
	{
		auto& input = InputManager::GetInputManager();
		if (input->GetUpButton(L"A"))
		{
			if (!m_IsConfirmed)
			{
				RemoveGameObject<Sprite>(m_Line);
				m_Line = nullptr;
				m_IsAPushed = false;
			}
			else
			{
				m_IsAPushed = false;
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToSelectStage");
			}
		}
	}

	void TitleStage::UpdateStartSpriteBlink()
	{
		if (m_IsConfirmed && m_StartSprite != nullptr)
		{
			static float blinkTime = 0.0f;
			blinkTime += App::GetApp()->GetElapsedTime();

			float alpha = 0.5f + 0.3f * sinf(blinkTime * 5.0f);
			m_StartSprite->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, alpha));
		}

	}

	void TitleStage::FadeStarSprite()
	{
		static float blinkTime = 0.0f;
		blinkTime += App::GetApp()->GetElapsedTime();

		float alphaR = 0.3f + 0.3f * sinf(blinkTime * 3.0f);
		float alphaB = 0.3f + 0.3f * sinf(blinkTime * 3.0f - 1.0f);
		float alphaY = 0.3f + 0.3f * sinf(blinkTime * 3.0f - 2.0f);

		m_FadeStarRedSp->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, alphaR));
		m_FadeStarBlueSp->SetDiffuse(Col4(0.0f, 0.0f, 1.0f, alphaB));
		m_FadeStarYellowSp->SetDiffuse(Col4(1.0f, 1.0f, 0.0f, alphaY));
	}
}
//end basecross
