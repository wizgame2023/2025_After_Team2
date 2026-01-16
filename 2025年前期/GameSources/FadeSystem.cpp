/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	FadeSystem::FadeSystem(const shared_ptr<Stage>& ptr, float time, function<void()> func):
		Object(ptr),m_FadeTime(time),m_Function(func){}

	void FadeSystem::OnCreate() {
		auto sprite = m_Stage->AddGameObject<Sprite>(L"MENU", Vec3(), Vec2(), Anchor::Center);
		sprite->MatchToScreenSize();
		sprite->SetDiffuse(Col4(0, 0, 0, 1));
		sprite->SetLayer(10);
		m_Fade = sprite->AddComponent<SpriteFade>(m_FadeTime * 2.0f);

		m_Fade->StartFade(FadeState::OutToIn);
	}
	void FadeSystem::OnUpdate() {

		if (m_Fade->IsHalfFinish() && m_Function) {
			m_Function();
		}
		if (m_Fade->IsFinish()) {
			m_Stage->RemoveGameObject<Sprite>(m_Fade->GetGameObject());
			m_Stage->RemoveGameObject<FadeSystem>(GetThis<FadeSystem>());
		}
	}
}
//end basecross
