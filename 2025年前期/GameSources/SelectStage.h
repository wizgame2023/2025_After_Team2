/*!
@file SelectStage.h
@brief セレクトステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class Sprite;
	class NumberSprite;

	//--------------------------------------------------------------------------------------
	//	セレクトステージクラス
	//--------------------------------------------------------------------------------------
	class SelectStage : public Stage 
	{
		int m_StageNum = 1;

		int m_Count = 0;

		bool m_IsStick = false;

		vector<Vec3> m_NumPositions;


		shared_ptr<NumberSprite> m_NumSp;

		shared_ptr<Sprite> m_BackSp;
		//ビューの作成
		void CreateViewLight();
	public:
		//構築と破棄
		SelectStage() :Stage() {}
		virtual ~SelectStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		int GetStageNum() { return m_Count; }

	private:
		void SpriteCreate();
		void NumSprite();
	};


}
//end basecross

