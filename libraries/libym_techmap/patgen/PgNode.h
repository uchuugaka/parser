#ifndef LIBYM_TECHMAP_PATGEN_PGNODE_H
#define LIBYM_TECHMAP_PATGEN_PGNODE_H

/// @file libym_techmap/patgen/PgNode.h
/// @brief PgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen_nsdef.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

class PatGen;

//////////////////////////////////////////////////////////////////////
/// @class PgNode PgNode.h "PgNode.h"
/// @brief パタン生成用のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class PgNode
{
  friend class PatGen;

private:

  /// @brief コンストラクタ
  PgNode();

  /// @brief デストラクタ
  ~PgNode();


public:

  /// @brief ノード番号を返す．
  ymuint
  id() const;

  /// @brief 'lock' 状態を得る．
  bool
  is_locked() const;

  /// @brief 入力の時 true を返す．
  bool
  is_input() const;

  /// @brief AND の時 true を返す．
  bool
  is_and() const;

  /// @brief XOR の時 true を返す．
  bool
  is_xor() const;

  /// @brief 入力の時に入力番号を返す．
  ymuint
  input_id() const;

  /// @brief AND/XOR の時にファンインのノードを返す．
  /// @param[in] pos 位置 ( 0 or 1 )
  PgNode*
  fanin(ymuint pos) const;

  /// @brief AND/XOR の時にファンインの極性を返す．
  /// @param[in] pos 位置 ( 0 or 1 )
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  fanin_inv(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力に設定する．
  /// @param[in] input_id 入力番号
  void
  set_input(ymuint input_id);

  /// @brief ANDに設定する．
  /// @param[in] inv0, inv1 ファンインの反転属性
  void
  set_and(bool inv0,
	  bool inv1);

  /// @brief XORに設定する．
  void
  set_xor();

  /// @brief 'lock' する．
  /// @note ファンインに再帰する．
  void
  set_locked();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いる定数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの種類
  enum tType {
    kInput = 0,
    kAnd   = 1,
    kXor   = 2
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号
  ymuint32 mId;

  // ノードの種類 ( 2bit )
  //  + 入力番号 ( 30bit)
  // or
  //  + ファンインの極性 ( 1bit x 2)
  ymuint32 mType;

  // lock ビット
  bool mLocked;

  // ファンインのノード
  PgNode* mFanin[2];

  // ハッシュ表中のリンク
  PgNode* mLink;

};


//////////////////////////////////////////////////////////////////////
/// @class PgHandle PgNode.h "PgNode.h"
/// @brief PgNode と極性を表すクラス
//////////////////////////////////////////////////////////////////////
class PgHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 反転属性
  explicit
  PgHandle(PgNode* node = NULL,
	   bool inv = false);

  /// @brief デストラクタ
  ~PgHandle();


public:

  /// @brief ノードを取り出す．
  PgNode*
  node() const;

  /// @brief 反転属性を取り出す．
  bool
  inv() const;

  /// @brief 値を設定する．
  /// @param[in] node ノード
  /// @param[in] inv 反転属性
  void
  set(PgNode* node,
      bool inv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポインタと反転属性を合わせたもの
  ympuint mData;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード番号を返す．
inline
ymuint
PgNode::id() const
{
  return mId;
}

// @brief 'lock' 状態を得る．
inline
bool
PgNode::is_locked() const
{
  return mLocked;
}

// @brief 入力の時 true を返す．
inline
bool
PgNode::is_input() const
{
  return ((mType & 3U) == kInput);
}

// @brief AND の時 true を返す．
inline
bool
PgNode::is_and() const
{
  return ((mType & 3U) == kAnd);
}

// @brief XOR の時 true を返す．
inline
bool
PgNode::is_xor() const
{
  return ((mType & 3U) == kXor);
}

// @brief 入力の時に入力番号を返す．
inline
ymuint
PgNode::input_id() const
{
  return mType >> 2;
}

// @brief AND/XOR の時にファンインのノードを返す．
// @param[in] pos 位置 ( 0 or 1 )
inline
PgNode*
PgNode::fanin(ymuint pos) const
{
  return mFanin[pos];
}

// @brief AND/XOR の時にファンインの極性を返す．
// @param[in] pos 位置 ( 0 or 1 )
// @retval true 反転あり
// @retval false 反転なし
inline
bool
PgNode::fanin_inv(ymuint pos) const
{
  return static_cast<bool>((mType >> (pos + 2)) & 1U);
}

// @brief 入力に設定する．
// @param[in] input_id 入力番号
inline
void
PgNode::set_input(ymuint input_id)
{
  mType = (input_id << 2) | kInput;
}

// @brief ANDに設定する．
// @param[in] inv0, inv1 ファンインの反転属性
inline
void
PgNode::set_and(bool inv0,
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
inline
void
PgNode::set_xor()
{
  mType = kXor;
}

// @brief コンストラクタ
// @param[in] node ノード
// @param[in] inv 反転属性
inline
PgHandle::PgHandle(PgNode* node,
		   bool inv)
{
  set(node, inv);
}

// @brief デストラクタ
inline
PgHandle::~PgHandle()
{
}

// @brief ノードを取り出す．
inline
PgNode*
PgHandle::node() const
{
  return reinterpret_cast<PgNode*>(mData & ~1UL);
}

// @brief 反転属性を取り出す．
inline
bool
PgHandle::inv() const
{
  return static_cast<bool>(mData & 1UL);
}

// @brief 値を設定する．
// @param[in] node ノード
// @param[in] inv 反転属性
inline
void
PgHandle::set(PgNode* node,
	      bool inv)
{
  // bool に対する & 1UL は不必要だが念のため．
  mData = reinterpret_cast<ympuint>(node) | (static_cast<ympuint>(inv) & 1UL);
}

END_NAMESPACE_YM_TECHMAP_PATGEN

#endif // LIBYM_TECHMAP_PATGEN_PGNODE_H
