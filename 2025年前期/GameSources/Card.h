#pragma once

#include "stdafx.h"

#include "Json.h"

#include "GameManager.h"

namespace basecross {

	class CardData {
	protected:
		wstring m_Id;
	public:
		CardData() = default;
		CardData(const wstring& id): m_Id(id){}
		virtual void Load(shared_ptr<JsonObject>& data) {}
		virtual GimmickObjects GetType() { return GimmickObjects::None; }
		virtual wstring GetExpainKey() { return L""; }
		virtual shared_ptr<Gimmicks> CreateGimmick(shared_ptr<Stage>& stage) { return nullptr; }
		virtual Vec3 GetVelocity() { return {}; }
		virtual int GetCount() { return 0; }
		wstring GetId() { return m_Id; }
	};

	class PlayerCard : public CardData {
		Vec3 m_Velocity;
	public:
		PlayerCard(const wstring& id) : CardData(id) {}
		virtual void Load(shared_ptr<JsonObject>& data) {
			auto str = data->At<JsonString>(L"direction")->GetValue();
			m_Velocity = GameManager::GetInstance().DirectionStrToVec(str);
		}
		virtual GimmickObjects GetType() { return GimmickObjects::SetPlayer; }
		virtual wstring GetExpainKey() { return L"EXPAIN_PL"; }
		virtual Vec3 GetVelocity() { return m_Velocity; }
		virtual shared_ptr<Gimmicks> CreateGimmick(shared_ptr<Stage>& stage)override {

			auto gimmick = stage->AddGameObject<GimmickSetPlayer>();

			gimmick->SetValue(m_Velocity);

			return gimmick;

		}


	};
	class GoalCard : public CardData {
		Vec3 m_Direction;
	public:
		GoalCard(const wstring& id) : CardData(id) {}
		virtual void Load(shared_ptr<JsonObject>& data) {
			auto str = data->At<JsonString>(L"direction")->GetValue();
			m_Direction = GameManager::GetInstance().DirectionStrToVec(str);
		}

		virtual GimmickObjects GetType() { return GimmickObjects::Goal; }
		virtual wstring GetExpainKey() { return L"EXPAIN_GL"; }
		virtual Vec3 GetVelocity() { return m_Direction; }
		virtual shared_ptr<Gimmicks> CreateGimmick(shared_ptr<Stage>& stage)override {
			auto gimmick = stage->AddGameObject<GimmickGoal>();
			gimmick->SetValue(m_Direction);
			return gimmick;
		}

	};

	class CourseCard : public CardData {
		Vec3 m_Direction;
	public:
		CourseCard(const wstring& id) : CardData(id) {}
		virtual void Load(shared_ptr<JsonObject>& data) {
			auto str = data->At<JsonString>(L"direction")->GetValue();
			m_Direction = GameManager::GetInstance().DirectionStrToVec(str);
		}
		virtual GimmickObjects GetType() { return GimmickObjects::CourseCorrection; }
		virtual wstring GetExpainKey() { return L"EXPAIN_ARRW"; }
		virtual Vec3 GetVelocity() { return m_Direction; }
		virtual shared_ptr<Gimmicks> CreateGimmick(shared_ptr<Stage>& stage)override {
			auto gimmick = stage->AddGameObject<GimmickCourseCorrection>();
			gimmick->SetValue(m_Direction);
			gimmick->SetCount(1);
			return gimmick;
		}
	};

	class InverterCard : public CardData {
	public:
		InverterCard(const wstring& id) : CardData(id) {}
		virtual void Load(shared_ptr<JsonObject>& data) {}
		virtual GimmickObjects GetType() { return GimmickObjects::Inverter; }
		virtual wstring GetExpainKey() { return L"EXPAIN_INV"; }
		virtual shared_ptr<Gimmicks> CreateGimmick(shared_ptr<Stage>& stage)override {
			auto gimmick = stage->AddGameObject<GimmickInverter>();
			return gimmick;
		}
	};

	class TeleportCard : public CardData {
		Vec3 m_TeleportTarget;
		int m_Length;
	public:
		TeleportCard(const wstring& id) : CardData(id) {}
		virtual void Load(shared_ptr<JsonObject>& data) {
			auto str = data->At<JsonString>(L"direction")->GetValue();
			auto direction = GameManager::GetInstance().DirectionStrToVec(str);
			m_Length = data->At<JsonNumber>(L"value")->GetIntValue();
			m_TeleportTarget = direction * m_Length;
		}

		virtual GimmickObjects GetType() { return GimmickObjects::Teleporter; }
		virtual wstring GetExpainKey() { return L"EXPAIN_TP"; }
		virtual Vec3 GetVelocity() { return (Vec3)XMVector3Normalize(m_TeleportTarget); }
		virtual int GetCount() { return m_Length; }
		virtual shared_ptr<Gimmicks> CreateGimmick(shared_ptr<Stage>& stage)override {
			auto gimmick = stage->AddGameObject<GimmickTeleporter>();
			gimmick->SetValue(m_TeleportTarget);
			return gimmick;
		}

	};

	class RollCard : public CardData {
		int m_RollCount;
	public:
		RollCard(const wstring& id): CardData(id){}
		virtual GimmickObjects GetType() { return GimmickObjects::Roll; }
		virtual void Load(shared_ptr<JsonObject>& data) {
			m_RollCount = data->At<JsonNumber>(L"value")->GetIntValue();
		}
		virtual wstring GetExpainKey() { return L"EXPAIN_ROLL"; }
		virtual int GetCount() { return m_RollCount; }
		virtual shared_ptr<Gimmicks> CreateGimmick(shared_ptr<Stage>& stage)override {
			auto gimmick = stage->AddGameObject<GimmickRoll>();
			gimmick->SetCount(m_RollCount);
			return gimmick;
		}
	};


	class CardFactory {
		using Factory = function<shared_ptr<CardData>(const wstring&)>;
	public:
		static bool Register(const wstring& name, Factory func) {
			GetMap()[name] = func;
			return true;
		}

		static shared_ptr<CardData> Create(const wstring& name) {
			auto& map = GetMap();
			if (map.find(name) != map.end()) {
				return map[name](name);
			}
			return nullptr;
		}

		static map<wstring, Factory>& GetMap() {
			static map<wstring, Factory> map;
			return map;
		}
	};

#define REGISTER_CARD(name,type) \
	static bool reg_##type = CardFactory::Register(name, [](const wstring& id) {return make_shared<type>(id); });

		REGISTER_CARD(L"player", PlayerCard)
		REGISTER_CARD(L"goal", GoalCard)
		REGISTER_CARD(L"course", CourseCard)
		REGISTER_CARD(L"teleporter", TeleportCard)
		REGISTER_CARD(L"inverter", InverterCard)
		REGISTER_CARD(L"roll",RollCard)

}

