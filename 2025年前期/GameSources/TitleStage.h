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

        shared_ptr<Sprite> m_StartSprite;
        shared_ptr<Sprite> m_Cursor;
        shared_ptr<Sprite> m_Line;
        vector<shared_ptr<Sprite>> m_ColorPalettes;
        vector<shared_ptr<Sprite>> m_Handlers;

        Vec3 m_StartPos;
        float m_Fade = 0.0f;
        bool m_IsBlinking = true; // スタート文字点滅制御
		bool m_IsAPushed = false;
        bool m_IsConfirmed = false;


    public:
        TitleStage() :Stage() {}
        virtual ~TitleStage() {}
        virtual void OnCreate()override;
        virtual void OnUpdate()override;

    private:
        void SpriteCreate();
        void StartSpriteCreate();
        void Json();
    };
}
