/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void Object::OnCreate() {
		m_Transform = GetComponent<Transform>();
		//初期化
		m_Transform->SetPosition(Vec3());
		m_Transform->SetRotation(Vec3());
		m_Transform->SetScale(Vec3(1.0f));

		//m_Stage = GetTypeStage<GameStage>();
	}
	void Object::OnUpdate() {

	}

	void Object::SetForward(Vec3 vec) {
		Mat4x4 rotMatrix = static_cast<Mat4x4>(XMMatrixLookAtLH(Vec3(0, 0, 0), -vec, Vec3(0, 1, 0)));
		rotMatrix = inverse(rotMatrix);
		Quat Qt = rotMatrix.quatInMatrix();
		Qt.normalize();

		m_Transform->SetQuaternion(Qt);
	}
}
//end basecross
