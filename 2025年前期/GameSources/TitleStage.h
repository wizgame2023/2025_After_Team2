/*!
@file TitleStage.h
@brief タイトルステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Sprite;

	//--------------------------------------------------------------------------------------
	//	タイトルステージクラス
	//--------------------------------------------------------------------------------------
	class TitleStage : public Stage {

		//ビューの作成
		void CreateViewLight();

		shared_ptr<Sprite> m_StartSprite;

		float m_Fade;

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

