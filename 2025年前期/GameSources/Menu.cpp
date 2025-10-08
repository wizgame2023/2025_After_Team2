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
		m_Coursor->SetMoveArea(m_BackGround->GetAnchorPosition(Anchor::TopRight), m_BackGround->GetAnchorPosition(Anchor::BottomLeft));
	}
	void Menu::OnUpdate() {
		auto& input = InputManager::GetInputManager();
		auto& gameManager = GameManager::GetInstance();
		if (input->GetDownButton(gameManager.GetKeyConfig(L"putGimmick"))) {
			m_ColorHandle = OnCoursorHandle(m_ColorPalette);
			m_GimmikcHandle = OnCoursorHandle(m_GimmcikSprites);

			if (m_ColorHandle != -1 || m_GimmikcHandle != -1) {
				m_Line.push_back(m_MenuStage->AddGameObject<Sprite>(m_ColorTexture, m_Coursor->GetPosition(), Vec2(10,10), Anchor::Center));
				if (m_ColorHandle != -1) {
					m_Line.back()->SetDiffuse(m_ColorPalette[m_ColorHandle]->GetDiffuse());
				}
			}

		}

		if (m_ColorHandle != -1 || m_GimmikcHandle != -1) {
			Vec2 handlePosition;

			handlePosition = m_ColorHandle != -1 
				? m_ColorPalette[m_ColorHandle]->GetPosition() : m_GimmcikSprites[m_GimmikcHandle]->GetPosition();

			Vec2 direction = static_cast<Vec2>(m_Coursor->GetPosition()) - handlePosition;
			auto& sprite = m_Line.back();
			sprite->SetSize(Vec2(sprite->GetSize().x, direction.length()));
			sprite->SetPosition(static_cast<Vec3>(handlePosition + direction / 2.0f));
			sprite->VectorToward(direction.normalize());

			if (input->GetUpButton(gameManager.GetKeyConfig(L"putGimmick"))) {
				int handle = -1;
				handle = m_ColorHandle != -1 ? OnCoursorHandle(m_GimmcikSprites) : OnCoursorHandle(m_ColorPalette);
				if (handle == -1) {
					m_MenuStage->RemoveGameObject<Sprite>(m_Line.back());
					m_Line.pop_back();
				}
				else if(m_ColorHandle == -1){
					m_Line.back()->SetDiffuse(m_ColorPalette[handle]->GetDiffuse());
				}

				m_ColorHandle = m_GimmikcHandle = -1;
			}
		}
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
