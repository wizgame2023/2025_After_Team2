/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	class SkyCube : public Object {
		wstring m_TexKey;
	public:
		SkyCube(const shared_ptr<Stage>& ptr,const wstring& key):Object(ptr), m_TexKey(key){}
		~SkyCube(){}

		virtual void OnCreate()override;
	};
}
//end basecross
