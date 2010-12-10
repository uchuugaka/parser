#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIDECLELEM_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIDECLELEM_H

/// @file libym_verilog/elaborator/ei/EiDeclElem.h
/// @brief EiDeclElem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiDecl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclElem EiDeclElem.h "EiDeclElem.h"
/// @brief ElbDeclArray の要素を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclElem :
  public ElbDecl
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] parent_array 親の配列
  /// @param[in] index_list インデックスのリスト
  EiDeclElem(const PtBase* pt_expr,
	     ElbDeclArray* parent_array,
	     const vector<ElbExpr*>& index_list);

  /// @brief デストラクタ
  virtual
  ~EiDeclElem();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;

  /// @brief ファイル位置を返す．
  virtual
  FileRegion
  file_region() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  virtual
  const VlNamedObj*
  parent() const;

  /// @brief 名前の取得
  virtual
  const char*
  name() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlDecl の関数
  //////////////////////////////////////////////////////////////////////

  /// @breif 値の型を返す．
  /// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
  virtual
  tVpiValueType
  value_type() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const;

  /// @brief MSB の値を返す．
  /// @retval 範囲のMSBの値 範囲指定を持つとき
  /// @retval -1 範囲指定を持たないとき
  virtual
  int
  left_range_const() const;

  /// @brief LSB の値を返す．
  /// @retval 範囲のLSBの値 範囲指定を持つとき
  /// @retval -1 範囲指定を持たないとき
  virtual
  int
  right_range_const() const;

  /// @brief ビット幅を返す．
  virtual
  ymuint
  bit_size() const;

  /// @brief オフセット値の取得
  /// @param[in] index インデックス
  /// @retval index に対するオフセット値 index が範囲内に入っている時．
  /// @retval -1 index が範囲外の時
  virtual
  int
  bit_offset(int index) const;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  virtual
  tVpiVarType
  data_type() const;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  virtual
  tVpiNetType
  net_type() const;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  virtual
  tVpiVsType
  vs_type() const;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  drive0() const;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  drive1() const;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  virtual
  tVpiStrength
  charge() const;

  /// @brief delay の取得
  /// @retval delay
  /// @retval NULL delay の指定なし
  virtual
  const VlDelay*
  delay() const;

  /// @brief 配列要素の時に true を返す．
  virtual
  bool
  is_array_member() const;

  /// @brief 多次元の配列要素の時に true を返す．
  virtual
  bool
  is_multi_array_member() const;

  /// @brief 配列要素の時に親の配列を返す．
  virtual
  VlDecl*
  parent_array() const;

  /// @brief 1次元配列要素の時にインデックスを返す．
  virtual
  const VlExpr*
  index() const;

  /// @brief 多次元配列要素の時にインデックスのリストを返す．
  /// @param[out] index_list インデックスのリストを格納する変数
  virtual
  void
  index(vector<const VlExpr*>& index_list) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号付きに補正する．
  /// @note このクラスで呼ばれることはない．
  virtual
  void
  set_signed();

  /// @brief スカラー値を返す．
  virtual
  tVpiScalarVal
  get_scalar() const;

  /// @brief スカラー値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_scalar(tVpiScalarVal val);

  /// @brief 論理値を返す．
  virtual
  tVpiScalarVal
  get_logic() const;

  /// @brief real 型の値を返す．
  virtual
  double
  get_real() const;

  /// @brief real 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_real(double val);

  /// @brief bitvector 型の値を返す．
  /// @param[out] val 値
  /// @param[in] req_type 要求される型
  virtual
  void
  get_bitvector(BitVector& val,
		tVpiValueType req_type = kVpiValueNone) const;

  /// @brief bitvector 型の値を設定する．
  /// @param[in] val 値
  virtual
  void
  set_bitvector(const BitVector& val);

  /// @brief ビット選択値を返す．
  /// @param[in] index ビット位置
  virtual
  tVpiScalarVal
  get_bitselect(int index) const;

  /// @brief ビット値を設定する．
  /// @param[in] index ビット位置
  /// @param[in] val 値
  virtual
  void
  set_bitselect(int index,
		tVpiScalarVal val);

  /// @brief 範囲選択値を返す．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[out] val 値
  virtual
  void
  get_partselect(int left,
		 int right,
		 BitVector& val) const;

  /// @brief 範囲値を設定する．
  /// @param[in] left 範囲の MSB
  /// @param[in] right 範囲の LSB
  /// @param[in] val 値
  virtual
  void
  set_partselect(int left,
		 int right,
		 const BitVector& val);

  /// @brief 範囲のMSBの取得
  /// @retval 範囲のMSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  _left_range() const;

  /// @brief 範囲のLSBの取得
  /// @retval 範囲のLSB 範囲を持つとき
  /// @retval NULL 範囲を持たないとき
  virtual
  ElbExpr*
  _right_range() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief インデックス式を評価してオフセットを計算する．
  ymuint
  calc_offset() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の定義要素
  const PtBase* mPtExpr;

  // 親の配列
  ElbDeclArray* mArray;

  // インデックスのリスト
  vector<ElbExpr*> mIndexList;

};

END_NAMESPACE_YM_VERILOG

#endif //  LIBYM_VERILOG_ELABORATOR_EI_EIDECLELEM_H
