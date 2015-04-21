﻿#ifndef DTPGSATBASEM_H
#define DTPGSATBASEM_H

/// @file DtpgSatBaseM.h
/// @brief DtpgSatBaseM のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgSatBaseM DtpgSatBaseM.h "DtpgSatBaseM.h"
/// @brief 複数の故障の検出に使える CNF 式を生成するタイプの SatEngine
//////////////////////////////////////////////////////////////////////
class DtpgSatBaseM :
  public DtpgSat
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  DtpgSatBaseM(const string& sat_type,
	       const string& sat_option,
	       ostream* sat_outp,
	       BackTracer& bt,
	       DetectOp& dop,
	       UntestOp& uop);

  /// @brief デストラクタ
  virtual
  ~DtpgSatBaseM();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] network 対象のネットワーク
  /// @param[in] fmgr 故障マネージャ
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] fault_list 対象の故障リスト
  /// @param[out] stats 結果を格納する構造体
  virtual
  void
  run(TpgNetwork& network,
      FaultMgr& fmgr,
      Fsim& fsim,
      const vector<const TpgFault*>& fault_list,
      DtpgStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] node_set 対象のノード集合
  /// @param[in] fnode_list 対象の故障を持つノードのリスト
  /// @param[in] flist 対象の故障リスト
  virtual
  void
  run_multi(const NodeSet& node_set,
	    const vector<const TpgNode*>& fnode_list,
	    const vector<const TpgFault*>& flist) = 0;

  /// @brief DFS で MFFC を求める．
  /// @param[in] node 対象のノード
  /// @param[in] fmgr 故障マネージャ
  void
  dfs_mffc(const TpgNode* node,
	   FaultMgr& fmgr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障を持つノードのリスト
  vector<const TpgNode*> mFaultNodeList;

  // 対象の故障リスト
  vector<const TpgFault*> mFaultList;

  // 対象の故障を表すマーク配列
  vector<bool> mFaultMark;

  // 作業用のマーク
  vector<bool> mMark;

};

END_NAMESPACE_YM_SATPG

#endif // DTPGSATBASEM_H
