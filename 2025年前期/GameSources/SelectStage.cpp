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

	void SelectStage::CreateJson()
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
		m_BackBoardSp->SetLayer(15);

		m_RollSpRight = AddGameObject<Sprite>(L"RollUI", Vec3(650.0f, 650.0f, 0.0f), Vec2(900, 900), Anchor::Center);
		m_RollSpLeft = AddGameObject<Sprite>(L"RollUI", Vec3(-750.0f, -610.0f, 0.0f), Vec2(900, 900), Anchor::Center);
		m_RollSpRight->VectorToward(Vec2(cos(m_Angle100), sin(XM_PI + (m_Angle100))));

		auto L = AddGameObject<Sprite>(L"L", Vec3(-565.0f, -40.0f, 0.0f), Vec2(65), Anchor::Center);
		auto R = AddGameObject<Sprite>(L"R", Vec3(15.0f, -40.0f, 0.0f), Vec2(65), Anchor::Center);
		L->SetDiffuse(Col4(0.0f));
		m_stickSp.push_back(L);
		m_stickSp.push_back(R);

		auto stageJson = Json(L"Json/stage.json");
		auto dataArray = stageJson.At<JsonArray>(L"tutorial")->GetObjectArray();

		m_StageNum = dataArray.size();        // 表示する数字の数

		NumSpriteCreate(9, 3, 100, 100, -300, 200);
		StageSp();
		AnserSp();
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

		m_TotalPage = m_StageNum / pageNum;

		if (m_StageNum % pageNum > 0)
		{
			m_TotalPage++;
		}

		for (int page = 0; page < m_TotalPage; page++)
		{
			wstring groupName = L"SELECT" + to_wstring(page);

			CreateButton(groupName);

			for (int i = 0; i < pageNum; i++)
			{
				int num = i + 1 + (pageNum * page);

				if (num > m_StageNum) break;

				int row = i / maxPerRow;
				int col = i % maxPerRow;

				float offsetX = (baseWidth + spacing) * col;
				float offsetY = -(baseHeight + rowSpacing) * row;

				Vec3 position = Vec3(startX + offsetX, centerY + offsetY, 0.0f);
				m_NumPositions.push_back(position);  // 位置を保存
				Vec2 size = Vec2(130, 130);


				ButtonManager::Create(GetThis<Stage>(), groupName, L"SelectCursorUI", Col4(1, 1, 1, 1), position + m_OffsetPos, size * 1.4,
					[num](shared_ptr<ObjectInterface>& object)
					{
						//i番目のステージに飛ぶ
						auto selectStage = static_pointer_cast<SelectStage>(object);
						selectStage->NextStage(num - 1);
					});
				

				int digit = (num >= 10) ? 2 : 1;
				int set = (num >= 10) ? 1 : 0;

				// SpriteCreate
				auto numSp = AddGameObject<NumberSprite>(L"NumUI", position, size, digit);
				numSp->SetUpdateActive(false);
				numSp->UpdateNumber(num);
				auto sprites = numSp->GetNumberSprites();

				auto underline = AddGameObject<Sprite>
					(
						L"Underline",
						static_cast<Vec3>(numSp->GetNumberSprites()[0]->GetAnchorPosition(Anchor::Bottom) + Vec2(45.0f * set, 30.0f)),
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
	void SelectStage::StageSp()
	{

		auto stageJson = Json(L"Json/stage.json");

		auto dataArray = stageJson.At<JsonArray>(L"tutorial")->GetObjectArray();

		for (int i = 0; i < dataArray.size(); i++)
		{
			auto data = dataArray[i];
			wstring key = data->At<JsonString>(L"texture")->GetValue();
			auto stageSp = AddGameObject<Sprite>(key, Vec3(325.0f, 75.0f, 0.0f), Vec2(580 * 0.65f, 335 * 0.75), Anchor::Center);
			m_StageSp.push_back(stageSp);
		}

	}

	void SelectStage::AnserSp()
	{
		auto scene = App::GetApp()->GetScene<Scene>();
		Vec3 numberPos = Vec3(420.0f, -200, 0.0f);
		float numberSize = 150;

		for (int i = 0; i < m_StageNum; i++)
		{
			vector <shared_ptr<Sprite>> numbers;
			int maxCount = scene->GetAnserCount(i);
			int currentCount = scene->GetClearPath(i).size();
			wstring maxCountKey = L"ICON_" + to_wstring(maxCount);
			wstring currentCountKey = L"ICON_" + to_wstring(currentCount);

			auto num = AddGameObject<Sprite>(maxCountKey, numberPos, Vec2(numberSize), Anchor::Center);
			auto numMax = AddGameObject<Sprite>(currentCountKey, numberPos + Vec3(-numberSize / 2.0f, numberSize / 2.0f, 0.0f), Vec2(numberSize), Anchor::Center);

			numbers.push_back(num);
			numbers.push_back(numMax);
			m_AnserSp.push_back(numbers);

		}
		auto stick = AddGameObject<Sprite>(L"ICON_Stick", numberPos, Vec2(numberSize), Anchor::Center);
		auto anser = AddGameObject<Sprite>(L"anserUI", Vec3(200, -130, 0.0f), Vec2(150, 50), Anchor::Center);

	}

	void SelectStage::OnCreate()
	{
		try {

			//ビューとライトの作成
			CreateViewLight();
			CreateJson();
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


		if (input->GetDownButton(L"R") && m_Page < m_TotalPage - 1)
		{
			m_Page++;

			m_stickSp[0]->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
			if (m_Page == m_TotalPage - 1)
			{
				m_stickSp[1]->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 0.0f));
			}

			ButtonManager::instance->CloseAll();
			ButtonManager::instance->OpenAndUse(L"SELECT" + to_wstring(m_Page));
		}
		else if (input->GetDownButton(L"L") && m_Page > 0)
		{
			m_Page--;

			m_stickSp[1]->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
			if (m_Page == 0)
			{
				m_stickSp[0]->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 0.0f));
			}

			ButtonManager::instance->CloseAll();
			ButtonManager::instance->OpenAndUse(L"SELECT" + to_wstring(m_Page));
		}

		auto index = ButtonManager::instance->GetSelectIndex(L"SELECT" + to_wstring(m_Page));
		index = index + (m_Page * 9);
		for (int i = 0; i < m_StageSp.size(); i++)
		{
			m_StageSp[i]->SetDrawActive(index == i);
			for (int j = 0; j < 2; j++)
			{
				m_AnserSp[i][j]->SetDrawActive(index == i);
			}
		}


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
