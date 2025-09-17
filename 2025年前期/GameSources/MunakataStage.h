/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Map;
	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class MStage : public GameStage {
		shared_ptr<Map> m_StageMap;
		int m_SelectColorIndex;
	public:
		//構築と破棄
		MStage() :GameStage() {}
		virtual ~MStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};


}
//end basecross

