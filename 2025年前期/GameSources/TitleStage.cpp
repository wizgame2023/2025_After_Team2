/*!
@file TitleStage.cpp
@brief タイトルステージの実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	タイトルステージ実体
	//--------------------------------------------------------------------------------------
	void TitleStage::CreateViewLight()
	{
		const Vec3 eye(0.0f, 5.0f, -5.0f);
		const Vec3 at(0.0f);
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(eye);
		PtrCamera->SetAt(at);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	void TitleStage::Json()
	{
		ResourceManager::Load(L"titleResource.json");

		ResourceManager::RegisterTexture(L"UI");
	}


	void TitleStage::SpriteCreate()
	{
		Json();

		auto titleSp = AddGameObject<Sprite>(L"TitleLogoUI", Vec3(0.0f), Vec2(1000, 500), Anchor::Bottom);
		m_Cursor = AddGameObject<Sprite>(L"MOUSE_CURSOR", Vec3(0.0f, -100.0f, 0.0f), Vec2(30, 30), Anchor::Center);
		m_Cursor->SetLayer(3);
		StartSpriteCreate();

	}

	void TitleStage::StartSpriteCreate()
	{
		auto colorSize = Vec2(50, 50);
		for (int i = 0; i < 2; i++)
		{
			int xPos = (i == 0) ? -150.0f : 150.0f;
			int handleIndex = (i == 0) ? 0 : -1;

			auto colorPalette = AddGameObject<Sprite>(L"ColorPaletteUI", Vec3(xPos, -150.0f, 0.0f), colorSize, Anchor::Center);
			colorPalette->SetLayer(2);
			m_ColorPalettes.push_back(colorPalette);

			if (i==0)
			{
				auto handle = AddGameObject<Sprite>(
					L"HANDLER",
					static_cast<Vec3>(m_ColorPalettes[i]->GetAnchorPosition(Anchor::Right) + handleIndex),
					colorSize * 0.5, Anchor::Center);

				handle->SetLayer(1);

				m_Handlers.push_back(handle);
			}
			else
			{
				auto handle = AddGameObject<Sprite>(
					L"HANDLER",
					static_cast<Vec3>(m_ColorPalettes[i]->GetAnchorPosition(Anchor::Left) + handleIndex),
					colorSize * 0.5, Anchor::Center);

				handle->SetLayer(1);

				m_Handlers.push_back(handle);
			}
		}

	}


	void TitleStage::OnCreate()
	{
		try {
			//ビューとライトの作成
			CreateViewLight();
			SpriteCreate();
		}
		catch (...) {
			throw;
		}
	}

	void TitleStage::OnUpdate()
	{
		auto& input = InputManager::GetInputManager();

		// カーソル移動処理（例：スティックで動かす）
		Vec2 stick = input->GetLStick();
		if (stick.lengthSqr() > 0.01f)
		{
			auto pos = m_Cursor->GetPosition();
			pos.x += stick.x * 5.0f;
			pos.y += stick.y * 5.0f;
			m_Cursor->SetPosition(pos);
		}

		if (input->GetDownButton(L"A"))
		{
			if (!m_ColorPalettes.empty())
			{
				auto cursorPos = m_Cursor->GetPosition();
				float minDist = FLT_MAX;
				float judgeThreshold = 50.0f; // 判定開始距離

				bool found = false;

				for (auto& palette : m_ColorPalettes)
				{
					auto palettePos = palette->GetPosition();
					float dist = (cursorPos - palettePos).length();

					if (dist < minDist)
					{
						minDist = dist;
						m_StartPos = palettePos;
					}

					// ★一定距離以内なら判定開始
					if (dist <= judgeThreshold)
					{
						found = true;
					}
				}

				if (found && m_Line == nullptr)
				{
					m_Line = AddGameObject<Sprite>(L"ColorPaletteUI", m_StartPos, Vec2(20, 20), Anchor::Center);
					m_Line->SetLayer(1);
					m_IsAPushed = true;
				}
			}
		}
		if (m_IsAPushed)
		{
			Vec3 currentPos = m_Cursor->GetPosition();
			Vec3 dist = currentPos - m_StartPos;

			// ラインを伸ばす処理
			m_Line->SetSize(Vec2(m_Line->GetSize().x, dist.length()));
			m_Line->SetAnchorPosition(m_StartPos + dist / 2.0f, Anchor::Center);
			m_Line->VectorToward(static_cast<Vec2>(dist.normalize()));

			// ここから「遠い方のパレット判定」
			if (m_ColorPalettes.size() >= 2)
			{
				// スタートに選ばれなかった方を「遠い方」とする
				Vec3 farPalettePos;
				if ((m_StartPos - m_ColorPalettes[0]->GetPosition()).length() <
					(m_StartPos - m_ColorPalettes[1]->GetPosition()).length())
				{
					farPalettePos = m_ColorPalettes[1]->GetPosition();
				}
				else
				{
					farPalettePos = m_ColorPalettes[0]->GetPosition();
				}

				// ラインの終端（カーソル位置）が遠い方に近づいたか判定
				float threshold = 30.0f; // ★一定距離のしきい値
				if ((currentPos - farPalettePos).length() <= threshold)
				{
					// ラインを遠い方にスナップ
					m_Line->SetSize(Vec2(m_Line->GetSize().x, (farPalettePos - m_StartPos).length()));
					m_Line->SetAnchorPosition(m_StartPos + (farPalettePos - m_StartPos) / 2.0f, Anchor::Center);
					m_Line->VectorToward(static_cast<Vec2>(farPalettePos - m_StartPos).normalize());
					m_IsConfirmed = true;

					if (m_StartSprite == nullptr)
					{
						m_StartSprite = AddGameObject<Sprite>(L"StartUI", Vec3(0.0f, -50.0f, 0.0f), Vec2(512, 128), Anchor::Center);
					}


				}
				else
				{
					RemoveGameObject<Sprite>(m_StartSprite);
					m_StartSprite = nullptr;
					m_IsConfirmed = false;
				}
			}
		}
		if(input->GetUpButton(L"A"))
		{
			if (!m_IsConfirmed)
			{
				RemoveGameObject<Sprite>(m_Line);
				m_Line = nullptr;
				m_IsAPushed = false;
			}
			else
			{
				m_IsAPushed = false;

				// 確定した場合の処理（例：ステージ遷移）
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToSelectStage");
			}
		}

		if (m_IsConfirmed && m_StartSprite != nullptr)
		{
			// 時間経過でアルファ値を変化させる
			static float blinkTime = 0.0f;
			blinkTime += App::GetApp()->GetElapsedTime(); // 経過時間を加算

			float alpha = 0.3f + 0.3f * sinf(blinkTime * 5.0f);
			m_StartSprite->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, alpha));
		}

	}
}
//end basecross
