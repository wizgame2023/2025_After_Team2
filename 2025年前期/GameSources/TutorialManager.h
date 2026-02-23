/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class TutorialStep;
	class TutorialManager : public SingletonBase<TutorialManager>{
		unordered_map<wstring, shared_ptr<TutorialStep>> m_StepMap;

		shared_ptr<TutorialStep> m_CurrentStep;
	protected:
		friend class SingletonBase<TutorialManager>;
		TutorialManager() = default;
	public:
		void Start(const wstring& name);
		void Update();
		void End();

		bool IsActive() {
			return m_CurrentStep != nullptr;
		}

		void RegisterStep(const wstring& name, const shared_ptr<TutorialStep>& step);
	};

	class TutorialStep {
	protected:
		shared_ptr<Stage> m_Stage;

		shared_ptr<Sprite> m_CloseIcon;
		shared_ptr<Sprite> m_Window;
		shared_ptr<Sprite> m_Explain;

		int m_PageIndex;

		bool m_IsCompleted;
		Vec2 m_WindowSize;
		Vec3 m_Position;
		wstring m_ExplainKey;
		vector<wstring> m_ExplainKeys;

		float m_DrawDelayTime;
		float m_DelayTimer;
	public:
		TutorialStep(const shared_ptr<Stage>& ptr,const vector<wstring>& explain = {}, Vec3 position = Vec3(), float width = 100, float height = 100, float delayTime = 0.0f) : m_Stage(ptr),
			m_IsCompleted(false), m_ExplainKeys(explain), m_DelayTimer(0.0f), m_DrawDelayTime(delayTime),
			m_Position(position),m_WindowSize(width, height), m_PageIndex(0){ }
		virtual ~TutorialStep(){}

		virtual void Start();
		virtual void Update();
		virtual void End();

		bool IsCompleted() {
			return m_IsCompleted;
		}
	};

	class Cursor;
	class PutGimmickTutorial : public TutorialStep {
		shared_ptr<Cursor> m_Cursor;
		shared_ptr<Sprite> m_Line;
		shared_ptr<Sprite> m_BackGround;

		array<shared_ptr<Sprite>, 2> m_Sprites;

		float m_LoopTime;
		float m_ResetTime;
		float m_Timer;
	public:
		PutGimmickTutorial(const shared_ptr<Stage>& ptr,const array<shared_ptr<Sprite>, 2>& array, float loop, float reset = 0.5f) :
			TutorialStep(ptr, { L"TUTORIAL_INFO_1",L"TUTORIAL_INFO_2"}, Vec3(-100.0f, 200.0f, 0.0f), 600, 300),
			m_Sprites(array),
			m_Timer(0.0f), m_ResetTime(reset),m_LoopTime(loop)
		{ }
		virtual ~PutGimmickTutorial() {}

		virtual void Start();
		virtual void Update();
		virtual void End();

	};
	

}
//end basecross
