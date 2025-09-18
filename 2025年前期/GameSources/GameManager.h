/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	class GameManager : public SingletonBase<GameManager> {
		friend class SingletonBase<GameManager>;
		GameManager() = default;

		vector<shared_ptr<MoveSphere>> m_Spheres;
	public:
		void AddSphere(const shared_ptr<MoveSphere> sphere) {
			m_Spheres.push_back(sphere);
		}
		vector<shared_ptr<MoveSphere>> GetSpheres()const {
			return m_Spheres;
		}
	};
}
//end basecross
