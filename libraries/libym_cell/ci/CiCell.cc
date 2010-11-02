
/// @file libym_cell/CiCell.cc
/// @brief CiCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CiCell.h"
#include "CiPin.h"
#include "CiTiming.h"
#include "CiBus.h"
#include "CiBundle.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiCell::CiCell() :
  mInputNum(0),
  mInputArray(NULL),
  mOutputNum(0),
  mOutputArray(NULL),
  mInoutNum(0),
  mInoutArray(NULL),
  mPinNum(0),
  mPinArray(NULL),
  mBusNum(0),
  mBusArray(NULL),
  mBundleNum(0),
  mBundleArray(NULL)
{
}

// @brief デストラクタ
CiCell::~CiCell()
{
  // メモリの確保は別のクラスが担当している．
}

// @brief 名前の取得
string
CiCell::name() const
{
  return mName;
}

// @brief 面積の取得
CellArea
CiCell::area() const
{
  return mArea;
}

// @brief 入力ピン数の取得
ymuint
CiCell::input_num() const
{
  return mInputNum;
}

// @brief 入力ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const CellPin*
CiCell::input(ymuint pos) const
{
  return &mInputArray[pos];
}

// @brief 出力ピン数の取得
ymuint
CiCell::output_num() const
{
  return mOutputNum;
}

// @brief 出力ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < output_num() )
const CellPin*
CiCell::output(ymuint pos) const
{
  return &mOutputArray[pos];
}

// @brief 入出力ピン数の取得
ymuint
CiCell::inout_num() const
{
  return mInoutNum;
}

// @brief 入出力ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < inout_num() )
const CellPin*
CiCell::inout(ymuint pos) const
{
  return &mInoutArray[pos];
}

// @brief ピン数の取得
ymuint
CiCell::pin_num() const
{
  return mPinNum;
}

// @brief ピンの取得
// @param[in] pin_id ピン番号 ( 0 <= pin_id < pin_num()
const CellPin*
CiCell::pin(ymuint pin_id) const
{
  return mPinArray[pin_id];
}

// @brief 名前からピンの取得
// @param[in] name ピン名
// @return name という名前をピンを返す．
// @note なければ NULL を返す．
const CellPin*
CiCell::pin(const string& name) const
{
  // 未完
  return NULL;
}

// @brief バス数の取得
ymuint
CiCell::bus_num() const
{
  return mBusNum;
}

// @brief バスの取得
// @param[in] pos 位置番号 ( 0 <= pos < bus_num() )
const CellBus*
CiCell::bus(ymuint pos) const
{
  return &mBusArray[pos];
}

// @brief 名前からバスの取得
// @param[in] name バス名
// @return name という名前のバスを返す．
// @note なければ NULL を返す．
const CellBus*
CiCell::bus(const string& name) const
{
  // 未完
  return NULL;
}

// @brief バンドル数の取得
ymuint
CiCell::bundle_num() const
{
  return mBundleNum;
}

// @brief バンドルの取得
// @param[in] pos 位置番号 ( 0 <= pos < bundle_num() )
const CellBundle*
CiCell::bundle(ymuint pos) const
{
  return &mBundleArray[pos];
}

// @brief 名前からバンドルの取得
const CellBundle*
CiCell::bundle(const string& name) const
{
  // 未完
  return NULL;
}


END_NAMESPACE_YM_CELL

