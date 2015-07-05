﻿#ifndef SNAND_H
#define SNAND_H

/// @file SnAnd.h
/// @brief SnAnd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SnGate.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM

//////////////////////////////////////////////////////////////////////
/// @class SnAnd SimNode.h
/// @brief ANDノード
//////////////////////////////////////////////////////////////////////
class SnAnd :
  public SnGate
{
public:

  /// @brief コンストラクタ
  SnAnd(ymuint32 id,
	const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnAnd();


public:

  /// @brief 正常値の計算を行う．(3値版)
  /// @note 結果は mGval0, mGval1 に格納される．
  virtual
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  /// @note 結果は mFval0, mFval1 に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

  /// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
  virtual
  PackedVal
  calc_gobs3(ymuint ipos);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnAnd2 SimNode.h
/// @brief 2入力ANDノード
//////////////////////////////////////////////////////////////////////
class SnAnd2 :
  public SnGate2
{
public:

  /// @brief コンストラクタ
  SnAnd2(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnAnd2();


public:

  /// @brief 正常値の計算を行う．(3値版)
  /// @note 結果は mGval0, mGval1 に格納される．
  virtual
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  /// @note 結果は mFval0, mFval1 に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

  /// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
  virtual
  PackedVal
  calc_gobs3(ymuint ipos);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnAnd3 SimNode.h
/// @brief 3入力ANDノード
//////////////////////////////////////////////////////////////////////
class SnAnd3 :
  public SnGate3
{
public:

  /// @brief コンストラクタ
  SnAnd3(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnAnd3();


public:

  /// @brief 正常値の計算を行う．(3値版)
  /// @note 結果は mGval0, mGval1 に格納される．
  virtual
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  /// @note 結果は mFval0, mFval1 に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

  /// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
  virtual
  PackedVal
  calc_gobs3(ymuint ipos);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnAnd4 SimNode.h
/// @brief 4入力ANDノード
//////////////////////////////////////////////////////////////////////
class SnAnd4 :
  public SnGate4
{
public:

  /// @brief コンストラクタ
  SnAnd4(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnAnd4();


public:

  /// @brief 正常値の計算を行う．(3値版)
  /// @note 結果は mGval0, mGval1 に格納される．
  virtual
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  /// @note 結果は mFval0, mFval1 に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

  /// @brief ゲートの入力から出力までの可観測性を計算する．(3値版)
  virtual
  PackedVal
  calc_gobs3(ymuint ipos);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnNand SimNode.h
/// @brief NANDノード
//////////////////////////////////////////////////////////////////////
class SnNand :
  public SnAnd
{
public:

  /// @brief コンストラクタ
  SnNand(ymuint32 id,
	 const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNand();


public:

  /// @brief 正常値の計算を行う．(3値版)
  /// @note 結果は mGval0, mGval1 に格納される．
  virtual
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  /// @note 結果は mFval0, mFval1 に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnNand2 SimNode.h
/// @brief 2入力NANDノード
//////////////////////////////////////////////////////////////////////
class SnNand2 :
  public SnAnd2
{
public:

  /// @brief コンストラクタ
  SnNand2(ymuint32 id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNand2();


public:

  /// @brief 正常値の計算を行う．(3値版)
  /// @note 結果は mGval0, mGval1 に格納される．
  virtual
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  /// @note 結果は mFval0, mFval1 に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnNand3 SimNode.h
/// @brief 3入力NANDノード
//////////////////////////////////////////////////////////////////////
class SnNand3 :
  public SnAnd3
{
public:

  /// @brief コンストラクタ
  SnNand3(ymuint32 id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNand3();


public:

  /// @brief 正常値の計算を行う．(3値版)
  /// @note 結果は mGval0, mGval1 に格納される．
  virtual
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  /// @note 結果は mFval0, mFval1 に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SnNand4 SimNode.h
/// @brief 4入力NANDノード
//////////////////////////////////////////////////////////////////////
class SnNand4 :
  public SnAnd4
{
public:

  /// @brief コンストラクタ
  SnNand4(ymuint32 id,
	  const vector<SimNode*>& inputs);

  /// @brief デストラクタ
  virtual
  ~SnNand4();


public:

  /// @brief 正常値の計算を行う．(3値版)
  /// @note 結果は mGval0, mGval1 に格納される．
  virtual
  void
  _calc_gval3();

  /// @brief 故障値の計算を行う．(3値版)
  /// @param[in] mask マスク
  /// @note 結果は mFval0, mFval1 に格納される．
  virtual
  void
  _calc_fval3(PackedVal mask);

  /// @brief 内容をダンプする．
  virtual
  void
  dump(ostream& s) const;

};

END_NAMESPACE_YM_SATPG_FSIM

#endif // SNAND_H
