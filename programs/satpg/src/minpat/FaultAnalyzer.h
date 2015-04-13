﻿#ifndef FAULTANALYZER_H
#define FAULTANALYZER_H

/// @file FaultAnalyzer.h
/// @brief FaultAnalyzer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "NodeValList.h"
#include "FaultInfo.h"
#include "NodeSet.h"
#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FaultAnalyzer FaultAnalyzer.h "FaultAnalyzer.h"
/// @brief 故障間の関係を解析するクラス
//////////////////////////////////////////////////////////////////////
class FaultAnalyzer
{
public:

  /// @brief コンストラクタ
  /// @param[in] verbose 表示を制御するフラグ
  FaultAnalyzer(bool verbose);

  /// @brief デストラクタ
  virtual
  ~FaultAnalyzer();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @param[in] max_node_id ノード番号の最大値 + 1
  /// @param[in] fault_list 故障リスト
  void
  init(ymuint max_node_id,
       const vector<TpgFault*>& fault_list);

  /// @brief 故障シミュレーションを行い，故障検出パタンを記録する．
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] tvmgr テストベクタのマネージャ
  /// @param[in] tv_list テストベクタのリスト
  /// @param[in] rg 乱数生成器
  /// @param[in] npat 内部で生成する乱数パタンの数
  ///
  /// 結果は mFaultInfoArray の pat_list に格納される．
  void
  get_pat_list(Fsim& fsim,
	       TvMgr& tvmgr,
	       const vector<TestVector*>& tv_list,
	       RandGen& rg,
	       ymuint npat);

  /// @brief 支配故障を求める．
  ///
  /// 結果は mDomFaultList に格納される．
  void
  get_dom_faults();

  /// @brief 十分割当と必要割当を求める．
  ///
  /// 結果は mFaultInfoArray に格納される．
  void
  analyze_faults();

  /// @brief 故障間の衝突性を調べる．
  void
  analyze_conflict();

  /// @brief 衝突リストを得る．
  void
  get_conf_list(vector<pair<ymuint, ymuint> >& conf_list);

  /// @brief 支配故障リストを得る．
  const vector<TpgFault*>&
  dom_fault_list() const;

  /// @brief 故障の情報を得る．
  const vector<FaultInfo>&
  fault_info_array() const;

  /// @brief 入力番号リストを得る．
  const vector<vector<ymuint> >&
  input_list_array() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 表示を制御するフラグ
  bool mVerbose;

  // 最大ノード番号
  ymuint mMaxNodeId;

  // 最大故障番号
  ymuint mMaxFaultId;

  // オリジナルの故障リスト
  vector<TpgFault*> mOrigFaultList;

  // 支配故障リスト
  vector<TpgFault*> mDomFaultList;

  // ノードごとに関係する入力の番号のリストを収める配列
  vector<vector<ymuint> > mInputListArray;

  // ノードごとに関係する入力の番号のリストを収める配列
  vector<vector<ymuint> > mInputList2Array;

  // ノードごとに NodeSet を収める配列
  vector<NodeSet> mNodeSetArray;

  // 故障ごとの情報を収める配列
  vector<FaultInfo> mFaultInfoArray;

};

END_NAMESPACE_YM_SATPG

#endif // FAULTANALYZER_H
