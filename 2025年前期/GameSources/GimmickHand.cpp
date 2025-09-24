/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	map<Gimmicks::Objects, wstring> GimmickCard::m_CoverTexKeys = {};

	void GimmickCard::OnCreate() {
		Object::OnCreate();

		m_Card = GetStage()->AddGameObject<Sprite>(L"TEMP_GIMMICK",Vec3(0.0f),Vec2(),Anchor::Left);

		m_CoverTexKeys[Gimmicks::Objects::Goal] = L"TEMP_GIMMICK_GOAL";
		m_CoverTexKeys[Gimmicks::Objects::SetPlayer] = L"TEMP_GIMMICK_PLAYER";
		m_CoverTexKeys[Gimmicks::Objects::Upper] = L"TEMP_GIMMICK_UPPER";
		m_CoverTexKeys[Gimmicks::Objects::CourseCorrection] = L"TEMP_GIMMICK_COURSE";
		
		if (m_Type != Gimmicks::Objects::None) {
			SetCover(m_Type);
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
	void GimmickCard::SetCover(Gimmicks::Objects type) {
		m_CoverSprite = GetStage()->AddGameObject<Sprite>(m_CoverTexKeys[type], Vec3(0.0f), Vec2(), Anchor::Center);
		m_Type = type;
	}
	void GimmickHand::OnCreate() {

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
	Gimmicks::Objects GimmickHand::Use() {
		auto card = m_Hand[m_SelectIndex];
		m_HandSprite[m_SelectIndex]->Remove();
		m_Hand.erase(m_Hand.begin() + m_SelectIndex);
		m_HandSprite.erase(m_HandSprite.begin() + m_SelectIndex);
		if (m_SelectIndex >= m_Hand.size()) {
			m_SelectIndex = m_Hand.size() - 1;
		}
		return card;
	}
}
//end basecross
