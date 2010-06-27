
/// @file libym_mvn/MvLogOp.cc
/// @brief MvLogOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvLogOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvAnd::MvAnd(MvModule* module,
	     ymuint bit_width) :
  MvBinaryOp(module, bit_width, bit_width, bit_width)
{
}

// @brief デストラクタ
MvAnd::~MvAnd()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvAnd::type() const
{
  return kAnd;
}


//////////////////////////////////////////////////////////////////////
// クラス MvOr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvOr::MvOr(MvModule* module,
	     ymuint bit_width) :
  MvBinaryOp(module, bit_width, bit_width, bit_width)
{
}

// @brief デストラクタ
MvOr::~MvOr()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvOr::type() const
{
  return kOr;
}


//////////////////////////////////////////////////////////////////////
// クラス MvXor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvXor::MvXor(MvModule* module,
	     ymuint bit_width) :
  MvBinaryOp(module, bit_width, bit_width, bit_width)
{
}

// @brief デストラクタ
MvXor::~MvXor()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvXor::type() const
{
  return kXor;
}

END_NAMESPACE_YM_MVN
