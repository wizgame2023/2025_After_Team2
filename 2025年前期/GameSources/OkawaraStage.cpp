/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	void OStage::OnCreate() 
	{
		try
		{
			GameStage::OnCreate();

			auto sphere = AddGameObject<MoveSphere>(2.0f,Vec3(0.0f,0.0f,1.0f));
			sphere->SetTarget(sphere->GetPosition() + Vec3(0.0f, 1.0f, 0.0f));
			sphere->SetVelocity(Vec3(1.0f, 0.0f, 0.0f));
			
		}
		catch (...)
		{
			throw;
		}
	}

	void OStage::OnUpdate() 
	{

	}

}
//end basecross
