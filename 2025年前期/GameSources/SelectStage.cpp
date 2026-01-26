/*!
@file SelectStage.cpp
@brief セレクトステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//セレクトステージクラス実体
	//--------------------------------------------------------------------------------------
	void SelectStage::CreateViewLight()
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

	void SelectStage::Json()
	{
		ResourceManager::Load(L"selectResource.json");

		ResourceManager::RegisterWav(L"Sound");
		ResourceManager::RegisterTexture(L"UI");
	}

	void SelectStage::SpriteCreate()
	{
		Json();

		auto backBoardSp = AddGameObject<Sprite>(L"SelectBackGround", Vec3(0.0f), Vec2(1280, 800), Anchor::Center);
		auto selectSp = AddGameObject<Sprite>(L"SelectUI", Vec3(-640.0f, 400.0f, 0.0f), Vec2(256, 64), Anchor::TopLeft);

		m_BackBoardSp = AddGameObject<Sprite>(L"BackBoardUI", Vec3(0.0f), Vec2(1280, 800), Anchor::Center);
		m_BackBoardSp->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 0.0f));
		m_BackBoardSp->SetLayer(3);

		m_RollSpRight = AddGameObject<Sprite>(L"RollUI", Vec3(600.0f, 600.0f, 0.0f), Vec2(900, 900), Anchor::Center);
		m_RollSpLeft = AddGameObject<Sprite>(L"RollUI", Vec3(-700.0f, -560.0f, 0.0f), Vec2(900, 900), Anchor::Center);
		m_RollSpRight->VectorToward(Vec2(cos(m_Angle100), sin(XM_PI + (m_Angle100))));

		m_StageNum = 10;        // 表示する数字の数
		int maxPerRow = 5;         // 1行に表示する最大数
		float baseWidth = 100.0f;
		float spacing = 100.0f;
		float baseHeight = 100.0f;
		float rowSpacing = 200.0f;

		for (int i = 0; i < m_StageNum; i++)
		{
			int row = i / maxPerRow;
			int col = i % maxPerRow;

			float totalWidth = maxPerRow * baseWidth + (maxPerRow - 1) * spacing;
			float startX = -totalWidth / 2.0f;

			float offsetX = (baseWidth + spacing) * col;
			float offsetY = -(baseHeight + rowSpacing) * row;

			Vec3 position = Vec3(startX + offsetX, 200 + offsetY, 0.0f);
			m_NumPositions.push_back(position);  // 位置を保存
			Vec2 size = Vec2(100, 100);

			int digit = (i + 1 >= 10) ? 2 : 1;

			// SpriteCreate 内
			auto numSp = AddGameObject<NumberSprite>(L"NumUI", position, size, digit);
			numSp->UpdateNumber(i + 1);
			// リストに追加
			m_NumSpList.push_back(numSp);

			if (m_BackSp == nullptr)
			{
				m_BackSp = AddGameObject<Sprite>(L"SelectCursorUI", position + m_OffsetPos, size * 1.6f, Anchor::Center);
				m_BackSp->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 10.0f));
			}

		}
	}



	void SelectStage::OnCreate()
	{
		try {

			//ビューとライトの作成
			CreateViewLight();

			SpriteCreate();

			SoundManager::GetInstance().PlayBGM(L"SelectBGM", 2.0f);

		}
		catch (...) {
			throw;
		}
	}

	void SelectStage::OnUpdate()
	{
		auto& input = InputManager::GetInputManager();

		m_RollVelocity -= 0.5f;

		if (m_IsNextRoll)
		{
			if (m_Angle100 + XMConvertToRadians(m_RollVelocity) <= m_RollSpEndRight)
			{
				m_IsNextRoll = false;
				m_RollVelocity = 0.0f;
			}

			m_RollSpLeft->VectorToward(Vec2(cos(m_Angle270 + XMConvertToRadians(m_RollVelocity)), sin(m_Angle270 + XMConvertToRadians(m_RollVelocity))));


		}
		else if (!m_IsNextRoll)
		{
			if (m_Angle100 + XMConvertToRadians(m_RollVelocity) <= m_RollSpEndRight)
			{
				m_IsNextRoll = true;
				m_RollVelocity = 0.0f;
			}

			m_RollSpRight->VectorToward(Vec2(cos(m_Angle100 + XMConvertToRadians(m_RollVelocity)), sin(m_Angle100 + XMConvertToRadians(m_RollVelocity))));

		}


		if (input->GetDownButton(L"A") && !m_IsButton)
		{
			m_StartSE = SoundManager::GetInstance().PlaySE(L"selectDecision");
			m_IsButton = true;
			StartFade();
		}
		else if (input->GetDownButton(L"B") && !m_IsButton)
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
		}
		else
		{
			m_IsButton = false;
		}

		if (m_StartSE != nullptr)
		{
			if (m_BackBoardSp->GetComponent<SpriteFade>()->IsFinish())
			{
				shared_ptr<int> index = make_shared<int>(m_Count);
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage", index);
			}
		}


		if (input->GetLStick().x == 0.0f) {
			m_IsStick = false;
		}

		if (fabs(input->GetLStick().x) > 0.2f)
		{
			m_StickHoldTime += App::GetApp()->GetElapsedTime();
			if (!m_IsStick || m_StickHoldTime > 0.3f)
			{
				if (input->GetLStick().x > 0.2f)
				{
					m_Count++;
				}
				else {
					m_Count--;
				}

				// 範囲チェック
				if (m_Count < 0) m_Count = m_StageNum - 1;
				if (m_Count >= m_StageNum) m_Count = 0;

				auto targetPos = m_NumPositions[m_Count];
				m_BackSp->SetPosition(targetPos+ m_OffsetPos);


				m_IsStick = true;
				m_StickHoldTime = 0.0f; // 次の移動までの待ち時間
			}
		}
		else 
		{
			m_IsStick = false;
			m_StickHoldTime = 0.0f;
		}


	}

	void SelectStage::StartFade()
	{
		m_BackBoardSp->AddComponent<SpriteFade>(0.7f);
		m_BackBoardSp->GetComponent<SpriteFade>()->StartFade(FadeState::Out);
	}

}
//end basecross
