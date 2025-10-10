/*!
@file TitleStage.h
@brief タイトルステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	タイトルステージクラス
	//--------------------------------------------------------------------------------------
	class TitleStage : public Stage {
		//ビューの作成
		void CreateViewLight();
	public:
		//構築と破棄
		TitleStage() :Stage() {}
		virtual ~TitleStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		void SpriteCreate();
	};


}
//end basecross

