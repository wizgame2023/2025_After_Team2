/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class GimmickHand;

	class GameManager : public SingletonBase<GameManager> {
		friend class SingletonBase<GameManager>;
		GameManager():m_Tick(0.0f),m_UpdateTicks(2.0f) {
			
		}

		shared_ptr<Stage> m_Stage;

		vector<shared_ptr<MoveCube>> m_Balls;

		shared_ptr<GimmickHand> m_Hand;

		shared_ptr<Map> m_Map;

		vector<shared_ptr<Sprite>> m_EffectSprite;

		float m_Tick;
		float m_UpdateTicks;
		bool m_IsGameClear;
	public:
		/// <summary>
		/// 登録されている情報を初期化する
		/// </summary>
		void Reset() {
			m_Balls.clear();
			m_Hand = nullptr;
			m_Map = nullptr;
			m_Stage = nullptr;
			m_IsGameClear = false;
		}

		/// <summary>
		/// プレイヤーを登録
		/// </summary>
		/// <param name="sphere">プレイヤー</param>
		void AddBall(const shared_ptr<MoveCube> sphere) {
			m_Balls.push_back(sphere);
		}

		/// <summary>
		/// プレイヤーの情報を取得
		/// </summary>
		/// <returns>プレイヤーデータ</returns>
		vector<shared_ptr<MoveCube>> GetBalls()const {
			return m_Balls;
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

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();


		void DrawGoalEffect();

		void DeleteEffectSprite() {
			for (int i = 0; i < m_EffectSprite.size(); i++) {
				m_Stage->RemoveGameObject<Sprite>(m_EffectSprite[i]);
			}
			m_EffectSprite.clear();
		}
	};
}
//end basecross
