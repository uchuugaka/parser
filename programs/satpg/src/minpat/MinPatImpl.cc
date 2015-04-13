﻿
/// @file MinPatBase.cc
/// @brief MinPatBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatBase.h"
#include "MinPatStats.h"
#include "TvMgr.h"
#include "FaultMgr.h"
#include "TpgNetwork.h"
#include "FaultAnalyzer.h"
#include "Verifier.h"

#include "YmUtils/RandGen.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス MinPatBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MinPatBase::MinPatBase()
{
}

// @brief デストラクタ
MinPatBase::~MinPatBase()
{
}

// @brief テストベクタの最小化を行なう．
// @param[in] network 対象のネットワーク
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fmgr 故障マネージャ
// @param[in] fsim2 2値の故障シミュレータ(検証用)
// @param[out] tv_list テストベクタのリスト
// @param[out] stats 実行結果の情報を格納する変数
void
MinPatBase::run(TpgNetwork& network,
		TvMgr& tvmgr,
		FaultMgr& fmgr,
		Fsim& fsim2,
		vector<TestVector*>& tv_list,
		MinPatStats& stats)
{
  StopWatch total_timer;
  total_timer.start();

  ymuint max_node_id = network.max_node_id();

#if 0
  bool verbose = true;
  FaultAnalyzer analyzer(verbose);

  const vector<TpgFault*>& fault_list = fmgr.det_list();
  analyzer.init(max_node_id, fault_list);

  ymuint npat0 = 10000;
  RandGen rg;
  analyzer.get_pat_list(fsim2, tvmgr, tv_list, rg, npat0);

  bool dom_fast = false;
  analyzer.get_dom_faults(dom_fast);

  analyzer.analyze_faults();

  analyzer.analyze_conflict();

  vector<pair<ymuint, ymuint> > edge_list;
  analyzer.get_conf_list(edge_list);
#endif

  StopWatch local_timer;
  local_timer.start();
  cout << "coloring start" << endl;

  FgMgr fgmgr(max_node_id);

  { // 最初の故障を選ぶ
    TpgFault* fault = get_first_fault();

    // 最初のグループを作る．
    ymuint gid = fgmgr.new_group();

    // 故障を追加する．
    fgmgr.add_fault(gid, fault);
  }

  // 未処理の故障がある限り以下の処理を繰り返す．
  for ( ; ; ) {
    // 故障を選ぶ．
    TpgFault* fault = get_next_fault();
    if ( fault == NULL ) {
      break;
    }

    // 故障を追加できるグループを見つける．
    ymuint gid = find_group(fgmgr, fault);
    if ( gid == fgmgr.group_num() ) {
      // 見つからなかった．
      // 新たなグループを作る．
      gid = fgmgr.new_group();
    }

    // 故障を追加する．
    fgmgr.add_fault(gid, fault);
  }

  local_timer.stop();
  cout << " # of fault groups = " << fgmgr.group_num() << endl;
  cout << "CPU time (coloring)          " << local_timer.time() << endl;

  // テストパタンを作る．
  vector<TestVector*> new_tv_list;
  fgmgr.make_testvector(network, tvmgr, new_tv_list);

  ymuint orig_num = tv_list.size();

  tv_list = new_tv_list;

  {
    // 検証しておく．
    Verifier ver(fmgr, fsim2);
    if ( ver.check(tv_list) ) {
      cout << "No Errors" << endl;
    }
  }

  total_timer.stop();
  USTime time = total_timer.time();

  stats.set(orig_num, tv_list.size(), time);
}

END_NAMESPACE_YM_SATPG
