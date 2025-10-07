#pragma once
#include "stdafx.h"
#include "Json.h"
#include "GameManager.h"
namespace basecross {

	class CardData {
	public:
		CardData() = default;
		virtual void Load(shared_ptr<JsonObject>& data) {}
		virtual GimmickObjects GetType() { return GimmickObjects::None; }
		virtual shared_ptr<Gimmicks> CreateGimmick(shared_ptr<Stage>& stage) { return nullptr; }
	};
	class PlayerCard : public CardData {
	public:
		Vec3 m_Velocity;
		virtual void Load(shared_ptr<JsonObject>& data) {
			auto str = data->At<JsonString>(L"direction")->GetValue();
			m_Velocity = GameManager::GetInstance().DirectionStrToVec(str);
		}
		virtual GimmickObjects GetType() { return GimmickObjects::SetPlayer; }

		virtual shared_ptr<Gimmicks> CreateGimmick(shared_ptr<Stage>& stage)override {
			auto gimmick = stage->AddGameObject<GimmickSetPlayer>();
			gimmick->SetValue(m_Velocity);
			return gimmick;
		}

	};
	class GoalCard : public CardData {
	public:
		Vec3 m_Direction;
		virtual void Load(shared_ptr<JsonObject>& data) {
			auto str = data->At<JsonString>(L"direction")->GetValue();
			m_Direction = GameManager::GetInstance().DirectionStrToVec(str);
		}
		virtual GimmickObjects GetType() { return GimmickObjects::Goal; }
		virtual shared_ptr<Gimmicks> CreateGimmick(shared_ptr<Stage>& stage)override {
			auto gimmick = stage->AddGameObject<GimmickGoal>();
			gimmick->SetValue(m_Direction);
			return gimmick;
		}
	};
	class CourseCard : public CardData {
	public:
		Vec3 m_Direction;
		virtual void Load(shared_ptr<JsonObject>& data) {
			auto str = data->At<JsonString>(L"direction")->GetValue();
			m_Direction = GameManager::GetInstance().DirectionStrToVec(str);
		}
		virtual GimmickObjects GetType() { return GimmickObjects::CourseCorrection; }
		virtual shared_ptr<Gimmicks> CreateGimmick(shared_ptr<Stage>& stage)override {
			auto gimmick = stage->AddGameObject<GimmickCourseCorrection>();
			gimmick->SetValue(m_Direction);
			return gimmick;
		}
	};

	class InverterCard : public CardData {
	public:
		virtual void Load(shared_ptr<JsonObject>& data) {
		}
		virtual GimmickObjects GetType() { return GimmickObjects::Inverter; }
		virtual shared_ptr<Gimmicks> CreateGimmick(shared_ptr<Stage>& stage)override {
			auto gimmick = stage->AddGameObject<GimmickInverter>();
			return gimmick;
		}

	};

	class TeleportCard : public CardData {
	public:
		Vec3 m_TeleportTarget;
		virtual void Load(shared_ptr<JsonObject>& data) {
			auto target = data->At<JsonArray>(L"target")->GetFloatArray();
			m_TeleportTarget = Vec3(target[0], target[1], target[2]);
		}
		virtual GimmickObjects GetType() { return GimmickObjects::Teleporter; }
		virtual shared_ptr<Gimmicks> CreateGimmick(shared_ptr<Stage>& stage)override {
			auto gimmick = stage->AddGameObject<GimmickTeleporter>();
			gimmick->SetValue(m_TeleportTarget);
			return gimmick;
		}

	};


	class CardFactory {
		using Factory = function<shared_ptr<CardData>()>;
	public:
		static bool Register(const wstring& name, Factory func) {
			GetMap()[name] = func;
			return true;
		}
		static shared_ptr<CardData> Create(const wstring& name) {
			auto& map = GetMap();
			if (map.find(name) != map.end()) {
				return map[name]();
			}
			return nullptr;
		}

		static map<wstring, Factory>& GetMap() {
			static map<wstring, Factory> map;
			return map;
		}
	};
#define REGISTER_CARD(name,type) \
	static bool reg_##type = CardFactory::Register(name, []() {return make_shared<type>(); });

	REGISTER_CARD(L"player",PlayerCard)
	REGISTER_CARD(L"goal", GoalCard)
	REGISTER_CARD(L"course", CourseCard)
	REGISTER_CARD(L"teleporter", TeleportCard)
	REGISTER_CARD(L"inverter", InverterCard)
}
