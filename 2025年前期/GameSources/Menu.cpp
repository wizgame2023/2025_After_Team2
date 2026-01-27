/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void GameMenu::OnCreate() {

		Vec2 mainViewSize = Vec2(m_MainViewPort.Width, m_MainViewPort.Height);
		Vec2 screenSize = Vec2(App::GetApp()->GetGameWidth(), App::GetApp()->GetGameHeight());

		Vec2 fullMenuSize = Vec2(screenSize.x - mainViewSize.x, screenSize.y);
		Vec2 gameMenuSize = Vec2(fullMenuSize.x, fullMenuSize.y * 0.75f);
		Vec2 explainMenuSize = Vec2(fullMenuSize.x, fullMenuSize.y - gameMenuSize.y);

		Vec3 gameMenuTopLeft = Vec3(screenSize.x / 2.0f - fullMenuSize.x, fullMenuSize.y / 2.0f, 0);

		m_BackGround = m_Stage->AddGameObject<Sprite>(m_BackGroundTexture, gameMenuTopLeft, gameMenuSize, Anchor::TopLeft);
		m_BackGround->SetLayer(0);

		m_ExplainBox = m_Stage->AddGameObject<Sprite>(L"MENU_EXPLAIN", Vec3(), explainMenuSize, Anchor::TopLeft);
		m_ExplainBox->SetLayer(0);
		m_ExplainBox->SetAnchorPosition(static_cast<Vec3>(m_BackGround->GetAnchorPosition(Anchor::BottomLeft)), Anchor::TopLeft);

		m_CurrentExplain = m_Stage->AddGameObject<Sprite>(L"EXPAIN_PL", m_ExplainBox->GetPosition() + Vec3(10,-10,0), explainMenuSize * 0.9f, Anchor::TopLeft);
		m_CurrentExplain->SetLayer(1);

		float paletteStartPositionY = 70.0f;
		Vec2 colorPalettePosition = Vec2(gameMenuTopLeft.x + 100.0f, gameMenuTopLeft.y - paletteStartPositionY);
		Vec2 gimmickPosition = Vec2(gameMenuTopLeft.x + fullMenuSize.x - 150.0f, gameMenuTopLeft.y - paletteStartPositionY);
		Vec2 iconSize = Vec2(50.0f, 50.0f);

		float durationY = 65.0f;
		m_ConnectOffsetX = iconSize.x / 2.0f;

		m_OperateInfo = m_Stage->AddGameObject<Sprite>(L"OPERATE_INFO", Vec3(), Vec2(fullMenuSize.y * 0.3f, fullMenuSize.y * 0.3f) * 0.9f, Anchor::Center);
		m_OperateInfo->SetAnchorPosition(static_cast<Vec3>(m_BackGround->GetAnchorPosition(Anchor::TopLeft)) - Vec3(20.0f,10.0f,0.0f), Anchor::TopRight);

		auto& gameManager = GameManager::GetInstance();

		auto colorTable = gameManager.GetLevelManager()->GetMap()->GetColorTable();
		Json colorJson = Json(L"Json/color.json");

		for (int i = 0; i < colorTable.size(); i++){
			auto colorAry = colorJson.At<JsonArray>(colorTable[i])->GetFloatArray();
			auto color = Col4(colorAry[0], colorAry[1], colorAry[2], 1.0f);

			auto sprite = m_Stage->AddGameObject<Sprite>(
				m_ColorTexture,
				Vec3(colorPalettePosition.x, colorPalettePosition.y - i * durationY,0.0f), iconSize,
				Anchor::Center);
			sprite->SetDiffuse(color);
			sprite->SetLayer(2);

			auto handler = m_Stage->AddGameObject<Sprite>(
				L"HANDLER",
				static_cast<Vec3>(sprite->GetAnchorPosition(Anchor::Right)), iconSize / 2.0f,
				Anchor::Center);
			handler->SetLayer(1);

			m_ColorPalette.push_back(sprite);
			m_HandlerSprites.push_back(handler);
		}


		auto cards = gameManager.GetLevelManager()->GetHand()->GetCardData();
		for (int i = 0; i < cards.size(); i++) {
			auto sprite = m_Stage->AddGameObject<Sprite>(
				cards[i]->GetIconKey(),
				Vec3(gimmickPosition.x, gimmickPosition.y - i * durationY, 0.0f), iconSize,
				Anchor::Center);
			sprite->SetLayer(2);
			auto explain = m_Stage->AddGameObject<Sprite>(
				L"ICON_EXPLAIN_FRAME",
				Vec3(gimmickPosition.x + iconSize.x, gimmickPosition.y - i * durationY, 0.0f), iconSize,
				Anchor::Center);
			explain->SetLayer(2);
			explain->SetDiffuse(Col4(0, 0, 0, 1));

			auto draw = explain->GetComponent<PCTSpriteDraw>();
			wstring direction = gameManager.DirectionVecToStr(cards[i]->GetVelocity());
			if (!direction.empty()) {
				transform(direction.begin(), direction.end(), direction.begin(), ::toupper);
				wstring direKey = L"ICON_" + direction;
				draw->AddTextureResource(direKey);
			}
			else {
				draw->AddTextureResource(L"ICON_NO_DIRE");
			}
			int count = cards[i]->GetCount();
			if (count > 0) {
				draw->AddTextureResource(L"ICON_" + to_wstring(count));
			}
			else {
				draw->AddTextureResource(L"ICON_NO_NUMBER");
			}

			auto handler = m_Stage->AddGameObject<Sprite>(
				L"HANDLER",
				static_cast<Vec3>(sprite->GetAnchorPosition(Anchor::Left)), iconSize / 2.0f,
				Anchor::Center);
			handler->SetLayer(1);
			m_GimmickIcons.push_back(sprite);
			m_HandlerSprites.push_back(handler);
			m_ExplainIcons.push_back(explain);
		}

		m_Cursor = m_Stage->AddGameObject<Cursor>(L"MOUSE_CURSOR");
		m_Cursor->SetCoursorSize(25.0f);
		m_Cursor->SetMoveSpeed(450.0f);
		m_Cursor->SetMoveArea(m_BackGround->GetAnchorPosition(Anchor::TopRight), m_ExplainBox->GetAnchorPosition(Anchor::TopLeft));
	
		Vec2 poseMenuButtonSize = Vec2(200.0f, 60.0f);
		m_PoseMenu = m_Stage->AddGameObject<PoseMenu>(GetThis<GameMenu>(),gameMenuTopLeft, fullMenuSize, poseMenuButtonSize);
		
		auto soundMenu = m_Stage->AddGameObject<SoundMenu>(gameMenuTopLeft, fullMenuSize);
		soundMenu->Close();
		m_PoseMenu->SetSoundMenu(soundMenu);
		m_PoseMenu->Close();
	}
	void GameMenu::OnUpdate() {
		auto& input = InputManager::GetInputManager();
		auto& gameManager = GameManager::GetInstance();

		if (gameManager.GetFlowManager()->IsPut() && !m_PoseMenu->IsOpen()) {
			m_Cursor->SetUpdateActive(true);
		}
		else {
			m_Cursor->SetUpdateActive(false);
			return;
		}
		if (TutorialManager::GetInstance().IsActive()) return;
		
		if (input->GetDownButton(gameManager.GetKeyConfig(L"undo")) && m_Lines.size() > 0) {
			auto line = m_Lines.back();
			m_Lines.pop_back();
			m_Stage->RemoveGameObject<Sprite>(line.m_Line);
			gameManager.GetLevelManager()->RemovePair(line.m_PairHandle);
		}
		if (input->GetDownButton(gameManager.GetKeyConfig(L"putGimmick"))) {
			UpdateOnCoursorHandle();
		}
		if (input->GetDownButton(gameManager.GetKeyConfig(L"openPose"))) {
			SetDrawActive(false);
		}
		if (input->GetLStick().lengthSqr() > 0.01f) {
			m_IsCursor = true;
		}

		if (input->GetDownButton(L"DUp")) {
			m_IsCursor = false;
			if (m_CursorHandle > 0) m_CursorHandle--;
		}
		if (input->GetDownButton(L"DDown")) {
			m_IsCursor = false;
			if (m_CursorHandle < m_ColorPalette.size() * 2 - 1) m_CursorHandle++;
		}
		if (input->GetDownButton(L"DRight")) {
			m_IsCursor = false;
			if (m_CursorHandle < m_ColorPalette.size()) m_CursorHandle += m_ColorPalette.size();
		}
		if (input->GetDownButton(L"DLeft")) {
			m_IsCursor = false;
			if (m_CursorHandle > m_ColorPalette.size() - 1) m_CursorHandle -= m_ColorPalette.size();
		}
		if (!m_IsCursor) {
			int index = m_CursorHandle % m_ColorPalette.size();
			int handleNum = m_CursorHandle / m_ColorPalette.size();

			if (handleNum == 0) {
				m_Cursor->SetPosition(m_ColorPalette[index]->GetPosition());
			}
			else {
				m_Cursor->SetPosition(m_GimmickIcons[index]->GetPosition());
			}
		}
		

		if (m_ColorHandle != -1 || m_GimmickHandle != -1) {
			if (!m_CurrentLine)return;

			Vec3 coursorPosition = m_Cursor->GetPosition();
			Vec3 handlePosition;
			if (m_ColorHandle != -1) {
				handlePosition = m_ColorPalette[m_ColorHandle]->GetAnchorPosition(Anchor::Right);
				int handle = OnCoursorHandle(m_GimmickIcons);
				if (handle != -1) {
					coursorPosition = m_GimmickIcons[handle]->GetAnchorPosition(Anchor::Left);
				}
			}
			else {
				handlePosition = m_GimmickIcons[m_GimmickHandle]->GetAnchorPosition(Anchor::Left);
				int handle = OnCoursorHandle(m_ColorPalette);
				if (handle != -1) {
					coursorPosition = m_ColorPalette[handle]->GetAnchorPosition(Anchor::Right);
				}
			}
			


			DrawLine(handlePosition, coursorPosition);

			if (input->GetUpButton(gameManager.GetKeyConfig(L"putGimmick"))) {
				if (UpdateOnCoursorHandle() && m_ColorHandle != -1 && m_GimmickHandle != -1) {
					stack<vector<Line>::iterator> eraseIteraters;
					for (auto it = m_Lines.begin(); it != m_Lines.end(); it++) {
						auto& pair = (*it).m_PairHandle;
						if (pair.first == m_ColorHandle || pair.second == m_GimmickHandle) {
							m_Stage->RemoveGameObject<Sprite>((*it).m_Line);
							eraseIteraters.push(it);
						}
					}
					while (!eraseIteraters.empty()) {
						m_Lines.erase(eraseIteraters.top());
						eraseIteraters.pop();
					}
					gameManager.GetLevelManager()->AddPair(pair<int, int>{m_ColorHandle, m_GimmickHandle});
					m_Lines.push_back({ m_CurrentLine,pair<int,int>{m_ColorHandle,m_GimmickHandle} });
					SoundManager::GetInstance().PlaySE(L"Put");
				}
				else {
					m_Stage->RemoveGameObject<Sprite>(m_CurrentLine);
				}
				m_CurrentLine = nullptr;
				m_ColorHandle = m_GimmickHandle = -1;
			}
		}

		DrawExpain();
		if (input->GetDownButton(L"Y"))
		{
			if (m_HintStartPos.empty()) {
				LoadHintData();
			}
			HintCreate(); // 描画とインデックス進行を内部で処理
		}
	}

	bool GameMenu::UpdateOnCoursorHandle() {
		int colorHandle = OnCoursorHandle(m_ColorPalette);
		int gimmickHandle = OnCoursorHandle(m_GimmickIcons);
		
		if (colorHandle == -1 && gimmickHandle == -1) return false;

		if (!m_CurrentLine) {
			m_CurrentLine = m_Stage->AddGameObject<Sprite>(m_ColorTexture, m_Cursor->GetPosition(), Vec2(10, 10), Anchor::Center);
			m_CurrentLine->SetLayer(1);
		}

		if (colorHandle >= 0) {
			m_ColorHandle = colorHandle;
			m_CurrentLine->SetDiffuse(m_ColorPalette[colorHandle]->GetDiffuse());
		}
		if (gimmickHandle >= 0) {
			m_GimmickHandle = gimmickHandle;
		}
		return true;
	}
	int GameMenu::OnCoursorHandle(vector<shared_ptr<Sprite>>& sprites) {
		for (int i = 0; i < sprites.size(); i++) {
			auto& sprite = sprites[i];
			if (m_Cursor->IsOnArea(
				sprite->GetAnchorPosition(Anchor::TopRight), sprite->GetAnchorPosition(Anchor::BottomLeft))) {
				return i;
			}
		}
		return -1;
	}
	void GameMenu::SetDrawActive(bool flag) {
		if (!flag)	m_PoseMenu->Open();

		m_Cursor->SetUpdateActive(flag);
		for (auto& colorPalette : m_ColorPalette) {
			colorPalette->SetDrawActive(flag);
		}
		for (auto& gimmickSprite : m_GimmickIcons) {
			gimmickSprite->SetDrawActive(flag);
		}
		for (auto& handleSprite : m_HandlerSprites) {
			handleSprite->SetDrawActive(flag);
		}
		for (auto& eplainIcon : m_ExplainIcons) {
			eplainIcon->SetDrawActive(flag);
		}
		for (auto& line : m_Lines) {
			line.m_Line->SetDrawActive(flag);
		}
		m_BackGround->SetDrawActive(flag);
		m_CurrentExplain->SetDrawActive(flag);
		m_ExplainBox->SetDrawActive(flag);
		m_OperateInfo->SetDrawActive(flag);
	}
	vector<pair<int, int>> GameMenu::ConvertColorGimmickHandles(vector<Line>& lines) {
		vector<pair<int, int>> p;
		for (auto& line : lines) {
			auto& handles = line.m_PairHandle;
			p.push_back(pair<int, int>{handles.first, handles.second});
		}
		return p;
	}
	void GameMenu::DrawLine(Vec3 start, Vec3 end) {

		Vec3 direction = end - start;
		m_CurrentLine->SetSize(Vec2(m_CurrentLine->GetSize().x, direction.length()));
		m_CurrentLine->SetAnchorPosition(start + direction / 2.0f,Anchor::Center);
		m_CurrentLine->VectorToward(static_cast<Vec2>(direction.normalize()));

	}
	void GameMenu::DrawExpain() {
		auto cards = GameManager::GetInstance().GetLevelManager()->GetHand()->GetCardData();
		if (m_GimmickHandle != -1) {
			wstring key = cards[m_GimmickHandle]->GetExplainKey();
			if (key != L"") {
				m_CurrentExplain->SetTextureKey(key);
			}
		}
		else {
			int handle = OnCoursorHandle(m_GimmickIcons);
			if (handle != -1) {
				wstring key = cards[handle]->GetExplainKey();
				if (key != L"") {
					m_CurrentExplain->SetTextureKey(key);
				}
			}
		}
	}

	void GameMenu::HintCreate()
	{
		int hintDrawCount = 0;
		const int maxDrawPerCall = 2; // ← 同時に描画する本数

		while (m_HintIndex < m_HintStartPos.size() && hintDrawCount < maxDrawPerCall)
		{
			bool alreadyMatched = IsHintPairMatched(m_HintIndex);
			Vec3 startPos = m_HintStartPos[m_HintIndex];
			Vec3 endPos = m_HintEndPos[m_HintIndex];

			bool alreadyDrawn = false;
			for (const auto& line : m_HintLines)
			{
				Vec3 linePos = line->GetPosition();
				float lineLength = line->GetSize().y;

				Vec3 expectedPos = startPos + (endPos - startPos) / 2.0f;
				float expectedLength = (endPos - startPos).length();

				if ((linePos - expectedPos).length() < 0.1f &&
					std::abs(lineLength - expectedLength) < 0.1f)
				{
					alreadyDrawn = true;
					break;
				}
			}

			if (alreadyDrawn || alreadyMatched)
			{
				m_HintIndex++;
				continue;
			}

			auto line = m_Stage->AddGameObject<Sprite>(L"HintLine", startPos, Vec2(10, 10), Anchor::Center);
			Vec3 direction = endPos - startPos;
			line->SetSize(Vec2(line->GetSize().x, direction.length()));
			line->SetPosition(startPos + direction / 2.0f);
			line->VectorToward(static_cast<Vec2>(direction.normalize()));

			m_HintLines.push_back(line);
			m_HintIndex++;
			hintDrawCount++;
		}

	}
	void GameMenu::LoadHintData()
	{
		auto& gameManager = GameManager::GetInstance();
		auto colorTable = gameManager.GetLevelManager()->GetMap()->GetColorTable();
		Json levelJson = gameManager.GetLevelManager()->GetJson();
		auto hintArray = levelJson.At<JsonArray>(L"hint");
		auto objectArray = hintArray->GetObjectArray();
		auto cards = gameManager.GetLevelManager()->GetHand()->GetCardData();

		for (const auto& hintObj : objectArray)
		{
			auto color = hintObj->At<JsonString>(L"color")->GetValue();
			auto id = hintObj->At<JsonString>(L"id")->GetValue();

			Vec3 startPos = Vec3();
			Vec3 endPos = Vec3();
			int colorIndex = -1;
			int gimmickIndex = -1;

			for (size_t j = 0; j < colorTable.size(); ++j)
			{
				if (color == colorTable[j])
				{
					startPos = m_ColorPalette[j]->GetPosition();
					colorIndex = static_cast<int>(j);
					break;
				}
			}

			for (size_t j = 0; j < cards.size(); ++j)
			{
				if (id == cards[j]->GetId())
				{
					endPos = m_GimmickIcons[j]->GetPosition();
					gimmickIndex = static_cast<int>(j);
					break;
				}
			}

			if (startPos != Vec3() && endPos != Vec3())
			{
				m_HintStartPos.push_back(startPos);
				m_HintEndPos.push_back(endPos);
				m_HintPairs.push_back({ colorIndex, gimmickIndex }); // ? ここが重要
			}
		}
	}

	void PoseMenu::OnCreate() {
		m_BackGround = m_Stage->AddGameObject<Sprite>(L"POSE_MENU", m_TopLeftPosition, m_MenuSize, Anchor::TopLeft);
		
		Vec3 buttonTopLeft = m_TopLeftPosition + Vec3(m_MenuSize.x / 2.0f - m_ButtonSize.x / 2.0f, -100.0f, 0.0f);
		Vec3 duration = Vec3(0.0f,m_ButtonSize.y / 2.0f,0.0f);
		duration.y += m_ButtonSize.y;
		//音量
		ButtonManager::Create(m_Stage, L"POSE", L"TEMP_SOUND_MENU", L"SELECT_SOUND_MENU", buttonTopLeft, m_ButtonSize,GetThis<PoseMenu>(),
			[](shared_ptr<ObjectInterface>& object) {
				auto pose = dynamic_pointer_cast<PoseMenu>(object);
				pose->SettingSound();
			});
		//タイトルに戻る
		ButtonManager::Create(m_Stage, L"POSE", L"TEMP_TITLE_MENU", L"SELECT_TITLE_MENU", buttonTopLeft - duration, m_ButtonSize, GetThis<PoseMenu>(),
			[](shared_ptr<ObjectInterface>& object) {
				auto pose = dynamic_pointer_cast<PoseMenu>(object);
				pose->MoveTitleStage();
			});
		//セレクトステージに戻る
		ButtonManager::Create(m_Stage, L"POSE", L"TEMP_SELECT_MENU", L"SELECT_SELECT_MENU", buttonTopLeft - duration * 2.0f, m_ButtonSize, GetThis<PoseMenu>(),
			[](shared_ptr<ObjectInterface>& object) {
				auto pose = dynamic_pointer_cast<PoseMenu>(object);
				pose->MoveSelectStage();
			});
		//最初から始める
		ButtonManager::Create(m_Stage, L"POSE", L"TEMP_NEW_MENU", L"SELECT_NEW_MENU", buttonTopLeft - duration * 3.0f, m_ButtonSize, GetThis<PoseMenu>(),
			[](shared_ptr<ObjectInterface>& object) {
				auto pose = dynamic_pointer_cast<PoseMenu>(object);
				pose->CloseNewGame();
			});
		//ギミック解説
		ButtonManager::Create(m_Stage, L"POSE", L"TEMP_EXPAIN_MENU", L"SELECT_EXPAIN_MENU", buttonTopLeft - duration * 4.0f, m_ButtonSize, GetThis<PoseMenu>(),
			[](shared_ptr<ObjectInterface>& object) {
				auto pose = dynamic_pointer_cast<PoseMenu>(object);
				pose->OpenExpainGimmicks();
			});
		//ゲームに戻る
		ButtonManager::Create(m_Stage, L"POSE", L"TEMP_BACK_MENU", L"SELECT_BACK_MENU", buttonTopLeft - duration * 5.0f, m_ButtonSize, GetThis<PoseMenu>(),
			[](shared_ptr<ObjectInterface>& object) {
				auto pose = dynamic_pointer_cast<PoseMenu>(object);
				pose->Close();
			});

		ButtonManager::instance->AddAcceptButton(L"POSE", XINPUT_GAMEPAD_A);//決定ボタン
		ButtonManager::instance->SetInput(L"POSE", InputData(XINPUT_GAMEPAD_DPAD_DOWN, 1));//選択(上)
		ButtonManager::instance->SetInput(L"POSE", InputData(XINPUT_GAMEPAD_DPAD_UP, -1));//選択(下)
		ButtonManager::instance->SetInput(L"POSE", InputData(StickMode::LY, 1, 0.1f));//選択(左スティック)
		ButtonManager::instance->SetLoop(true);
		ButtonManager::instance->SetSound(L"decision");
		ButtonManager::instance->SetSelectSound(L"cursorMove");

		m_ExplainMenu = m_Stage->AddGameObject<ExplainMenu>();
		m_ExplainMenu->Close();

		Close();
	}
	void PoseMenu::Open() {
		ButtonManager::instance->OpenAndUse(L"POSE");
		SetDrawActive(true);
		m_BackGround->SetDrawActive(true);
		SoundManager::GetInstance().PlaySE(L"decision");
	}
	void PoseMenu::Close() {
		ButtonManager::instance->Close(L"POSE");
		m_BackGround->SetDrawActive(false);
		m_GameMenu->SetDrawActive(true);
		SetDrawActive(false);
	}
	void PoseMenu::MoveSelectStage() {
		PostEvent(0.0f, nullptr, App::GetApp()->GetScene<Scene>(), L"ToSelectStage");
	}
	void PoseMenu::MoveTitleStage() {
		PostEvent(0.0f, nullptr, App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
	}
	void PoseMenu::SettingSound() {
		m_SoundMenu->Open();
	}
	void PoseMenu::CloseNewGame() {
		int stageNumber = GameManager::GetInstance().GetLevelManager()->GetStageNumber();
		PostEvent(0.0f, nullptr, App::GetApp()->GetScene<Scene>(), L"ToGameStage", make_shared<int>(stageNumber));
	}
	void PoseMenu::OpenExpainGimmicks() {
		m_ExplainMenu->Open();
	}

	void ExplainMenu::OnCreate(){
		m_ExplainStr = m_Stage->AddGameObject<Sprite>(L"", Vec3(-120,-100,0), Vec2(450, 225), Anchor::Center);
		m_BackGround = m_Stage->AddGameObject<Sprite>(L"MENU_GIMMICK", Vec3(-640,400,0), Vec2(800, 800), Anchor::TopLeft);
		m_BackGround->SetLayer(-1);

		AddExplain(ExplainData{ L"ICON_PL",L"",L"EXPAIN_PL" });
		AddExplain(ExplainData{ L"ICON_GOAL",L"",L"EXPAIN_GL" });
		AddExplain(ExplainData{ L"ICON_ARROW",L"",L"EXPAIN_ARRW" });
		AddExplain(ExplainData{ L"ICON_TP",L"",L"EXPAIN_TP" });
		AddExplain(ExplainData{ L"ICON_INV",L"",L"EXPAIN_INV" });
		AddExplain(ExplainData{ L"ICON_ROLL",L"",L"EXPAIN_ROLL" });

		CreateExplain();
	}
	void ExplainMenu::CreateExplain() {
		Vec2 explainTabSize = Vec2(100, 100);
		Vec3 topLeft = static_cast<Vec3>(m_BackGround->GetAnchorPosition(Anchor::TopLeft) + Vec3(100,-50,0));
		for (int i = 0; i < m_ExplainDatas.size(); i++) {
			ButtonManager::Create(m_Stage, L"EXPLAIN", m_ExplainDatas[i].m_MenuIconKey, L"ICON_EXPLAIN_FRAME", topLeft - Vec3(0, explainTabSize.y, 0) * i, explainTabSize,
				[](shared_ptr<ObjectInterface>& object) {
				});
		}
		

		ButtonManager::instance->SetInput(L"EXPLAIN", InputData(XINPUT_GAMEPAD_DPAD_DOWN, 1));//選択(上)
		ButtonManager::instance->SetInput(L"EXPLAIN", InputData(XINPUT_GAMEPAD_DPAD_UP, -1));//選択(下)
		ButtonManager::instance->SetInput(L"EXPLAIN", InputData(StickMode::LY, 1, 0.1f));//選択(左スティック)

	}
	void ExplainMenu::OnUpdate() {
		if (!ButtonManager::instance->CompareUseGroup(L"EXPLAIN")) return;

		int index = ButtonManager::instance->GetSelectIndex(L"EXPLAIN");

		m_ExplainStr->SetTextureKey(m_ExplainDatas[index].m_ExplainKey);

		if (InputManager::GetInputManager()->GetDownButton(L"Start")) {
			Close();
			SoundManager::GetInstance().PlaySE(L"decision");
		}
	}

	void ExplainMenu::Open() {
		m_BackGround->SetDrawActive(true);
		m_ExplainStr->SetDrawActive(true);
		ButtonManager::instance->OpenAndUse(L"EXPLAIN");
	}
	void ExplainMenu::Close() {
		m_BackGround->SetDrawActive(false);
		m_ExplainStr->SetDrawActive(false);
		ButtonManager::instance->Close(L"EXPLAIN");
		ButtonManager::instance->OpenAndUse(L"POSE");
	}


	void SoundMenu::OnCreate() {
		m_SoundMenu = m_Stage->AddGameObject<Sprite>(L"SOUND_MENU", m_MenuPosition, m_MenuSize, Anchor::TopLeft);

		m_SoundBars = { nullptr,nullptr };
		m_SoundBarFrames = { nullptr,nullptr };

		m_BarSize = Vec2(m_MenuSize.x * 0.5f, 0);
		m_BarSize.y = m_BarSize.x * 0.16f;
		Vec3 barPosition = static_cast<Vec3>(m_SoundMenu->GetAnchorPosition(Anchor::Left)) + Vec3(m_MenuSize.x / 3.25f, 85, 0);
		Vec3 barPosition2 = barPosition - Vec3(0, 150, 0);
		m_SoundBars[0] = m_Stage->AddGameObject<Sprite>(L"SOUND_BAR",barPosition , m_BarSize, Anchor::TopLeft);
		m_SoundBarFrames[0] = m_Stage->AddGameObject<Sprite>(L"SOUND_BAR_FRAME",barPosition , m_BarSize, Anchor::TopLeft);
		m_SoundBars[1] = m_Stage->AddGameObject<Sprite>(L"SOUND_BAR", barPosition2, m_BarSize, Anchor::TopLeft);
		m_SoundBarFrames[1] = m_Stage->AddGameObject<Sprite>(L"SOUND_BAR_FRAME", barPosition2, m_BarSize, Anchor::TopLeft);


		Vec2 poseMenuButtonSize = Vec2(200.0f, 60.0f);
		Vec3 offset = Vec3(-poseMenuButtonSize.x / 1.5f, poseMenuButtonSize.y, 0);
		ButtonManager::Create(m_Stage, L"SOUND", L"SOUND_BGM", L"SOUND_BGM_SELECT", barPosition + offset, poseMenuButtonSize,
			[](shared_ptr<ObjectInterface>& object) {
			});
		ButtonManager::Create(m_Stage, L"SOUND", L"SOUND_SE", L"SOUND_SE_SELECT", barPosition2 + offset, poseMenuButtonSize,
			[](shared_ptr<ObjectInterface>& object) {
			});

		ButtonManager::instance->SetInput(L"SOUND", InputData(XINPUT_GAMEPAD_DPAD_DOWN, 1));//選択(上)
		ButtonManager::instance->SetInput(L"SOUND", InputData(XINPUT_GAMEPAD_DPAD_UP, -1));//選択(下)
		ButtonManager::instance->SetInput(L"SOUND", InputData(StickMode::LY, 1, 0.5f));//選択(左スティック)
		ButtonManager::instance->SetLoop(true);


	}
	void SoundMenu::OnUpdate() {
		if (!m_SoundMenu->GetDrawActive()) return;
		auto& input = InputManager::GetInputManager();
		if (input->GetDownButton(L"Start")) {
			Close();
			SoundManager::GetInstance().PlaySE(L"decision");
			return;
		}
		float stickX = input->GetLStick().x;
		if (fabs(stickX) > 0.5f) {
			int selectIndex = ButtonManager::instance->GetSelectIndex(L"SOUND");
			if (stickX > 0) {
				if (selectIndex == 0) {
					SoundManager::GetInstance().BGMVolumeUp(0.01f);
				}
				else {
					SoundManager::GetInstance().SEVolumeUp(0.01f);
				}
			}
			else {
				if (selectIndex == 0) {
					SoundManager::GetInstance().BGMVolumeDown(0.01f);
				}
				else {
					SoundManager::GetInstance().SEVolumeDown(0.01f);
				}
			}
		}
		
		float seVolume = SoundManager::GetInstance().GetSEVolume();
		float bgmVolume = SoundManager::GetInstance().GetBGMVolume();

		m_SoundBars[0]->SetSize(Vec2(m_BarSize.x * bgmVolume, m_BarSize.y));
		m_SoundBars[1]->SetSize(Vec2(m_BarSize.x * seVolume, m_BarSize.y));
	}

	void SoundMenu::Open() {
		ButtonManager::instance->Close(L"POSE");
		ButtonManager::instance->OpenAndUse(L"SOUND");
		m_SoundMenu->SetDrawActive(true);
		m_SoundBars[0]->SetDrawActive(true);
		m_SoundBars[1]->SetDrawActive(true);
		m_SoundBarFrames[0]->SetDrawActive(true);
		m_SoundBarFrames[1]->SetDrawActive(true);
	}
	void SoundMenu::Close() {
		ButtonManager::instance->Close(L"SOUND");
		ButtonManager::instance->OpenAndUse(L"POSE");
		m_SoundMenu->SetDrawActive(false);
		m_SoundBars[0]->SetDrawActive(false);
		m_SoundBars[1]->SetDrawActive(false);
		m_SoundBarFrames[0]->SetDrawActive(false);
		m_SoundBarFrames[1]->SetDrawActive(false);
	}


	void Cursor::OnCreate() {
		m_Cursor = m_Stage->AddGameObject<Sprite>(m_CoursorTexture, Vec3(), Vec2(), Anchor::TopLeft);
		m_Cursor->SetLayer(10);
	}
	void Cursor::OnUpdate() {
		auto& input = InputManager::GetInputManager();
		float elpased = App::GetApp()->GetElapsedTime();
		if (m_Mode == CoursorMode::Stick) {
			Vec2 stick = input->GetLStick();
			if (stick.length() > 0.1f) {
				stick = stick.normalize();
				Vec3 position = m_Cursor->GetPosition();
				position += static_cast<Vec3>(stick) * m_MoveSpeed * elpased;

				m_Cursor->SetPosition(position);
				LimitMoveArea();
			}
		}
	}

	Vec3 Cursor::LimitMoveArea() {
		Vec2 position = m_Cursor->GetAnchorPosition(Anchor::Center);

		Vec2 left = m_Cursor->GetAnchorPosition(Anchor::Left);
		Vec2 right = m_Cursor->GetAnchorPosition(Anchor::Right);
		Vec2 top = m_Cursor->GetAnchorPosition(Anchor::Top);
		Vec2 bottom = m_Cursor->GetAnchorPosition(Anchor::Bottom);

		if (left.x < m_MoveArea.m_Min.x) {
			m_Cursor->SetAnchorPosition(Vec3(m_MoveArea.m_Min.x, position.y, 0), Anchor::Left);
			position = m_Cursor->GetPosition();
		}
		if (right.x > m_MoveArea.m_Max.x) {
			m_Cursor->SetAnchorPosition(Vec3(m_MoveArea.m_Max.x, position.y, 0), Anchor::Right);
			position = m_Cursor->GetPosition();
		}
		if (top.y > m_MoveArea.m_Max.y) {
			m_Cursor->SetAnchorPosition(Vec3(position.x,m_MoveArea.m_Max.y, 0), Anchor::Top);
			position = m_Cursor->GetPosition();
		}
		if (bottom.y < m_MoveArea.m_Min.y) {
			m_Cursor->SetAnchorPosition(Vec3(position.x,m_MoveArea.m_Min.y, 0), Anchor::Bottom);
			position = m_Cursor->GetPosition();
		}
		return {};
	}

	bool Cursor::IsOnArea(Vec2 max, Vec2 min) {
		Vec3 position = m_Cursor->GetPosition();

		if (max.x < position.x || min.x > position.x) {
			return false;
		}
		if (max.y < position.y || min.y > position.y) {
			return false;
		}
		return true;
	}


}
//end basecross
