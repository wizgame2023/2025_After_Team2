#pragma once
#include "stdafx.h"

namespace basecross{

	class MovieWindow : public Object {
		Vec2 m_WindowSize;
	public:
		MovieWindow(const shared_ptr<Stage>& ptr,Vec2 windowSize):Object(ptr), m_WindowSize(windowSize){}
		~MovieWindow(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void Play(const wstring& filename);

		void Stop();
	};
}
//end basecross
