/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	void MovieWindow::OnCreate() {
		m_Transform = GetComponent<Transform>();
		//初期化
		m_Transform->SetPosition(Vec3());
		m_Transform->SetRotation(Vec3());
		m_Transform->SetScale(Vec3(1.0f));
	}
	void MovieWindow::OnUpdate() {
		
	}
	void MovieWindow::Play(const wstring& filename) {
		if (filename != L"") {
			Vec3 position = GetPosition();
			RECT rect = { position.x,position.y,position.x + m_WindowSize.x,position.y + m_WindowSize.y };
			App::GetApp()->PlayMovie(filename, &rect);
		}
	}
	void MovieWindow::Stop() {
		App::GetApp()->ClearMovie();
	}
}
//end basecross
