
/// @file LibComp.cc
/// @brief LibComp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LibComp.h"
#include "LcClass.h"
#include "LcGroup.h"
#include "LcPatNode.h"
#include "LcPatHandle.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"
#include "ym_logic/LogExpr.h"
#include "ym_logic/NpnMap.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

BEGIN_NONAMESPACE

// 論理式の変数を map にしたがって変換する．
LogExpr
xform_expr(const LogExpr& expr,
	   const NpnMapM& map)
{
  ymuint ni = map.ni();
  ymuint no = map.no();
  assert_cond( no == 1, __FILE__, __LINE__);
  VarLogExprMap vlm;
  for (ymuint i = 0; i < ni; ++ i) {
    NpnVmap imap = map.imap(i);
    ymuint j = imap.pos();
    LogExpr expr;
    if ( imap.pol() == kPolPosi) {
      expr = LogExpr::make_posiliteral(j);
    }
    else {
      expr = LogExpr::make_negaliteral(j);
    }
    vlm.insert(make_pair(i, expr));
  }
  LogExpr cexpr = expr.compose(vlm);
  if ( map.omap(0).pol() == kPolNega ) {
    cexpr = ~cexpr;
  }
  return cexpr;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス LibComp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LibComp::LibComp()
{
}

// @brief デストラクタ
LibComp::~LibComp()
{
}

// @brief セルのグループ化，クラス化を行う．
void
LibComp::compile(const CellLibrary& library)
{
  mLogicMgr.init();
  mFFMgr.init();
  mLatchMgr.init();
  mPatMgr.init();

  // XOR のパタンを登録しておく．
  // これはちょっとしたハック
  {
    LogExpr lit0 = LogExpr::make_posiliteral(0);
    LogExpr lit1 = LogExpr::make_posiliteral(1);
    LogExpr xor_ex = lit0 ^ lit1;
    reg_expr(xor_ex);
  }

  ymuint nc = library.cell_num();
  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = library.cell(i);

    if ( cell->is_logic() ) {
      mLogicMgr.add_cell(cell);

      // パタンを作る．
      ymuint no2 = cell->output_num2();
      if ( no2 != 1 ) {
	// 出力ピンが複数あるセルは対象外
	continue;
      }
      if ( !cell->has_logic(0) ) {
	// 論理式を持たないセルも対象外
	continue;
      }
      if ( cell->has_tristate(0) ) {
	// three_state 属性を持つセルも対象外
	continue;
      }

      LogExpr expr = cell->logic_expr(0);
      reg_expr(expr);
    }
    else if ( cell->is_ff() ) {
      mFFMgr.add_cell(cell);
    }
    else if ( cell->is_latch() ) {
      mLatchMgr.add_cell(cell);
    }
  }
}

// @brief expr から生成されるパタンを登録する．
void
LibComp::reg_expr(const LogExpr& expr)
{
  // expr に対応する LcGroup を求める．
  TvFunc f = expr.make_tv();
  LcGroup* fgroup = mLogicMgr.find_group(TvFuncM(f));
  const LcClass* fclass = fgroup->parent();

  // fclass->rep_func() を用いる理由は論理式に現れる変数が
  // 真のサポートとは限らないから
  if ( fclass->repfunc().ni() > 1 ) {
    // expr を変換したパタンを登録する．
    LogExpr cexpr = xform_expr(expr, fgroup->map());
    assert_cond( !cexpr.is_constant(), __FILE__, __LINE__);

    mPatMgr.reg_pat(cexpr, fclass->id());
  }
}

// @brief 論理セルグループの情報を取り出す．
const LcGroupMgr&
LibComp::logic_group_mgr() const
{
  return mLogicMgr;
}

// @brief FFセルグループの情報を取り出す．
const LcGroupMgr&
LibComp::ff_group_mgr() const
{
  return mFFMgr;
}

// @brief ラッチセルグループの情報を取り出す．
const LcGroupMgr&
LibComp::latch_group_mgr() const
{
  return mLatchMgr;
}

// @brief パタングラフの情報を取り出す．
const LcPatMgr&
LibComp::pat_mgr() const
{
  return mPatMgr;
}

// @brief グラフ構造全体をダンプする．
// @param[in] s 出力先のストリーム
void
LibComp::display(ostream& s) const
{
  // 論理セルグループの情報を出力する．
  s << "*** LogicGroupMgr BEGIN ***" << endl;
  mLogicMgr.display(s);
  s << "*** LogicGroupMgr END ***" << endl
    << endl;

  // FFグループの情報を出力する．
  s << "*** FFGroupMgr BEGIN ***" << endl;
  mFFMgr.display(s);
  s << "*** FFGroupMgr END ***" << endl
    << endl;

  // ラッチグループの情報を出力する．
  s << "*** LatchGroupMgr BEGIN ***" << endl;
  mLatchMgr.display(s);
  s << "*** LatchGroupMgr END ***" << endl
    << endl;

  // パタングラフの情報を出力する．
  mPatMgr.display(s);
}

END_NAMESPACE_YM_CELL_LIBCOMP
