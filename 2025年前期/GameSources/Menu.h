/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	class Cursor;
	class PoseMenu;
	class MovieWindow;
	class ExplainMenu;
	class SoundMenu;

	struct Line {
		shared_ptr<Sprite> m_Line;
		pair<int, int> m_PairHandle;

	};
	class GameMenu : public Object {
		bool m_IsCursor;

		map<GimmickObjects, wstring> m_GimmickTextures;
		shared_ptr<Stage> m_Stage;
		shared_ptr<Cursor> m_Cursor;
		shared_ptr<Sprite> m_CurrentLine;


		shared_ptr<Sprite> m_CurrentExplain;
		shared_ptr<Sprite> m_ExplainBox;
		shared_ptr<Sprite> m_OperateInfo;

		shared_ptr<Sprite> m_BackGround;
		vector<shared_ptr<Sprite>> m_ColorPalette;
		vector<shared_ptr<Sprite>> m_GimmickIcons;
		vector<shared_ptr<Sprite>> m_HandlerSprites;
		vector<shared_ptr<Sprite>> m_ExplainIcons;
		vector<Line> m_Lines;

		wstring m_ColorTexture;
		wstring m_BackGroundTexture;

		Viewport m_MainViewPort;

		shared_ptr<MovieWindow> m_MovieWindow;
		shared_ptr<PoseMenu> m_PoseMenu;
		int m_ColorHandle;	//指定中の色
		int m_GimmickHandle;//指定中のギミック
		int m_CursorHandle;	//カーソルがある番号(色+ギミックの合計値が最大)

		float m_ConnectOffsetX;

		bool UpdateOnCoursorHandle();
		vector<pair<int, int>> ConvertColorGimmickHandles(vector<Line>& lines);
		int FindColorHandleIndex(int colorHandle);
		int FindGimmickHandleIndex(int gimmickHandle);
		void DrawLine(Vec3 start, Vec3 end);
		void DrawExpain();
	public:
		GameMenu(const shared_ptr<Stage>& ptr,const wstring& colorTex,const wstring& backGroundTex,Viewport& mainViewport) 
			: Object(ptr),
			m_ColorTexture(colorTex),m_BackGroundTexture(backGroundTex),m_MainViewPort(mainViewport), 
			m_ConnectOffsetX(0),
			m_Stage(ptr), m_ColorHandle(-1),m_GimmickHandle(-1), m_IsCursor(true){}
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
		shared_ptr<Sprite> m_BackGround;
		shared_ptr<ExplainMenu> m_ExplainMenu;
		shared_ptr<SoundMenu> m_SoundMenu;

		Vec3 m_TopLeftPosition;
		Vec2 m_MenuSize;
		Vec2 m_ButtonSize;
	public:
		PoseMenu(const shared_ptr<Stage>& ptr,const shared_ptr<GameMenu>& menu,Vec3 topLeft,Vec2 menuSize,Vec2 buttonSize):Object(ptr),
			m_GameMenu(menu),
			m_TopLeftPosition(topLeft),m_MenuSize(menuSize), m_ButtonSize(buttonSize) { }

		virtual void OnCreate()override;

		void SetSoundMenu(const shared_ptr<SoundMenu>& menu) {
			m_SoundMenu = menu;
		}
		bool IsOpen() {
			return GetDrawActive();
		}

		void Open();
		void Close();
		void MoveSelectStage();
		void MoveTitleStage();
		void SettingSound();
		void CloseNewGame();
		void OpenExpainGimmicks();
	};
	struct ExplainData {
		wstring m_MenuIconKey;
		wstring m_MovieFilename;
		wstring m_ExplainKey;
		ExplainData(const wstring& icon,const wstring& movie,const wstring& explain):
			m_MenuIconKey(icon),m_MovieFilename(movie),m_ExplainKey(explain){ }
	};
	class ExplainMenu : public Object{
		shared_ptr<Sprite> m_BackGround;
		shared_ptr<Sprite> m_ExplainStr;
		vector<shared_ptr<Sprite>> m_Icons;
		shared_ptr<MovieWindow> m_ExplainMovie;

		vector<ExplainData> m_ExplainDatas;

		void CreateExplain();
	public:
		ExplainMenu(const shared_ptr<Stage>& ptr) :Object(ptr){}
		virtual ~ExplainMenu(){}

		virtual void OnCreate();
		virtual void OnUpdate();

		void AddExplain(ExplainData& data) {
			m_ExplainDatas.push_back(data);
		}
		void Open();
		void Close();
	};

	class SoundMenu : public Object {
		shared_ptr<Sprite> m_SoundMenu;
		array<shared_ptr<Sprite>, 2> m_SoundBars;
		array<shared_ptr<Sprite>, 2> m_SoundBarFrames;

		Vec2 m_MenuSize;
		Vec3 m_MenuPosition;

		Vec2 m_BarSize;
	public:
		SoundMenu(const shared_ptr<Stage>& ptr,Vec3 menuPosition,Vec2 menuSize) :
			m_MenuPosition(menuPosition),m_MenuSize(menuSize), m_BarSize(1,1), Object(ptr) {}

		virtual void OnCreate();
		virtual void OnUpdate();

		void Open();
		void Close();
	};

	enum class CoursorMode {
		Stick,Mouse
	};

	class Cursor : public Object {
		shared_ptr<Sprite> m_Cursor;
		CoursorMode m_Mode;

		wstring m_CoursorTexture;

		AABB m_MoveArea;
		float m_MoveSpeed;
	public:
		Cursor(const shared_ptr<Stage>& ptr,const wstring& coursorTex) : 
			Object(ptr), m_Mode(CoursorMode::Stick),m_CoursorTexture(coursorTex){}

		virtual void OnCreate()override;
		virtual void OnUpdate()override;

		Vec3 GetPosition() {
			return m_Cursor->GetPosition();
		}
		void SetPosition(Vec3 position) {
			m_Cursor->SetPosition(position);
		}

		void SetCoursorMode(CoursorMode mode) {
			m_Mode = mode;
		}

		void SetMoveArea(Vec2 max, Vec2 min) {
			m_MoveArea.m_Max = max;
			m_MoveArea.m_Min = min;
			auto center = m_MoveArea.GetCenter();
			m_Cursor->SetPosition(center);
		}
		void SetMoveSpeed(float speed) {
			m_MoveSpeed = speed;
		}
		void SetCoursorSize(float size) {
			m_Cursor->SetSize(Vec2(size));
		}
		Vec3 LimitMoveArea();

		bool IsOnArea(Vec2 max, Vec2 min);

		void SetUpdateActive(bool flag) {
			Object::SetUpdateActive(flag);
			m_Cursor->SetDrawActive(flag);
		}
		void SetDrawActive(bool flag) {
			Object::SetDrawActive(flag);
			m_Cursor->SetDrawActive(flag);
		}

		void Destroy() {
			m_Stage->RemoveGameObject<Sprite>(m_Cursor);
			m_Stage->RemoveGameObject<Cursor>(GetThis<Cursor>());
		}
	};


	
}
//end basecross
