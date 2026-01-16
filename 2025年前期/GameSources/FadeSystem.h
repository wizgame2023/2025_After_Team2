/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	class FadeSystem : public Object {

		shared_ptr<SpriteFade> m_Fade;
		function<void()> m_Function;
		float m_FadeTime;
	public:
		FadeSystem(const shared_ptr<Stage>& ptr,float time, function<void()> func);
		virtual ~FadeSystem(){}

		virtual void OnCreate();
		virtual void OnUpdate();
	};
}
//end basecross
