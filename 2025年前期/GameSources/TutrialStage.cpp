/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	void TutorialStage::CreateResorce() {
		ResourceManager::Load(L"gameResource.json");
		ResourceManager::RegisterTexture(L"Texture");
		ResourceManager::RegisterTexture(L"UI");
	}

	void TutorialStage::OnCreate() {
		try {
			GameStage::OnCreate();
		}
		catch (...) {
			throw;
		}
	}

	void TutorialStage::OnUpdate() {
		GameStage::OnUpdate();
	}
	void TutorialStage::OnEvent(const shared_ptr<Event>& event) {
		
	}
}
//end basecross
