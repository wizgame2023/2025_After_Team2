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

		void RegisterStep(const wstring& name, shared_ptr<TutorialStep>& step);
	};

	class TutorialStep {
		shared_ptr<Stage> m_Stage;
		bool m_IsCompleted;
	public:
		TutorialStep(const shared_ptr<Stage>& ptr) : m_Stage(ptr), m_IsCompleted(false){}
		virtual ~TutorialStep(){}

		void Start(){}
		void Update(){}
		void End(){}
	};


}
//end basecross
