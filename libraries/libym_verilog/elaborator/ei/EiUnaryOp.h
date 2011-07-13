#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIUNARYOP_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIUNARYOP_H

/// @file libym_verilog/elaborator/ei/EiUnaryOp.h
/// @brief EiExpr の単項演算子の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiUnaryOp.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

//////////////////////////////////////////////////////////////////////
// expression を表すクラス
// IEEE Std 1364-2001 26.6.25 Simple expressions
// IEEE Std 1364-2001 26.6.26 Expressions
//
// operation の分類
// オペランドの数
//
// - 1個
//   - 返り値 scalar, オペランド scalar
//     - NotOp
//   - 返り値 scalar, オペランド bitvector
//     - UnaryAndOp, UnaryNandOp, UnaryOrOp, UnaryNorOp,
//       UnaryXorOp, UnaryXNorOp
//   - 返り値 オペランドと同一, オペランド bitvector
//     - BitNegOp
//   - 返り値 オペランドと同一, オペランド any
//     - PlusOp, MinusOp
//   - 返り値 none, オペランド none
//     - PosedgeOp, NegedgeOp
//////////////////////////////////////////////////////////////////////


#include "EiOperation.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiUnaryOp EiUnaryOp.h "EiUnaryOp.h"
/// @brief 単項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class EiUnaryOp :
  public EiOperation
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiUnaryOp(const PtExpr* pt_expr,
	    ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiUnaryOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数の時 true を返す．
  /// @note オペランドが定数ならこの式も定数となる．
  virtual
  bool
  is_const() const;

  /// @brief オペランド数を返す．
  virtual
  ymuint
  operand_num() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号
  virtual
  ElbExpr*
  _operand(ymuint pos) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 派生クラス用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オペランドを返す．
  const ElbExpr*
  operand1() const { return mOpr1; }

  /// @brief オペランドを返す．
  ElbExpr*
  operand1() { return mOpr1; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド
  ElbExpr* mOpr1;

};


//////////////////////////////////////////////////////////////////////
/// @class EiNotOp EiUnaryOp.h "EiUnaryOp.h"
/// @note 論理否定演算子
//////////////////////////////////////////////////////////////////////
class EiNotOp :
  public EiUnaryOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiNotOp(const PtExpr* pt_expr,
	  ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiNotOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  VlValueType
  value_type() const;

  /// @brief オペランドに要求されるデータ型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
  /// @note kVpiOperation の時，意味を持つ．
  /// @note それ以外では kVpiValueNone を返す．
  /// 通常はオペランドの式の value_type() に一致するが，
  /// その式が self-typed の場合には異なることもある．
  virtual
  VlValueType
  operand_type(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(const VlValueType& type);

};


//////////////////////////////////////////////////////////////////////
/// @class EiBitNeg EiUnaryOp.h "EiUnaryOp.h"
/// @brief ビットワイズ反転演算子
//////////////////////////////////////////////////////////////////////
class EiBitNegOp :
  public EiUnaryOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiBitNegOp(const PtExpr* pt_expr,
	     ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiBitNegOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  VlValueType
  value_type() const;

  /// @brief オペランドに要求されるデータ型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
  /// @note kVpiOperation の時，意味を持つ．
  /// @note それ以外では kVpiValueNone を返す．
  /// 通常はオペランドの式の value_type() に一致するが，
  /// その式が self-typed の場合には異なることもある．
  virtual
  VlValueType
  operand_type(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の設定用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(const VlValueType& type);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式の型
  VlValueType mType;

};


//////////////////////////////////////////////////////////////////////
/// @class EiReductionOp EiUnaryOp.h "EiUnaryOp.h"
/// @note リダクション演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiReductionOp :
  public EiUnaryOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiReductionOp(const PtExpr* pt_expr,
		ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiReductionOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  VlValueType
  value_type() const;

  /// @brief オペランドに要求されるデータ型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
  /// @note kVpiOperation の時，意味を持つ．
  /// @note それ以外では kVpiValueNone を返す．
  /// 通常はオペランドの式の value_type() に一致するが，
  /// その式が self-typed の場合には異なることもある．
  virtual
  VlValueType
  operand_type(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(const VlValueType& type);

};


//////////////////////////////////////////////////////////////////////
/// @class EiUnaryArithOp EiUnaryOp.h "EiUnaryOp.h"
//////////////////////////////////////////////////////////////////////
class EiUnaryArithOp :
  public EiUnaryOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiUnaryArithOp(const PtExpr* pt_expr,
		 ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiUnaryArithOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  VlValueType
  value_type() const;

  /// @brief オペランドに要求されるデータ型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
  /// @note kVpiOperation の時，意味を持つ．
  /// @note それ以外では kVpiValueNone を返す．
  /// 通常はオペランドの式の value_type() に一致するが，
  /// その式が self-typed の場合には異なることもある．
  virtual
  VlValueType
  operand_type(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の設定用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(const VlValueType& type);

};


//////////////////////////////////////////////////////////////////////
/// @class EiEventEdgeOp EiUnaryOp.h "EiUnaryOp.h"
/// @brief posdedge / negaedge 演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiEventEdgeOp :
  public EiUnaryOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド
  EiEventEdgeOp(const PtExpr* pt_expr,
		ElbExpr* opr1);

  /// @brief デストラクタ
  virtual
  ~EiEventEdgeOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  VlValueType
  value_type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の設定用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  set_reqsize(const VlValueType& type);

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIUNARYOP_H
