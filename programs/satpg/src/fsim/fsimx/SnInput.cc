
/// @file SnInput.cc
/// @brief SnInput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SnInput.h"


BEGIN_NAMESPACE_YM_SATPG_FSIMX

//////////////////////////////////////////////////////////////////////
// SnInput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnInput::SnInput(ymuint32 id) :
  SimNode(id)
{
  set_level(0);
}

// @brief デストラクタ
SnInput::~SnInput()
{
}

// @brief ファンイン数を得る．
ymuint
SnInput::nfi() const
{
  return 0;
}

// @brief pos 番めのファンインを得る．
SimNode*
SnInput::fanin(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 正常値の計算を行う．(3値版)
PackedVal3
SnInput::_calc_gval3()
{
  assert_not_reached(__FILE__, __LINE__);
  return PackedVal3();
}

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnInput::_calc_fval3()
{
  assert_not_reached(__FILE__, __LINE__);
  return PackedVal3();
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnInput::calc_gobs3(ymuint ipos)
{
  return kPvAll0;
}

// @brief 内容をダンプする．
void
SnInput::dump(ostream& s) const
{
  s << "INPUT" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnBuff
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnBuff::SnBuff(ymuint32 id,
	       const vector<SimNode*>& inputs) :
  SnGate1(id, inputs)
{
}

// @brief デストラクタ
SnBuff::~SnBuff()
{
}

// @brief 正常値の計算を行う．(3値版)
PackedVal3
SnBuff::_calc_gval3()
{
  return mFanin->gval();
}

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnBuff::_calc_fval3()
{
  return mFanin->fval();
}

// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
PackedVal
SnBuff::calc_gobs3(ymuint ipos)
{
  return kPvAll1;
}

// @brief 内容をダンプする．
void
SnBuff::dump(ostream& s) const
{
  s << "BUFF(" << mFanin->id() << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// SnNot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SnNot::SnNot(ymuint32 id,
	     const vector<SimNode*>& inputs) :
  SnBuff(id, inputs)
{
}

// @brief デストラクタ
SnNot::~SnNot()
{
}

// @brief 正常値の計算を行う．(3値版)
PackedVal3
SnNot::_calc_gval3()
{
  return ~mFanin->gval();
}

// @brief 故障値の計算を行う．(3値版)
PackedVal3
SnNot::_calc_fval3()
{
  return ~mFanin->fval();
}

// @brief 内容をダンプする．
void
SnNot::dump(ostream& s) const
{
  s << "NOT(" << mFanin->id() << ")" << endl;
}

END_NAMESPACE_YM_SATPG_FSIMX
