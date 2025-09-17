/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Map;
	class MoveSphere;
	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class MStage : public GameStage {
		shared_ptr<Map> m_StageMap;
		shared_ptr<MoveSphere> m_Sphere;

		void CreateViewLight();

	public:
		//�\�z�Ɣj��
		MStage() :GameStage() {}
		virtual ~MStage() {}
		//������
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};


}
//end basecross

