#ifndef PTNODE_EXPR_H
#define PTNODE_EXPR_H

/// @file PtNode_expr.h
/// @brief PtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"
#include "YmVerilog/verilog.h"
#include "YmVerilog/pt/PtP.h"
#include "YmVerilog/pt/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ExprListNode PtNode_expr.h
/// @brief 式のリストを表すノード
//////////////////////////////////////////////////////////////////////
class ExprListNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] expr_array 式の配列
  ExprListNode(const QString& label,
	       const PtExprArray& expr_array);

  /// @brief デストラクタ
  virtual
  ~ExprListNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role
  virtual
  QVariant
  data(int column,
       int role) const;

  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ラベル
  QString mLabel;

  // 式の配列
  PtExprArray mExprArray;

};


//////////////////////////////////////////////////////////////////////
/// @class ExprNode PtNode_expr.h
/// @brief 式を表すノード
//////////////////////////////////////////////////////////////////////
class ExprNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] expr 式
  ExprNode(const QString& label,
	   const PtExpr* expr);

  /// @brief デストラクタ
  virtual
  ~ExprNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role
  virtual
  QVariant
  data(int column,
       int role) const;

  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ラベル
  QString mLabel;

  // 式
  const PtExpr* mExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // PTNODE_EXPR_H
