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
		bool m_IsButton = true;
		bool m_IsNextRoll = true;

		float m_Angle100 = XMConvertToRadians(100);
		float m_RollSpEndRight = 0.0f;

		float m_Angle270 = XM_PI + (XM_PI * 0.5f);
		float m_RollSpEndLeft = XM_PI;


		float m_RollVelocity = 0.0f;



		vector<Vec3> m_NumPositions;


		shared_ptr<NumberSprite> m_NumSp;

		shared_ptr<Sprite> m_BackSp;
		shared_ptr<Sprite> m_RollSpRight;
		shared_ptr<Sprite> m_RollSpLeft;


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
		void Json();
	};


}
//end basecross

