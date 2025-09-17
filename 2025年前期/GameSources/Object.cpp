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
	}
	void Object::OnUpdate() {

	}
}
//end basecross
