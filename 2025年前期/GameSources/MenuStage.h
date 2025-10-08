#pragma once
#include "stdafx.h"

namespace basecross {
	class Menu;
	class MenuStage : public Stage {
		void CreateViewLight();
		void CreateResorce();
		shared_ptr<Menu> m_Menu;
	public:
		//\’z‚Æ”jŠü
		MenuStage() :Stage() {}
		virtual ~MenuStage() {}
		//‰Šú‰»
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};

}
