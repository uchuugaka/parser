#ifndef YM_NETWORKS_BDNLATCH_H
#define YM_NETWORKS_BDNLATCH_H

/// @file ym_networks/BdnLatch.h
/// @brief BdnLatch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

class BdnAuxData;

//////////////////////////////////////////////////////////////////////
/// @class BdnLatch BdnLatch.h "ym_networks/BdnLatch.h"
/// @brief ラッチを表すクラス
//////////////////////////////////////////////////////////////////////
class BdnLatch :
  public DlElem
{
  friend class BdnMgrImpl;

private:

  /// @brief コンストラクタ
  BdnLatch();

  /// @brief デストラクタ
  ~BdnLatch();


public:

  /// @brief ID番号の取得
  /// @return ID番号を返す．
  /// @note ID番号はラッチの生成時に BdnMgr により自動的に割り振られる．
  /// @sa BdnMgr
  ymuint
  id() const;

  /// @brief 名前を返す．
  string
  name() const;

  /// @brief データ出力ノードを返す．
  const BdnNode*
  output() const;

  /// @brief データ出力ノードを返す．
  BdnNode*
  _output();

  /// @brief データ入力ノードを返す．
  const BdnNode*
  input() const;

  /// @brief データ入力ノードを返す．
  BdnNode*
  _input();

  /// @brief ラッチイネーブルノードを返す．
  const BdnNode*
  enable() const;

  /// @brief ラッチイネーブルノードを返す．
  BdnNode*
  _enable();

  /// @brief クリア信号のノードを返す．
  /// @note NULL の場合もある．
  const BdnNode*
  clear() const;

  /// @brief クリア信号のノードを返す．
  /// @note NULL の場合もある．
  BdnNode*
  _clear();

  /// @brief プリセット信号のノードを返す．
  /// @note NULL の場合もある．
  const BdnNode*
  preset() const;

  /// @brief プリセット信号のノードを返す．
  /// @note NULL の場合もある．
  BdnNode*
  _preset();


private:
  //////////////////////////////////////////////////////////////////////
  // 内容を変更する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  /// @param[in] name 名前
  /// @param[in] output 出力ノード
  /// @param[in] input 入力ノード
  /// @param[in] enable ラッチイネーブルノード
  void
  set(const string& name,
      BdnNode* output,
      BdnNode* input,
      BdnNode* enable);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 名前
  string mName;

  // データ出力ノード
  BdnNode* mOutput;

  // データ入力ノード
  BdnNode* mInput;

  // ラッチイネーブルノード
  BdnNode* mEnable;

  // クリア信号ノード
  BdnNode* mClear;

  // プリセット信号ノード
  BdnNode* mPreset;

  // BdnNode の付加するデータ
  BdnAuxData* mAuxData;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号の取得
// @return ID番号を返す．
// @note ID番号はラッチの生成時に BdnMgr により自動的に割り振られる．
// @sa BdnMgr
inline
ymuint
BdnLatch::id() const
{
  return mId;
}

// @brief 名前を返す．
inline
string
BdnLatch::name() const
{
  return mName;
}

// @brief データ出力ノードを返す．
inline
const BdnNode*
BdnLatch::output() const
{
  return mOutput;
}

// @brief データ出力ノードを返す．
inline
BdnNode*
BdnLatch::_output()
{
  return mOutput;
}

// @brief データ入力ノードを返す．
inline
const BdnNode*
BdnLatch::input() const
{
  return mInput;
}

// @brief データ入力ノードを返す．
inline
BdnNode*
BdnLatch::_input()
{
  return mInput;
}

// @brief ラッチイネーブルノードを返す．
inline
const BdnNode*
BdnLatch::enable() const
{
  return mEnable;
}

// @brief ラッチイネーブルノードを返す．
inline
BdnNode*
BdnLatch::_enable()
{
  return mEnable;
}

// @brief クリア信号のノードを返す．
inline
const BdnNode*
BdnLatch::clear() const
{
  return mClear;
}

// @brief クリア信号のノードを返す．
inline
BdnNode*
BdnLatch::_clear()
{
  return mClear;
}

// @brief プリセット信号のノードを返す．
inline
const BdnNode*
BdnLatch::preset() const
{
  return mPreset;
}

// @brief プリセット信号のノードを返す．
inline
BdnNode*
BdnLatch::_preset()
{
  return mPreset;
}

END_NAMESPACE_YM_NETWORKS

#endif // YM_NETWORKS_BDNLATCH_H