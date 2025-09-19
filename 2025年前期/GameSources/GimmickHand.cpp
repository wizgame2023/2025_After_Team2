/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void GimmickCard::OnCreate() {
		Object::OnCreate();

		m_Card = GetStage()->AddGameObject<Sprite>(L"TEMP_GIMMICK",Vec3(0.0f),Vec2(),Anchor::Left);
	}
	void GimmickCard::OnUpdate() {

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
				offset += Vec3(0.0f, cardHalfScale.y, 0.0f);
			}

			card->ScreenAnchor(Anchor::BottomLeft, offset);
		}
	}
	shared_ptr<Gimmicks> GimmickHand::Use() {
		auto card = m_Hand[m_SelectIndex];
		GetStage()->RemoveGameObject<GimmickCard>(m_HandSprite[m_SelectIndex]);
		m_Hand.erase(m_Hand.begin() + m_SelectIndex);
		m_HandSprite.erase(m_HandSprite.begin() + m_SelectIndex);
		return card;
	}
}
//end basecross
