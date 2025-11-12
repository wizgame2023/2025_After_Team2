#pragma once
#include "stdafx.h"

namespace basecross {
	class GameMenu;
	class MenuStage : public Stage {
		void CreateViewLight();
		void CreateResorce();
		shared_ptr<GameMenu> m_Menu;
	public:
		//\’z‚Æ”jŠü
		MenuStage() :Stage() {}
		virtual ~MenuStage() {}
		//‰Šú‰»
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};

}
