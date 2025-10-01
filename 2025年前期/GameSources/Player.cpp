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

		m_Draw = AddComponent<PNTStaticModelDraw>();
		m_Draw->SetMeshResource(L"PLAYER_MD");
		m_Draw->SetDiffuse(Col4(1, 1, 1, 1));

		Mat4x4 mat;
		mat.affineTransformation(Vec3(0.4f), Vec3(), Vec3(), Vec3(0.0f,-0.5f,0.0f));
		m_Draw->SetMeshToTransformMatrix(mat);

		Vec3 mapSize = GameManager::GetInstance().GetMap()->GetMapSize();

		SetMoveArea(AABB(Vec3(-1.0f, -100.0f, 0.0f), Vec3(mapSize.x - 1, 5.0f, mapSize.y)));

		SetMoveSec(1.0f);
		GameManager::GetInstance().AddBall(GetThis<MoveCube>());
	}
	void MoveCube::OnUpdate() {
		if (!m_IsEffecting) {
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
	bool MoveCube::CheckArea() {
		Vec3 position = GetPosition() + m_Velocity.normalize();
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
}
//end basecross

