/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Map;
	class MoveBall;
	class GimmickHand;
	static bool isChild = false;

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class MStage : public GameStage {
	public:
		//構築と破棄
		MStage() :GameStage(){}
		virtual ~MStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};


}
//end basecross

