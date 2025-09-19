/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void GameManager::Update() {
		if (m_Hand->IsEmpty()) {
			for (auto& sphere : m_Spheres) {
				//�v���C���[���ғ��J�n
				sphere->SetUpdateActive(true);
			}
		}
	}
}
//end basecross
