/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class Coursor;
	struct Line {
		shared_ptr<Sprite> m_Line;
		pair<int, int> m_PairHandle;
	};
	class Menu : public Object {
		map<GimmickObjects, wstring> m_GimmickTextures;
		shared_ptr<Stage> m_MenuStage;
		shared_ptr<Coursor> m_Coursor;
		shared_ptr<Sprite> m_CurrentLine;

		shared_ptr<Sprite> m_BackGround;
		vector<shared_ptr<Sprite>> m_ColorPalette;
		vector<shared_ptr<Sprite>> m_GimmcikSprites;
		vector<Line> m_Lines;

		wstring m_ColorTexture;
		wstring m_BackGroundTexture;

		Viewport m_MainViewPort;

		int m_ColorHandle;
		int m_GimmikcHandle;

		bool UpdateOnCoursorHandle();
		vector<pair<int, int>> ConvertColorGimmickHandles(vector<Line>& lines);
		void DrawLine(Vec3 start, Vec3 end);
	public:
		Menu(const shared_ptr<Stage>& ptr,const wstring& colorTex,const wstring& backGroundTex,Viewport& mainViewport) 
			: Object(ptr),
			m_ColorTexture(colorTex),m_BackGroundTexture(backGroundTex),m_MainViewPort(mainViewport), 
			m_MenuStage(ptr), m_ColorHandle(-1),m_GimmikcHandle(-1){}
		virtual ~Menu(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		int OnCoursorHandle(vector<shared_ptr<Sprite>>& sprites);
	};

	enum class CoursorMode {
		Stick,Mouse
	};
	class Coursor : public Object {
		shared_ptr<Sprite> m_Coursor;
		CoursorMode m_Mode;
		shared_ptr<Stage> m_MenuStage;

		wstring m_CoursorTexture;

		AABB m_MoveArea;
		float m_MoveSpeed;
	public:
		Coursor(const shared_ptr<Stage>& ptr,const wstring& coursorTex) : 
			Object(ptr),m_MenuStage(ptr), m_Mode(CoursorMode::Stick),m_CoursorTexture(coursorTex){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		Vec3 GetPosition() {
			return m_Coursor->GetPosition();
		}

		void SetCoursorMode(CoursorMode mode) {
			m_Mode = mode;
		}

		void SetMoveArea(Vec2 max, Vec2 min) {
			m_MoveArea.m_Max = max;
			m_MoveArea.m_Min = min;
			auto center = m_MoveArea.GetCenter();
			m_Coursor->SetPosition(center);
		}
		void SetMoveSpeed(float speed) {
			m_MoveSpeed = speed;
		}
		void SetCoursorSize(float size) {
			m_Coursor->SetSize(Vec2(size));
		}
		Vec3 LimitMoveArea();

		bool IsOnArea(Vec2 max, Vec2 min);
	};
}
//end basecross
