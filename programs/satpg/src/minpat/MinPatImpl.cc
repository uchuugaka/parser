﻿
/// @file MinPatImpl.cc
/// @brief MinPatImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatImpl.h"
#include "MinPatStats.h"
#include "TvMgr.h"
#include "TestVector.h"
#include "FaultMgr.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "Fsim.h"
#include "KDet2.h"
#include "NodeSet.h"
#include "NodeValList.h"
#include "Verifier.h"
#include "GcSolver.h"
#include "GcSolver2.h"
#include "GcSolver3.h"
#include "GcNode.h"

#include "TpgCnf0.h"
#include "TpgCnf1.h"
#include "TpgCnf2.h"

#include "YmUtils/RandGen.h"
#include "YmUtils/StopWatch.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief インスタンスを生成する関数
MinPat*
new_MinPat()
{
  return new MinPatImpl();
}

BEGIN_NONAMESPACE

// パタン番号リストの包含関係を調べる．
// 0 bit: list1 のみの要素がある．
// 1 bit: list2 のみの要素がある．
// 2 bit: list1 と list2 に共通の要素がある．
//
// リストの内容は昇順にソートされていると仮定する．
ymuint
check_pat_list(const vector<ymuint>& tv_list1,
	       const vector<ymuint>& tv_list2)
{
  bool flag_10 = false;
  bool flag_01 = false;
  bool flag_11 = false;
  ymuint ans = 0U;
  ymuint n1 = tv_list1.size();
  ymuint n2 = tv_list2.size();
  ymuint p1 = 0;
  ymuint p2 = 0;
  while ( p1 < n1 && p2 < n2 ) {
    ymuint v1 = tv_list1[p1];
    ymuint v2 = tv_list2[p2];
    if ( v1 < v2 ) {
      ans |= 1U;
      ++ p1;
    }
    else if ( v1 > v2 ) {
      ans |= 2U;
      ++ p2;
    }
    else {
      ans |= 4U;
      ++ p1;
      ++ p2;
    }
    if ( ans == 7U ) {
      return ans;
    }
  }
  if ( p1 < n1 ) {
    ans |= 1U;
  }
  if ( p2 < n2 ) {
    ans |= 2U;
  }
  return ans;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス MinPatImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MinPatImpl::MinPatImpl()
{
}

// @brief デストラクタ
MinPatImpl::~MinPatImpl()
{
}

// @brief テストベクタの最小化を行なう．
// @param[in] network 対象のネットワーク
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fmgr 故障マネージャ
// @param[in] fsim2 2値の故障シミュレータ(検証用)
// @param[in] simple simple モードの時に true にするフラグ
// @param[out] tv_list テストベクタのリスト
// @param[out] stats 実行結果の情報を格納する変数
void
MinPatImpl::run(TpgNetwork& network,
		TvMgr& tvmgr,
		FaultMgr& fmgr,
		Fsim& fsim2,
		bool simple,
		vector<TestVector*>& tv_list,
		MinPatStats& stats)
{
  StopWatch total_timer;
  StopWatch local_timer;

  total_timer.start();
  local_timer.start();

  ymuint orig_num = tv_list.size();

  Verifier ver(fmgr, fsim2);

  // 故障番号の最大値を求める．
  mMaxFaultId = 0;
  const vector<TpgFault*>& f_list = fmgr.det_list();
  ymuint fault_num = f_list.size();
  for (ymuint i = 0; i < fault_num; ++ i) {
    TpgFault* f = f_list[i];
    if ( mMaxFaultId < f->id() ) {
      mMaxFaultId = f->id();
    }
  }
  ++ mMaxFaultId;

  mMaxNodeId = network.max_node_id();

  mInputListArray.clear();
  mInputListArray.resize(mMaxNodeId);

  mFaultInfoArray.clear();
  mFaultInfoArray.resize(mMaxFaultId);

  { // 各故障(に関連したノード)に関係する入力番号のリストを作る．
    // ここでいう関係とは TFI of TFO of fault location のこと
    // 計算には NodeSet を使う．
    NodeSet node_set;
    for (ymuint i = 0; i < fault_num; ++ i) {
      TpgFault* f = f_list[i];
      const TpgNode* node = f->node();
      vector<ymuint>& input_list = mInputListArray[node->id()];
      if ( !input_list.empty() ) {
	continue;
      }
      node_set.mark_region(mMaxNodeId, node);
      for (ymuint j = 0; j < node_set.tfo_tfi_size(); ++ j) {
	const TpgNode* node1 = node_set.tfo_tfi_node(j);
	if ( node1->is_input() ) {
	  input_list.push_back(node1->input_id());
	}
      }
      // ソートしておく．
      sort(input_list.begin(), input_list.end());
    }
  }

  // ドロップ無しの故障シミュレーター
  KDet2 kdet(fsim2, f_list);

  ymuint npat0 = 1000;
  ymuint npat = orig_num + npat0;

  cout << "  fault simulation (npat = " << npat << ") starts."
       << endl;
  vector<vector<ymuint> > pat_list_array;
  { // もともとのパタンに乱数パタンを npat0 個加える．
    // 結果は各故障ごとに検出パタン番号を pat_list_array に入れる．
    RandGen rg;
    vector<TestVector*> tmp_tv_list(npat);
    for (ymuint i = 0; i < orig_num; ++ i) {
      TestVector* tv = tv_list[i];
      tmp_tv_list[i] = tv;
    }
    for (ymuint i = 0; i < npat0; ++ i) {
      TestVector* tv = tvmgr.new_vector();
      tv->set_from_random(rg);
      tmp_tv_list[i + orig_num] = tv;
    }

    kdet.run(tmp_tv_list, pat_list_array);

    // 乱数パタンは削除しておく．
    for (ymuint i = 0; i < npat0; ++ i) {
      TestVector* tv = tmp_tv_list[i + orig_num];
      tvmgr.delete_vector(tv);
    }

    for (ymuint i = 0; i < fault_num; ++ i) {
      TpgFault* fault = f_list[i];
      mFaultInfoArray[fault->id()].mPatList = pat_list_array[fault->id()];
    }
  }

  cout << "  fault simulation ends." << endl;

  vector<TpgFault*> dom_fault_list;

  get_dom_faults(f_list, dom_fault_list);

  analyze_faults(dom_fault_list);

  GcSolver gcsolver(dom_fault_list, mMaxNodeId);

  vector<pair<ymuint, ymuint> > conf_list;
  analyze_conflict(dom_fault_list, conf_list);
  for (ymuint i = 0; i < conf_list.size(); ++ i) {
    gcsolver.connect(conf_list[i].first, conf_list[i].second);
  }

  local_timer.reset();
  local_timer.start();
  cout << "coloring start" << endl;
  ymuint nc = gcsolver.coloring();
  cout << " # of fault groups = " << nc << endl;
  local_timer.stop();
  cout << "CPU time (coloring)          " << local_timer.time() << endl;

  // テストパタンを作る．
  TpgCnf0 tpg_cnf0(string(), string(), NULL);
  vector<TestVector*> new_tv_list;
  new_tv_list.reserve(nc);
  for (ymuint col = 1; col <= nc; ++ col) {
    const NodeValList& suf_list = gcsolver.suf_list(col);
    TestVector* tv = tvmgr.new_vector();
    bool stat = tpg_cnf0.get_testvector(network, suf_list, tv);
    ASSERT_COND( stat );
    new_tv_list.push_back(tv);
    if ( false ) {
      cout << "#" << col << endl
	   << " faults: ";
      const vector<TpgFault*>& fault_list = gcsolver.fault_list(col);
      for (ymuint i = 0; i < fault_list.size(); ++ i) {
	TpgFault* f = fault_list[i];
	cout << " " << f->str();
      }
      cout << endl;
      cout << tv << endl;
    }
  }
  tv_list = new_tv_list;
  {
    // 検証しておく．
    if ( ver.check(tv_list) ) {
      cout << "No Errors" << endl;
    }
  }

  total_timer.stop();
  USTime time = total_timer.time();

  stats.set(orig_num, tv_list.size(), time);
}


// @brief 支配故障を求める．
void
MinPatImpl::get_dom_faults(const vector<TpgFault*>& fault_list,
			   vector<TpgFault*>& dom_fault_list)
{
  StopWatch local_timer;

  local_timer.start();

  HashSet<ymuint> input_hash;

  ymuint n_sat1 = 0;

  vector<bool> dom_flag(mMaxFaultId, false);

  ymuint fault_num = fault_list.size();
  // シミュレーション結果から故障の支配関係のヒントを作り，
  // SAT で正確に判定する．
  for (ymuint i1 = 0; i1 < fault_num; ++ i1) {
    TpgFault* f1 = fault_list[i1];

    // input_list1 の要素をハッシュに登録する．
    const vector<ymuint>& input_list1 = mInputListArray[f1->node()->id()];
    input_hash.clear();
    for (ymuint i = 0; i < input_list1.size(); ++ i) {
      input_hash.add(input_list1[i]);
    }

    const vector<ymuint>& tv_list1 = mFaultInfoArray[f1->id()].mPatList;
    for (ymuint i2 = i1 + 1; i2 < fault_num; ++ i2) {
      TpgFault* f2 = fault_list[i2];
      if ( dom_flag[f2->id()] ) {
	continue;
      }

      // input_list2 の要素でハッシュに登録されている要素があれば
      // input_list1 と input_list2 は共通部分を持つ．
      const vector<ymuint>& input_list2 = mInputListArray[f2->node()->id()];
      bool intersect = false;
      for (ymuint i = 0; i < input_list2.size(); ++ i) {
	if ( input_hash.check(input_list2[i]) ) {
	  intersect = true;
	  break;
	}
      }
      if ( !intersect ) {
	// 共通部分を持たない故障は独立
	continue;
      }

      const vector<ymuint>& tv_list2 = mFaultInfoArray[f2->id()].mPatList;

      ymuint stat = check_pat_list(tv_list1, tv_list2);

      if ( (stat & 1U) == 0U ) {
	// f1 が 0 のときは f2 も 0 だった．
	++ n_sat1;
	TpgCnf2 tpg_cnf(string(), string(), NULL);
	if ( tpg_cnf.check_dominance(f1, f2, mMaxNodeId) ) {
	  //cout << f2->str() << " is dominated by " << f1->str() << endl;
	  dom_flag[f2->id()] = true;
	}
      }
      if ( !dom_flag[f2->id()] && (stat & 2U) == 0U ) {
	// f2 が 0 のときは f1 も 0 だった．
	++ n_sat1;
	TpgCnf2 tpg_cnf(string(), string(), NULL);
	if ( tpg_cnf.check_dominance(f2, f1, mMaxNodeId) ) {
	  //cout << f1->str() << " is dominated by " << f2->str() << endl;
	  dom_flag[f1->id()] = true;
	  break;
	}
      }
    }
  }

  // 支配されていない故障を dom_fault_list に入れる．
  dom_fault_list.clear();
  dom_fault_list.reserve(fault_num);
  for (ymuint i = 0; i < fault_num; ++ i) {
    TpgFault* f = fault_list[i];
    if ( !dom_flag[f->id()] ) {
      dom_fault_list.push_back(f);
    }
  }

  ymuint dom_fault_num = dom_fault_list.size();

  local_timer.stop();

  cout << "Total    " << fault_num << " original faults" << endl;
  cout << "Total    " << dom_fault_num << " dominator faults" << endl;
  cout << "Total    " << n_sat1 << " dominance test" << endl;
  cout << "CPU time for dominance test" << local_timer.time() << endl;
}

void
MinPatImpl::analyze_faults(const vector<TpgFault*> fault_list)
{
  StopWatch local_timer;

  local_timer.start();

  ymuint fnum = fault_list.size();
  for (ymuint i = 0; i < fnum; ++ i) {
    TpgFault* fault = fault_list[i];
    FaultInfo& fault_info = mFaultInfoArray[fault->id()];

    TpgCnf1 tpg_cnf(string(), string(), NULL);

    tpg_cnf.make_fval_cnf(fault, mMaxNodeId);

    NodeValList& suf_list = fault_info.mSufList;
    NodeValList& pi_suf_list = fault_info.mPiSufList;
    NodeValList& ma_list = fault_info.mMaList;;

    bool stat = tpg_cnf.get_suf_list2(suf_list, pi_suf_list);
    ASSERT_COND( stat );

    // 必要割当を求める．
    ymuint n = suf_list.size();
    for (ymuint i = 0; i < n; ++ i) {
      NodeVal nv = suf_list[i];

      NodeValList list1;
      const TpgNode* node = nv.node();
      bool val = nv.val();
      list1.add(node, !val);
      if ( tpg_cnf.check_conflict(list1) ) {
	ma_list.add(node, val);
      }
    }
  }

  local_timer.stop();

  cout << "CPU time for fault analysis" << local_timer.time() << endl;
}

/// @brief 故障間の衝突性を調べる．
void
MinPatImpl::analyze_conflict(const vector<TpgFault*>& fault_list,
			     vector<pair<ymuint, ymuint> >& conf_list)
{
  StopWatch local_timer;

  local_timer.start();

  StopWatch conf1_timer;
  StopWatch conf2_timer;
  StopWatch conf3_timer;
  StopWatch conf4_timer;
  StopWatch int1_timer;
  StopWatch int2_timer;

  ymuint n_sat2 = 0;
  ymuint n_conf = 0;
  ymuint n_conf1 = 0;
  ymuint n_conf2 = 0;
  ymuint n_conf3 = 0;
  ymuint n_conf4 = 0;
  ymuint n_int1 = 0;
  ymuint n_int2 = 0;

  // シミュレーション結果を用いてコンフリクトチェックのスクリーニングを行う．
  HashSet<ymuint> input_hash;
  ymuint fault_num = fault_list.size();
  for (ymuint i1 = 0; i1 < fault_num; ++ i1) {
    TpgFault* f1 = fault_list[i1];

    TpgCnf1 tpg_cnf(string(), string(), NULL);

    tpg_cnf.make_fval_cnf(f1, mMaxNodeId);

    // input_list1 の要素をハッシュに登録する．
    const vector<ymuint>& input_list1 = mInputListArray[f1->node()->id()];
    input_hash.clear();
    for (ymuint i = 0; i < input_list1.size(); ++ i) {
      input_hash.add(input_list1[i]);
    }

    const FaultInfo& fi1 = mFaultInfoArray[f1->id()];
    const vector<ymuint>& tv_list1 = fi1.mPatList;
    const NodeValList& suf_list1 = fi1.mSufList;
    const NodeValList& pi_suf_list1 = fi1.mPiSufList;
    const NodeValList& ma_list1 = fi1.mMaList;

    for (ymuint i2 = i1 + 1; i2 < fault_num; ++ i2) {
      TpgFault* f2 = fault_list[i2];

      // input_list2 の要素でハッシュに登録されている要素があれば
      // input_list1 と input_list2 は共通部分を持つ．
      const vector<ymuint>& input_list2 = mInputListArray[f2->node()->id()];
      bool int_stat = false;
      for (ymuint i = 0; i < input_list2.size(); ++ i) {
	if ( input_hash.check(input_list2[i]) ) {
	  int_stat = true;
	  break;
	}
      }
      if ( !int_stat ) {
	// 共通部分を持たない故障は独立
	continue;
      }

      const FaultInfo& fi2 = mFaultInfoArray[f2->id()];
      const vector<ymuint>& tv_list2 = fi2.mPatList;

      ymuint stat = check_pat_list(tv_list1, tv_list2);
      if ( (stat & 4U) != 0U ) {
	// f1 と f2 が同時に1なのでコンフリクトしない．
	continue;
      }

      const NodeValList& suf_list2 = fi2.mSufList;
      const NodeValList& pi_suf_list2 = fi2.mPiSufList;
      const NodeValList& ma_list2 = fi2.mMaList;

      conf1_timer.start();
      if ( check_conflict(ma_list1, ma_list2) ) {
	// 必要割当そのものがコンフリクトしている．
	++ n_conf;
	++ n_conf1;
	conf_list.push_back(make_pair(i1, i2));
	conf1_timer.stop();
	continue;
      }
      conf1_timer.stop();

      int1_timer.start();
      if ( !check_conflict(pi_suf_list1, pi_suf_list2) ) {
	// 十分割当が両立しているのでコンフリクトしない．
	++ n_int1;
	int1_timer.stop();
	continue;
      }
      int1_timer.stop();

      int2_timer.start();
      if ( !check_conflict(suf_list1, suf_list2) && tpg_cnf.check_intersect(suf_list2) ) {
	// f2 の十分割当のもとで f1 が検出できれば f1 と f2 はコンフリクトしない．
	++ n_int2;
	int2_timer.stop();
	continue;
      }
      int2_timer.stop();

      conf3_timer.start();
      if ( tpg_cnf.check_conflict(ma_list2) ) {
	// f2 の必要割当のもとで f1 が検出できなければ f1 と f2 はコンフリクトしている．
	++ n_conf;
	++ n_conf3;
	conf_list.push_back(make_pair(i1, i2));
	conf3_timer.stop();
	continue;
      }
      conf3_timer.stop();

      conf4_timer.start();
      // f1 と f2 が同時に 1 になることがない．
      ++ n_sat2;
      TpgCnf2 tpg_cnf2(string(), string(), NULL);
      if ( tpg_cnf2.check_conflict(f1, f2, mMaxNodeId) ) {
	++ n_conf;
	++ n_conf4;
	conf_list.push_back(make_pair(i1, i2));
      }
      conf4_timer.stop();
    }
  }

  local_timer.stop();

  cout << "Total    " << setw(6) << n_conf  << " conflicts" << endl;
  cout << "Total    " << setw(6) << n_conf1 << " conflicts (ma_list)" << endl;
  cout << "Total    " << setw(6) << n_conf3 << " conflicts (single suf_list)" << endl;
  cout << "Total    " << setw(6) << n_conf4 << " conflicts (exact)" << endl;
  cout << "Total    " << setw(6) << n_sat2  << " exact test" << endl;
  cout << "Total    " << setw(6) << n_int1  << " pi_suf_list intersection check" << endl;
  cout << "Total    " << setw(6) << n_int2  << " suf_list intersection check" << endl;
  cout << "Total CPU time " << local_timer.time() << endl;
  cout << "CPU time (simple ma_list)    " << conf1_timer.time() << endl;
  cout << "CPU time (single conflict)   " << conf3_timer.time() << endl;
  cout << "CPU time (exact conflict)    " << conf4_timer.time() << endl;
  cout << "CPU time (siple pi_suf_list) " << int1_timer.time() << endl;
  cout << "CPU time (single suf_list)   " << int2_timer.time() << endl;
}

END_NAMESPACE_YM_SATPG
