#ifndef FSIM_FSIM_H
#define FSIM_FSIM_H

/// @file src/fsim/Fsim.h
/// @brief Fsim のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Fsim.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "fsim_nsdef.h"
#include "ModBase.h"
#include "PackedVal.h"
#include "ym_networks/tgnet.h"
#include "ym_logic/LogExpr.h"
#include "EventQ.h"
#include "FsimFault.h"


BEGIN_NAMESPACE_YM_ATPG

class SaFault;
class TestVector;

END_NAMESPACE_YM_ATPG


BEGIN_NAMESPACE_YM_ATPG_FSIM

class SimFFR;
class SimNode;

//////////////////////////////////////////////////////////////////////
/// @class Fsim Fsim.h "Fsim.h"
/// @brief 故障シミュレーションを行うモジュール
/// @sa ModBase
//////////////////////////////////////////////////////////////////////
class Fsim :
  public ModBase
{
public:

  /// @brief コンストラクタ
  Fsim();

  /// @brief デストラクタ
  virtual
  ~Fsim();


public:

  /// @brief 対象の故障をセットする
  /// @param[in] flist 対象の故障リスト
  void
  set_faults(const vector<SaFault*>& flist);

  /// @brief 故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[out] det_faults 検出された故障を格納するリスト
  void
  run(TestVector* tv,
      list<SaFault*>& det_faults);

  /// @brief 故障シミュレーションを行う．
  /// @param[in] tv_array テストベクタの配列
  /// @param[out] det_faults 検出された故障を格納するリストの配列
  void
  run(const vector<TestVector*>& tv_array,
      vector<list<SaFault*> >& det_faults);

  /// @brief 一つのパタンで一つの故障に対するシミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] f 対象の故障
  bool
  run(TestVector* tv,
      SaFault* f);


private:
  //////////////////////////////////////////////////////////////////////
  // ModBase の継承クラスが用意する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークをセットした後に呼ばれる関数
  virtual
  void
  after_set_network(const TgNetwork& network,
		    TvMgr& tvmgr);

  /// @brief 故障リストの内容が変わった後に呼ばれる関数
  virtual
  void
  after_update_faults(const vector<SaFault*>& flist);


private:

  /// @brief FFR 内の故障シミュレーションを行う．
  PackedVal
  ffr_simulate(SimFFR* ffr);

  /// @brief イベントキューを用いてシミュレーションを行う．
  PackedVal
  eventq_simulate();

  /// @brief ffr 内の故障が検出可能か調べる．
  void
  fault_sweep(SimFFR* ffr,
	      list<SaFault*>& det_faults);


private:
  //////////////////////////////////////////////////////////////////////
  // TgNode と SimNode の対応関係を表すためのデータ構造
  //////////////////////////////////////////////////////////////////////

  struct EdgeMap {
    SimNode* mNode;
    ymuint32 mPos;
  };


private:

  /// @brief 現在保持している SimNode のネットワークを破棄する．
  /// 内部で clear_faults() を呼ぶ．
  void
  clear();

  /// @brief FsimFault を破棄する．
  void
  clear_faults();

  /// @brief node に対応する SimNode を得る．
  SimNode*
  find_simnode(const TgNode* node) const;

  /// @brief node の pos 番めの入力に対応する枝を得る．
  void
  find_simedge(const TgNode* node,
	       ymuint pos,
	       SimNode*& simnode,
	       ymuint& ipos) const;

  /// @brief 外部入力ノードを作る．
  SimNode*
  make_input();

  /// @brief logic ノードを作る．
  SimNode*
  make_logic(const LogExpr& lexp,
	     const vector<SimNode*>& inputs,
	     const vector<EdgeMap*>& emap);

  /// @brief logic ノードを作る．
  SimNode*
  make_node(tTgGateType type,
	    const vector<SimNode*>& inputs);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  const TgNetwork* mNetwork;

  // TgNode の gid をキーにして SimNode を入れる配列
  vector<SimNode*> mSimMap;

  // TgNode の gid と入力番号から枝の情報を取り出す配列
  vector<vector<EdgeMap> > mEdgeMap;

  // 全ての SimNode を納めた配列
  vector<SimNode*> mNodeArray;

  // 外部入力に対応する SimNode を納めた配列
  vector<SimNode*> mInputArray;

  // 外部出力に対応する SimNode を納めた配列
  vector<SimNode*> mOutputArray;

  // 入力からのトポロジカル順に並べた logic ノードの配列
  vector<SimNode*> mLogicArray;

  // FFR を納めた配列
  vector<SimFFR> mFFRArray;

  // イベントキュー
  EventQ mEventQ;

  // 値を消去する必要のあるノードを入れておく配列
  vector<SimNode*> mClearArray;

  // 故障シミュレーション用の故障の配列
  vector<FsimFault> mFsimFaults;

};

END_NAMESPACE_YM_ATPG_FSIM

#endif // FSIM_FSIM_H
