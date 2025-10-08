/*!
@file SelectStage.h
@brief セレクトステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	セレクトステージクラス
	//--------------------------------------------------------------------------------------
	class SelectStage : public Stage {
		//ビューの作成
		void CreateViewLight();
	public:
		//構築と破棄
		SelectStage() :Stage() {}
		virtual ~SelectStage() {}
		//初期化
		virtual void OnCreate()override;
	};


}
//end basecross

