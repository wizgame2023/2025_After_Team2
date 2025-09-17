/*!
@file Player.h
@brief �v���C���[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class MoveSphere : public Object {
		float m_Speed;	//�ړ����x
		Vec3 m_Velocity;	//�i�s����
		Vec3 m_TargetPosition;	//�ڕW�n�_
		bool m_IsTarget;	//�ڕW�n�_�Ɍ�������

		shared_ptr<PNTStaticDraw> m_Draw;
	public:
		MoveSphere(const shared_ptr<Stage>& ptr, float speed, Vec3 velocity);
		MoveSphere(const shared_ptr<Stage>& ptr) : MoveSphere(ptr,1.0f,Vec3(1.0f,0.0f,0.0f)){}
		virtual ~MoveSphere(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void SetVelocity(Vec3 velocity) {
			m_Velocity = velocity;
		}
		Vec3 GetVelocity() {
			return m_Velocity;
		}

		void SetTarget(Vec3 position) {
			m_TargetPosition = position;
			m_IsTarget = true;
		}
	};
}
//end basecross

