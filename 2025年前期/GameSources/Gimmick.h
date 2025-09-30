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
		bool m_IsGoalFlag;
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

}
//end basecross
