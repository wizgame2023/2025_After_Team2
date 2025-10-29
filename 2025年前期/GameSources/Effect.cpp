/*!
@file Effect.cpp
@brief エフェクトの実体
@authors Okawara Yushin
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	Effect::Effect(const shared_ptr<Stage>& StagePtr, wstring name, Vec3 pos, Vec3 rot, Vec3 size) :
		GameObject(StagePtr),
		m_FileName(name),
		m_Pos(pos),
		m_Rot(rot),
		m_Size(size),
		m_FrameSpeed(0.01667f),
		m_speed(1.0f),
		m_MoveVec(Vec3(0.0f)),
		m_Axis(Vec3(0.0f)),
		m_Angle(0.0f)
	{
	}

	void Effect::OnCreate()
	{
		auto d3D11Device = App::GetApp()->GetDeviceResources()->GetD3DDevice();
		auto d3D11DeviceContext = App::GetApp()->GetDeviceResources()->GetD3DDeviceContext();;
		m_Renderer = EffekseerRendererDX11::Renderer::Create(d3D11Device, d3D11DeviceContext, 8000);
		// エフェクトのマネージャーの作成
		m_Manager = Effekseer::Manager::Create(8000);
		// 描画モジュールの設定
		m_Manager->SetSpriteRenderer(m_Renderer->CreateSpriteRenderer());
		m_Manager->SetRibbonRenderer(m_Renderer->CreateRibbonRenderer());
		m_Manager->SetRingRenderer(m_Renderer->CreateRingRenderer());
		m_Manager->SetTrackRenderer(m_Renderer->CreateTrackRenderer());
		m_Manager->SetModelRenderer(m_Renderer->CreateModelRenderer());

		// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
		// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
		m_Manager->SetTextureLoader(m_Renderer->CreateTextureLoader());
		m_Manager->SetModelLoader(m_Renderer->CreateModelLoader());
		m_Manager->SetMaterialLoader(m_Renderer->CreateMaterialLoader());
		m_Manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

		auto& app = App::GetApp();
		wstring path = app->GetDataDirWString();
		wstring effectPath = path + L"Effects/EffectsData/";
		wstring wstrEfk = effectPath + m_FileName;
		m_Effect = Effekseer::Effect::Create(m_Manager, (const char16_t*)wstrEfk.c_str());

	}

	void Effect::OnUpdate()
	{
		// エフェクトの更新処理を行う
		float frame = 1 / m_FrameSpeed;
		float frameSpeed = frame / 60.0f;
		m_Manager->Update(frameSpeed);
		SetViewProj();

	}

	void Effect::OnDraw()
	{
		if (m_IsRelease == false)
		{
			m_Handle = m_Manager->Play(m_Effect, m_Pos.x, m_Pos.y, m_Pos.z);// 初期位置の設定

			m_IsRelease = true;
		}

		// エフェクトの描画開始処理
		m_Renderer->BeginRendering();

		// エフェクトの描画を実行
		m_Manager->Draw();

		// エフェクトの描画終了処理
		m_Renderer->EndRendering();
	}

	void Mat4x4Matrix44(const bsm::Mat4x4& src, Effekseer::Matrix44& dest) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				dest.Values[i][j] = src(i, j);
			}
		}
	}

	void  Effect::SetViewProj()
	{
		auto stage = GetStage();
		if (dynamic_pointer_cast<GameStage>(stage) == nullptr) return;

		auto& camera = stage->GetView()->GetTargetCamera();
		Effekseer::Matrix44 v, p;
		Mat4x4Matrix44(camera->GetViewMatrix(), v);
		Mat4x4Matrix44(camera->GetProjMatrix(), p);
		m_Renderer->SetCameraMatrix(v);
		m_Renderer->SetProjectionMatrix(p);

	}

	void Effect::SetEffectPoison(const Vec3& pos)
	{
		m_Pos = pos;
		m_Manager->SetLocation(m_Handle, m_Pos.x, m_Pos.y, m_Pos.z);// 位置の設定
	}

	Vec3 Effect::GetEffectPoison()
	{
		return m_Pos;
	}

	void Effect::AddEffectPoison(const Vec3& moveVec)
	{
		m_MoveVec = moveVec;
		m_EffectPosVector.X = m_MoveVec.x;
		m_EffectPosVector.Y = m_MoveVec.y;
		m_EffectPosVector.Z = m_MoveVec.z;

		m_Manager->AddLocation(m_Handle, m_EffectPosVector);//現在に加算する値を設定
	}

	void Effect::SetEffectSize(const Vec3& size)
	{
		m_Size = size;
		m_Manager->SetScale(m_Handle, m_Size.x, m_Size.y, m_Size.z);// スケールを設定
	}

	Vec3 Effect::GetEffectSize()
	{
		return m_Size;
	}

	void Effect::SetEffectRotation(const Vec3& rot)
	{
		m_Rot = rot;
		m_Manager->SetRotation(m_Handle, m_Rot.x, m_Rot.y, m_Rot.z);// 回転を設定
	}

	Vec3 Effect::GetEffectRotation()
	{
		return m_Rot;
	}

	void Effect::SetEffectQuaternion(const Vec3& axis, const float& angle)
	{
		m_Axis = axis;
		m_Angle = angle;

		m_EffectAxisVector.X = m_Axis.x;
		m_EffectAxisVector.Y = m_Axis.y;
		m_EffectAxisVector.Z = m_Axis.z;

		m_Manager->SetRotation(m_Handle, m_EffectAxisVector, m_Angle);
	}
	void Effect::SetEffectFrameSpeed(const float& speed)
	{
		m_FrameSpeed = speed;

	}

	float Effect::GetEffectFrameSpeed()
	{
		return m_FrameSpeed;
	}

	void Effect::SetEffectSpeed(const float& speed)
	{
		m_speed = speed;
		m_Manager->SetSpeed(m_Handle, m_speed);// 再生スピードの設定
	}

	float Effect::GetEffectSpeed()
	{
		return m_speed;
	}

	wstring Effect::GetEffectName()
	{
		return m_FileName;
	}
}
//end basecross


