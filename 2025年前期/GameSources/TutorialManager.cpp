/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void TutorialManager::Start(const wstring& name) {
		if (!m_StepMap[name]) return;
		if (m_StepMap[name]->IsCompleted()) return;
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

	void TutorialManager::RegisterStep(const wstring& name, const shared_ptr<TutorialStep>& step) {
		if (m_StepMap.find(name) != m_StepMap.end()) return;
		m_StepMap[name] = step;
	}

	void TutorialStep::Start() {
		m_Window = m_Stage->AddGameObject<Sprite>(L"TUTORIAL_WINDOW", m_Position, m_WindowSize, Anchor::Center);
		m_Window->SetLayer(10);
		Vec3 explainPosition = m_Position + Vec3(0, 25, 0);
		m_Explain = m_Stage->AddGameObject<Sprite>(m_ExplainKeys[m_PageIndex], explainPosition, m_WindowSize * 0.85f, Anchor::Center);
		m_Explain->SetLayer(11);
		if (m_ExplainKeys.size() > 1) {
			m_CloseIcon = m_Stage->AddGameObject<Sprite>(L"TUTORIAL_NEXT", explainPosition, m_WindowSize * 0.85f, Anchor::Center);
		}
		else {
			m_CloseIcon = m_Stage->AddGameObject<Sprite>(L"TUTORIAL_CLOSE", explainPosition, m_WindowSize * 0.85f, Anchor::Center);
		}
		m_CloseIcon->SetLayer(10);

	}
	void TutorialStep::Update() {
		auto& input = InputManager::GetInputManager();
		if (input->GetDownButton(L"A")) {
			if(m_PageIndex + 1 < static_cast<int>(m_ExplainKeys.size())){
				m_PageIndex++;
				m_Explain->SetTextureKey(m_ExplainKeys[m_PageIndex]);
				if(m_PageIndex == static_cast<int>(m_ExplainKeys.size()) - 1){
					m_CloseIcon->SetTextureKey(L"TUTORIAL_CLOSE");
				}
			}
			else {
				TutorialManager::GetInstance().End();
			}
		}
	}
	void TutorialStep::End() {
		m_IsCompleted = true;
		m_Stage->RemoveGameObject<Sprite>(m_Window);
		m_Stage->RemoveGameObject<Sprite>(m_Explain);
		m_Stage->RemoveGameObject<Sprite>(m_CloseIcon);
	}

	void PutGimmickTutorial::Start() {
		TutorialStep::Start();
		m_Cursor = m_Stage->AddGameObject<Cursor>(L"MOUSE_CURSOR");
		m_Cursor->SetPosition(m_Sprites[0]->GetPosition());
		m_Cursor->SetCoursorSize(25.0f);
		m_Cursor->SetUpdateActive(false);
		m_Cursor->SetDrawActive(true);

		m_Line = m_Stage->AddGameObject<Sprite>(L"TEMP_COLOR_PALETTE", m_Cursor->GetPosition(), Vec2(10, 10), Anchor::Center);
		m_Line->SetLayer(9);

		m_BackGround = m_Stage->AddGameObject<Sprite>(L"MENU", Vec3(), Vec2(), Anchor::Center);
		m_BackGround->MatchToScreenSize();
		m_BackGround->SetDiffuse(Col4(0, 0, 0, 0.5f));
		m_BackGround->SetLayer(8);

		m_Sprites[0]->SetLayer(10);
		m_Sprites[1]->SetLayer(10);
	}
	void PutGimmickTutorial::Update() {
		TutorialStep::Update();

		float elapsed = App::GetApp()->GetElapsedTime();

		Vec3 direction = m_Sprites[1]->GetPosition() - m_Sprites[0]->GetPosition();
		float allDistance = direction.length();
		direction.normalize();

		Vec3 cursorPosition = m_Cursor->GetPosition();
		float distance = (m_Sprites[1]->GetPosition() - cursorPosition).length();
		Vec3 lineDirection = cursorPosition - m_Sprites[0]->GetPosition();

		if (distance > 0) {
			Vec3 moveAmount = direction * (allDistance / m_LoopTime) * elapsed;

			if (moveAmount.length() > distance) {
				cursorPosition = m_Sprites[1]->GetPosition();
			}
			else {
				cursorPosition += moveAmount;
			}

			m_Cursor->SetPosition(cursorPosition);
		}
		else {
			m_Timer += elapsed;
			if (m_ResetTime < m_Timer) {
				m_Cursor->SetPosition(m_Sprites[0]->GetPosition());
				m_Timer = 0.0f;
			}
			lineDirection = Vec3(m_Sprites[1]->GetAnchorPosition(Anchor::Right) - m_Sprites[0]->GetAnchorPosition(Anchor::Left));
		}
		m_Line->SetSize(Vec2(m_Line->GetSize().x, lineDirection.length()));
		m_Line->SetAnchorPosition(Vec3(m_Sprites[0]->GetAnchorPosition(Anchor::Left)) + lineDirection / 2.0f, Anchor::Center);
		m_Line->VectorToward(static_cast<Vec2>(lineDirection.normalize()));

	}
	void PutGimmickTutorial::End() {
		TutorialStep::End();
		m_Cursor->Destroy();
		m_Stage->RemoveGameObject<Sprite>(m_Line);
		m_Stage->RemoveGameObject<Sprite>(m_BackGround);

		m_Sprites[0]->SetLayer(2);
		m_Sprites[1]->SetLayer(2);
	}
}
//end basecross
