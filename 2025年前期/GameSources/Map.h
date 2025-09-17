/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	struct MapData {
		Col4 m_Color;
		int m_Height;
	};
	class Map : public Object {
		vector<vector<MapData>> m_Map;
	public:
		Map(const shared_ptr<Stage>& ptr) : Object(ptr){}
		virtual ~Map(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void Load();
	};
}
//end basecross
