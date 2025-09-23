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
		GameManager() = default;

		vector<shared_ptr<MoveSphere>> m_Spheres;

		shared_ptr<GimmickHand> m_Hand;

		shared_ptr<Map> m_Map;

		float m_Tick;
	public:
		/// <summary>
		/// 登録されている情報を初期化する
		/// </summary>
		void Reset() {
			m_Spheres.clear();
			m_Hand = nullptr;
			m_Map = nullptr;
		}

		/// <summary>
		/// プレイヤーを登録
		/// </summary>
		/// <param name="sphere">プレイヤー</param>
		void AddSphere(const shared_ptr<MoveSphere> sphere) {
			m_Spheres.push_back(sphere);
		}

		/// <summary>
		/// プレイヤーの情報を取得
		/// </summary>
		/// <returns>プレイヤーデータ</returns>
		vector<shared_ptr<MoveSphere>> GetSpheres()const {
			return m_Spheres;
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

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();
	};
}
//end basecross
