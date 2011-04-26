
/// @file libym_cell/dotlib/PtMgr.cc
/// @brief PtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtMgr.h"
#include "PtNode.h"
#include "PtNodeImpl.h"
#include "PtLibrary.h"
#include "PtCell.h"
#include "PtBus.h"
#include "PtBundle.h"
#include "PtPin.h"
#include "PtValue.h"
#include "PtValueImpl.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtMgr::PtMgr() :
  mAlloc(4096)
{
  mSimpleNum = 0;
  mComplexNum = 0;
  mGroupNum = 0;
  mCellNum = 0;
  mBusNum = 0;
  mBundleNum = 0;
  mPinNum = 0;
  mIntNum = 0;
  mFloatNum = 0;
  mStrNum = 0;
  mOprNum = 0;
}

// @brief デストラクタ
PtMgr::~PtMgr()
{
}

// @brief 初期化する．
// @note 以前，生成されたオブジェクトは(デストラクタ抜きで)破壊される．
void
PtMgr::init()
{
  mAlloc.destroy();
}

// @brief simple attribute を表す PtNode を生成する．
// @param[in] value 値
PtNode*
PtMgr::new_ptsimple(const PtValue* value)
{
  ++ mSimpleNum;
  void* p = mAlloc.get_memory(sizeof(PtSimpleNode));
  return new (p) PtSimpleNode(value);
}

// @brief complex attribute を表す PtNode を生成する．
// @param[in] value_list 値のリスト
PtNode*
PtMgr::new_ptcomplex(const vector<const PtValue*>& value_list)
{
  ++ mComplexNum;
  void* p = mAlloc.get_memory(sizeof(PtComplexNode));
  return new (p) PtComplexNode(value_list);
}

// @brief group statement を表す PtNode を生成する．
// @param[in] value_list 値のリスト
PtNode*
PtMgr::new_ptgroup(const vector<const PtValue*>& value_list)
{
  ++ mGroupNum;
  void* p = mAlloc.get_memory(sizeof(PtGroupNode));
  return new (p) PtGroupNode(value_list);
}

// @brief PtLibrary を生成する．
// @param[in] name ライブラリ名
PtLibrary*
PtMgr::new_ptlibrary(const PtValue* name)
{
  void* p = mAlloc.get_memory(sizeof(PtLibrary));
  return new (p) PtLibrary(name);
}

// @brief PtCell を生成する．
// @param[in] name セル名
PtCell*
PtMgr::new_ptcell(const PtValue* name)
{
  ++ mCellNum;
  void* p = mAlloc.get_memory(sizeof(PtCell));
  return new (p) PtCell(name);
}

// @brief PtBus を生成する．
// @param[in] name バス名
PtBus*
PtMgr::new_ptbus(const PtValue* name)
{
  ++ mBusNum;
  void* p = mAlloc.get_memory(sizeof(PtBus));
  return new (p) PtBus(name);
}

// @brief PtBundle を生成する．
// @param[in] name バンドル名
PtBundle*
PtMgr::new_ptbundle(const PtValue* name)
{
  ++ mBundleNum;
  void* p = mAlloc.get_memory(sizeof(PtBundle));
  return new (p) PtBundle(name);
}

// @brief PtPin を生成する．
// @param[in] name ピン名
PtPin*
PtMgr::new_ptpin(const PtValue* name)
{
  ++ mPinNum;
  void* p = mAlloc.get_memory(sizeof(PtPin));
  return new (p) PtPin(name);
}

// @brief 整数値を表す PtValue を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtValue*
PtMgr::new_int(int value,
	       const FileRegion& loc)
{
  ++ mIntNum;
  void* p = mAlloc.get_memory(sizeof(PtInt));
  return new (p) PtInt(value, loc);
}

// @brief 実数値を表す PtValue を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtValue*
PtMgr::new_float(double value,
		 const FileRegion& loc)
{
  ++ mFloatNum;
  void* p = mAlloc.get_memory(sizeof(PtFloat));
  return new (p) PtFloat(value, loc);
}

// @brief 定数シンボルを表す PtValue を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtValue*
PtMgr::new_string(ShString value,
		  const FileRegion& loc)
{
  ++ mStrNum;
  void* p = mAlloc.get_memory(sizeof(PtString));
  return new (p) PtString(value, loc);
}

// @brief 演算子を表す PtValue を生成する．
// @param[in] type 型
// @param[in] opr1, opr2 オペランド
PtValue*
PtMgr::new_opr(tTokenType type,
		 PtValue* opr1,
		 PtValue* opr2)
{
  ++ mOprNum;
  void* p = mAlloc.get_memory(sizeof(PtOpr));
  return new (p) PtOpr(type, opr1, opr2);
}

void
PtMgr::show_stats()
{
  cout << "PtSimple:     " << setw(7) << mSimpleNum
       << " x " << setw(3) << sizeof(PtSimpleNode)
       << " = " << setw(10) << mSimpleNum * sizeof(PtSimpleNode) << endl
       << "PtComplex:    " << setw(7) << mComplexNum
       << " x " << setw(3) << sizeof(PtComplexNode)
       << " = " << setw(10) << mComplexNum * sizeof(PtComplexNode) << endl
       << "PtGroup:      " << setw(7) << mGroupNum
       << " x " << setw(3) << sizeof(PtGroupNode)
       << " = " << setw(10) << mGroupNum * sizeof(PtGroupNode) << endl
       << "PtCell:       " << setw(7) << mCellNum
       << " x " << setw(3) << sizeof(PtCell)
       << " = " << setw(10) << mCellNum * sizeof(PtCell) << endl
       << "PtPin:        " << setw(7) << mPinNum
       << " x " << setw(3) << sizeof(PtPin)
       << " = " << setw(10) << mPinNum * sizeof(PtPin) << endl
       << "PtInt:        " << setw(7) << mIntNum
       << " x " << setw(3) << sizeof(PtInt)
       << " = " << setw(10) << mIntNum * sizeof(PtInt) << endl
       << "PtFloat:      " << setw(7) << mFloatNum
       << " x " << setw(3) << sizeof(PtFloat)
       << " = " << setw(10) << mFloatNum * sizeof(PtFloat) << endl
       << "PtString:     " << setw(7) << mStrNum
       << " x " << setw(3) << sizeof(PtString)
       << " = " << setw(10) << mStrNum * sizeof(PtString) << endl
       << "PtOpr:        " << setw(7) << mOprNum
       << " x " << setw(3) << sizeof(PtOpr)
       << " = " << setw(10) << mOprNum * sizeof(PtOpr) << endl
       << "Total memory:               = "
       << setw(10) << mAlloc.used_size() << endl
       << endl
       << "Allocated memory:           = "
       << setw(10) << mAlloc.allocated_size() << endl
       << "ShString:                   = "
       << setw(10) << ShString::allocated_size() << endl;
}

END_NAMESPACE_YM_CELL_DOTLIB