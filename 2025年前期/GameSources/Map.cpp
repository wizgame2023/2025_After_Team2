/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void Map::OnCreate() {
		Object::OnCreate();
		//m_ColorTable = { Col4(1,0,0,1),Col4(0,1,0,1) };
	}
	void Map::OnUpdate() {
		auto device = App::GetApp()->GetInputDevice().GetControlerVec()[0];
		if (device.bConnected) {
			if (device.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP) {
				m_SelectColorIndex++;
				m_SelectColorIndex = min(m_ColorTable.size() - 1, m_SelectColorIndex);
			}
			if (device.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				m_SelectColorIndex--;
				m_SelectColorIndex = max(0, m_SelectColorIndex);
			}
			HighlightBox(m_ColorTable[m_SelectColorIndex]);
		}
	}

	void Map::Load() {
		int maxHeight = -100;

		//maxHeight = 0;
		//m_Map.push_back({});

		//for (int i = 0; i < 3; i++) {
		//	Col4 color;
		//	if (i == 0) {
		//		color = Col4(1.0f, 0.0f, 0.0f, 1.0f);
		//	}
		//	else if(i == 1) {
		//		color = Col4(0.0f, 0.0f, 0.0f, 0.0f);
		//	}
		//	else {
		//		color = Col4(0.0f, 1.0f, 0.0f, 1.0f);
		//	}

		//	auto box = m_Stage->AddGameObject<TempBox>(Vec3(i, m_GroundHeight - 0.5f, 0), color);
		//	box->SetScale(Vec3(1.0f, 0.1f, 1.0f));

		//	//m_Map[0].push_back({ color,0,Vec3(i, m_GroundHeight, 0),box,GimmickObjects::None,nullptr });
		//}

		Json mapJson;
		mapJson.Load(App::GetApp()->GetDataDirWString() + L"Level/level1.json");

		vector<int> mapSize = mapJson.At<JsonArray>(L"mapSize")->GetIntArray();
		auto mapData = mapJson.At<JsonArray>(L"map")->GetObjectArray();

		for (int i = 0; i < mapSize[1]; i++) {
			m_Map.push_back({});
			for (int j = 0; j < mapSize[0]; j++) {
				m_Map[i].push_back({ L"",0,Vec3(),nullptr,GimmickObjects::None,nullptr });
			}
		}

		for (auto& data : mapData) {
			vector<int> pos = data->At<JsonArray>(L"pos")->GetIntArray();
			float height = data->At<JsonNumber>(L"height")->GetIntValue();
			wstring colorStr = data->At<JsonString>(L"color")->GetValue();
			if (find(m_ColorTable.begin(), m_ColorTable.end(), colorStr) == m_ColorTable.end()) {
				m_ColorTable.push_back(colorStr);
			}

			if (maxHeight < height) {
				maxHeight = height;
			}

			Vec3 position = Vec3(pos[0], m_GroundHeight - 0.5f, pos[1]);
			auto box = m_Stage->AddGameObject<TempBox>(position, colorStr);
			box->SetScale(Vec3(1.0f, 0.1f, 1.0f));

			MapData m = { colorStr,height,position,box,GimmickObjects::None,nullptr };
			m_Map[pos[1]][pos[0]] = m;
		}
		for (int i = 0; i < mapSize[1]; i++) {
			for (int j = 0; j < mapSize[0]; j++) {
				if (m_Map[i][j].m_ColorStr == L"") {
					auto box = m_Stage->AddGameObject<TempBox>(Vec3(j, m_GroundHeight - 0.5f, i), L"clear");
					box->SetScale(Vec3(1.0f, 0.1f, 1.0f));
					m_Map[i][j].m_Temp = box;
				}
			}
		}

		/*for (int i = 0; i < 5; i++) {
			m_Map.push_back({});
			for (int j = 0; j < 5; j++) {
				int colorRnd = rand() % m_ColorTable.size();
				Col4 color = m_ColorTable[colorRnd];

				int height = rand() % 5;
				if (maxHeight < height) {
					maxHeight = height;
				}
				auto box = m_Stage->AddGameObject<TempBox>(Vec3(j, m_GroundHeight - 0.5f, i), color);
				box->SetScale(Vec3(1.0f, 0.1f, 1.0f));

				m_Map[i].push_back({ color,height,Vec3(j, m_GroundHeight, i),box,GimmickObjects::None,nullptr });
			}
		}*/

		m_MapHeight = maxHeight;
		m_CenterY = m_GroundHeight + static_cast<float>(maxHeight) / 2.0f;
	}

	void Map::HighlightBox(const wstring& colorText) {
		for (auto& mapVec : m_Map) {
			for (auto& map : mapVec) {
				auto box = map.m_Temp;
				auto draw = box->GetComponent<PNTStaticDraw>();
				if (colorText == map.m_ColorStr) {
					draw->SetDiffuse(box->GetDefaultColor() - Col4(0.3f, 0.3f, 0.3f, 0));
				}
				else if (map.m_TempGimmick) {
					draw->SetDiffuse(box->GetDefaultColor() + Col4(0.6f, 0.6f, 0.6f, 0.0f));
				}
				else{
					draw->SetDiffuse(box->GetDefaultColor());
				}
			}
		}
	}
	void Map::PutGimmick(GimmickObjects type) {
		wstring color = m_ColorTable[m_SelectColorIndex];
		for (auto& mapVec : m_Map) {
			for (auto& map : mapVec) {
				if (color == map.m_ColorStr) {
					//すでに設置しているなら破壊
					if (map.m_TempGimmick != nullptr) {
						continue;
					}
					map.m_TempGimmick = CreateGimmick(type);

					map.m_TempGimmick->SetPosition(map.m_Position + Vec3(0.0f, map.m_Height + 0.5f, 0.0f));
					map.m_TempGimmick->SetScale(Vec3(0.5f, 0.5f, 0.5f));
					map.m_GimmickType = type;

				}
			}
		}
	}
	GimmickObjects Map::RecoverGimmick() {
		wstring color = m_ColorTable[m_SelectColorIndex];
		GimmickObjects type = GimmickObjects::None;
		for (auto& mapVec : m_Map) {
			for (auto& map : mapVec) {
				if (color == map.m_ColorStr) {
					//すでに設置しているなら破壊
					if (map.m_TempGimmick != nullptr) {
						m_Stage->RemoveGameObject<Gimmicks>(map.m_TempGimmick);
						
						map.m_TempGimmick = nullptr;
						
						type = map.m_GimmickType;
					}
				}
			}
		}
		return type;
	}

	void TempBox::OnCreate() {
		Object::OnCreate();

		SetPosition(m_Position);

		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		
		Json colorJson = Json(App::GetApp()->GetDataDirWString() + L"Json/color.json");
		auto color = colorJson.At<JsonArray>(m_ColorStr)->GetFloatArray();
		m_DefaultColor = Col4(color[0], color[1], color[2], 1.0f);
		draw->SetDiffuse(m_DefaultColor);
	}
}
//end basecross
