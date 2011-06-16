
/// @file libym_logic/bdd/bmc/bmc_ite.cc
/// @brief ITE 演算を行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bmc_ite.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrClassic.h"
#include "BmcCompTbl.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

tBddEdge y_edge;
tBddEdge xy_edge;

END_NONAMESPACE


// f が真の時 g を，偽の時 h を選ぶ関数
tBddEdge
BddMgrClassic::ite_op(tBddEdge f,
		      tBddEdge g,
		      tBddEdge h)
{
  if ( check_error(f) || check_error(g) || check_error(h) ) {
    return kEdgeError;
  }
  if ( check_overflow(f) || check_overflow(g) || check_overflow(h) ) {
    return kEdgeOverflow;
  }
  if ( check_one(f) ) {
    return g;
  }
  if ( check_zero(f) ) {
    return h;
  }
  if ( g == h ) {
    return g;
  }
  if ( check_reverse(g, h) ) {
    return xor_op(f, h);
  }
  if ( check_one(g) || f == g ) {
    // f | h
    return or_op(f, h);
  }
  if ( check_zero(g) || check_reverse(f, g) ) {
    // ~f & h
    return and_op(negate(f), h);
  }
  if ( check_one(h) || check_reverse(f, h) ) {
    // ~f | g
    return or_op(negate(f), g);
  }
  if ( check_zero(h) || f == h ) {
    // f & g
    return and_op(f, g);
  }
  // この時点で f, g, h は終端ではない．

  // 演算結果テーブルが当たりやすくなるように順序を正規化する．
  if ( g > h ) {
    tBddEdge tmp = g;
    g = h;
    h = tmp;
    // then と else を入れ替えたんだから条件も反転しなければならない．
    f = negate(f);
  }

  // さらに g に否定属性を付けないように正規化する．
  tPol ans_pol = get_pol(g);
  g = addpol(g, ans_pol);
  h = addpol(h, ans_pol);

  Node* f_vp = get_node(f);
  Node* g_vp = get_node(g);
  Node* h_vp = get_node(h);
  Var* f_var = f_vp->var();
  Var* g_var = g_vp->var();
  Var* h_var = h_vp->var();
  tLevel f_level = f_var->level();
  tLevel g_level = g_var->level();
  tLevel h_level = h_var->level();

  tBddEdge result;

  tPol f_pol = get_pol(f);
  if ( check_zero(f_vp->edge0(f_pol)) &&
       check_one(f_vp->edge1(f_pol)) &&
       f_level < g_level && f_level < h_level ) {
    result = new_node(f_var, h, g);
  }
  else if ( check_one(f_vp->edge0(f_pol)) &&
	      check_zero(f_vp->edge1(f_pol)) &&
	      f_level < g_level && f_level < h_level ) {
    result = new_node(f_var, g, h);
  }
  else {
    result = mIteTable->get(f, g, h);
    if ( result == kEdgeInvalid ) {
      tPol g_pol = get_pol(g);
      tPol h_pol = get_pol(h);
      tLevel top = f_level;
      Var* var = f_var;
      if ( top > g_level) {
	top = g_level;
	var = g_var;
      }
      if ( top > h_level ) {
	top = h_level;
	var = h_var;
      }
      tBddEdge f_0, f_1;
      tBddEdge g_0, g_1;
      tBddEdge h_0, h_1;
      split1(top, f_level, f, f_vp, f_pol, f_0, f_1);
      split1(top, g_level, g, g_vp, g_pol, g_0, g_1);
      split1(top, h_level, h, h_vp, h_pol, h_0, h_1);
      tBddEdge r_0 = ite_op(f_0, g_0, h_0);
      tBddEdge r_1 = ite_op(f_1, g_1, h_1);
      result = new_node(var, r_0, r_1);
      mIteTable->put(f, g, h, result);
    }
  }
  return addpol(result, ans_pol);
}

// multiple compose 演算を行うために最初に呼ばれる関数．
void
BddMgrClassic::compose_start()
{
  // 念のため
  clear_varmark();
  mLastLevel = 0;
}

// multiple compose 演算を行う変数と置き換え関数を登録する関数
void
BddMgrClassic::compose_reg(tVarId id,
			   tBddEdge e)
{
  Var* var = var_of(id);
  if ( var ) {
    var->mMark = 1;
    var->mCompEdge = e;
    tLevel level = var->level();
    if ( mLastLevel < level ) {
      mLastLevel = level;
    }
  }
}

// multiple compose 演算の本体
tBddEdge
BddMgrClassic::compose(tBddEdge e)
{
  tBddEdge ans = compose_step(e);
  clear_varmark();
  mCmpTable->clear();
  return ans;
}

tBddEdge
BddMgrClassic::compose_step(tBddEdge f)
{
  if ( check_error(f) ) {
    return kEdgeError;
  }
  if ( check_overflow(f) ) {
    return kEdgeOverflow;
  }
  if ( check_leaf(f) ) {
    return f;
  }

  Node* f_vp = get_node(f);
  Var* f_var = f_vp->var();
  if ( f_var->level() > mLastLevel ) {
    return f;
  }

  // 極性の反転に関して線形性を有するので極性は取り除く．
  tPol f_pol = get_pol(f);
  f = addpol(f, f_pol);

  tBddEdge result = mCmpTable->get(f);
  if ( result == kEdgeInvalid ) {
    tBddEdge f_0 = f_vp->edge0();
    tBddEdge f_1 = f_vp->edge1();
    tBddEdge r_0 = compose_step(f_0);
    tBddEdge r_1 = compose_step(f_1);
    tBddEdge tmp;
    if ( f_var->mMark == 1 ) {
      tmp = f_var->mCompEdge;
    }
    else {
      tmp = new_node(f_var, kEdge0, kEdge1);
    }
    result = ite_op(tmp, r_1, r_0);
    mCmpTable->put(f, result);
  }

  return addpol(result, f_pol);
}

// x_level の変数を y_level まで「押し込む」．
// pol が kPolNega の時は 0-枝と 1-枝を取り替える．
tBddEdge
BddMgrClassic::push_down(tBddEdge e,
			 tLevel x_level,
			 tLevel y_level,
			 tPol pol)
{
  if ( check_error(e) ) {
    return kEdgeError;
  }
  if ( check_overflow(e) ) {
    return kEdgeOverflow;
  }

  y_edge = BddMgr::make_bdd(varid(y_level), kEdge0, kEdge1);
  xy_edge = BddMgr::make_bdd(varid(x_level), kEdge0, addpol(y_edge, pol));
  activate(xy_edge);
  tBddEdge ans = pd_step(e, x_level, y_level, pol);
  deactivate(xy_edge);
  return ans;
}

tBddEdge
BddMgrClassic::pd_step(tBddEdge e,
		       tLevel x_level,
		       tLevel y_level,
		       tPol pol)
{
  if ( check_leaf(e) ) {
    return e;
  }

  Node* vp = get_node(e);
  if ( vp->level() > y_level ) {
    // 変化なし
    return e;
  }
  if ( vp->level() > x_level ) {
    // 一つ繰り上げる．
    return pd_step2(e, y_level);
  }

  // 極性を正規化しておく．
  tPol e_pol = get_pol(e);
  e = addpol(e, e_pol);
  tBddEdge result = mPushDownTable->get(e, xy_edge);
  if ( result == kEdgeInvalid ) {
    if ( vp->level() == x_level ) {
      tBddEdge e0 = vp->edge0();
      tBddEdge e1 = vp->edge1();
      result = pd_step3(e0, e1, y_level, pol);
    }
    else {
      tBddEdge e0 = vp->edge0();
      tBddEdge e1 = vp->edge1();
      tBddEdge r0 = pd_step(e0, x_level, y_level, pol);
      tBddEdge r1 = pd_step(e1, x_level, y_level, pol);
      result = new_node(vp->var(), r0, r1);
      mPushDownTable->put(e, xy_edge, result);
    }
  }
  return addpol_ifvalid(result, e_pol);
}

tBddEdge
BddMgrClassic::pd_step2(tBddEdge e,
			tLevel y_level)
{
  if ( check_leaf(e) ) {
    return e;
  }

  Node* vp = get_node(e);
  if ( vp->level() > y_level ) {
    return e;
  }

  // 極性を正規化しておく．
  tPol e_pol = get_pol(e);
  e = addpol(e, e_pol);
  tBddEdge result = mPushDownTable2->get(e, xy_edge);
  if ( result == kEdgeInvalid ) {
    tBddEdge e0 = vp->edge0();
    tBddEdge e1 = vp->edge1();
    tBddEdge r0 = pd_step2(e0, y_level);
    tBddEdge r1 = pd_step2(e1, y_level);

    tVarId vid = varid(vp->level() - 1);
    Var* var = var_of(vid);
    if ( !var ) {
      var = alloc_var(vid);
    }
    result = new_node(var, r0, r1);
    mPushDownTable2->put(e, xy_edge, result);
  }
  return addpol_ifvalid(result, e_pol);
}

tBddEdge
BddMgrClassic::pd_step3(tBddEdge e0,
			tBddEdge e1,
			tLevel y_level,
			tPol pol)
{
  Node* vp0 = get_node(e0);
  Node* vp1 = get_node(e1);
  tPol pol0 = get_pol(e0);
  tPol pol1 = get_pol(e1);
  tLevel top_level = kLevelMax;
  if ( vp0 && top_level > vp0->level() ) {
    top_level = vp0->level();
  }
  if ( vp1 && top_level > vp1->level() ) {
    top_level = vp1->level();
  }

  tBddEdge result = mPushDownTable3->get(e0, e1, xy_edge);
  if ( result == kEdgeInvalid ) {
    if ( top_level > y_level ) {
      tVarId vid = varid(y_level);
      Var* var = var_of(vid);
      if ( !var ) {
	var = alloc_var(vid);
      }
      if ( pol == kPolPosi ) {
	result = new_node(var, e0, e1);
      }
      else {
	result = new_node(var, e1, e0);
      }
    }
    else {
      tBddEdge e00, e01;
      tBddEdge e10, e11;
      if ( vp0 && top_level == vp0->level() ) {
	e00 = vp0->edge0(pol0);
	e01 = vp0->edge1(pol0);
      }
      else {
	e00 = e01 = e0;
      }
      if ( vp1 && top_level == vp1->level() ) {
	e10 = vp1->edge0(pol1);
	e11 = vp1->edge1(pol1);
      }
      else {
	e10 = e11 = e1;
      }
      tBddEdge r0 = pd_step3(e00, e10, y_level, pol);
      tBddEdge r1 = pd_step3(e01, e11, y_level, pol);
      tVarId vid = varid(top_level - 1);
      Var* var = var_of(vid);
      if ( !var ) {
	var = alloc_var(vid);
      }
      result = new_node(var, r0, r1);
    }
    mPushDownTable3->put(e0, e1, xy_edge, result);
  }
  return result;
}

// generalized cofactor 演算
tBddEdge
BddMgrClassic::gcofactor(tBddEdge e1,
			 tBddEdge e2)
{
  if ( check_error(e1) || check_error(e2) ) {
    return kEdgeError;
  }
  if ( check_overflow(e1) || check_overflow(e2) ) {
    return kEdgeOverflow;
  }
  if ( check_zero(e2) ) {
    return kEdgeError;
  }
  if ( check_cube(e2) ) {
    return cube_division(e1, e2);
  }
  return gcofactor_step(e1, e2);
}

// constrain演算
// 注: c はkEdge0であってはならない．
tBddEdge
BddMgrClassic::gcofactor_step(tBddEdge f,
			      tBddEdge c)
{
  if ( check_zero(c) ) {
    return kEdgeError;
  }
  if ( check_leaf(f) || check_one(c) ) {
    return f;
  }
  if ( f == c ) {
    return kEdge1;
  }
  if ( check_reverse(f, c) ) {
    return kEdge0;
  }

  // この時点で f, c は終端ではない．

  // (~f) / c の結果は ~(f / c) なので f を正規化する．
  tPol f_pol = get_pol(f);
  f = addpol(f, f_pol);
  tBddEdge result = mCofacTable->get(f, c);
  if ( result == kEdgeInvalid ) {
    Node* f_v = get_node(f);
    Node* c_v = get_node(c);
    tPol c_p = get_pol(c);
    Var* f_var = f_v->var();
    Var* c_var = c_v->var();
    tLevel f_level = f_var->level();
    tLevel c_level = c_var->level();

    tBddEdge f_0, f_1;
    if ( f_level <= c_level ) {
      f_0 = f_v->edge0();
      f_1 = f_v->edge1();
    }
    else {
      f_0 = f_1 = f;
    }

    if ( f_level < c_level ) {
      tBddEdge r0 = gcofactor_step(f_0, c);
      tBddEdge r1 = gcofactor_step(f_1, c);
      result = new_node(f_var, r0, r1);
    }
    else {
      tBddEdge c_0 = c_v->edge0(c_p);
      tBddEdge c_1 = c_v->edge1(c_p);
      if ( check_zero(c_0) ) {
	result = gcofactor_step(f_1, c_1);
      }
      else if ( check_zero(c_1) ) {
	result = gcofactor_step(f_0, c_0);
      }
      else {
	tBddEdge r0 = gcofactor_step(f_0, c_0);
	tBddEdge r1 = gcofactor_step(f_1, c_1);
	result = new_node(c_var, r0, r1);
      }
    }
    mCofacTable->put(f, c, result);
  }

  // 極性を元に戻す．
  result = addpol(result, f_pol);

  return result;
}

// 一つの変数に対する cofactor を計算する．
tBddEdge
BddMgrClassic::scofactor(tBddEdge e1,
			 tVarId id,
			 tPol pol)
{
  clear_varmark();
  Var* var = var_of(id);
  if ( !var ) {
    // var がないということは e1 中に含まれていない
    return e1;
  }
  if ( pol == kPolPosi ) {
    var->mMark = 1;
  }
  else {
    var->mMark = 2;
  }
  mLastLevel = var->level();
  tBddEdge ans = cubediv_step(e1);
  mCubedivTable->clear();
  clear_varmark();
  return ans;
}

// C がキューブの場合の拘束演算
tBddEdge
BddMgrClassic::cube_division(tBddEdge f,
			     tBddEdge c)
{
  clear_varmark();
  tBddEdge e = c;
  Node* vp = get_node(e);
  tPol pol = get_pol(e);
  mLastLevel = 0;
  while ( vp != 0 ) {
    tBddEdge e0 = vp->edge0(pol);
    tBddEdge e1 = vp->edge1(pol);
    Var* var = vp->var();
    mLastLevel = var->level();
    if ( check_zero(e0) ) {
      var->mMark = 1;
      e = e1;
    }
    else {
      var->mMark = 2;
      e = e0;
    }
    vp = get_node(e);
    pol = get_pol(e);
  }
  tBddEdge ans = cubediv_step(f);
  mCubedivTable->clear();
  clear_varmark();
  return ans;
}

// restricter がキューブの時は必ず O(n):nは第一引数のBDDのサイズ
// でできる
tBddEdge
BddMgrClassic::cubediv_step(tBddEdge f)
{
  if ( check_leaf(f) ) {
    return f;
  }
  // この時点で f, g は終端ではない．

  Node* f_vp = get_node(f);
  Var* f_var = f_vp->var();
  tLevel f_level = f_var->level();
  if ( f_level > mLastLevel ) {
    return f;
  }

  // (~f) / c の結果は ~(f / c) なので f を正規化する．
  tPol f_pol = get_pol(f);
  f = addpol(f, f_pol);

  tBddEdge result = mCubedivTable->get(f);
  if ( result == kEdgeInvalid ) {
    if ( f_var->mMark == 1 ) {
      // 肯定のリテラルが現れているので 1枝の結果を返す．
      tBddEdge f_1 = f_vp->edge1();
      result = cubediv_step(f_1);
    }
    else if ( f_var->mMark == 2 ) {
      // 否定のリテラルが現れているので 0枝の結果を返す．
      tBddEdge f_0 = f_vp->edge0();
      result = cubediv_step(f_0);
    }
    else {
      // この変数はキューブには現れない
      tBddEdge f_0 = f_vp->edge0();
      tBddEdge f_1 = f_vp->edge1();

      tBddEdge r_0 = cubediv_step(f_0);
      tBddEdge r_1 = cubediv_step(f_1);

      result = new_node(f_var, r_0, r_1);
    }
    mCubedivTable->put(f, result);
  }

  // 極性を元に戻す．
  result = addpol(result, f_pol);

  return result;
}

// Davio展開のモーメント項($f_{\overline{x}} \oplus f_x$)を
// 求める処理
// メモリ不足のためにエラーとなる可能性がある．
tBddEdge
BddMgrClassic::xor_moment(tBddEdge e,
			  tVarId idx)
{
  if ( check_error(e) ) {
    return kEdgeError;
  }
  if ( check_overflow(e) ) {
    return kEdgeOverflow;
  }
  Var* var = var_of(idx);
  if ( !var ) {
    // この変数がないということは答は0
    return kEdge0;
  }
  mLastLevel = var->level();
  tBddEdge ans = xcofactor_step(e);
  mXcofactorTable->clear();
  return ans;
}

// Davio 展開もモーメント項を求める．
tBddEdge
BddMgrClassic::xcofactor_step(tBddEdge f)
{
  if ( check_leaf(f) ) {
    // 終端ならば x でコファクターをとっても違いがないので
    // 答は 0 となる．
    return kEdge0;
  }

  // この時点で e は終端ではない．
  Node* vp = get_node(f);
  Var* var = vp->var();
  tLevel level = var->level();
  if ( level > mLastLevel ) {
    // 今のレベルはコファクタリングすべきレベルよりも下なので
    // 答は 0 となる．
    return kEdge0;
  }

  // f の結果と ~f の結果は等しいので
  // 極性を正規化する．
  tPol f_pol = get_pol(f);
  f = addpol(f, f_pol);

  // まずハッシュを探す．
  tBddEdge result = mXcofactorTable->get(f);
  if ( result == kEdgeInvalid ) {
    // ハッシュになかった．
    tBddEdge e_0 = vp->edge0();
    tBddEdge e_1 = vp->edge1();
    if ( level == mLastLevel ) {
      result = xor_op(e_0, e_1);
    }
    else { // level < mLastLevel
      tBddEdge r_0 = xcofactor_step(e_0);
      tBddEdge r_1 = xcofactor_step(e_1);
      result = new_node(var, r_0, r_1);
    }
    // ハッシュに登録しておく．
    mXcofactorTable->put(f, result);
  }
  return result;
}

END_NAMESPACE_YM_BDD
