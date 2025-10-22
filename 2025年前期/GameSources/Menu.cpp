/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void Menu::OnCreate() {
		m_GimmickTextures[GimmickObjects::Goal] = L"TEMP_GIMMICK_GOAL";
		m_GimmickTextures[GimmickObjects::SetPlayer] = L"TEMP_GIMMICK_PLAYER";
		m_GimmickTextures[GimmickObjects::CourseCorrection] = L"TEMP_GIMMICK_COURSE";
		m_GimmickTextures[GimmickObjects::Roll] = L"TEMP_GIMMICK_COURSE";
		m_GimmickTextures[GimmickObjects::Killer] = L"TEMP_GIMMICK_UPPER";
		m_GimmickTextures[GimmickObjects::Teleporter] = L"TEMP_GIMMICK_UPPER";

		Vec2 mainViewSize = Vec2(m_MainViewPort.Width, m_MainViewPort.Height);
		Vec2 screenSize = Vec2(App::GetApp()->GetGameWidth(), App::GetApp()->GetGameHeight());
		Vec2 menuSize = Vec2(screenSize.x - mainViewSize.x, screenSize.y);
		Vec3 menuPosition = Vec3(screenSize.x / 2.0f - menuSize.x, 0, 0);
		m_BackGround = m_MenuStage->AddGameObject<Sprite>(m_BackGroundTexture, menuPosition, menuSize,Anchor::Left);
		float duretionY = 75.0f;
		Vec2 colorPalettePosition = Vec2(menuPosition.x + 100.0f, menuPosition.y + menuSize.y / 2.0f - 100.0f);
		Vec2 gimmickPosition = Vec2(menuPosition.x + menuSize.x - 100.0f, menuPosition.y + menuSize.y / 2.0f - 100.0f);

		auto expainMenu = m_MenuStage->AddGameObject<Sprite>(m_BackGroundTexture, Vec3(menuPosition.x, menuPosition.y - screenSize.y * 0.25f,0.0f), Vec2(menuSize.x,menuSize.y * 0.25f), Anchor::TopLeft);
		auto& gameManager = GameManager::GetInstance();

		auto colorTable = gameManager.GetMap()->GetColorTable();
		Json colorJson = Json(L"Json/color.json");

		for (int i = 0; i < colorTable.size(); i++){
			auto colorAry = colorJson.At<JsonArray>(colorTable[i])->GetFloatArray();
			auto color = Col4(colorAry[0], colorAry[1], colorAry[2], 1.0f);

			auto sprite = m_MenuStage->AddGameObject<Sprite>(
				m_ColorTexture,
				Vec3(colorPalettePosition.x, colorPalettePosition.y - i * duretionY,0.0f), Vec2(50, 50),
				Anchor::Center);
			sprite->SetDiffuse(color);
			m_ColorPalette.push_back(sprite);
		}


		auto cards = gameManager.GetHand()->GetCardData();
		for (int i = 0; i < cards.size(); i++) {
			auto sprite = m_MenuStage->AddGameObject<Sprite>(
				m_GimmickTextures[cards[i]->GetType()],
				Vec3(gimmickPosition.x, gimmickPosition.y - i * duretionY, 0.0f), Vec2(50, 50),
				Anchor::Center);
			m_GimmcikSprites.push_back(sprite);
		}

		m_Coursor = m_MenuStage->AddGameObject<Coursor>(L"TEMP_GIMMICK_COURSE");
		m_Coursor->SetCoursorSize(25.0f);
		m_Coursor->SetMoveSpeed(300.0f);
		m_Coursor->SetMoveArea(m_BackGround->GetAnchorPosition(Anchor::TopRight), expainMenu->GetAnchorPosition(Anchor::TopLeft));
	}
	void Menu::OnUpdate() {
		auto& input = InputManager::GetInputManager();
		auto& gameManager = GameManager::GetInstance();

		if (input->GetDownButton(gameManager.GetKeyConfig(L"undo")) && m_Lines.size() > 0) {
			auto line = m_Lines.back();
			m_Lines.pop_back();
			m_MenuStage->RemoveGameObject<Sprite>(line.m_Line);
		}
		if (input->GetDownButton(gameManager.GetKeyConfig(L"putGimmick"))) {
			UpdateOnCoursorHandle();
		}

		if (m_ColorHandle != -1 || m_GimmikcHandle != -1) {
			if (!m_CurrentLine)return;

			Vec3 coursorPosition = m_Coursor->GetPosition();
			Vec3 handlePosition;
			if (m_ColorHandle != -1) {
				handlePosition = m_ColorPalette[m_ColorHandle]->GetPosition();
				int handle = OnCoursorHandle(m_GimmcikSprites);
				if (handle != -1) {
					coursorPosition = m_GimmcikSprites[handle]->GetPosition();
				}
			}
			else {
				handlePosition = m_GimmcikSprites[m_GimmikcHandle]->GetPosition();
				int handle = OnCoursorHandle(m_ColorPalette);
				if (handle != -1) {
					coursorPosition = m_ColorPalette[handle]->GetPosition();
				}
			}			

			DrawLine(handlePosition, coursorPosition);

			if (input->GetUpButton(gameManager.GetKeyConfig(L"putGimmick"))) {
				if (UpdateOnCoursorHandle() && m_ColorHandle != -1 && m_GimmikcHandle != -1) {
					stack<vector<Line>::iterator> eraseIteraters;
					for (auto it = m_Lines.begin(); it != m_Lines.end(); it++) {
						auto& pair = (*it).m_PairHandle;
						if (pair.first == m_ColorHandle || pair.second == m_GimmikcHandle) {
							m_MenuStage->RemoveGameObject<Sprite>((*it).m_Line);
							eraseIteraters.push(it);
						}
					}
					while (!eraseIteraters.empty()) {
						m_Lines.erase(eraseIteraters.top());
						eraseIteraters.pop();
					}
					m_Lines.push_back({ m_CurrentLine,pair<int,int>{m_ColorHandle,m_GimmikcHandle} });
				}
				else {
					m_MenuStage->RemoveGameObject<Sprite>(m_CurrentLine);
				}
				m_CurrentLine = nullptr;
				m_ColorHandle = m_GimmikcHandle = -1;
			}
		}

		gameManager.UpdatePair(ConvertColorGimmickHandles(m_Lines));
	}

	bool Menu::UpdateOnCoursorHandle() {
		int colorHandle = OnCoursorHandle(m_ColorPalette);
		int gimmickHandle = OnCoursorHandle(m_GimmcikSprites);
		
		if (colorHandle == -1 && gimmickHandle == -1) return false;

		if (!m_CurrentLine) {
			m_CurrentLine = m_MenuStage->AddGameObject<Sprite>(m_ColorTexture, m_Coursor->GetPosition(), Vec2(10, 10), Anchor::Center);
		}

		if (colorHandle >= 0) {
			m_ColorHandle = colorHandle;
			m_CurrentLine->SetDiffuse(m_ColorPalette[colorHandle]->GetDiffuse());
		}
		if (gimmickHandle >= 0) {
			m_GimmikcHandle = gimmickHandle;
		}
		return true;
	}
	int Menu::OnCoursorHandle(vector<shared_ptr<Sprite>>& sprites) {
		for (int i = 0; i < sprites.size(); i++) {
			auto& sprite = sprites[i];
			if (m_Coursor->IsOnArea(
				sprite->GetAnchorPosition(Anchor::TopRight), sprite->GetAnchorPosition(Anchor::BottomLeft))) {
				return i;
			}
		}
		return -1;
	}
	vector<pair<int, int>> Menu::ConvertColorGimmickHandles(vector<Line>& lines) {
		vector<pair<int, int>> p;
		for (auto& line : lines) {
			auto& handles = line.m_PairHandle;
			p.push_back(pair<int, int>{handles.first, handles.second});
		}
		return p;
	}
	void Menu::DrawLine(Vec3 start, Vec3 end) {

		Vec3 direction = end - start;
		m_CurrentLine->SetSize(Vec2(m_CurrentLine->GetSize().x, direction.length()));
		m_CurrentLine->SetPosition(start + direction / 2.0f);
		m_CurrentLine->VectorToward(static_cast<Vec2>(direction.normalize()));

	}

	void Coursor::OnCreate() {
		m_Coursor = m_MenuStage->AddGameObject<Sprite>(m_CoursorTexture, Vec3(), Vec2(), Anchor::Center);
	}
	void Coursor::OnUpdate() {
		auto& input = InputManager::GetInputManager();
		float elpased = App::GetApp()->GetElapsedTime();
		if (m_Mode == CoursorMode::Stick) {
			Vec2 stick = input->GetLStick();
			if (stick.length() > 0.1f) {
				stick = stick.normalize();
				Vec3 position = m_Coursor->GetPosition();
				position += static_cast<Vec3>(stick) * m_MoveSpeed * elpased;

				m_Coursor->SetPosition(position);
				LimitMoveArea();
			}
		}
	}

	Vec3 Coursor::LimitMoveArea() {
		Vec3 position = m_Coursor->GetPosition();

		Vec2 left = m_Coursor->GetAnchorPosition(Anchor::Left);
		Vec2 right = m_Coursor->GetAnchorPosition(Anchor::Right);
		Vec2 top = m_Coursor->GetAnchorPosition(Anchor::Top);
		Vec2 bottom = m_Coursor->GetAnchorPosition(Anchor::Bottom);

		if (left.x < m_MoveArea.m_Min.x) {
			m_Coursor->SetAnchorPosition(Vec3(m_MoveArea.m_Min.x, position.y, 0), Anchor::Left);
			position = m_Coursor->GetPosition();
		}
		if (right.x > m_MoveArea.m_Max.x) {
			m_Coursor->SetAnchorPosition(Vec3(m_MoveArea.m_Max.x, position.y, 0), Anchor::Right);
			position = m_Coursor->GetPosition();
		}
		if (top.y > m_MoveArea.m_Max.y) {
			m_Coursor->SetAnchorPosition(Vec3(position.x,m_MoveArea.m_Max.y, 0), Anchor::Top);
			position = m_Coursor->GetPosition();
		}
		if (bottom.y < m_MoveArea.m_Min.y) {
			m_Coursor->SetAnchorPosition(Vec3(position.x,m_MoveArea.m_Min.y, 0), Anchor::Bottom);
			position = m_Coursor->GetPosition();
		}
		return {};
	}

	bool Coursor::IsOnArea(Vec2 max, Vec2 min) {
		Vec3 position = m_Coursor->GetPosition();

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
