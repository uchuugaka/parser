#ifndef YM_NETWORKS_CMN_CMNDFFCELL_H
#define YM_NETWORKS_CMN_CMNDFFCELL_H

/// @file ym_networks/CmnDffCell.h
/// @brief CmnDffCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/cmn.h"
#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class CmnDffCell CmnDffCell.h "ym_networks/CmnDffCell.h"
/// @brief CMN で用いる D-FF セルの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class CmnDffCell
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] cell セル
  /// @param[in] pos_array ピン情報の配列
  /// @note pos_array の意味は以下の通り
  ///  - pos_array[0] : データ入力のピン番号     (3bit)
  ///  - pos_array[1] : クロック入力のピン番号   (3bit)
  ///  - pos_array[2] : クリア入力のピン番号     (3bit) | ピン情報 (2bit)
  ///  - pos_array[3] : プリセット入力のピン番号 (3bit) | ピン情報 (2bit)
  ///  - pos_array[4] : 肯定出力のピン番号       (3bit)
  ///  - pos_array[5] : 否定出力のピン番号       (3bit)
  CmnDffCell(const Cell* cell,
	     ymuint pos_array[]);

  /// @brief デストラクタ
  ~CmnDffCell();


public:

  /// @brief セルを得る．
  const Cell*
  cell() const;

  /// @brief クロック入力のタイプを返す．
  /// @retval 1 positive edge
  /// @retval 2 negative edge
  ymuint
  clock_sense() const;

  /// @brief クリア入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 High sensitive
  /// @retval 2 Low sensitive
  ymuint
  clear_sense() const;

  /// @brief プリセット入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 High sensitive
  /// @retval 2 Low sensitive
  ymuint
  preset_sense() const;

  /// @brief クリア入力を持つタイプの時に true を返す．
  bool
  has_clear() const;

  /// @brief プリセット入力を持つタイプの時に true を返す．
  bool
  has_preset() const;

  /// @brief データ入力のピン番号を返す．
  ymuint
  data_pos() const;

  /// @brief クロック入力のピン番号を返す．
  ymuint
  clock_pos() const;

  /// @brief クリア入力のピン番号を返す．
  /// @note クリア入力がない場合の値は不定
  ymuint
  clear_pos() const;

  /// @brief プリセット入力のピン番号を返す．
  /// @note プリセット入力がない場合の値は不定
  ymuint
  preset_pos() const;

  /// @brief 肯定出力のピン番号を返す．
  ymuint
  q_pos() const;

  /// @brief 否定出力のピン番号を返す．
  ymuint
  iq_pos() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル
  const Cell* mCell;

  // ピン情報，ピン番号パックしたもの
  ymuint32 mBits;

};

END_NAMESPACE_YM_NETWORKS


#endif // YM_NETWORKS_CMN_CMNDFFCELL_H