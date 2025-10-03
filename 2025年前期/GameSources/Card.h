#pragma once
#include "stdafx.h"
#include "Json.h"
#include "GameManager.h"
namespace basecross {
	struct CardData {
		CardData() = default;
		virtual void Load(shared_ptr<JsonObject>& data) {}
		virtual GimmickObjects GetType() { return GimmickObjects::None; }
	};
	struct PlayerCard : public CardData {
		Vec3 m_Velocity;
		virtual void Load(shared_ptr<JsonObject>& data) {
			auto str = data->At<JsonString>(L"direction")->GetValue();
			m_Velocity = GameManager::GetInstance().DirectionStrToVec(str);
		}
		virtual GimmickObjects GetType() { return GimmickObjects::SetPlayer; }

	};
	struct GoalCard : public CardData {
		Vec3 m_Direction;
		virtual void Load(shared_ptr<JsonObject>& data) {
			auto str = data->At<JsonString>(L"direction")->GetValue();
			m_Direction = GameManager::GetInstance().DirectionStrToVec(str);
		}
		virtual GimmickObjects GetType() { return GimmickObjects::Goal; }
	};
	struct CourseCard : public CardData {
		Vec3 m_Direction;
		virtual void Load(shared_ptr<JsonObject>& data) {
			auto str = data->At<JsonString>(L"direction")->GetValue();
			m_Direction = GameManager::GetInstance().DirectionStrToVec(str);
		}
		virtual GimmickObjects GetType() { return GimmickObjects::CourseCorrection; }
	};
}
