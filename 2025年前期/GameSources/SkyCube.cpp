/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void SkyCube::OnCreate() {
		Object::OnCreate();
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_SPHERE");
		draw->SetTextureResource(m_TexKey);
		draw->SetRasterizerState(RasterizerState::CullFront);

		SetScale(Vec3(20));
	}
}
//end basecross
