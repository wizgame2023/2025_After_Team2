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
		m_IsTarget(false),m_IsActive(true)
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
		
		position = LimitArea(position);
		SetPosition(position);
	}

	Vec3 MoveSphere::LimitArea(Vec3 position) {

		bool isActive = true;
		Vec3 scale = GetScale();

		float up = position.y + scale.y;
		float down = position.y - scale.y;
		float right = position.x + scale.x;
		float left = position.x - scale.x;
		float front = position.z - scale.z;
		float back = position.z - scale.z;
		
		if (up > m_MoveArea.m_Max.y) {
			position.y = m_MoveArea.m_Max.y - scale.y;
			m_IsTarget = false;
		}
		if (down < m_MoveArea.m_Min.y) {
			position.y = m_MoveArea.m_Min.y + scale.y;
			m_IsTarget = false;
		}

		if (right > m_MoveArea.m_Max.x) {
			position.x = m_MoveArea.m_Max.x - scale.x;
			isActive = false;
		}
		if (left < m_MoveArea.m_Min.x) {
			position.x = m_MoveArea.m_Min.x + scale.x;
			isActive = false;
		}

		if (front > m_MoveArea.m_Max.z) {
			position.z = m_MoveArea.m_Max.z - scale.z;
			isActive = false;
		}
		if (back < m_MoveArea.m_Min.z) {
			position.z = m_MoveArea.m_Min.z + scale.z;
			isActive = false;
		}

		m_IsActive = isActive;

		return position;
	}
}
//end basecross

