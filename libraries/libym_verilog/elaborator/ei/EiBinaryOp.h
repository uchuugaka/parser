#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIBINARYOP_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIBINARYOP_H

/// @file libym_verilog/elaborator/ei/EiBinaryOp.h
/// @brief EiExpr の2項演算子の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiBinaryOp.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

//////////////////////////////////////////////////////////////////////
/// expression を表すクラス
/// IEEE Std 1364-2001 26.6.25 Simple expressions
/// IEEE Std 1364-2001 26.6.26 Expressions
///
/// operation の分類
/// オペランドの数
///
/// - 2個
///   - 返り値 scalar, オペランド any
///     - EqOp, NeqOp, CaseEqOp, CaseNeqOp, GtOp, GeOp, LtOp, LeOp
///   - 返り値 scalar, オペランド scalar
///     - LogAndOp, LogOrOp
///   - 返り値 オペランドと同一, オペランド bitvector
///     - BitAndOp, BitOrOp, BitXorOp, BitXNorOp
///   - 返り値 オペランドと同一, オペランド any
///     - AddOp, SubOp, MultOp, DivOp, ModOp
///     - PowerOp
///   - 返り値 第1オペランドと同一, オペランド any
///     - LShiftOp, RShiftOp, ArithLShiftOp, ArithRShiftOp
//////////////////////////////////////////////////////////////////////


#include "EiOperation.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiBinaryOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 二項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class EiBinaryOp :
  public EiOperation
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiBinaryOp(const PtExpr* pt_expr,
	     ElbExpr* opr1,
	     ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiBinaryOp();


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
  // EiOperation の仮想関数
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

  /// @brief 第1オペランドを返す．
  const ElbExpr*
  operand1() const { return mOpr[0]; }

  /// @brief 第2オペランドを返す．
  const ElbExpr*
  operand2() const { return mOpr[1]; }

  /// @brief 第1オペランドを返す．
  ElbExpr*
  operand1() { return mOpr[0]; }

  /// @brief 第2オペランドを返す．
  ElbExpr*
  operand2() { return mOpr[1]; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド
  ElbExpr* mOpr[2];

};


//////////////////////////////////////////////////////////////////////
/// @class EiCompareOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 比較演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiCompareOp :
  public EiBinaryOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiCompareOp(const PtExpr* pt_expr,
	      ElbExpr* opr1,
	      ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiCompareOp();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランドの型
  VlValueType mOprType;

};


//////////////////////////////////////////////////////////////////////
/// @class EiBinaryLogOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 2項論理演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiBinaryLogOp :
  public EiBinaryOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiBinaryLogOp(const PtExpr* pt_expr,
		ElbExpr* opr1,
		ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiBinaryLogOp();


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
/// @class EiBinaryBitOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 2項論理演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiBinaryBitOp :
  public EiBinaryOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiBinaryBitOp(const PtExpr* pt_expr,
		ElbExpr* opr1,
		ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiBinaryBitOp();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式の型
  VlValueType mType;

};


//////////////////////////////////////////////////////////////////////
/// @class EiBinaryArithOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 2項算術演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiBinaryArithOp :
  public EiBinaryOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiBinaryArithOp(const PtExpr* pt_expr,
		  ElbExpr* opr1,
		  ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiBinaryArithOp();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式の型
  VlValueType mType;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPowerOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief べき乗算演算子
//////////////////////////////////////////////////////////////////////
class EiPowerOp :
  public EiBinaryOp
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiPowerOp(const PtExpr* pt_expr,
	    ElbExpr* opr1,
	    ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiPowerOp();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式の型
  VlValueType mType;

};


//////////////////////////////////////////////////////////////////////
/// @class EiShiftOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief シフト演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiShiftOp :
  public EiBinaryOp
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] opr1 オペランド1
  /// @param[in] opr2 オペランド2
  EiShiftOp(const PtExpr* pt_expr,
	    ElbExpr* opr1,
	    ElbExpr* opr2);

  /// @brief デストラクタ
  virtual
  ~EiShiftOp();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式の型
  VlValueType mType;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIBINARYOP_H
