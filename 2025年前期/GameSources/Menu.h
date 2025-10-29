/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class Coursor;
	class PoseMenu;
	struct Line {
		shared_ptr<Sprite> m_Line;
		pair<int, int> m_PairHandle;

	};
	class GameMenu : public Object {
		map<GimmickObjects, wstring> m_GimmickTextures;
		shared_ptr<Stage> m_MenuStage;
		shared_ptr<Coursor> m_Cursor;
		shared_ptr<Sprite> m_CurrentLine;

		shared_ptr<Sprite> m_BackGround;
		vector<shared_ptr<Sprite>> m_ColorPalette;
		vector<shared_ptr<Sprite>> m_GimmcikSprites;
		vector<Line> m_Lines;

		wstring m_ColorTexture;
		wstring m_BackGroundTexture;

		Viewport m_MainViewPort;

		shared_ptr<PoseMenu> m_PoseMenu;
		int m_ColorHandle;
		int m_GimmikcHandle;

		bool UpdateOnCoursorHandle();
		vector<pair<int, int>> ConvertColorGimmickHandles(vector<Line>& lines);
		void DrawLine(Vec3 start, Vec3 end);
	public:
		GameMenu(const shared_ptr<Stage>& ptr,const wstring& colorTex,const wstring& backGroundTex,Viewport& mainViewport) 
			: Object(ptr),
			m_ColorTexture(colorTex),m_BackGroundTexture(backGroundTex),m_MainViewPort(mainViewport), 
			m_MenuStage(ptr), m_ColorHandle(-1),m_GimmikcHandle(-1){}
		virtual ~GameMenu(){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		int OnCoursorHandle(vector<shared_ptr<Sprite>>& sprites);


		void SetDrawActive(bool flag);

		//ヒント
		void HintCreate();
		void LoadHintData();
		vector<Vec3> m_HintStartPos;
		vector<Vec3> m_HintEndPos;
		vector<shared_ptr<Sprite>> m_HintLines;
		vector<pair<int, int>> m_HintPairs;
		size_t m_HintIndex = 0; // 現在表示するヒントのインデックス

		bool IsHintPairMatched(size_t hintIndex)
		{
			if (hintIndex >= m_HintPairs.size()) return false;

			auto& hintPair = m_HintPairs[hintIndex];

			for (const auto& line : m_Lines)
			{
				if (line.m_PairHandle == hintPair ||
					line.m_PairHandle == pair<int, int>{hintPair.second, hintPair.first})
				{
					return true;
				}
			}

			return false;
		}

	};

	class PoseMenu : public Object {
		shared_ptr<GameMenu> m_GameMenu;
		shared_ptr<Stage> m_MenuStage;
		Vec3 m_TopLeftPosition;
		Vec2 m_ButtonSize;
	public:
		PoseMenu(const shared_ptr<Stage>& ptr,const shared_ptr<GameMenu>& menu,Vec3 topLeft,Vec2 size):Object(ptr),
			m_MenuStage(ptr),m_GameMenu(menu),
			m_TopLeftPosition(topLeft),m_ButtonSize(size){}

		virtual void OnCreate()override;

		void Open();
		void Close();
		void MoveSelectStage();
		void MoveTitleStage();
		void SettingSound();
		void CloseNewGame();
		void OpenExpainGimmicks();
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

		void SetUpdateActive(bool flag) {
			Object::SetUpdateActive(flag);
			m_Coursor->SetDrawActive(flag);
		}


	};
}
//end basecross
