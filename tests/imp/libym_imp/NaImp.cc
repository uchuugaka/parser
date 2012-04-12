
/// @file NaImp.cc
/// @brief NaImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "NaImp.h"
#include "ImpMgr.h"
#include "ImpInfo.h"
#include "ImpNode.h"
#include "ImpValList.h"
#include "ym_logic/SatSolver.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_NETWORKS

BEGIN_NONAMESPACE

#define USE_IMPVALLIST

#if defined(YM_DEBUG)
bool debug = false;
#else
bool debug = false;
#endif

inline
Literal
to_literal(ymuint id,
	   ymuint val)
{
  return Literal(VarId(id), (val == 0) ? kPolNega : kPolPosi);
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス NaImp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NaImp::NaImp()
{
}

// @brief デストラクタ
NaImp::~NaImp()
{
}

// @brief ネットワーク中の間接含意を求める．
// @param[in] imp_mgr マネージャ
// @param[in] direct_imp 直接含意のリスト
// @param[in] imp_info 間接含意のリスト
void
NaImp::learning(ImpMgr& imp_mgr,
		const ImpInfo& direct_imp,
		ImpInfo& imp_info)
{
  cout << "NaImp start" << endl;

  //imp_mgr.print_network(cout);

  ymuint n = imp_mgr.node_num();

  imp_info.set_size(n);

  vector<ImpValList> imp_lists(n * 2);

  StopWatch timer;
  timer.start();

  // direct_imp の情報を imp_lists にコピーする．
  {
    vector<vector<ImpVal> > imp_lists_array(n * 2);
    for (ymuint src_id = 0; src_id < n; ++ src_id) {
      if ( imp_mgr.is_const(src_id) ) {
	continue;
      }
      ImpNode* node = imp_mgr.node(src_id);
      if ( node == NULL ) {
	continue;
      }
      // 自分自身を追加する．
      imp_lists_array[src_id * 2 + 0].push_back(ImpVal(src_id, 0));
      imp_lists_array[src_id * 2 + 1].push_back(ImpVal(src_id, 1));

      for (ymuint src_val = 0; src_val < 2; ++ src_val) {
	const vector<ImpVal>& imp_list = direct_imp.get(src_id, src_val);
	for (vector<ImpVal>::const_iterator p = imp_list.begin();
	     p != imp_list.end(); ++ p) {
	  ymuint dst_id = p->id();
	  if ( imp_mgr.is_const(dst_id) ) {
	    continue;
	  }
	  ymuint dst_val = p->val();
	  ymuint src_val1 = src_val ^ 1;
	  ymuint dst_val1 = dst_val ^ 1;
	  imp_lists_array[dst_id * 2 + dst_val].push_back(ImpVal(src_id, src_val));
	  imp_lists_array[src_id * 2 + src_val1].push_back(ImpVal(dst_id, dst_val1));
	}
      }
    }
    for (ymuint i = 0; i < n; ++ i) {
      for (ymuint val = 0; val < 2; ++ val) {
	vector<ImpVal>& imp_list = imp_lists_array[i * 2 + val];
	sort(imp_list.begin(), imp_list.end());
	ImpValList& dst_list = imp_lists[i * 2 + val];
	dst_list.insert(imp_list);
      }
    }
  }

  if ( debug ) {
    for (ymuint i = 0; i < n; ++ i) {
      for (ymint val = 0; val < 2; ++ val) {
	ImpValList& imp_list = imp_lists[i * 2 + val];
	cout << "Node#" << i << ":" << val << endl;
	imp_list.print(cout);
      }
    }
  }
#if 1
  timer.stop();
  USTime time = timer.time();
  cout << "phase0:" << time << endl;
#endif

  // 論理ノードの割り当て情報を作る．
  vector<ImpNode*> node_list;
  imp_mgr.get_node_list(node_list);

  for ( ; ; ) {
    ymuint delta = 0;
    for (vector<ImpNode*>::iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      ImpNode* node = *p;
      ymuint id = node->id();
      if ( imp_mgr.is_const(id) ) {
	continue;
      }
      ymuint idx_0 = id * 2 + 0;
      ymuint idx_1 = id * 2 + 1;

      const ImpEdge& e0 = node->fanin0();
      ImpNode* node0 = e0.src_node();
      ymuint id0 = node0->id();
      bool inv0 = e0.src_inv();
      ymuint idx0_0 = id0 * 2 + (inv0 ? 1 : 0);
      ymuint idx0_1 = idx0_0 ^ 1;

      const ImpEdge& e1 = node->fanin1();
      ImpNode* node1 = e1.src_node();
      ymuint id1 = node1->id();
      bool inv1 = e1.src_inv();
      ymuint idx1_0 = id1 * 2 + (inv1 ? 1: 0);
      ymuint idx1_1 = idx1_0 ^ 1;

      // 出力が0になる条件は入力が0になる条件のユニオン
      {
	const ImpValList& imp_list0 = imp_lists[idx0_0];
	const ImpValList& imp_list1 = imp_lists[idx1_0];
	ImpValList& imp_list = imp_lists[idx_0];
	if ( debug ) {
	  cout << "cup_merge" << endl;
	  cout << "Node#" << id << ": 0 ";
	  imp_list.print(cout);
	  cout << "  fanin0: ";
	  if ( inv0 ) {
	    cout << "~";
	  }
	  cout << "Node#" << id0 << " ";
	  imp_list0.print(cout);
	  cout << "  fanin1: ";
	  if ( inv1 ) {
	    cout << "~";
	  }
	  cout << "Node#" << id1 << " ";
	  imp_list1.print(cout);
	}
	ymuint nprev = imp_list.num();
	imp_list.merge(imp_list0);
	imp_list.merge(imp_list1);
	delta += imp_list.num() - nprev;
	if ( debug ) {
	  cout << "  result" << endl
	       << "   ";
	  imp_list.print(cout);
	  cout << endl;
	}
      }
      // 出力が1になる条件は入力が1になる条件のインターセクション
      {
	const ImpValList& imp_list0 = imp_lists[idx0_1];
	const ImpValList& imp_list1 = imp_lists[idx1_1];
	ImpValList& imp_list = imp_lists[idx_1];
	if ( debug ) {
	  cout << "cap_merge" << endl;
	  cout << "Node#" << id << ": 1 ";
	  imp_list.print(cout);
	  cout << "  fanin0: ";
	  if ( inv0 ) {
	    cout << "~";
	  }
	  cout << "Node#" << id0 << " ";
	  imp_list0.print(cout);
	  cout << "  fanin1: ";
	  if ( inv1 ) {
	    cout << "~";
	  }
	  cout << "Node#" << id1 << " ";
	  imp_list1.print(cout);
	}
	ymuint nprev = imp_list.num();
	imp_list.cap_merge(imp_list0, imp_list1);
	delta += imp_list.num() - nprev;
	if ( debug ) {
	  cout << "  result" << endl
	       << "   ";
	  imp_list.print(cout);
	  cout << endl;
	}
      }
    }
#if 1
    cout << "phase1: delta = " << delta << endl;
#endif

    for (vector<ImpNode*>::reverse_iterator p = node_list.rbegin();
	 p != node_list.rend(); ++ p) {
      ImpNode* node = *p;
      ymuint id = node->id();
      ymuint idx_0 = id * 2 + 0;
      ymuint idx_1 = id * 2 + 1;

      const ImpEdge& e0 = node->fanin0();
      ImpNode* node0 = e0.src_node();
      ymuint id0 = node0->id();
      bool inv0 = e0.src_inv();
      ymuint idx0_0 = id0 * 2 + (inv0 ? 1 : 0);
      ymuint idx0_1 = idx0_0 ^ 1;

      const ImpEdge& e1 = node->fanin1();
      ImpNode* node1 = e1.src_node();
      ymuint id1 = node1->id();
      bool inv1 = e1.src_inv();
      ymuint idx1_0 = id1 * 2 + (inv1 ? 1: 0);
      ymuint idx1_1 = idx1_0 ^ 1;

      if ( !imp_mgr.is_const(id1) ) {
	// 出力の0の条件とファンイン0の1の条件の共通部分がファンイン1の0の条件となる．
	const ImpValList& imp_list_o_0 = imp_lists[idx_0];
	const ImpValList& imp_list_i0_1 = imp_lists[idx0_1];
	ImpValList& imp_list_i1_0 = imp_lists[idx1_0];
	if ( debug ) {
	  cout << "cap_merge" << endl;
	  cout << "Node#" << id << ":0 ";
	  imp_list_o_0.print(cout);
	  cout << "fanin0: ";
	  if ( inv0 ) {
	    cout << "~";
	  }
	  cout << "Node#" << id0 << ":1";
	  imp_list_i0_1.print(cout);
	}
	ymuint nprev = imp_list_i1_0.num();
	imp_list_i1_0.cap_merge(imp_list_o_0, imp_list_i0_1);
	delta += imp_list_i1_0.num() - nprev;
	if ( debug ) {
	  cout << "  result" << endl
	       << "   ";
	  imp_list_i1_0.print(cout);
	  cout << endl
	       << endl;
	}
      }
      if ( !imp_mgr.is_const(id0) ) {
	// 出力の0の条件とファンイン1の1の条件の共通部分がファンイン0の0の条件となる．
	const ImpValList& imp_list_o_0 = imp_lists[idx_0];
	const ImpValList& imp_list_i1_1 = imp_lists[idx1_1];
	ImpValList& imp_list_i0_0 = imp_lists[idx0_0];
	if ( debug ) {
	  cout << "cap_merge" << endl;
	  cout << "Node#" << id << ":0";
	  imp_list_o_0.print(cout);
	  cout << "fanin1: ";
	  if ( inv1 ) {
	    cout << "~";
	  }
	  cout << "Node#" << id1 << ":1";
	  imp_list_i1_1.print(cout);
	}
	ymuint nprev = imp_list_i0_0.num();
	imp_list_i0_0.cap_merge(imp_list_o_0, imp_list_i1_1);
	delta += imp_list_i0_0.num() - nprev;
	if ( debug ) {
	  cout << "  result" << endl
	       << "   ";
	  imp_list_i0_0.print(cout);
	  cout << endl
	       << endl;
	}
      }

      if ( !imp_mgr.is_const(id0) ) {
	// 出力の1の条件がファンイン0の1の条件となる．
	const ImpValList& imp_list_o_1 = imp_lists[idx_1];
	ImpValList& imp_list_i0_1 = imp_lists[idx0_1];
	if ( debug ) {
	  cout << "Node#" << id << ":1";
	  imp_list_o_1.print(cout);
	  cout << "  fanin0: ";
	  if ( inv0 ) {
	    cout << "~";
	  }
	  cout << "Node#" << id0 << ":1";
	  imp_list_i0_1.print(cout);
	}
	ymuint nprev = imp_list_i0_1.num();
	imp_list_i0_1.merge(imp_list_o_1);
	delta += imp_list_i0_1.num() - nprev;
	if ( debug ) {
	  cout << "  result" << endl
	       << "   ";
	  imp_list_i0_1.print(cout);
	  cout << endl
	       << endl;
	}
      }

      if ( !imp_mgr.is_const(id1) ) {
	// 出力の1の条件がファンイン1の1の条件となる．
	const ImpValList& imp_list_o_1 = imp_lists[idx_1];
	ImpValList& imp_list_i1_1 = imp_lists[idx1_1];
	if ( debug ) {
	  cout << "Node#" << id << ":1";
	  imp_list_o_1.print(cout);
	  cout << "  fanin1: ";
	  if ( inv1 ) {
	    cout << "~";
	  }
	  cout << "Node#" << id1 << ":1";
	  imp_list_i1_1.print(cout);
	}
	ymuint nprev = imp_list_i1_1.num();
	imp_list_i1_1.merge(imp_list_o_1);
	delta += imp_list_i1_1.num() - nprev;
	if ( debug ) {
	  cout << "  result" << endl
	       << "   ";
	  imp_list_i1_1.print(cout);
	  cout << endl
	       << endl;
	}
      }
    }
#if 1
    cout << "phase2: delta = " << delta << endl;
#endif
    if ( delta == 0 ) {
      break;
    }
  }

  // imp_lists の情報から imp_info を作る．
  {
    vector<vector<ImpVal> > imp_list_array(n * 2);
    for (ymuint dst_id = 0; dst_id < n; ++ dst_id) {
      if ( imp_mgr.is_const(dst_id) ) {
	continue;
      }
      ImpNode* node = imp_mgr.node(dst_id);
      if ( node == NULL ) {
	continue;
      }
      for (ymuint dst_val = 0; dst_val < 2; ++ dst_val) {
	const ImpValList& imp_list = imp_lists[dst_id * 2 + dst_val];
	for (ImpValListIter p = imp_list.begin();
	     p != imp_list.end(); ++ p) {
	  const ImpVal& val = *p;
	  ymuint src_id = val.id();
	  ymuint src_val = val.val();
	  if ( src_id == dst_id ) {
	    continue;
	  }
	  imp_list_array[src_id * 2 + src_val].push_back(ImpVal(dst_id, dst_val));
	  imp_list_array[dst_id * 2 + (dst_val ^ 1)].push_back(ImpVal(src_id, src_val ^ 1));
	}
      }
    }
    // imp_list_array の内容を imp_info にコピーする．
    imp_info.set(imp_list_array);
  }

#if 0
  // 検証
  {
    SatSolver solver1;
    for (ymuint id = 0; id < n; ++ id) {
      VarId vid = solver1.new_var();
      assert_cond( vid.val() == id, __FILE__, __LINE__);
    }

    // ImpMgr から CNF を作る．
    for (ymuint id = 0; id < n; ++ id) {
      ImpNode* node = imp_mgr.node(id);
      if ( node == NULL ) continue;
      if ( node->is_input() ) continue;

      assert_cond( node->is_and(), __FILE__, __LINE__);

      Literal lit(VarId(id), kPolPosi);

      const ImpEdge& e0 = node->fanin0();
      ImpNode* node0 = e0.src_node();
      bool inv0 = e0.src_inv();
      Literal lit0(VarId(node0->id()), inv0 ? kPolNega : kPolPosi);

      const ImpEdge& e1 = node->fanin1();
      ImpNode* node1 = e1.src_node();
      bool inv1 = e1.src_inv();
      Literal lit1(VarId(node1->id()), inv1 ? kPolNega : kPolPosi);

      solver1.add_clause(lit0, ~lit);
      solver1.add_clause(lit1, ~lit);
      solver1.add_clause(~lit0, ~lit1, lit);
    }

    ymuint nerr = 0;
    for (ymuint src_id = 0; src_id < n; ++ src_id) {
      for (ymuint src_val = 0; src_val < 2; ++ src_val) {
	Literal lit0(to_literal(src_id, src_val));
	const vector<ImpVal>& imp_list = imp_info.get(src_id, src_val);
	for (vector<ImpVal>::const_iterator p = imp_list.begin(); p != imp_list.end(); ++ p) {
	  ymuint dst_id = p->id();
	  ymuint dst_val = p->val();
	  Literal lit1(to_literal(dst_id, dst_val));
	  vector<Literal> tmp(2);
	  tmp[0] = lit0;
	  tmp[1] = ~lit1;
	  vector<Bool3> model;
	  if ( solver1.solve(tmp, model) != kB3False ) {
	    cout << "ERROR: Node#" << src_id << ": " << src_val
		 << " ==> Node#" << dst_id << ": " << dst_val << endl;
	    ++ nerr;
	  }
	}
      }
    }
    cout << "Total " << nerr << " errors" << endl;
  }
#endif

}

END_NAMESPACE_YM_NETWORKS
