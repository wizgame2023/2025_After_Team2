/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void Object::OnCreate() {
		m_Transform = GetComponent<Transform>();
	}
	void Object::OnUpdate() {

	}
}
//end basecross
