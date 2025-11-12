/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	void GimmickHand::OnCreate() {
		Object::OnCreate();
	}
	void GimmickHand::OnUpdate() {

	}
	void GimmickHand::LoadHands(shared_ptr<JsonArray>& items) {
		auto itemArray = items->GetObjectArray();
		for (auto& item : itemArray) {
			auto id = item->At<JsonString>(L"id")->GetValue();
			auto count = item->At<JsonNumber>(L"count")->GetIntValue();

			shared_ptr<CardData> card;
			card = CardFactory::Create(id);
			if (!card) continue;
			card->Load(item);

			Add(card);
		}
	}
	shared_ptr<CardData> GimmickHand::Use() {
		auto card = Get();
		m_Hand.erase(m_Hand.begin() + m_SelectIndex);
		if (m_SelectIndex >= m_Hand.size()) {
			m_SelectIndex = m_Hand.size() - 1;
		}
		return card;
	}
	shared_ptr<CardData> GimmickHand::Get() {
		return m_Hand[m_SelectIndex];
	}
	shared_ptr<CardData> GimmickHand::Get(int index) {
		return m_Hand[index];
	}
}
//end basecross
