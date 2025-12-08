/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"
#include "Sprite.h"
namespace basecross{
	class GimmickHand;


	class EntityManager {
		vector<shared_ptr<MoveCube>> m_Players;
	public:
		void Update();

		bool IsUpdate();

		void StartPlayer();
		void StopPlayer();

		void AddPlayer(shared_ptr<MoveCube>& player) {
			m_Players.push_back(player);
		}
		vector<shared_ptr<MoveCube>> GetPlayers() { return m_Players; }

		void DestroyPlayer(shared_ptr<MoveCube>& player);
	};


	enum class GameState {
		Put, Game, Over, Clear
	};
	class GameFlowManager {
		GameState m_GameState;

		float m_UpdateTicks;
		float m_Tick;
		float m_Rate;
	public:
		GameFlowManager():
			m_GameState(GameState::Put),
			m_UpdateTicks(0.0f),m_Tick(0.0f),m_Rate(1.0f){ }

		void SetUpdateTick(float ticks) { m_UpdateTicks = ticks; }
		void SetTickRate(float rate) { m_Rate = rate; }

		void Update();

		bool CompareState(GameState state) {
			return m_GameState == state;
		}

		float GetGameTick() { return m_UpdateTicks / m_Rate; }

		bool IsPut() {
			return CompareState(GameState::Put);
		}
		bool IsGame() { 
			return CompareState(GameState::Game);
		}
		bool IsClear() {
			return CompareState(GameState::Clear);
		}
		bool IsOver() {
			return CompareState(GameState::Over);
		}
		bool IsFinished() {
			return IsClear() || IsOver();
		}


		void GameClear();
		void GameOver();
		void GameRestart();
		void GameStart();
	};


	class LevelManager {
		Json m_MapFile;	//Jsonファイルデータ

		shared_ptr<GimmickHand> m_Hand;	//ギミックデータ
		shared_ptr<Map> m_Map;	//マップデータ

		vector<pair<int, int>> m_CurrentPairs;	//現在の組み合わせ
		vector<pair<int, int>> m_BeforePairs;	//前フレームの組み合わせ
	public:
		LevelManager() : m_MapFile{} {}

		void Update();
		void Load(const wstring& key);

		Json GetJson()const { return m_MapFile; }
		shared_ptr<Map> GetMap()const { return m_Map; }
		shared_ptr<GimmickHand> GetHand()const { return m_Hand; }
		
		Vec3 GetMapSize();

		vector<pair<int, int>> GetCurrentPairs()const { return m_CurrentPairs; }
		vector<pair<int, int>> GetBeforePairs()const { return m_BeforePairs; }

		/// <summary>
		/// 開始可能か判別
		/// </summary>
		/// <returns>開始判定</returns>
		bool IsStart();

		/// <summary>
		/// 組み合わせに追加
		/// </summary>
		/// <param name="gimmickPair">組み合わせ</param>
		void AddPair(pair<int,int>& gimmickPair);

		/// <summary>
		/// 組み合わせから削除
		/// </summary>
		/// <param name="gimmickPair">組み合わせ</param>
		void RemovePair(pair<int, int>& gimmickPair);

		/// <summary>
		/// 重複した色とギミックを使用した組み合わせを削除
		/// </summary>
		/// <param name="color">色の番号</param>
		/// <param name="gimmick">ギミックの番号</param>
		void RemovePair(int color, int gimmick);
	};

	class GameManager : public SingletonBase<GameManager> {
		friend class SingletonBase<GameManager>;
		GameManager() {}

		shared_ptr<Stage> m_Stage;
		shared_ptr<Stage> m_MenuStage;

		shared_ptr<LevelManager> m_LevelManager;
		shared_ptr<GameFlowManager> m_GameFlowManager;
		shared_ptr<EntityManager> m_EntityManager;

		vector<shared_ptr<Sprite>> m_EffectSprite;
		shared_ptr<SpriteFade> m_SpriteFade;

		map<wstring, Vec3> m_DirectionMap;

		bool m_IsFading;

		Json m_KeyConfigFile;

		//リザルト
		void ResultCreate();
		void ResultUpdate();
		float m_ResultTime;
		int m_CurrentStarIndex = 0;
		int m_GameEvaluation = 3;
		vector<shared_ptr<Sprite>> m_StarSp;
		vector<shared_ptr<Sprite>> m_EvaluationSp;

		void StartFade();
	public:
		/// <summary>
		/// 登録されている情報を初期化する
		/// </summary>
		void Reset();

		shared_ptr<LevelManager> GetLevelManager()const { return m_LevelManager; }
		shared_ptr<GameFlowManager> GetFlowManager()const { return m_GameFlowManager; }
		shared_ptr<EntityManager> GetEntityManager()const { return m_EntityManager; }

		/// <summary>
		/// ゲームをリスタートする。
		/// </summary>
		/// <param name="isAll">すべて最初からにするか</param>
		void RestartGame(bool isAll = false);

		void SetGameStage(const shared_ptr<Stage>& stage) {
			m_Stage = stage;
		}
		void SetMenuStage(const shared_ptr<Stage>& stage) {
			m_MenuStage = stage;
		}
		shared_ptr<GameStage> GetGameStage() {
			return dynamic_pointer_cast<GameStage>(m_Stage);
		}
		shared_ptr<MenuStage> GetMenuStage() {
			return dynamic_pointer_cast<MenuStage>(m_MenuStage);
		}

		Vec3 DirectionStrToVec(const wstring& str) {
			return m_DirectionMap[str];
		}
		wstring DirectionVecToStr(Vec3& vec) {
			for (auto& dire : m_DirectionMap) {
				if (dire.second == vec) {
					return dire.first;
				}
			}
			return L"";
		}

		void Start();
		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		void InputUpdate();

		wstring GetKeyConfig(const wstring& key) {
			return m_KeyConfigFile.At<JsonString>(key)->GetValue();
		}

		void DrawGoalEffect();

		void DeleteEffectSprite() {
			for (int i = 0; i < m_EffectSprite.size(); i++) {
				m_Stage->RemoveGameObject<Sprite>(m_EffectSprite[i]);
			}
			m_EffectSprite.clear();
		}

		void SetGameEvaluation(int eval)
		{
			m_GameEvaluation = eval;
		}

	};

}
//end basecross
