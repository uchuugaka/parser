
/// @file libym_verilog/tests/vlview/VmDeclNode.cc
/// @brief VlDeclNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmDeclNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmDeclNode.h"
#include "VmMiscNode.h"
#include "ym_verilog/vl/VlModule.h"
#include "ym_verilog/vl/VlUdp.h"
#include "ym_verilog/vl/VlTaskFunc.h"
#include "ym_verilog/vl/VlIODecl.h"
#include "ym_verilog/vl/VlDecl.h"
#include "ym_verilog/vl/VlRange.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief IODeclListNode を追加する．
// @param[in] label ラベル
// @param[in] iodecl_list IO宣言のリスト
void
VmNode::add_child(const QString& label,
		  const vector<const VlIODecl*>& iodecl_list) const
{
  add_child( new VmIODeclListNode(vl_mgr(), label, iodecl_list) );
}


//////////////////////////////////////////////////////////////////////
// クラス VmIODeclListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] label ラベル
// @param[in] io_array IO宣言の配列
VmIODeclListNode::VmIODeclListNode(const VlMgr& vl_mgr,
				   const QString& label,
				   const vector<const VlIODecl*>& io_array) :
  VmNode(vl_mgr),
  mLabel(label),
  mIOArray(io_array)
{
}

// @brief デストラクタ
VmIODeclListNode::~VmIODeclListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmIODeclListNode::data(int column,
		       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmIODeclListNode::loc() const
{
  if ( mIOArray.empty() ) {
    // 多分ないと思うけど
    return FileRegion();
  }
  else {
    FileRegion first = mIOArray.front()->file_region();
    FileRegion last = mIOArray.back()->file_region();
    return FileRegion(first, last);
  }
}

// @brief 子供の配列を作る．
void
VmIODeclListNode::expand() const
{
  for (vector<const VlIODecl*>::const_iterator p = mIOArray.begin();
       p != mIOArray.end(); ++ p) {
    const VlIODecl* io = *p;
    add_child( new VmIODeclNode(vl_mgr(), mLabel, io) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmIODeclNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] label ラベル
// @param[in] io IO宣言要素
VmIODeclNode::VmIODeclNode(const VlMgr& vl_mgr,
			   const QString& label,
			   const VlIODecl* io) :
  VmNode(vl_mgr),
  mLabel(label),
  mIODecl(io)
{
}

// @brief デストラクタ
VmIODeclNode::~VmIODeclNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmIODeclNode::data(int column,
		   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      return mIODecl->name();
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmIODeclNode::loc() const
{
  return mIODecl->file_region();
}

// @brief 子供の配列を作る．
void
VmIODeclNode::expand() const
{
  add_child( new VmDirNode(vl_mgr(), mIODecl->direction()) );
  add_child("vpiSigned", mIODecl->is_signed());
#if 0
  add_child("vpiScalar", mIODecl->is_scalar());
  add_child("vpiVector", mIODecl->is_vector());
#endif
  add_child("vpiSize", mIODecl->bit_size());

  const VlModule* module = mIODecl->module();
  if ( module ) {
    add_child("vpiModule", module->full_name());
  }
  const VlUdpDefn* udp = mIODecl->udp_defn();
  if ( udp ) {
    add_child("vpiUdpDefn", udp->def_name());
  }
  const VlTask* task = mIODecl->task();
  if ( task ) {
    add_child("vpiTask", task->full_name());
  }
  const VlFunction* func = mIODecl->function();
  if ( func ) {
    add_child("vpiFunction", func->full_name());
  }
  add_child("vpiLeftRange", mIODecl->left_range());
  add_child("vpiRightRange", mIODecl->right_range());
}


//////////////////////////////////////////////////////////////////////
// クラス VmDeclListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] label ラベル
// @param[in] decl_array 宣言要素の配列
VmDeclListNode::VmDeclListNode(const VlMgr& vl_mgr,
			       const QString& label,
			       const vector<const VlDecl*>& decl_array) :
  VmNode(vl_mgr),
  mLabel(label),
  mDeclArray(decl_array)
{
}

// @brief デストラクタ
VmDeclListNode::~VmDeclListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmDeclListNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmDeclListNode::loc() const
{
  if ( mDeclArray.empty() ) {
    return FileRegion();
  }
  else {
    FileRegion first = mDeclArray.front()->file_region();
    FileRegion last = mDeclArray.back()->file_region();
    return FileRegion(first, last);
  }
}

// @brief 子供の配列を作る．
void
VmDeclListNode::expand() const
{
  for (vector<const VlDecl*>::const_iterator p = mDeclArray.begin();
       p != mDeclArray.end(); ++ p) {
    add_child(mLabel, *p);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmDeclNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] label ラベル
// @param[in] declitem 宣言要素
VmDeclNode::VmDeclNode(const VlMgr& vl_mgr,
		       const QString& label,
		       const VlDecl* decl) :
  VmNode(vl_mgr),
  mLabel(label),
  mDecl(decl)
{
}

// @brief デストラクタ
VmDeclNode::~VmDeclNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmDeclNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      return mDecl->name();
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmDeclNode::loc() const
{
  return mDecl->file_region();
}

// @brief 子供の配列を作る．
void
VmDeclNode::expand() const
{
  const char* nm = NULL;
  switch ( mDecl->type() ) {
  case kVpiNet:             nm = "Net"; break;
  case kVpiNetArray:        nm = "NetArray"; break;
  case kVpiReg:             nm = "Reg"; break;
  case kVpiRegArray:        nm = "RegArray"; break;
  case kVpiIntegerVar:      nm = "IntegerVar"; break;
  case kVpiRealVar:         nm = "RealVar"; break;
  case kVpiTimeVar:         nm = "TimeVar"; break;
  case kVpiVarSelect:       nm = "VarSelect"; break;
  case kVpiNamedEvent:      nm = "NamedEvent"; break;
  case kVpiNamedEventArray: nm = "NamedEventArray"; break;
  case kVpiParameter:       nm = "Parameter"; break;
  case kVpiSpecParam:       nm = "SpecParam"; break;
  default: assert_not_reached( __FILE__, __LINE__ );
  }
  add_child("vpiType", nm);
  add_child("vpiFullName", mDecl->full_name());
#if 0
  add_child("vpiExpanded", mDecl->is_expanded());
  add_child("vpiImplicitDecl", mDecl->is_implicit_decl());
  add_child("vpiNetDeclAssign", mDecl->has_net_decl_assign());
  add_child("vpiScalar", mDecl->is_scalar());
  add_child("vpiVector", mDecl->is_vector());
  add_child("vpiExplicitScalar", mDecl->is_explicit_scalar());
  add_child("vpiExplicitVector", mDecl->is_explicit_vector());
  add_child("vpiSigned", mDecl->is_signed());
#endif
  add_child("vpiSize", mDecl->bit_size());
  add_child("vpiModule", mDecl->parent_module()->full_name());
  add_child("vpiScope", mDecl->parent()->full_name());
#if 0
  add_child("vpiArray", mDecl->is_array());
  add_child("vpiMultiArray", mDecl->is_multi_array());
#endif
  // 親の配列

  ymuint n = mDecl->dimension();
  for (ymuint i = 0; i < n; ++ i) {
    add_child( new VmRangeNode(vl_mgr(), "vpiRange", mDecl->range(i)) );
  }

  if ( mDecl->type() == kVpiNet ) {
    add_child( new VmStrengthValNode(vl_mgr(), "vpiStrength0", mDecl->drive0()) );
    add_child( new VmStrengthValNode(vl_mgr(), "vpiStrength1", mDecl->drive1()) );
    add_child( new VmStrengthValNode(vl_mgr(), "vpiChargeStrength", mDecl->charge()) );
#if 0
    add_child( new VmDelayNode("vpiDelay", mDecl->delay()) );
#endif
  }

#if 0
  add_child("vpiIndex", mDecl->index());

  if ( mDecl->port_inst() ) {
    add_child("vpiPortInst", mDecl->port_inst());
  }
  if ( mDecl->ports() ) {
    add_child("vpiPorts", mDecl->ports());
  }
#endif

  if ( mDecl->left_range() ) {
    add_child("vpiLeftRange", mDecl->left_range());
    add_child("vpiRightRange", mDecl->right_range());
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmRangeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] label ラベル
// @param[in] range 範囲
VmRangeNode::VmRangeNode(const VlMgr& vl_mgr,
			 const QString& label,
			 const VlRange* range) :
  VmNode(vl_mgr),
  mLabel(label),
  mRange(range)
{
}

// @brief デストラクタ
VmRangeNode::~VmRangeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmRangeNode::data(int column,
		  int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmRangeNode::loc() const
{
  return mRange->file_region();
}

// @brief 子供の配列を作る．
void
VmRangeNode::expand() const
{
  add_child("vpiLeftRange", mRange->left_range());
  add_child("vpiRightRange", mRange->right_range());
}

END_NAMESPACE_YM_VERILOG
