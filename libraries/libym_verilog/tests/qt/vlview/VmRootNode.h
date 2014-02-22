#ifndef VMROOTNODE_H
#define VMROOTNODE_H

/// @file VmRootNode.h
/// @brief VmRootNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"
#include "verilog/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VmRootNode VmRootNode.h
/// @brief 根のノード
//////////////////////////////////////////////////////////////////////
class VmRootNode :
  public VmNode1
{
public:

  /// @brief コンストラクタ
  /// @param[in] vl_mgr VlMgr
  VmRootNode(const VlMgr& vl_mgr);

  /// @brief デストラクタ
  virtual
  ~VmRootNode();


public:

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role
  virtual
  QVariant
  data(int column,
       int role) const;

  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;

};

END_NAMESPACE_YM_VERILOG

#endif // VMROOTNODE_H
