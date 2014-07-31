
/// @file VmRootNode.cc
/// @brief VmRootNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VmRootNode.h"
#include "YmVerilog/VlMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VmRootNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
VmRootNode::VmRootNode(const VlMgr& vl_mgr) :
  VmNode1(vl_mgr)
{
}

// @brief デストラクタ
VmRootNode::~VmRootNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role
QVariant
VmRootNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Root";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmRootNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmRootNode::expand() const
{
  if ( !vl_mgr().udp_list().empty() ) {
    add_udplist();
  }
  if ( !vl_mgr().topmodule_list().empty() ) {
    add_modulelist();
  }
}

END_NAMESPACE_YM_VERILOG
