/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class TutorialStage : public GameStage {
		shared_ptr<Sprite> CurrentExplain;
		//ビューの作成
		void CreateViewLight();
		void CreateResorce();

	public:
		//構築と破棄
		TutorialStage() :GameStage() {}
		virtual ~TutorialStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnEvent(const shared_ptr<Event>& event)override;
	};


}
//end basecross

