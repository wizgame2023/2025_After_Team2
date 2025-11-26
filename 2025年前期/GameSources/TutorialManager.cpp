/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void TutorialManager::Start(const wstring& name) {
		if (m_CurrentStep) {
			m_CurrentStep->End();
		}
		m_CurrentStep = m_StepMap[name];
		m_CurrentStep->Start();
	}
	void TutorialManager::Update() {
		if (m_CurrentStep) {
			m_CurrentStep->Update();
		}
	}
	void TutorialManager::End() {
		if (m_CurrentStep) {
			m_CurrentStep->End();
		}
		m_CurrentStep = nullptr;
	}

	void TutorialManager::RegisterStep(const wstring& name, shared_ptr<TutorialStep>& step) {
		m_StepMap[name] = step;
	}
}
//end basecross
