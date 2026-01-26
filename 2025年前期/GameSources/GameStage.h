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
	class SpriteFade;
	class GameStage : public Stage {
		//ビューの作成
		void CreateViewLight();
		void CreateResorce();

		shared_ptr<JsonObject> m_StageData;
	public:
		//構築と破棄
		GameStage(const shared_ptr<JsonObject> data = nullptr) :Stage(), m_StageData(data) {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDestroy()override;

	};


}
//end basecross

