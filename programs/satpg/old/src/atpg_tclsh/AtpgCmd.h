#ifndef ATPGCMD_H
#define ATPGCMD_H

/// @file include/AtpgCmd.h
/// @brief AtpgCmd のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "ym_YmTclpp/TclCmd.h"
#include "ym_YmNetworks/tgnet.h"


BEGIN_NAMESPACE_YM_SATPG

// 前方参照のためのクラス宣言
class FaultMgr;
class TvMgr;
class AtpgMgr;

//////////////////////////////////////////////////////////////////////
// ATPG のコマンドオブジェクトの基底クラス
// おもな仕事はエラーが起きた時にメッセージをセット
// するような関数の殻をかぶせること．
// あとは変換などの便利関数を入れてある．
//////////////////////////////////////////////////////////////////////
class AtpgCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  AtpgCmd(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~AtpgCmd();


protected:

  /// @brief ネットワークをセットした後に呼ぶ関数
  void
  after_set_network();

  /// @brief 故障リストを更新した後に呼ぶ関数
  void
  after_update_faults();

  /// @brief AtpgMgr を返す．
  AtpgMgr&
  mgr();


protected:

  /// @brief TgNetwork を取り出す．
  const TgNetwork&
  _network();

  /// @brief FaultMgr を取り出す．
  FaultMgr&
  _fault_mgr();

  /// @brief TvMgr を取り出す．
  TvMgr&
  _tv_mgr();


private:
  //////////////////////////////////////////////////////////////////////
  // メンバ変数
  //////////////////////////////////////////////////////////////////////

  // 共通のデータを管理するオブジェクト
  AtpgMgr* mMgr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief AtpgMgr を返す．
inline
AtpgMgr&
AtpgCmd::mgr()
{
  return *mMgr;
}

END_NAMESPACE_YM_SATPG

#endif // ATPGCMD_H
