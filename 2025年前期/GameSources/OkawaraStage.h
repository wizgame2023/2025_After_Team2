/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class OStage : public GameStage {
	public:
		//�\�z�Ɣj��
		OStage() :GameStage() {}
		virtual ~OStage() {}
		//������
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};


}
//end basecross

