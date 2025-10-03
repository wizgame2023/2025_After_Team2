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

		SetMoveArea(AABB(Vec3(-1.0f, -100.0f, -1.0f), Vec3(mapSize.x, 5.0f, mapSize.z)));

		SetMoveSec(0.5f);
		GameManager::GetInstance().AddCube(GetThis<MoveCube>());

		m_Sprites.push_back(m_Stage->AddGameObject<Board>(L"TEMP_ARROW_SPRITE", Vec3(), Vec3(1.0f, 1.0f, 1.0f), false));
		m_Sprites.push_back(m_Stage->AddGameObject<Board>(L"TEMP_ARROW_SPRITE", Vec3(), Vec3(1.0f, 1.0f, 1.0f), false));

		/*m_Velocity = Vec3(0, 0, 1);
		Vec3 side = cross(Vec3(0, 1, 0), m_Velocity.normalize());

		m_Sprites[0]->RotateVector(side);
		m_Sprites[1]->RotateVector(-side);*/
	}
	void MoveCube::OnUpdate() {
		m_Velocity = Vec3(0, 0, -1);
		//回転軸
		Vec3 side = cross(Vec3(0, 1, 0), m_Velocity.normalize());
		float angle = XM_PIDIV2;
		//回転行列
		auto rot = XMMatrixRotationAxis(side, angle);

		//左右の矢印の回転
		for (int i = 0; i < 2; i++) {
			//軸の補正(反転用)
			int correct = i == 0 ? 1 : -1;

			m_Sprites[i]->RotateVector(side * correct);

			m_Sprites[i]->GetTrans()->SetPosition(side * correct * 0.5f + GetPosition());

			auto world = m_Sprites[i]->GetTrans()->GetWorldMatrix();
			world.rotation((Quat)XMQuaternionRotationMatrix(rot));

			m_Sprites[i]->GetTrans()->SetQuaternion(m_Sprites[i]->GetTrans()->GetQuaternion() * world.quatInMatrix());
		}

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

			m_Transform->SetQuaternion(m_Transform->GetQuaternion() * world.quatInMatrix());
			position += moveAmount;
			position.y = CalcRotatingCenterY(m_RotateRad);
			m_RotateRad += rotateAmount;
			break;
		}
		case MoveState::Telepote: {
			position = m_TelepoteTarget;
			m_CurrentHeight = position.y;
			m_IsEffecting = false;
			break;
		}
		}

	
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

	void MoveCube::Destroy() {
		GameManager::GetInstance().DeleteCube(GetThis<MoveCube>());
	}
}
//end basecross

