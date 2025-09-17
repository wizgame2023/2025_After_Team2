/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	MoveSphere::MoveSphere(const shared_ptr<Stage>& ptr, float speed, Vec3 velocity) :
		Object(ptr),
		m_Speed(speed),
		m_Velocity(velocity),
		m_TargetPosition(Vec3()),
		m_IsTarget(false)
	{}

	void MoveSphere::OnCreate() {
		Object::OnCreate();

		m_Draw = AddComponent<PNTStaticDraw>();
		m_Draw->SetMeshResource(L"DEFAULT_SPHERE");
		m_Draw->SetDiffuse(Col4(1, 1, 1, 1));
	}
	void MoveSphere::OnUpdate() {
		float elapsed = App::GetApp()->GetElapsedTime();

		Vec3 position = GetPosition();
		if (m_IsTarget) {
			//�ڕW�܂ł̃x�N�g��
			Vec3 direction = m_TargetPosition - position;
			//�ڕW�܂ł̋���
			float distance = direction.length();
			direction = direction.normalize();
			//����̈ړ���
			Vec3 moveAmount = direction * m_Speed * elapsed;
			//�ړ��ʂ��ڕW�܂ł̋������傫��(�ڕW�n�_��ǂ��z��)�ꍇ�͈ړ��ʂ��������Ɏw��
			if (moveAmount.length() > distance) {
				moveAmount = direction * distance;
				m_IsTarget = false;
			}

			position += moveAmount;

		}
		else {
			m_Velocity = m_Velocity.normalize();
			position += m_Velocity * m_Speed * elapsed;
		}

		SetPosition(position);
	}
}
//end basecross

