
/// @file libym_verilog/elb/ElbStmt.cc
/// @brief ElbStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbStmt.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbStmt::ElbStmt() :
  mNext(NULL)
{
}

// @brief デストラクタ
ElbStmt::~ElbStmt()
{
}

// @brief 対象のスコープの取得
// @note kDisable/kParBlock/kSeqBlock で意味のある関数
// @note このクラスでは NULL を返す．
const VlNamedObj*
ElbStmt::scope() const
{
  return NULL;
}

// @brief task の実体を返す．
// @note kEnable で意味のある関数
// @note このクラスでは NULL を返す．
const VlTaskFunc*
ElbStmt::task() const
{
  return NULL;
}

// @brief user systf クラスへのポインタを返す．
// @note kSysEnable で意味のある関数
// @note このクラスでは NULL を返す．
const VlUserSystf*
ElbStmt::user_systf() const
{
  return NULL;
}

// @brief 引数の数の取得
// @note kEnable/kSysEnable で意味のある関数
// @note このクラスでは 0 を返す．
ymuint
ElbStmt::arg_num() const
{
  return 0;
}

// @brief 引数の取得
// @param[in] pos 位置 (0 <= pos < arg_num())
// @note kEnable/kSysEnable で意味のある関数
// @note このクラスでは NULL を返す．
const VlExpr*
ElbStmt::arg(ymuint pos) const
{
  return NULL;
}

// @brief control の取得
// @note このクラスでは NULL を返す．
const VlControl*
ElbStmt::control() const
{
  return NULL;
}

// @brief 本体のステートメントの取得
// @note kDc/kEc/kWait/kForever/kRepeat/kWhile/kFor/kIf で意味のある関数
// @note このクラスでは NULL を返す．
const VlStmt*
ElbStmt::body_stmt() const
{
  return NULL;
}

// @brief 式の取得
// @note kWait/kRepeat/kWhile/kFor/kIf/kCase/kCaseX/kCaseZ で意味のある関数
// @note このクラスでは NULL を返す．
const VlExpr*
ElbStmt::expr() const
{
  return NULL;
}

// @brief 左辺式の取得
// @note kAssign/kForce/kPcAssign/kRelease/kDeassign で意味のある関数
// @note このクラスでは NULL を返す．
const VlExpr*
ElbStmt::lhs() const
{
  return NULL;
}

// @brief 左辺式の要素数の取得
// @note 通常は1だが，連結演算子の場合はその子供の数となる．
// @note ただし，連結演算の入れ子はすべて平坦化して考える．
// @note このクラスでは0を返す．
ymuint
ElbStmt::lhs_elem_num() const
{
  return 0;
}

// @brief 左辺式の要素の取得
// @param[in] pos 位置 ( 0 <= pos < lhs_elem_num() )
// @note 連結演算子の見かけと異なり LSB 側が0番めの要素となる．
// @note このクラスでは NULL を返す．
const VlExpr*
ElbStmt::lhs_elem(ymuint pos) const
{
  return NULL;
}

// @brief 右辺式の取得
// @note kAssign/kForce/kPcAssign で意味のある関数
// @note このクラスでは NULL を返す．
const VlExpr*
ElbStmt::rhs() const
{
  return NULL;
}

// @brief 代入のブロッキング/ノンブロッキングの区別の取得
// @note このクラスでは false を返す．
bool
ElbStmt::is_blocking() const
{
  return false;
}

// @brief イベントプライマリの取得
// @note kEvent で意味のある関数
// @note このクラスでは NULL を返す．
const VlDecl*
ElbStmt::named_event() const
{
  return NULL;
}

// @brief 条件が成り立たなかったとき実行されるステートメントの取得
// @note kIf で意味のある関数
// @note このクラスでは NULL を返す．
const VlStmt*
ElbStmt::else_stmt() const
{
  return NULL;
}

// @brief case type の取得
// @return case type
// @note kCase/kCaseX/kCaseZ で意味のある関数
// @note このクラスでは kVpiCaseExact を返す．
tVpiCaseType
ElbStmt::case_type() const
{
  return kVpiCaseExact;
}

// @brief case item の要素数の取得
// @return case item の要素数
// @note kCase/kCaseX/kCaseZ で意味のある関数
// @note このクラスでは 0 を返す．
ymuint
ElbStmt::caseitem_num() const
{
  return 0;
}

// @brief case item の取得
// @param[in] pos 位置番号 (0 <= pos < caseitem_num())
// @note kCase/kCaseX/kCaseZ で意味のある関数
// @note このクラスでは NULL を返す．
const VlCaseItem*
ElbStmt::caseitem(ymuint pos) const
{
  return NULL;
}

// @brief 初期化代入文の取得
// @note kFor で意味のある関数
// @note このクラスでは NULL を返す．
const VlStmt*
ElbStmt::init_stmt() const
{
  return NULL;
}

// @brief 繰り返し代入文の取得
// @note kFor で意味のある関数
// @note このクラスでは NULL を返す．
const VlStmt*
ElbStmt::inc_stmt() const
{
  return NULL;
}

// @brief 子供のステートメントの数の取得
// @note kParBlock/kSeqBlock で意味のある関数
// @note このクラスでは 0 を返す．
ymuint
ElbStmt::child_stmt_num() const
{
  return 0;
}

// @brief 子供のステートメントの取得
// @param[in] pos 位置番号 (0 <= pos < stmt_num())
// @note kParBlock/kSeqBlock で意味のある関数
const VlStmt*
ElbStmt::child_stmt(ymuint pos) const
{
  return _child_stmt(pos);
}

// @brief caseitem を設定する．
// @param[in] pos 位置番号
// @param[in] pt_caseitem パース木の caseitem 定義
// @param[in] expr_array 条件式を納める配列
// @param[in] stmt 本体のステートメント
// @note このクラスでは何もしない．
void
ElbStmt::set_caseitem(ymuint pos,
		      const PtCaseItem* pt_caseitem,
		      ElbExpr** expr_array,
		      ElbStmt* stmt)
{
}

// @brief 子供ののステートメントの取得
// @note このクラスでは NULL を返す．
ElbStmt*
ElbStmt::_child_stmt(ymuint pos) const
{
  return NULL;
}

END_NAMESPACE_YM_VERILOG
