/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void MStage::OnCreate() {
		try {
			GameStage::OnCreate();
		}
		catch (...) {
			throw;
		}
	}

	void MStage::OnUpdate() {
		GameStage::OnUpdate();
	}

}
//end basecross
