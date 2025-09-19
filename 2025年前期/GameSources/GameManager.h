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
	public:
		void AddSphere(const shared_ptr<MoveSphere> sphere) {
			m_Spheres.push_back(sphere);
		}
		vector<shared_ptr<MoveSphere>> GetSpheres()const {
			return m_Spheres;
		}
		void SetHand(shared_ptr<GimmickHand> hand) {
			m_Hand = hand;
		}

		shared_ptr<GimmickHand> GetHand()const {
			return m_Hand;
		}

		void SetMap(shared_ptr<Map> map) {
			m_Map = map;
		}

		shared_ptr<Map> GetMap() const{
			return m_Map;
		}
		void Update();
	};
}
//end basecross
