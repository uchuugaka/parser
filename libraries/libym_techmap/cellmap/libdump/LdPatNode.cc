
/// @file libym_techmap/cellmap/libdump/LdPatNode.cc
/// @brief LdPatMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LdPatNode.h"


BEGIN_NAMESPACE_YM_CELLMAP_LIBDUMP

//////////////////////////////////////////////////////////////////////
// クラス LdPatNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LdPatNode::LdPatNode() :
  mType(0U),
  mLocked(false)
{
  mFanin[0] = NULL;
  mFanin[1] = NULL;
}

// @brief デストラクタ
LdPatNode::~LdPatNode()
{
}

// @brief ノード番号を返す．
ymuint
LdPatNode::id() const
{
  return mId;
}

// @brief 'lock' 状態を得る．
bool
LdPatNode::is_locked() const
{
  return mLocked;
}

// @brief 入力の時 true を返す．
bool
LdPatNode::is_input() const
{
  return ((mType & 3U) == kInput);
}

// @brief AND の時 true を返す．
bool
LdPatNode::is_and() const
{
  return ((mType & 3U) == kAnd);
}

// @brief XOR の時 true を返す．
bool
LdPatNode::is_xor() const
{
  return ((mType & 3U) == kXor);
}

// @brief 入力の時に入力番号を返す．
ymuint
LdPatNode::input_id() const
{
  return mType >> 2;
}

// @brief AND/XOR の時にファンインのノードを返す．
// @param[in] pos 位置 ( 0 or 1 )
LdPatNode*
LdPatNode::fanin(ymuint pos) const
{
  return mFanin[pos];
}

// @brief AND/XOR の時にファンインの極性を返す．
// @param[in] pos 位置 ( 0 or 1 )
// @retval true 反転あり
// @retval false 反転なし
bool
LdPatNode::fanin_inv(ymuint pos) const
{
  return static_cast<bool>((mType >> (pos + 2)) & 1U);
}

// @brief 入力に設定する．
// @param[in] input_id 入力番号
void
LdPatNode::set_input(ymuint input_id)
{
  mType = (input_id << 2) | kInput;
}

// @brief ANDに設定する．
// @param[in] inv0, inv1 ファンインの反転属性
void
LdPatNode::set_and(bool inv0,
		bool inv1)
{
  mType = kAnd;
  if ( inv0 ) {
    mType |= 4U;
  }
  if ( inv1 ) {
    mType |= 8U;
  }
}

// @brief XORに設定する．
void
LdPatNode::set_xor()
{
  mType = kXor;
}

// @brief 'lock' する．
// @note ファンインに再帰する．
void
LdPatNode::set_locked()
{
  if ( !mLocked ) {
    mLocked = true;
    if ( !is_input() ) {
      fanin(0)->set_locked();
      fanin(1)->set_locked();
    }
  }
}

END_NAMESPACE_YM_CELLMAP_LIBDUMP