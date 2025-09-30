/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	MoveCube::MoveCube(const shared_ptr<Stage>& ptr) : Object(ptr), 
		m_IsEffecting(false),
		m_MoveSpeed(0.0f){}

	void MoveCube::OnCreate() {
		Object::OnCreate();

		m_Draw = AddComponent<PNTStaticDraw>();
		m_Draw->SetMeshResource(L"DEFAULT_CUBE");
		m_Draw->SetDiffuse(Col4(1, 1, 1, 1));

		
		GameManager::GetInstance().AddBall(GetThis<MoveCube>());
	}
	void MoveCube::OnUpdate() {
		if (!m_IsEffecting) {
			int rnd = rand() % 4;

			vector<Vec3> rot = { Vec3(1,0,0),Vec3(-1,0,0) ,Vec3(0,0,1) ,Vec3(0,0,-1) };
			m_Velocity = rot[rnd];
			return;
		}

		float elapsed = App::GetApp()->GetElapsedTime();

		Vec3 position = GetPosition();

		switch (m_State) {
		case MoveState::Move: {

			break;
		}
		case MoveState::Telepote: {
			break;
		}
		}

		Vec3 move = (m_Target - position);
		float distance = move.length();

		Vec3 moveAmount = move.normalize() * elapsed * m_MoveSpeed;
		//移動量が目標までの距離より大きい(目標地点を追い越す)場合は移動量を距離分に指定
		if (moveAmount.length() > distance) {
			moveAmount = move.normalize() * distance;
			m_IsEffecting = false;
		}
		auto rot = XMMatrixRotationAxis(cross(Vec3(0, 1, 0), m_Velocity), m_RotateSpeed * elapsed);
		auto world = m_Transform->GetWorldMatrix();
		world.rotation((Quat)XMQuaternionRotationMatrix(rot));

		m_Transform->SetQuaternion(m_Transform->GetQuaternion() * world.quatInMatrix());
		position += moveAmount;
		SetPosition(position);
	}
	bool MoveCube::CheckArea(Vec3 position) {
		if (position.x > m_MoveArea.m_Max.x || position.x < m_MoveArea.m_Min.x) {
			return false;
		}
		if (position.y > m_MoveArea.m_Max.y || position.y < m_MoveArea.m_Min.y) {
			return false;
		}
		if (position.z > m_MoveArea.m_Max.z || position.z < m_MoveArea.m_Min.z) {
			return false;
		}

		return true;
	}

	MoveBall::MoveBall(const shared_ptr<Stage>& ptr, float speed, Vec3 velocity) :
		Object(ptr),
		m_Speed(speed),
		m_Velocity(velocity),
		m_TargetPosition(Vec3()),
		m_IsTarget(false),m_IsActive(true)
	{}

	void MoveBall::OnCreate() {
		Object::OnCreate();

		m_Draw = AddComponent<PNTStaticDraw>();
		m_Draw->SetMeshResource(L"DEFAULT_SPHERE");
		m_Draw->SetDiffuse(Col4(1, 1, 1, 1));
	}
	void MoveBall::OnUpdate() {
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

	Vec3 MoveBall::LimitArea(Vec3 position) {

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

