/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	// int型のものはすべてギミック用クラスに置き換えましょう

	class GimmickManager : public SingletonBase<GimmickManager> {
		friend class SingletonBase<GimmickManager>;
		GimmickManager() = default;

		vector<int> m_Gimmicks;	//手持ちのギミック
		vector<bool> m_UsedGimmicks;	//手持ちのギミックが使用済みかどうか
	public:
		void Reset() {
			m_Gimmicks.clear();
			m_UsedGimmicks.clear();
		}

		void AddGimmick(int i) {
			m_Gimmicks.push_back(i);
			m_UsedGimmicks.push_back(false);
		}

		int GetGimmick(int i) {
			if (m_Gimmicks.size() <= i) return -1;
			return m_Gimmicks[i];
		}
	};
}
//end basecross
