/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "Sprite.h"
namespace basecross{
	class GimmickHand;
	
	enum class GameState {
		Put,Game,Over,Clear
	};

	class GameManager : public SingletonBase<GameManager> {
		friend class SingletonBase<GameManager>;
		GameManager():m_Tick(0.0f),m_UpdateTicks(2.0f) {
			
		}

		shared_ptr<Stage> m_Stage;

		vector<shared_ptr<MoveCube>> m_Cubes;

		shared_ptr<GimmickHand> m_Hand;

		shared_ptr<Map> m_Map;

		vector<shared_ptr<Sprite>> m_EffectSprite;

		map<wstring, Vec3> m_DirectionMap;

		float m_Tick;
		float m_UpdateTicks;
		bool m_IsGameClear;
		GameState m_GameState;

		void DrawTempGimmicks();
	public:
		/// <summary>
		/// 登録されている情報を初期化する
		/// </summary>
		void Reset() {
			m_Cubes.clear();
			m_Hand = nullptr;
			m_Map = nullptr;
			m_Stage = nullptr;
			m_IsGameClear = false;
			m_GameState = GameState::Put;

			m_DirectionMap[L"south"] = Vec3(0, 0, -1);
			m_DirectionMap[L"north"] = Vec3(0, 0, 1);
			m_DirectionMap[L"east"] = Vec3(1, 0, 0);
			m_DirectionMap[L"west"] = Vec3(-1, 0, 0);
		}

		/// <summary>
		/// プレイヤーを登録
		/// </summary>
		/// <param name="sphere">プレイヤー</param>
		void AddCube(const shared_ptr<MoveCube> sphere) {
			m_Cubes.push_back(sphere);
		}

		/// <summary>
		/// プレイヤーの情報を取得
		/// </summary>
		/// <returns>プレイヤーデータ</returns>
		vector<shared_ptr<MoveCube>> GetCubes()const {
			return m_Cubes;
		}
		void DeleteCube(shared_ptr<MoveCube>& cube) {
			auto it = find(m_Cubes.begin(), m_Cubes.end(), cube);
			if (it != m_Cubes.end()) {
				m_Cubes.erase(it);
				m_Stage->RemoveGameObject<MoveCube>(cube);
			}
		}

		/// <summary>
		/// 手札を登録
		/// </summary>
		/// <param name="hand">手札</param>
		void SetHand(shared_ptr<GimmickHand> hand) {
			m_Hand = hand;
		}

		/// <summary>
		/// 手札を取得
		/// </summary>
		/// <returns>手札</returns>
		shared_ptr<GimmickHand> GetHand()const {
			return m_Hand;
		}

		/// <summary>
		/// ステージマップを登録
		/// </summary>
		/// <param name="map">ステージマップ</param>
		void SetMap(shared_ptr<Map> map) {
			m_Map = map;
		}

		/// <summary>
		/// ステージマップを取得
		/// </summary>
		/// <returns>ステージマップ</returns>
		shared_ptr<Map> GetMap() const{
			return m_Map;
		}

		void SetGameStage(const shared_ptr<Stage>& stage) {
			m_Stage = stage;
		}

		bool IsClear() {
			return m_IsGameClear;
		}

		bool CompareState(GameState state) {
			return m_GameState == state;
		}

		Vec3 DirectionStrToVec(const wstring& str) {
			return m_DirectionMap[str];
		}

		void Start();
		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();


		void DrawGoalEffect();
		void DrawOverEffect();

		void DeleteEffectSprite() {
			for (int i = 0; i < m_EffectSprite.size(); i++) {
				m_Stage->RemoveGameObject<Sprite>(m_EffectSprite[i]);
			}
			m_EffectSprite.clear();
		}
	};
}
//end basecross
