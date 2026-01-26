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

	void SelectStage::CreateButton(wstring wss)
	{
		ButtonManager::instance->SetInput(wss, InputData(XINPUT_GAMEPAD_DPAD_UP, -3));
		ButtonManager::instance->SetInput(wss, InputData(XINPUT_GAMEPAD_DPAD_DOWN, 3));
		ButtonManager::instance->SetInput(wss, InputData(XINPUT_GAMEPAD_DPAD_LEFT, -1));
		ButtonManager::instance->SetInput(wss, InputData(XINPUT_GAMEPAD_DPAD_RIGHT, 1));
		ButtonManager::instance->SetInput(wss, InputData(StickMode::LY, -3, 0.8f));
		ButtonManager::instance->SetInput(wss, InputData(StickMode::LX, 1, 0.8f));
		ButtonManager::instance->AddAcceptButton(wss, XINPUT_GAMEPAD_A);

	}

	void SelectStage::SpriteCreate()
	{
		auto backBoardSp = AddGameObject<Sprite>(L"SelectBackGround", Vec3(0.0f), Vec2(1280, 800), Anchor::Center);
		auto selectSp = AddGameObject<Sprite>(L"SelectUI", Vec3(-640.0f, 400.0f, 0.0f), Vec2(256, 64), Anchor::TopLeft);

		m_BackBoardSp = AddGameObject<Sprite>(L"BackBoardUI", Vec3(0.0f), Vec2(1280, 800), Anchor::Center);
		m_BackBoardSp->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 0.0f));
		m_BackBoardSp->SetLayer(3);

		m_RollSpRight = AddGameObject<Sprite>(L"RollUI", Vec3(650.0f, 650.0f, 0.0f), Vec2(900, 900), Anchor::Center);
		m_RollSpLeft = AddGameObject<Sprite>(L"RollUI", Vec3(-750.0f, -610.0f, 0.0f), Vec2(900, 900), Anchor::Center);
		m_RollSpRight->VectorToward(Vec2(cos(m_Angle100), sin(XM_PI + (m_Angle100))));

		m_StageNum = 18;        // 表示する数字の数

		NumSpriteCreate(2, 3, 100, 100, -250, 200);
	}

	void SelectStage::NumSpriteCreate(int pageNum,int maxRow,float width, float Height,float x,float y)
	{
		AddGameObject<ButtonManager>();

		int maxPerRow = maxRow;         // 1行に表示する最大数
		float baseWidth = width;
		float baseHeight = Height;
		float spacing = 70.0f;
		float rowSpacing = 70.0f;
		float centerX = x;
		float centerY = y;
		float totalWidth = maxPerRow * baseWidth + (maxPerRow - 1) * spacing;
		float startX = centerX - totalWidth / 2.0f;

		for (int page = 0; page < pageNum; page++)
		{
			wstring groupName = L"SELECT" + to_wstring(page);

			CreateButton(groupName);

			for (int i = 0; i < m_StageNum/ pageNum; i++)
			{
				int row = i / maxPerRow;
				int col = i % maxPerRow;

				float offsetX = (baseWidth + spacing) * col;
				float offsetY = -(baseHeight + rowSpacing) * row;

				Vec3 position = Vec3(startX + offsetX, centerY + offsetY, 0.0f);
				m_NumPositions.push_back(position);  // 位置を保存
				Vec2 size = Vec2(130, 130);

				ButtonManager::Create(GetThis<Stage>(), groupName, L"SelectCursorUI", Col4(1, 1, 1, 1), position+m_OffsetPos, size * 1.4,
					[i](shared_ptr<ObjectInterface>& object) {
						//i番目のステージに飛ぶ
						auto selectStage = static_pointer_cast<SelectStage>(object);
						selectStage->NextStage(i);
					});
				

				int digit = (i + 1 + (9 * page) >= 10) ? 2 : 1;

				// SpriteCreate
				auto numSp = AddGameObject<NumberSprite>(L"NumUI", position, size, digit);
				numSp->SetUpdateActive(false);
				numSp->UpdateNumber(i + 1 + (9 * page));
				auto sprites = numSp->GetNumberSprites();

				auto underline = AddGameObject<Sprite>
					(
						L"Underline",
						static_cast<Vec3>(numSp->GetNumberSprites()[0]->GetAnchorPosition(Anchor::Bottom) + Vec2(45.0f* page, 30.0f)),
						size, Anchor::Center
					);


				for (auto& sprite : sprites)
				{
					ButtonManager::instance->AddFrontSprite(groupName, i, sprite);
					ButtonManager::instance->AddFrontSprite(groupName, i, underline);
				}
				// リストに追加
				m_NumSpList.push_back(numSp);

			}
		}
		ButtonManager::instance->CloseAll();
		m_Page = 0;
		ButtonManager::instance->OpenAndUse(L"SELECT" + to_wstring(m_Page));
	}


	void SelectStage::OnCreate()
	{
		try {

			//ビューとライトの作成
			CreateViewLight();
			Json();
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


		if (input->GetDownButton(L"B") && !m_IsButton)
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


		if (input->GetDownButton(L"R") && m_Page < 1)
		{
			m_Page++;
			ButtonManager::instance->CloseAll();
			ButtonManager::instance->OpenAndUse(L"SELECT" + to_wstring(m_Page));
		}
		else if (input->GetDownButton(L"L") && m_Page > 0)
		{
			m_Page--;
			ButtonManager::instance->CloseAll();
			ButtonManager::instance->OpenAndUse(L"SELECT" + to_wstring(m_Page));
		}

		//auto scene = App::GetApp()->GetScene<Scene>();

		//if (!m_IsAnser)
		//{
		//	shared_ptr<Sprite> numbers[2];
		//	Vec3 cursorPos = m_BackSp->GetPosition();
		//	Vec3 numberPos = cursorPos + Vec3(0.0f, -120, 0.0f);
		//	float numberSize = 70;
		//	int maxCount = scene->GetAnserCount(m_Count);
		//	int currentCount = scene->GetClearPath(m_Count).size();

		//	wstring maxCountKey = L"ICON_" + to_wstring(maxCount);
		//	wstring currentCountKey = L"ICON_" + to_wstring(currentCount);

		//	numbers[0] = AddGameObject<Sprite>(maxCountKey, numberPos, Vec2(numberSize), Anchor::Center);
		//	numbers[1] = AddGameObject<Sprite>(currentCountKey, numberPos + Vec3(-numberSize / 2.0f, numberSize / 2.0f, 0.0f), Vec2(numberSize), Anchor::Center);
		//	auto stick = AddGameObject<Sprite>(L"ICON_Stick", numberPos, Vec2(numberSize), Anchor::Center);

		//	numbers[0]->SetLayer(10);
		//	numbers[1]->SetLayer(10);




		//	m_IsAnser = true;
		//}

	}

	void SelectStage::NextStage(int num)
	{
		m_Count = num;
		m_StartSE = SoundManager::GetInstance().PlaySE(L"selectDecision");
		m_IsButton = true;
		m_IsNextStage = true;
		StartFade();
	}
	void SelectStage::StartFade()
	{
		m_BackBoardSp->AddComponent<SpriteFade>(0.7f);
		m_BackBoardSp->GetComponent<SpriteFade>()->StartFade(FadeState::Out);
	}

}
//end basecross
