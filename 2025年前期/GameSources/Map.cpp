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
		auto& input = InputManager::GetInputManager();
		auto& gameManager = GameManager::GetInstance();

		HighlightBox(m_ColorTable[m_SelectColorIndex]);
	}

	void Map::Load(Json& map) {
		int maxHeight = -100;

		vector<int> mapSize = map.At<JsonArray>(L"mapSize")->GetIntArray();
		auto mapData = map.At<JsonArray>(L"map")->GetObjectArray();
		//データの初期化
		for (int i = 0; i < mapSize[1]; i++) {
			m_Map.push_back({});
			for (int j = 0; j < mapSize[0]; j++) {
				m_Map[i].push_back({ L"",0,Vec3(),nullptr,nullptr,nullptr });
			}
		}
		//データの読み込み
		for (auto& data : mapData) {
			wstring colorStr = GetFloorColor(data);

			shared_ptr<JsonArray> positionArray = data->At<JsonArray>(L"pos");
			vector<vector<int>> floorArea = { {0,0},{0,0} };
			if (positionArray) {
				floorArea[0] = positionArray->GetIntArray();
				floorArea[1] = positionArray->GetIntArray();
			}
			else {
				shared_ptr<JsonArray> topLeft = data->At<JsonArray>(L"posTL");
				shared_ptr<JsonArray> bottomRight = data->At<JsonArray>(L"posBR");

				if (!topLeft || !bottomRight) continue;

				floorArea[0] = topLeft->GetIntArray();
				floorArea[1] = bottomRight->GetIntArray();
			}
			auto gimmick = data->At<JsonObject>(L"gimmick");

			for (int y = floorArea[0][1]; y <= floorArea[1][1]; y++) {
				for (int x = floorArea[0][0]; x <= floorArea[1][0]; x++) {
					CreateFloor(vector<int>{ x,y }, colorStr, gimmick);
				}
			}

			//vector<int> pos = data->At<JsonArray>(L"pos")->GetIntArray();
			//float height = 0;
			//wstring colorStr = L"clear";
			//auto colorData = data->At<JsonString>(L"color");
			//if (colorData) {
			//	colorStr = colorData->GetValue();
			//	//新しい色が来たら追加
			//	if (colorStr != L"black" && find(m_ColorTable.begin(), m_ColorTable.end(), colorStr) == m_ColorTable.end()) {
			//		m_ColorTable.push_back(colorStr);
			//	}
			//}
			//else {
			//	colorStr = L"black";
			//}

			//auto gimmick = data->At<JsonObject>(L"gimmick");
			
			////高さの最大値を更新
			//if (maxHeight < height) {
			//	maxHeight = height;
			//}


			//グリッドの生成
		//	Vec3 position = Vec3(pos[0], m_GroundHeight - 0.5f, -pos[1]);
		//	shared_ptr<Floor> box = nullptr;
		//	if (colorStr != L"black") {
		//		box = m_Stage->AddGameObject<Floor>(position, colorStr);
		//		box->SetScale(Vec3(1.0f, 0.1f, 1.0f));
		//	}

		//	wstring gimmickId = L"";
		//	shared_ptr<Gimmicks> defaultGimmick = nullptr;
		//	shared_ptr<CardData> defaultCard = nullptr;
		//	if (gimmick) {
		//		gimmickId = gimmick->At<JsonString>(L"id")->GetValue();

		//		defaultCard = CardFactory::Create(gimmickId);
		//		if (defaultCard) {
		//			defaultCard->Load(gimmick);

		//			defaultGimmick = defaultCard->CreateGimmick(GetStage());
		//			defaultGimmick->SetPosition(position + Vec3(0.0f, height + 0.5f, 0.0f));
		//			defaultGimmick->SetScale(Vec3(0.5f, 0.5f, 0.5f));
		//		}
		//	}

		//	MapData m = { colorStr,height,position,box,defaultCard,defaultGimmick };
		//	m_Map[pos[1]][pos[0]] = m;
		//}
		}
		//無色部分の生成
		for (int i = 0; i < mapSize[1]; i++) {
			for (int j = 0; j < mapSize[0]; j++) {
				if (m_Map[i][j].m_ColorStr == L"") {
					CreateFloor(vector<int>{j, i}, L"clear", nullptr);
					/*auto box = m_Stage->AddGameObject<Floor>(Vec3(j, m_GroundHeight - 0.5f, -i), L"clear");
					box->SetScale(Vec3(1.0f, 0.1f, 1.0f));
					m_Map[i][j].m_Floor = box;*/
				}
			}
		}
		//m_MapHeight = maxHeight;
		//m_CenterY = m_GroundHeight + static_cast<float>(maxHeight) / 2.0f;
	}
	wstring Map::GetFloorColor(const shared_ptr<JsonObject>& data) {
		wstring colorStr = L"clear";
		auto colorData = data->At<JsonString>(L"color");
		if (colorData) {
			colorStr = colorData->GetValue();
			//新しい色が来たら追加
			if (colorStr != L"black" && find(m_ColorTable.begin(), m_ColorTable.end(), colorStr) == m_ColorTable.end()) {
				m_ColorTable.push_back(colorStr);
			}
		}
		else {
			colorStr = L"black";
		}
		return colorStr;
	}
	void Map::CreateFloor(vector<int>& cellPosition, const wstring& color, const shared_ptr<JsonObject>& gimmickObject) {
		Vec3 position = Vec3(cellPosition[0], m_GroundHeight - 0.5f, -cellPosition[1]);
		shared_ptr<Floor> box = nullptr;
		if (color != L"black") {
			box = m_Stage->AddGameObject<Floor>(position, color);
			box->SetScale(Vec3(1.0f, 0.1f, 1.0f));
		}

		wstring gimmickId = L"";
		shared_ptr<Gimmicks> defaultGimmick = nullptr;
		shared_ptr<CardData> defaultCard = nullptr;
		if (gimmickObject) {
			gimmickId = gimmickObject->At<JsonString>(L"id")->GetValue();

			defaultCard = CardFactory::Create(gimmickId);
			if (defaultCard) {
				defaultCard->Load(gimmickObject);

				defaultGimmick = defaultCard->CreateGimmick(GetStage());
				defaultGimmick->SetPosition(position + Vec3(0.0f, 0.5f, 0.0f));
				defaultGimmick->SetScale(Vec3(0.5f, 0.5f, 0.5f));
			}
		}

		MapData m = { color,0,position,box,defaultCard,defaultGimmick };
		m_Map[cellPosition[1]][cellPosition[0]] = m;
	}

	void Map::HighlightBox(const wstring& colorText) {
		for (auto& mapVec : m_Map) {
			for (auto& map : mapVec) {
				auto box = map.m_Floor;
				if (!box) continue;
				auto draw = box->GetComponent<PNTStaticDraw>();
				if (colorText == map.m_ColorStr) {
					draw->SetDiffuse(box->GetDefaultColor() - Col4(0.3f, 0.3f, 0.3f, 0));
				}
				else{
					draw->SetDiffuse(box->GetDefaultColor());
				}
			}
		}
	}
	void Map::PutGimmick(shared_ptr<CardData>& type) {
		PutGimmick(m_SelectColorIndex, type);
	}
	void Map::PutGimmick(int colorIdx,shared_ptr<CardData>& type) {
		wstring colorStr = m_ColorTable[colorIdx];
		for (auto& mapVec : m_Map) {
			for (auto& map : mapVec) {
				if (colorStr == map.m_ColorStr) {
					if (map.m_Gimmick != nullptr) {
						continue;
					}
					map.m_Gimmick = type->CreateGimmick(GetStage());

					map.m_Gimmick->SetPosition(map.m_Position + Vec3(0.0f, map.m_Height + 0.5f, 0.0f));
					map.m_Gimmick->SetScale(Vec3(0.5f, 0.5f, 0.5f));
					map.m_GimmickType = type;

				}
			}
		}
	}
	shared_ptr<CardData> Map::RecoverGimmick() {
		return RecoverGimmick(m_SelectColorIndex);
	}
	shared_ptr<CardData> Map::RecoverGimmick(int colorIdx) {
		wstring color = m_ColorTable[colorIdx];
		shared_ptr<CardData> type;
		for (auto& mapVec : m_Map) {
			for (auto& map : mapVec) {
				if (color == map.m_ColorStr) {
					//すでに設置しているなら破壊
					if (map.m_Gimmick != nullptr) {
						map.m_Gimmick->GimmickDelete();

						map.m_Gimmick = nullptr;

						type = map.m_GimmickType;
					}
				}
			}
		}
		return type;
	}

	void Floor::OnCreate() {
		Object::OnCreate();

		SetPosition(m_Position);

		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		draw->SetTextureResource(L"FLOOR");
		
		Json colorJson = Json(L"Json/color.json");
		auto color = colorJson.At<JsonArray>(m_ColorStr)->GetFloatArray();
		m_DefaultColor = Col4(color[0], color[1], color[2], 1.0f);
		draw->SetDiffuse(m_DefaultColor);
	}
}
//end basecross
