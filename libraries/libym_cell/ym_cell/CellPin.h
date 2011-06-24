#ifndef YM_CELL_CELLPIN_H
#define YM_CELL_CELLPIN_H

/// @file　ym_cell/CellPin.h
/// @brief CellPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellPin.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_cell/CellCapacitance.h"
#include "ym_logic/lexp_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellPin CellPin.h <ym_cell/CellPin.h>
/// @brief セルのピンを表すクラス
//////////////////////////////////////////////////////////////////////
class CellPin
{
public:
  //////////////////////////////////////////////////////////////////////
  /// @brief ピンの方向を表す列挙型
  //////////////////////////////////////////////////////////////////////
  enum tDirection {
    kDirInput,
    kDirOutput,
    kDirInout,
    kDirInternal
  };


  //////////////////////////////////////////////////////////////////////
  /// @brief タイミングの条件を表す列挙型
  //////////////////////////////////////////////////////////////////////
  enum tTimingSense {
    kSensePosiUnate = 1,
    kSenseNegaUnate = 2,
    kSenseNonUnate  = 3
  };

protected:

  /// @brief コンストラクタ
  CellPin() { }

  /// @brief デストラクタ
  virtual
  ~CellPin() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン番号を返す．
  virtual
  ymuint
  id() const = 0;

  /// @brief ピン名を返す．
  virtual
  string
  name() const = 0;

  /// @brief 方向を返す．
  virtual
  tDirection
  direction() const = 0;

  /// @brief 入力ピンの時に true を返す．
  virtual
  bool
  is_input() const = 0;

  /// @brief 出力ピンの時に true を返す．
  virtual
  bool
  is_output() const = 0;

  /// @brief 入出力ピンの時に true を返す．
  virtual
  bool
  is_inout() const = 0;

  /// @brief 内部ピンの時に true を返す．
  virtual
  bool
  is_internal() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 負荷容量を返す．
  virtual
  CellCapacitance
  capacitance() const = 0;

  /// @brief 立ち上がり時の負荷容量を返す．
  virtual
  CellCapacitance
  rise_capacitance() const = 0;

  /// @brief 立ち下がり時の負荷容量を返す．
  virtual
  CellCapacitance
  fall_capacitance() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 出力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理式を持っているときに true を返す．
  virtual
  bool
  has_function() const = 0;

  /// @brief 機能を表す論理式を返す．
  virtual
  LogExpr
  function() const = 0;

  /// @brief three_state 属性を持っているときに true を返す．
  virtual
  bool
  has_three_state() const = 0;

  /// @brief three_state 論理式を返す．
  virtual
  LogExpr
  three_state() const = 0;

  /// @brief 最大ファンアウト容量を返す．
  virtual
  CellCapacitance
  max_fanout() const = 0;

  /// @brief 最小ファンアウト容量を返す．
  virtual
  CellCapacitance
  min_fanout() const = 0;

  /// @brief 最大負荷容量を返す．
  virtual
  CellCapacitance
  max_capacitance() const = 0;

  /// @brief 最小負荷容量を返す．
  virtual
  CellCapacitance
  min_capacitance() const = 0;

  /// @brief 最大遷移時間を返す．
  virtual
  CellTime
  max_transition() const = 0;

  /// @brief 最小遷移時間を返す．
  virtual
  CellTime
  min_transition() const = 0;

  /// @brief タイミング情報の取得
  /// @param[in] ipos 開始ピン番号
  /// @param[in] timing_sense タイミング情報の摘要条件
  /// @return 条件に合致するタイミング情報を返す．
  /// @note なければ NULL を返す．
  virtual
  const CellTiming*
  timing(ymuint ipos,
	 tTimingSense sense) const = 0;

};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLPIN_H
