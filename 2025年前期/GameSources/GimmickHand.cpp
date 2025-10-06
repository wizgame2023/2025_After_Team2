/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	map<GimmickObjects, wstring> GimmickCard::m_CoverTexKeys = {};

	void GimmickCard::OnCreate() {
		Object::OnCreate();

		m_Card = m_Stage->AddGameObject<Sprite>(L"TEMP_GIMMICK",Vec3(0.0f),Vec2(),Anchor::Left);

		m_CoverTexKeys[GimmickObjects::Goal] = L"TEMP_GIMMICK_GOAL";
		m_CoverTexKeys[GimmickObjects::SetPlayer] = L"TEMP_GIMMICK_PLAYER";
		m_CoverTexKeys[GimmickObjects::Teleporter] = L"TEMP_GIMMICK_UPPER";
		m_CoverTexKeys[GimmickObjects::CourseCorrection] = L"TEMP_GIMMICK_COURSE";
		
		if (m_Type->GetType() != GimmickObjects::None) {
			SetCover(m_Type->GetType());
		}
	}
	void GimmickCard::OnUpdate() {
		if (m_CoverSprite != nullptr) {
			Vec3 cardPosition = m_Card->GetPosition();
			Vec2 cardScale = m_Card->GetSize();

			Vec3 coverPosition = cardPosition + Vec3(cardScale.x / 2.0f, cardScale.y / 4.0f, 0.0f);
			Vec2 coverScale = Vec2(cardScale.x / 2.0f, cardScale.y / 3.0f);
			m_CoverSprite->SetPosition(coverPosition);
			m_CoverSprite->SetSize(coverScale);
		}
	}
	void GimmickCard::SetCover(GimmickObjects type) {
		m_CoverSprite = m_Stage->AddGameObject<Sprite>(m_CoverTexKeys[type], Vec3(0.0f), Vec2(), Anchor::Center);
		//m_Type = type;
	}
	void GimmickHand::OnCreate() {
		Object::OnCreate();
	}
	void GimmickHand::OnUpdate() {
		float handCount = m_HandSprite.size();
		
		for (int i = 0; i < m_HandSprite.size(); i++) {
			auto card = m_HandSprite[i]->GetCardSprite();
			card->SetSize(m_CardSize);
			Vec2 cardHalfScale = m_CardSize * 0.25f;

			Vec3 offset = Vec3(i * cardHalfScale.x, 0.0f, 0.0f);

			if (i == m_SelectIndex) {
				//選択
				offset += Vec3(0.0f, cardHalfScale.y, 0.0f);
			}
			else {
				//未選択
			}

			card->ScreenAnchor(Anchor::BottomLeft, offset);
		}
	}
	void GimmickHand::LoadHands(shared_ptr<JsonArray>& items) {
		auto itemArray = items->GetObjectArray();
		for (auto& item : itemArray) {
			auto id = item->At<JsonString>(L"id")->GetValue();
			auto count = item->At<JsonNumber>(L"count")->GetIntValue();

			shared_ptr<CardData> card;
			if (id == L"player") {
				card = make_shared<PlayerCard>();
			}
			else if (id == L"goal") {
				card = make_shared<GoalCard>();
			}
			else if (id == L"course") {
				card = make_shared<CourseCard>();
			}
			else if (id == L"teleporter") {
				card = make_shared<TeleportCard>();
			}
			card->Load(item);

			Add(card);
		}
	}
	shared_ptr<CardData> GimmickHand::Use() {
		auto card = Get();
		m_HandSprite[m_SelectIndex]->Remove();
		m_Hand.erase(m_Hand.begin() + m_SelectIndex);
		m_HandSprite.erase(m_HandSprite.begin() + m_SelectIndex);
		if (m_SelectIndex >= m_Hand.size()) {
			m_SelectIndex = m_Hand.size() - 1;
		}
		return card;
	}
	shared_ptr<CardData> GimmickHand::Get() {
		return m_Hand[m_SelectIndex];
	}
}
//end basecross
