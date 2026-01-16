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
        bool m_IsNextRoll = false;

        float m_Angle100 = XMConvertToRadians(100);
        float m_RollSpEndRight = 0.0f;

        float m_Angle270 = XM_PI + (XM_PI * 0.5f);
        float m_RollSpEndLeft = XM_PI;

        float m_RollVelocity = 0.0f;

		Vec3 m_OffsetPos = Vec3(50.0f, -60.0f, 0.0f);

        vector<Vec3> m_NumPositions;
        vector<shared_ptr<NumberSprite>> m_NumSpList; // 数字スプライトリスト

        shared_ptr<Sprite> m_BackSp;
        shared_ptr<Sprite> m_RollSpRight;
        shared_ptr<Sprite> m_RollSpLeft;

        // スティック押しっぱなし対応
        float m_StickHoldTime = 0.0f;

        //ビューの作成
        void CreateViewLight();

    public:
        SelectStage() :Stage() {}
        virtual ~SelectStage() {}
        virtual void OnCreate()override;
        virtual void OnUpdate()override;

        int GetStageNum() { return m_Count; }

    private:
        void SpriteCreate();
        void Json();
    };


}
//end basecross

