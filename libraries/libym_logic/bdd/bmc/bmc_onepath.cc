
/// @file libym_logic/bdd/bmc/bmc_onepath.cc
/// @brief パスを求める関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bmc_onepath.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrClassic.h"


BEGIN_NAMESPACE_YM_BDD

// '1' の終端へ至る経路をひとつとってくる．
// 結果はその経路を表す論理積の形のBDDで返される．
BddEdge
BddMgrClassic::onepath(BddEdge e)
{
  if ( e.is_error() ) {
    return BddEdge::make_error();
  }
  if ( e.is_overflow() ) {
    return BddEdge::make_overflow();
  }
  if ( e.is_one() ) {
    return BddEdge::make_one();
  }

  Node* vp = get_node(e);
  tPol pol = e.pol();
  Var* var = vp->var();
  BddEdge l = vp->edge0(pol);
  BddEdge h = vp->edge1(pol);
  BddEdge result;
  if ( h.is_zero() ) {
    BddEdge chd = onepath(l);
    result = new_node(var, chd, BddEdge::make_zero());
  }
  else {
    BddEdge chd = onepath(h);
    result = new_node(var, BddEdge::make_zero(), chd);
  }
  return result;
}

// 最短の1パスを求める．
// 結果はその経路を表す論理積の形のBDDで返される．
BddEdge
BddMgrClassic::shortest_onepath(BddEdge e)
{
  if ( e.is_error() ) {
    return BddEdge::make_error();
  }
  if ( e.is_overflow() ) {
    return BddEdge::make_overflow();
  }

  hash_map<BddEdge, BddEdge> sp_assoc;
  return sp_step(e, sp_assoc);
}

// shortest_onepath 用の下請関数
BddEdge
BddMgrClassic::sp_step(BddEdge e,
		       hash_map<BddEdge, BddEdge>& sp_assoc)
{
  if ( e.is_one() ) {
    return BddEdge::make_one();
  }
  if ( e.is_zero() ) {
    return BddEdge::make_zero();
  }

  BddEdge result;
  hash_map<BddEdge, BddEdge>::iterator p = sp_assoc.find(e);
  if ( p == sp_assoc.end() ) {
    Node* vp = get_node(e);
    tPol pol = e.pol();
    BddEdge l = sp_step(vp->edge0(pol), sp_assoc);
    BddEdge h = sp_step(vp->edge1(pol), sp_assoc);
    int llen = sp_len(l);
    int hlen = sp_len(h);
    if ( hlen != -1 && llen > hlen + 1 ) {
      l = BddEdge::make_zero();
    }
    else if ( llen != -1 && llen < hlen + 1 ) {
      h = BddEdge::make_zero();
    }
    result = new_node(vp->var(), l, h);
    sp_assoc[e] = result;
  }
  else {
    result = p->second;
  }
  return result;
}

// sp_step 中で用いられる関数
ymint
BddMgrClassic::sp_len(BddEdge e)
{
  if ( e.is_zero() ) {
    return -1;
  }
  ymint len = 0;
  while ( !e.is_one() ) {
    Node* vp = get_node(e);
    tPol pol = e.pol();
    BddEdge l = vp->edge0(pol);
    if ( l.is_zero() ) {
      ++ len;
      e = vp->edge1(pol);
    }
    else {
      e = l;
    }
  }
  return len;
}

// 最短1パスの長さを求める．
tVarSize
BddMgrClassic::shortest_onepath_len(BddEdge e)
{
  if ( e.is_invalid() ) {
    return 0;
  }
  hash_map<BddEdge, ymint> assoc;
  ymint tmp = spl_step(e, assoc);
  assert_cond(tmp >= 0, __FILE__, __LINE__);
  return static_cast<tVarSize>(tmp);
}

// shortest_onepath_len() 中で用いられる下請関数
ymint
BddMgrClassic::spl_step(BddEdge e,
			hash_map<BddEdge, ymint>& assoc)
{
  if ( e.is_one() ) {
    return 0;
  }
  if ( e.is_zero() ) {
    return -1;
  }

  ymint result;
  hash_map<BddEdge, ymint>::iterator p = assoc.find(e);
  if ( p == assoc.end() ) {
    Node* vp = get_node(e);
    tPol pol = e.pol();
    ymint ans1 = spl_step(vp->edge0(pol), assoc);
    ymint ans2 = spl_step(vp->edge1(pol), assoc) + 1;
    if ( ans1 != -1 && ans1 < ans2 ) {
      result = ans1;
    }
    else {
      result = ans2;
    }
    assoc[e] = result;
  }
  else {
    result = p->second;
  }
  return result;
}

END_NAMESPACE_YM_BDD