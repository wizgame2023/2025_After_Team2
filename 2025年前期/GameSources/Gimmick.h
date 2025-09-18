/*!
@file Gimmick.h
@brief ÉMÉ~ÉbÉNÇ»Ç«
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	class Gimmicks : public Object
	{
		shared_ptr<PNTStaticDraw> m_Draw;

		bool m_IsGoalFlag;
	public:
		enum class Objects
		{
			Goal,
			SetPlayer,
			CourseCorrection,
			Upper
		};

	private:
		Objects m_Obj;


	public:
		Gimmicks(const shared_ptr<Stage>& ptrStage, Objects obj);
		~Gimmicks();

		void OnCreate() override;
		void OnUpdate() override;

		void Goal();
		void IsGoalDirection(const shared_ptr<MoveSphere>& player);
	};

}
//end basecross
