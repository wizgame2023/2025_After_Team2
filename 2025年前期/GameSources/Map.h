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
		vector<Col4> m_ColorTable;
	public:
		Map(const shared_ptr<Stage>& ptr) : Object(ptr){}
		virtual ~Map(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void Load();

		Vec3 GetMapSize() {
			return Vec3(m_Map[0].size(), 0, m_Map.size());
		}

		vector<Col4> GetColorTable() {
			return m_ColorTable;
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
