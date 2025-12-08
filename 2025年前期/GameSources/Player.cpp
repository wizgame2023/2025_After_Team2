/*!
@file Player.cpp
@brief プレイヤーなど実体
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

		Vec3 mapSize = GameManager::GetInstance().GetLevelManager()->GetMapSize();

		SetMoveArea(AABB(Vec3(-1.0f, -100.0f, -mapSize.z), Vec3(mapSize.x, 5.0f, 1.0f)));

		SetMoveSec(0.5f);
		GameManager::GetInstance().GetEntityManager()->AddPlayer(GetThis<MoveCube>());


	}
	void MoveCube::OnUpdate() {
		//回転軸
		Vec3 side = cross(Vec3(0, 1, 0), m_Velocity.normalize());
		float angle = XM_PIDIV2;
		//回転行列
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
			//移動量が目標までの距離より大きい(目標地点を追い越す)場合は移動量を距離分に指定
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
			//90度以上は回転させない
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
			m_TeleportTimer += elapsed;
			if (m_TeleportTimer > m_TeleportTime) {
				position = m_TelepoteTarget;
				SetDrawActive(true);
				m_CurrentHeight = position.y;
				if (m_AfterTeloprotTime > 0) {
					m_TeleportTime = m_AfterTeloprotTime;
					m_AfterTeloprotTime = 0.0f;
					m_TeleportTimer = 0.0f;
				}
				else {
					m_IsEffecting = false;
				}
			}
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
			auto mapData = GameManager::GetInstance().GetLevelManager()->GetMap()
				->GetMapData(Vec2(static_cast<int>(position.x), static_cast<int>(-position.z)));
			if (mapData.m_Gimmick) {
				mapData.m_Gimmick->End();
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
		//回転を0～90度に
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
		m_MoveVelocitySpeed = (m_TargetVelocity - m_Velocity) / GameManager::GetInstance().GetFlowManager()->GetGameTick();
	}
	void MoveCube::Move() {
		if (m_IsEffecting) return;
		m_IsEffecting = true;

		if (!CheckArea() || m_IsDead) {
			m_IsEffecting = false;
			m_Target = GetPosition();
		}
		m_Target = GetPosition() + m_Velocity.normalize();

		float gameTick = GameManager::GetInstance().GetFlowManager()->GetGameTick();
		m_MoveSpeed = (m_Target - GetPosition()).length() / gameTick;
		m_RotateSpeed = XM_PIDIV2 / gameTick;
		m_RotateRad = 0;
		m_State = MoveState::Move;
	}
	void MoveCube::Destroy() {
		SoundManager::GetInstance().PlaySE(L"Dead");
		GameManager::GetInstance().GetEntityManager()->DestroyPlayer(GetThis<MoveCube>());
	}
}
//end basecross

