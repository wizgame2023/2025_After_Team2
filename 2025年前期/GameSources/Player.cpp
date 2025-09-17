/*!
@file Player.cpp
@brief プレイヤーなど実体
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
			//目標までのベクトル
			Vec3 direction = m_TargetPosition - position;
			//目標までの距離
			float distance = direction.length();
			direction = direction.normalize();
			//今回の移動量
			Vec3 moveAmount = direction * m_Speed * elapsed;
			//移動量が目標までの距離より大きい(目標地点を追い越す)場合は移動量を距離分に指定
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

