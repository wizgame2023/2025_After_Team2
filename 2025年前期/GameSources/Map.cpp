/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void Map::OnCreate() {
		Object::OnCreate();
		m_ColorTable = { Col4(1,0,0,1),Col4(0,1,0,1),Col4(0,0,1,1),Col4(1,1,0,1),Col4(1,0,1,1) };
	}
	void Map::OnUpdate() {
		
	}

	void Map::Load() {
		for (int i = 0; i < 5; i++) {
			m_Map.push_back({});
			for (int j = 0; j < 5; j++) {
				int colorRnd = rand() % m_ColorTable.size();
				Col4 color = m_ColorTable[colorRnd];

				int height = rand() % 5;

				auto box = GetStage()->AddGameObject<TempBox>(Vec3(j, -5.0f, i), color);
				box->SetScale(Vec3(1.0f, 0.1f, 1.0f));

				m_Map[i].push_back({ color,height,Vec3(j, -5.0f, i),box,nullptr });

				
			}
		}
	}

	void Map::HighlightBox(Col4 color) {
		for (auto& mapVec : m_Map) {
			for (auto& map : mapVec) {
				auto box = map.m_Temp;
				auto draw = box->GetComponent<PNTStaticDraw>();
				if (color == map.m_Color) {
					draw->SetDiffuse(map.m_Color - Col4(0.3f,0.3f,0.3f,0));
				}
				else {
					draw->SetDiffuse(map.m_Color);
				}
			}
		}
	}
	void Map::PutGimmick(int i,Col4 color) {
		for (auto& mapVec : m_Map) {
			for (auto& map : mapVec) {
				if (color == map.m_Color) {
					//すでに設置しているなら破壊
					if (map.m_TempGimmick != nullptr) {
						GetStage()->RemoveGameObject<TempBox>(map.m_TempGimmick);
					}
					map.m_TempGimmick = GetStage()->AddGameObject<TempBox>(
						map.m_Position + Vec3(0.0f, map.m_Height, 0.0f),
						Col4(map.m_Color.x, map.m_Color.y, map.m_Color.z, 0.5f));

				}
			}
		}
	}


	void TempBox::OnCreate() {
		Object::OnCreate();

		SetPosition(m_Position);

		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		draw->SetDiffuse(m_Color);
	}
}
//end basecross
