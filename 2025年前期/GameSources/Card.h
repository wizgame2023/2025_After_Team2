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
		CardData(const wstring& id) : m_Id(id) {}
		virtual void Load(shared_ptr<JsonObject>& data) {}
		virtual GimmickObjects GetType() { return GimmickObjects::None; }
		virtual wstring GetExpainKey() { return L""; }
		virtual shared_ptr<Gimmicks> CreateGimmick(shared_ptr<Stage>& stage) { return nullptr; }
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

		virtual void Load(shared_ptr<JsonObject>& data) {

		}

		virtual GimmickObjects GetType() { return GimmickObjects::Inverter; }

		virtual wstring GetExpainKey() { return L""; }

		virtual shared_ptr<Gimmicks> CreateGimmick(shared_ptr<Stage>& stage)override {

			auto gimmick = stage->AddGameObject<GimmickInverter>();

			return gimmick;

		}

	};

	class TeleportCard : public CardData {

		Vec3 m_TeleportTarget;

	public:

		TeleportCard(const wstring& id) : CardData(id) {}

		virtual void Load(shared_ptr<JsonObject>& data) {

			auto target = data->At<JsonArray>(L"target")->GetFloatArray();

			m_TeleportTarget = Vec3(target[0], target[1], target[2]);

		}

		virtual GimmickObjects GetType() { return GimmickObjects::Teleporter; }

		virtual wstring GetExpainKey() { return L""; }

		virtual shared_ptr<Gimmicks> CreateGimmick(shared_ptr<Stage>& stage)override {

			auto gimmick = stage->AddGameObject<GimmickTeleporter>();

			gimmick->SetValue(m_TeleportTarget);

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

}

