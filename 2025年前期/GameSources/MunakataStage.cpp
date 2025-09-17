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

			auto sphere = AddGameObject<MoveSphere>(2.0f,Vec3(0.0f,0.0f,1.0f));
			sphere->SetPosition(Vec3(0.0f,-4.0f,0.0f));
			sphere->SetVelocity(Vec3(0.0f, 0.0f, 0.0f));
			
			m_StageMap = AddGameObject<Map>();
			m_StageMap->Load();

			Vec3 mapSize = m_StageMap->GetMapSize();

			sphere->SetMoveArea(AABB(Vec3(0, -5.0f, 0), Vec3(mapSize.x, 5.0f, mapSize.y)));

			GetView()->GetTargetCamera()->SetEye(Vec3(2.5f, 2.5f, -10.0f));
			GetView()->GetTargetCamera()->SetAt(Vec3(2.5f, -5.0f, 2.5f));
		}
		catch (...) {
			throw;
		}
	}

	void MStage::OnUpdate() {
		auto device = App::GetApp()->GetInputDevice().GetControlerVec()[0];
		if (device.bConnected) {
			auto colorTable = m_StageMap->GetColorTable();
			if (device.wPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
				m_SelectColorIndex++;
				m_SelectColorIndex = min(colorTable.size() - 1, m_SelectColorIndex);
			}
			if (device.wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
				m_SelectColorIndex--;
				m_SelectColorIndex = max(0, m_SelectColorIndex);
			}
			m_StageMap->HighlightBox(colorTable[m_SelectColorIndex]);

			if (device.wPressedButtons & XINPUT_GAMEPAD_A) {
				m_StageMap->PutGimmick(0, colorTable[m_SelectColorIndex]);
			}
		}
	}

}
//end basecross
