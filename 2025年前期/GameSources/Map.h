/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "GimmickHand.h"
namespace basecross{
	class Floor;

	struct MapData {
		wstring m_ColorStr;
		int m_Height;
		Vec3 m_Position;

		shared_ptr<Floor> m_Floor;
		shared_ptr<CardData> m_GimmickType;
		shared_ptr<Gimmicks> m_Gimmik;
	};
	class Map : public Object {
		vector<vector<MapData>> m_Map;
		int m_MapHeight;
		int m_CenterY;
		float m_GroundHeight;

		vector<wstring> m_ColorTable;

		int m_SelectColorIndex;
	public:
		Map(const shared_ptr<Stage>& ptr) : Object(ptr), m_SelectColorIndex(0), m_MapHeight(0), m_CenterY(0), m_GroundHeight(0){}
		virtual ~Map(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void Load();

		template<class GimmickType>
		vector<shared_ptr<GimmickType>> GetGimmicks() {
			vector<shared_ptr<GimmickType>> gimmicks;
			for (auto& mapVec : m_Map) {
				for (auto& map : mapVec) {
					if (auto casted = dynamic_pointer_cast<GimmickType>(map.m_Gimmik)) {
						gimmicks.push_back(casted);
					}
				}
			}
			return gimmicks;
		}
		vector<shared_ptr<Gimmicks>> GetGimmicks() {
			vector<shared_ptr<Gimmicks>> gimmicks;
			for (auto& mapVec : m_Map) {
				for (auto& map : mapVec) {
					if (map.m_Gimmik) {
						gimmicks.push_back(map.m_Gimmik);
					}
				}
			}
			return gimmicks;
		}

		vector<vector<MapData>>& GetMapData() {
			return m_Map;
		}

		MapData GetMapData(Vec2 pos) {
			int x = static_cast<int>(pos.x);
			int y = static_cast<int>(pos.y);
			if (y >= m_Map.size() || x >= m_Map[y].size()) return {};
			return m_Map[y][x];
		}
		Vec3 GetMapCenter() {
			Vec2 size = Vec2(static_cast<float>(m_Map[0].size() - 1), static_cast<float>(m_Map.size() - 1));
			return Vec3(size.x / 2.0f, m_CenterY, -size.y / 2.0f);
		}
		Vec3 GetMapSize() {
			return Vec3(static_cast<float>(m_Map[0].size()), m_MapHeight, static_cast<float>(m_Map.size()));
		}

		vector<wstring> GetColorTable() {
			return m_ColorTable;
		}
		wstring GetSelectColor() {
			if (m_SelectColorIndex >= m_ColorTable.size()) return L"";
			return m_ColorTable[m_SelectColorIndex];
		}
		void SetGroundHeight(float height) {
			m_GroundHeight = height;
		}

		bool CheckPutGimmick() {
			wstring color = m_ColorTable[m_SelectColorIndex];
			for (auto& mapVec : m_Map) {
				for (auto& map : mapVec) {
					if (color == map.m_ColorStr) {
						if (map.m_Gimmik != nullptr) {
							return true;
						}
					}
				}
			}
			return false;
		}

		void HighlightBox(const wstring& colorText);

		//引数にはギミックのオブジェクト
		void PutGimmick(shared_ptr<CardData>& type);
		void PutGimmick(int colorIdx,shared_ptr<CardData>& type);

		shared_ptr<CardData> RecoverGimmick();
		shared_ptr<CardData> RecoverGimmick(int colorIdx);
	};



	class Floor : public Object {
		Vec3 m_Position;
		wstring m_ColorStr;
		Col4 m_DefaultColor;
	public:
		Floor(const shared_ptr<Stage>& ptr,Vec3 position, wstring color) : Object(ptr), m_Position(position),m_ColorStr(color){}
		virtual ~Floor(){}

		virtual void OnCreate()override;

		Col4 GetDefaultColor()const {
			return m_DefaultColor;
		}
	};
}
//end basecross
