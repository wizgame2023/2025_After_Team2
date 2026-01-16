/*!
@file TitleStage.h
@brief タイトルステージ
*/
#pragma once
#include "stdafx.h"

namespace basecross 
{
    class Sprite;

    class TitleStage : public Stage 
    {
        void CreateViewLight();
        shared_ptr<SoundItem> temp;

        shared_ptr<Sprite> m_StartSprite;
        shared_ptr<Sprite> m_Cursor;
        shared_ptr<Sprite> m_Line;
        shared_ptr<Sprite> m_RollSp;
        shared_ptr<Sprite> m_RollStarSp;
        shared_ptr<Sprite> m_RollStarSp2;
        shared_ptr<Sprite> m_RollStarSp3;

        shared_ptr<Sprite> m_FadeStarRedSp;
		shared_ptr<Sprite> m_FadeStarBlueSp;
		shared_ptr<Sprite> m_FadeStarYellowSp;


        vector<shared_ptr<Sprite>> m_ColorPalettes;
        vector<shared_ptr<Sprite>> m_Handlers;

        Vec3 m_StartPos;

		int m_Count = 0;

        float m_Fade = 0.0f;
        float m_RollVelocity = 0.0f;
        float m_RollStarVelocity = 0.0f;
        float m_RollSpEnd = XM_PI;
        float m_Angle160 = XMConvertToRadians(160);
        float m_Angle270 = XM_PI + (XM_PI * 0.5f);
        bool m_IsBlinking = true; // スタート文字点滅制御
		bool m_IsAPushed = false;
        bool m_IsConfirmed = false;

        bool m_IsSE = false;

    public:
        TitleStage() :Stage() {}
        virtual ~TitleStage() {}
        virtual void OnCreate()override;
        virtual void OnUpdate()override;
        virtual void OnDestroy()override;

    private:
        void SpriteCreate();
        void StartSpriteCreate();
        void FadeStarSpriteCreate();
        void FadeStarSprite();

        void Json();

		void UpdateRollSprite();

	    void GameStartMaster();
        void UpdateCursor();
        void HandleAPressed();
        void UpdateLine();
        void HandleAReleased();
        void UpdateStartSpriteBlink();

    };
}
