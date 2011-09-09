
/// @file CiCell.cc
/// @brief CiCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiCell.h"
#include "ym_cell/CellGroup.h"
#include "CiPin.h"
#include "CiTiming.h"
#include "CiBus.h"
#include "CiBundle.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @param[in] alloc メモリアロケータ
// @param[in] logic_array 出力の論理式の配列
// @param[in] tristated_array トライステート条件の論理式の配列
CiCell::CiCell(ymuint id,
	       const ShString& name,
	       CellArea area,
	       ymuint ni,
	       ymuint no,
	       ymuint nio,
	       ymuint nb,
	       ymuint nc,
	       AllocBase& alloc,
	       const vector<LogExpr>& logic_array,
	       const vector<LogExpr>& tristate_array)
{
  mId = id;
  mName = name;
  mArea = area;

  mInputNum = ni;
  void* p = alloc.get_memory(sizeof(CiPin*) * ni);
  mInputArray = new (p) CiPin*[ni];

  mOutputNum = no;
  void* q = alloc.get_memory(sizeof(CiPin*) * no);
  mOutputArray = new (q) CiPin*[no];

  mInoutNum = nio;
  void* r = alloc.get_memory(sizeof(CiPin*) * nio);
  mInoutArray = new (r) CiPin*[nio];

  ymuint n = (ni + nio) * (no + nio) * 2;
  void* s = alloc.get_memory(sizeof(const CiTiming*) * n);
  mTimingArray = new (s) CiTiming*[n];
  for (ymuint i = 0; i < n; ++ i) {
    mTimingArray[i] = NULL;
  }

  // バス，バンドル関係は未完

  mBusNum = nb;

  mBundleNum = nc;

  ymuint no2 = no + nio;

  void* t = alloc.get_memory(sizeof(LogExpr) * no2);
  mLogicArray = new (t) LogExpr[no2];

  void* u = alloc.get_memory(sizeof(LogExpr) * no2);
  mTristateArray = new (u) LogExpr[no2];

  for (ymuint i = 0; i < no2; ++ i) {
    mLogicArray[i] = logic_array[i];
    mTristateArray[i] = tristate_array[i];
  }
}

// @brief デストラクタ
CiCell::~CiCell()
{
  // メモリの確保は別のクラスが担当している．
}

// @brief ID番号の取得
// @note ここで返される番号は CellLibrary::cell() の引数に対応する．
ymuint
CiCell::id() const
{
  return mId;
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

// @brief 入力ピン+入出力ピン数の取得
// @note input_num() + inout_num() に等しい．
ymuint
CiCell::input_num2() const
{
  return mInputNum + mInoutNum;
}

// @brief 入力ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < input_num2() )
const CellPin*
CiCell::input(ymuint pos) const
{
  return mInputArray[pos];
}

// @brief 出力ピン数の取得
ymuint
CiCell::output_num() const
{
  return mOutputNum;
}

// @brief 出力ピン+入出力ピン数の取得
// @note output_num() + inout_num() に等しい．
ymuint
CiCell::output_num2() const
{
  return mOutputNum + mInoutNum;
}

// @brief 出力ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < output_num2() )
const CellPin*
CiCell::output(ymuint pos) const
{
  return mOutputArray[pos];
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
  return mInoutArray[pos];
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

// @brief タイミング情報の取得
// @param[in] ipos 開始ピン番号
// @param[in] opos 終了ピン番号
// @param[in] timing_sense タイミング情報の摘要条件
// @return 条件に合致するタイミング情報を返す．
// @note なければ NULL を返す．
const CellTiming*
CiCell::timing(ymuint ipos,
	       ymuint opos,
	       tCellTimingSense sense) const
{
  ymuint base = opos * input_num2() + ipos;
  switch ( sense ) {
  case kCellPosiUnate:
    return mTimingArray[base + 0];

  case kCellNegaUnate:
    return mTimingArray[base + 1];

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  return NULL;
}

// @brief 属している CellGroup を返す．
const CellGroup*
CiCell::cell_group() const
{
  return mCellGroup;
}

// @brief 組み合わせ論理セルの時に true を返す．
bool
CiCell::is_logic() const
{
  return false;
}

// @brief FFセルの時に true を返す．
bool
CiCell::is_ff() const
{
  return false;
}

// @brief ラッチセルの時に true を返す．
bool
CiCell::is_latch() const
{
  return false;
}

// @brief 順序セル(非FF/非ラッチ)の場合に true を返す．
bool
CiCell::is_seq() const
{
  return false;
}

// @brief 論理セルの場合に出力の論理式を返す．
// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < output_num2() )
// @note 論理式中の変数番号は入力ピン番号に対応する．
LogExpr
CiCell::logic_expr(ymuint pin_id) const
{
  return mLogicArray[pin_id];
}

// @brief 出力の論理関数を返す．
// @param[in] pos 出力番号 ( 0 <= pos < output_num2() )
// @note FF/ラッチの場合は状態変数の変数(Q, XQ)が2つ入力に加わる．
TvFunc
CiCell::logic_function(ymuint pos) const
{
  return cell_group()->logic_function(pos);
}

// @brief トライステートセルの場合にトライステート条件式を返す．
// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < output_num2() )
// @note 論理式中の変数番号は入力ピン番号に対応する．
// @note 通常の論理セルの場合には定数0を返す．
LogExpr
CiCell::tristate_expr(ymuint pin_id) const
{
  return mTristateArray[pin_id];
}

// @brief 出力のトライステート条件関数を返す．
// @param[in] pos 出力番号 ( 0 <= pos < output_num2() )
// @note トライステートでない場合には定数0関数を返す．
TvFunc
CiCell::tristate_function(ymuint pos) const
{
  return cell_group()->tristate_function(pos);
}

// @brief FFセルの場合に次状態関数を表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiCell::next_state_expr() const
{
  return LogExpr::make_zero();
}

// @brief FFセルの場合に次状態関数を返す．
// @note それ以外の型の場合の返り値は不定
TvFunc
CiCell::next_state_function() const
{
  return cell_group()->next_state_function();
}

// @brief FFセルの場合にクロックのアクティブエッジを表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiCell::clock_expr() const
{
  return LogExpr::make_zero();
}

// @brief FFセルの場合にクロックのアクティブエッジを表す関数を返す．
// @note それ以外の型の場合の返り値は不定
TvFunc
CiCell::clock_function() const
{
  return cell_group()->clock_function();
}

// @brief FFセルの場合にスレーブクロックのアクティブエッジを表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiCell::clock2_expr() const
{
  return LogExpr::make_zero();
}

// @brief FFセルの場合にスレーブクロックのアクティブエッジを表す関数を返す．
// @note それ以外の型の場合の返り値は不定
TvFunc
CiCell::clock2_function() const
{
  return cell_group()->clock2_function();
}

// @brief ラッチセルの場合にデータ入力関数を表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiCell::data_in_expr() const
{
  return LogExpr::make_zero();
}

// @brief ラッチセルの場合にデータ入力関数を返す．
// @note それ以外の型の場合の返り値は不定
TvFunc
CiCell::data_in_function() const
{
  return cell_group()->data_in_function();
}

// @brief ラッチセルの場合にイネーブル条件を表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiCell::enable_expr() const
{
  return LogExpr::make_zero();
}

// @brief ラッチセルの場合にイネーブル条件を表す関数を返す．
// @note それ以外の型の場合の返り値は不定
TvFunc
CiCell::enable_function() const
{
  return cell_group()->enable_function();
}

// @brief ラッチセルの場合に2つめのイネーブル条件を表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiCell::enable2_expr() const
{
  return LogExpr::make_zero();
}

// @brief ラッチセルの場合に2つめのイネーブル条件を表す関数を返す．
// @note それ以外の型の場合の返り値は不定
TvFunc
CiCell::enable2_function() const
{
  return cell_group()->enable2_function();
}

// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
bool
CiCell::has_clear() const
{
  return false;
}

// @brief FFセル/ラッチセルの場合にクリア条件を表す論理式を返す．
// @note クリア端子がない場合の返り値は不定
LogExpr
CiCell::clear_expr() const
{
  return LogExpr::make_zero();
}

// @brief FFセル/ラッチセルの場合にクリア条件を表す関数を返す．
// @note クリア端子がない場合の返り値は不定
TvFunc
CiCell::clear_function() const
{
  return cell_group()->clear_function();
}

// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
bool
CiCell::has_preset() const
{
  return false;
}

// @brief FFセル/ラッチセルの場合にプリセット条件を表す論理式を返す．
// @note プリセット端子がない場合の返り値は不定
LogExpr
CiCell::preset_expr() const
{
  return LogExpr::make_zero();
}

// @brief FFセル/ラッチセルの場合にプリセット条件を表す関数を返す．
// @note プリセット端子がない場合の返り値は不定
TvFunc
CiCell::preset_function() const
{
  return cell_group()->preset_function();
}

// @brief タイミング情報を設定する．
// @param[in] opin_id 出力(入出力)ピン番号 ( *1 )
// @param[in] ipin_id 関連する入力(入出力)ピン番号 ( *2 )
// @param[in] sense タイミング条件
// @param[in] timing 設定するタイミング情報
// @note ( *1 ) opin_id で入出力ピンを表す時には入出力ピン番号
//  + cell->output_num() を使う．
// @note ( *2 ) ipin_id で入出力ピンを表す時には入出力ピン番号
//  + cell->input_num() を使う．
void
CiCell::set_timing(ymuint opin_id,
		   ymuint ipin_id,
		   tCellTimingSense sense,
		   CiTiming* timing)
{
  ymuint base = opin_id * input_num2() + ipin_id;
  switch ( sense ) {
  case kCellPosiUnate:
    mTimingArray[base + 0] = timing;
    break;

  case kCellNegaUnate:
    mTimingArray[base + 1] = timing;
    break;

  case kCellNonUnate:
    mTimingArray[base + 0] = timing;
    mTimingArray[base + 1] = timing;
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
}

// @brief 出力ピンの機能を設定する．
// @param[in] opin_id 出力(入出力)ピン番号 ( *1 )
// @param[in] function 機能を表す論理式
// @note ( *1 ) opin_id で入出力ピンを表す時には入出力ピン番号
//  + cell->output_num() を使う．
void
CiCell::set_logic_expr(ymuint opin_id,
		       const LogExpr& function)
{
  assert_cond( opin_id < output_num2(), __FILE__, __LINE__);
  mLogicArray[opin_id] = function;
}

// @brief 出力ピンの three_state 条件を設定する．
// @param[in] opin_id 出力(入出力)ピン番号 ( *1 )
// @param[in] expr three_state 条件を表す論理式
// @note ( *1 ) opin_id で入出力ピンを表す時には入出力ピン番号
//  + cell->output_num() を使う．
void
CiCell::set_tristate_expr(ymuint opin_id,
			  const LogExpr& expr)
{
  assert_not_reached(__FILE__, __LINE__);
}

END_NAMESPACE_YM_CELL
