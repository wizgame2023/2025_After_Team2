/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void Object::OnCreate() {
		m_Transform = GetComponent<Transform>();
		//������
		m_Transform->SetPosition(Vec3());
		m_Transform->SetRotation(Vec3());
		m_Transform->SetScale(Vec3(1.0f));
	}
	void Object::OnUpdate() {

	}
}
//end basecross
