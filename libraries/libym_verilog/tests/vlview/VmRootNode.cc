
/// @file libym_verilog/tests/vlview/VmRootNode.cc
/// @brief VlPtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPtNode_root.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmRootNode.h"
#include "VmUdpNode.h"
#include "VmModuleNode.h"
#include "ym_verilog/VlMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VmRootNode
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
VmRootNode::VmRootNode(const VlMgr& vl_mgr) :
  mVlMgr(vl_mgr)
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
  if ( !mVlMgr.udp_list().empty() ) {
    add_child( new VmUdpListNode(mVlMgr) );
  }
  if ( !mVlMgr.topmodule_list().empty() ) {
    add_child( new VmModuleListNode(mVlMgr) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmUdpListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
VmUdpListNode::VmUdpListNode(const VlMgr& vl_mgr) :
  mVlMgr(vl_mgr)
{
}

// @brief デストラクタ
VmUdpListNode::~VmUdpListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmUdpListNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "UDP list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmUdpListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmUdpListNode::expand() const
{
  const list<const VlUdpDefn*>& udp_list = mVlMgr.udp_list();
  for (list<const VlUdpDefn*>::const_iterator p = udp_list.begin();
       p != udp_list.end(); ++ p) {
    const VlUdpDefn* udp = *p;
    add_child( new VmUdpNode(udp) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmModuleListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
VmModuleListNode::VmModuleListNode(const VlMgr& vl_mgr) :
  mVlMgr(vl_mgr)
{
}

// @brief デストラクタ
VmModuleListNode::~VmModuleListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmModuleListNode::data(int column,
		       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Module list";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmModuleListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmModuleListNode::expand() const
{
  const list<const VlModule*>& module_list = mVlMgr.topmodule_list();
  for (list<const VlModule*>::const_iterator p = module_list.begin();
       p != module_list.end(); ++ p) {
    const VlModule* module = *p;
    add_child( new VmModuleNode(module) );
  }
}

END_NAMESPACE_YM_VERILOG