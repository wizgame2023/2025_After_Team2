/*!
@file Player.cpp
@brief ÉvÉåÉCÉÑÅ[Ç»Ç«é¿ëÃ
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	MoveCube::MoveCube(const shared_ptr<Stage>& ptr) : Object(ptr), 
		m_IsEffecting(false), m_IsBeforeEffecting(false),
		m_MoveSpeed(0.0f){}

	void MoveCube::OnCreate() {
		Object::OnCreate();

		m_Draw = AddComponent<PNTStaticModelDraw>();
		m_Draw->SetMeshResource(L"PLAYER_MD");
		m_Draw->SetDiffuse(Col4(1, 1, 1, 1));

		Mat4x4 mat;
		mat.affineTransformation(Vec3(0.4f), Vec3(), Vec3(0,-XM_PIDIV2,0), Vec3(0.0f,-0.5f,0.25f));
		m_Draw->SetMeshToTransformMatrix(mat);

		Vec3 mapSize = GameManager::GetInstance().GetMap()->GetMapSize();

		SetMoveArea(AABB(Vec3(-1.0f, -100.0f, -1.0f), Vec3(mapSize.x, 5.0f, mapSize.z)));

		SetMoveSec(0.5f);
		GameManager::GetInstance().AddCube(GetThis<MoveCube>());


	}
	void MoveCube::OnUpdate() {
		//âÒì]é≤
		Vec3 side = cross(Vec3(0, 1, 0), m_Velocity.normalize());
		float angle = XM_PIDIV2;
		//âÒì]çsóÒ
		auto rot = XMMatrixRotationAxis(side, angle);

		SetForward(m_Velocity);

		if (!m_IsEffecting) {
			return;
		}
		float elapsed = App::GetApp()->GetElapsedTime();

		Vec3 position = GetPosition();

		switch (m_State) {
		case MoveState::Move: {
			Vec3 move = (m_Target - position);
			float distance = move.length();

			Vec3 moveAmount = move.normalize() * elapsed * m_MoveSpeed;
			//à⁄ìÆó Ç™ñ⁄ïWÇ‹Ç≈ÇÃãóó£ÇÊÇËëÂÇ´Ç¢(ñ⁄ïWínì_Çí«Ç¢âzÇ∑)èÍçáÇÕà⁄ìÆó Çãóó£ï™Ç…éwíË
			if (moveAmount.length() > distance) {
				moveAmount = move.normalize() * distance;
				m_IsEffecting = false;
				if (!CheckArea()) {
					Destroy();
				}
			}
			float rotateAmount = m_RotateSpeed * elapsed;
			if (rotateAmount >= XM_PIDIV2 - m_RotateRad) {
				rotateAmount = XM_PIDIV2 - m_RotateRad;
			}
			//90ìxà»è„ÇÕâÒì]Ç≥ÇπÇ»Ç¢
			if (m_RotateRad >= XM_PIDIV2) rotateAmount = 0;

			rot = XMMatrixRotationAxis(side, rotateAmount);
			auto world = m_Transform->GetWorldMatrix();
			world.rotation((Quat)XMQuaternionRotationMatrix(rot));

			//m_Transform->SetQuaternion(m_Transform->GetQuaternion() * world.quatInMatrix());
			position += moveAmount;
			//position.y = CalcRotatingCenterY(m_RotateRad);
			m_RotateRad += rotateAmount;
			break;
		}
		case MoveState::Telepote: {
			position = m_TelepoteTarget;
			m_CurrentHeight = position.y;
			m_IsEffecting = false;
			break;
		}
		case MoveState::ChangeVelocity: {
			Vec3 direction = m_TargetVelocity - m_Velocity;
			Vec3 moveAmount = elapsed * m_MoveVelocitySpeed;
			if (moveAmount.length() > direction.length()) {
				moveAmount = direction;
				m_IsEffecting = false;
			}
			m_Velocity += moveAmount;
		}
		}
	
		if (m_IsBeforeEffecting && !m_IsEffecting) {
			auto mapData = GameManager::GetInstance().GetMap()->
				GetMapData(Vec2(static_cast<int>(position.x), static_cast<int>(position.z)));
			if (mapData.m_Gimmik) {
				mapData.m_Gimmik->End();
			}
		}
		m_IsBeforeEffecting = m_IsEffecting;
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

	float MoveCube::CalcRotatingCenterY(float rot) {
		//âÒì]Ç0Å`90ìxÇ…
		while (rot > XM_PIDIV2) {
			rot -= XM_PIDIV2;
		}

		float r = 1.0f / sqrt(2.0f);
		return m_CurrentHeight + r * sin(XM_PIDIV4 + rot) - 0.5f;
	}
	void MoveCube::ChangeVelocity(Vec3 velocity) {
		if (m_IsEffecting) return;
		m_IsEffecting = true;

		m_State = MoveState::ChangeVelocity;
		m_TargetVelocity = velocity;
		m_MoveVelocitySpeed = (m_TargetVelocity - m_Velocity) / GameManager::GetInstance().GetGameSpeed();
	}
	void MoveCube::Move() {
		if (m_IsEffecting) return;
		m_IsEffecting = true;

		if (!CheckArea() || m_IsDead) {
			m_IsEffecting = false;
			m_Target = GetPosition();
		}
		m_Target = GetPosition() + m_Velocity.normalize();
		m_MoveSpeed = (m_Target - GetPosition()).length() / GameManager::GetInstance().GetGameSpeed();
		m_RotateSpeed = XM_PIDIV2 / GameManager::GetInstance().GetGameSpeed();
		m_RotateRad = 0;
		m_State = MoveState::Move;
	}
	void MoveCube::Destroy() {
		SoundManager::GetInstance().PlaySE(L"Dead");
		GameManager::GetInstance().DeleteCube(GetThis<MoveCube>());
	}
}
//end basecross

