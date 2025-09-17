/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class TempBox;
	struct MapData {
		Col4 m_Color;
		int m_Height;
		Vec3 m_Position;

		shared_ptr<TempBox> m_Temp;
		shared_ptr<TempBox> m_TempGimmick;
	};
	class Map : public Object {
		vector<vector<MapData>> m_Map;
		int m_MapHeight;
		int m_CenterY;
		float m_GroundHeight;

		vector<Col4> m_ColorTable;

		int m_SelectColorIndex;
	public:
		Map(const shared_ptr<Stage>& ptr) : Object(ptr), m_SelectColorIndex(0), m_MapHeight(0), m_CenterY(0), m_GroundHeight(0){}
		virtual ~Map(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void Load();

		Vec3 GetMapCenter() {
			Vec2 size = Vec2(m_Map[0].size() - 1, m_Map.size() - 1);
			return Vec3(size.x / 2.0f, m_CenterY, size.x / 2.0f);
		}
		Vec3 GetMapSize() {
			return Vec3(m_Map[0].size(), m_MapHeight, m_Map.size());
		}

		vector<Col4> GetColorTable() {
			return m_ColorTable;
		}
		void SetGroundHeight(float height) {
			m_GroundHeight = height;
		}

		void HighlightBox(Col4 color);

		//引数にはギミックのオブジェクト
		void PutGimmick(int i,Col4 color);
	};



	class TempBox : public Object {
		Vec3 m_Position;
		Col4 m_Color;
	public:
		TempBox(const shared_ptr<Stage>& ptr,Vec3 position,Col4 color) : Object(ptr), m_Position(position),m_Color(color){}
		virtual ~TempBox(){}

		virtual void OnCreate()override;
	};
}
//end basecross
