/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void GameManager::Update() {
		if (m_Hand->IsEmpty()) {
			for (auto& sphere : m_Spheres) {
				//プレイヤーを稼働開始
				sphere->SetUpdateActive(true);
			}
		}
	}
}
//end basecross
