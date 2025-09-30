/*!
@file Gimmick.h
@brief ÉMÉ~ÉbÉNÇ»Ç«
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	enum class GimmickObjects
	{
		None,
		Goal,
		SetPlayer,
		CourseCorrection,
		Upper
	};

	class Gimmicks : public Object
	{
	protected:
		shared_ptr<MoveCube> m_Cube;

	public:
		Gimmicks(const shared_ptr<Stage>& ptrStage);
		~Gimmicks();

		virtual void OnCreate();
		virtual void Begin();
		virtual void Update();
		virtual void End();
		virtual void GimmickDelete();

		virtual GimmickObjects GetGimmickType()
		{
			return GimmickObjects::None;
		}
	};

	class GimmickGoal : public Gimmicks
	{
	public:

		GimmickGoal(const shared_ptr<Stage>& ptrGimmick);
		~GimmickGoal();
		GimmickObjects GetGimmickType()override
		{
			return GimmickObjects::Goal;
		};

		virtual void OnCreate();
		virtual void Begin();
		virtual void Update();
		virtual void End() {}
	};

	class GimmickSetPlayer : public Gimmicks
	{
		Vec3 m_Velocity;
	public:
		GimmickSetPlayer(const shared_ptr<Stage>& ptrGimmick);
		~GimmickSetPlayer();
		GimmickObjects GetGimmickType()override
		{
			return GimmickObjects::SetPlayer;
		};

		virtual void OnCreate();
		virtual void Begin();
		virtual void Update();
		virtual void End() {}

		void SetVelocity(Vec3 velocity)
		{
			m_Velocity = velocity;
		}
	};
}
//end basecross
