/*!
@file Effect.h
@brief エフェクト
@authors Okawara Yushin
*/

#pragma once
#include "stdafx.h"

// エフェクシア
#pragma comment(lib, "Effekseer.lib")
#pragma comment(lib, "EffekseerRendererDX11.lib")


namespace basecross {

	class Effect : public GameObject
	{
		// エフェクシアの設定に必要の変数
		Effekseer::EffectRef m_Effect;
		Effekseer::ManagerRef m_Manager;
		EffekseerRendererDX11::RendererRef m_Renderer;
		Effekseer::Handle m_Handle;

		Effekseer::Vector3D m_EffectPosVector;
		Effekseer::Vector3D m_EffectAxisVector;

		shared_ptr<Stage> m_Stage;

		wstring m_FileName;

		Vec3 m_Pos;
		Vec3 m_Size;
		Vec3 m_Rot;

		Vec3 m_MoveVec;
		Vec3 m_Axis;


		bool m_IsRelease;

		float m_FrameSpeed;
		float m_speed;
		float m_Angle;

		
	public:
		/*!
		@brief エフェクトのコンストラクタ
		@param[in] wstring エフェクト名
		@param[in] pos エフェクトの位置
		@param[in] rot エフェクトの向き
		@param[in] size エフェクトのサイズ
		@return なし
		*/
		Effect(const shared_ptr<Stage>& StagePtr, wstring name, Vec3 pos = Vec3(0.0f), Vec3 rot = Vec3(0.0f), Vec3 size = Vec3(1.0f));
		~Effect() 
		{
			// エフェクトのマネージャーの解放
			m_Manager.Reset();
			// 描画モジュールの解放
			m_Renderer.Reset();

			m_Manager->StopRoot(m_Effect);
		}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
		virtual void OnDestroy() override;


		void EffectDelete();

		/*!
		@brief エフェクトの位置を設定する関数
		@param[in] pos エフェクトの位置
		@return なし
		*/
		void SetEffectPoison(const Vec3& pos);

		/*!
		@brief エフェクトの位置を所得する関数
		@return エフェクトの位置
		*/
		Vec3 GetEffectPoison();

		/*!
		@brief エフェクトの位置を加算する関数
		@param[in] moveVec 加算するベクトルの値
		@return なし
		*/
		void AddEffectPoison(const Vec3& moveVec);

		/*!
		@brief エフェクトのサイズを設定する関数
		@param[in] size エフェクトのスケール
		@return なし
		*/
		void SetEffectSize(const Vec3& size);

		/*!
		@brief エフェクトのスケールを所得する関数
		@return エフェクトのスケール
		*/
		Vec3 GetEffectSize();

		/*!
		@brief エフェクトの向きを設定する関数
		@param[in] rot エフェクトの向き
		@return なし
		*/
		void SetEffectRotation(const Vec3& rot);

		/*!
		@brief エフェクトの向きを所得する関数
		@return エフェクトの向き
		*/
		Vec3 GetEffectRotation();

		/*!
		@brief エフェクトの向きを設定する関数
		@param[in] axis 軸
		@param[in] angle 角度(ラジアン)
		@return なし
		*/
		void SetEffectQuaternion(const Vec3& axis, const float& angle);

		/*!
		@brief エフェクトの色を設定する関数
		@param[in] cal 色
		@return なし
		*/
		void SetEffectColor(const Col4& cal);
		/*!
		@brief エフェクトのフレームの速さを設定する関数
		@param[in] speed エフェクトのフレームの速さ
		@return なし
		*/
		void SetEffectFrameSpeed(const float& speed);

		/*!
		@brief エフェクトのフレームの速さを所得する関数
		@return エフェクトのフレームの速さ
		*/
		float GetEffectFrameSpeed();

		/*!
		@brief エフェクトのフレームの速さを設定する関数
		@param[in] speed エフェクトのフレームの速さ
		@return なし
		*/
		void SetEffectSpeed(const float& speed);

		/*!
		@brief エフェクトのフレームの速さを所得する関数
		@return エフェクトのフレームの速さ
		*/
		float GetEffectSpeed();

		/*!
		@brief エフェクトの名前を所得する関数
		@return エフェクトの
		*/
		wstring GetEffectName();

		/*!
		@brief  再生中のエフェクトの描画終了を取得する関数
		@return 再生中のエフェクトが存在しない場合、true を返す
		*/
		bool EffectEnd();

		/*!
		@brief  再生中のエフェクトのインスタンス数を取得する関数
		@return 再生中のエフェクトのインスタンス数
		*/
		int GetEffectInstance();
	private:

		/*!
		@brief 投影行列を設定
		@return なし
		*/
		void SetViewProj();
	};
}
//end basecross
