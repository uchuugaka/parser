#ifndef VLPTNODE_H
#define VLPTNODE_H

/// @file libym_verilog/tests/vlview/VlPtNode.h
/// @brief VlPtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPtNode.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <QAbstractItemModel>
#include <ym_verilog/verilog.h>
#include <ym_utils/FileRegion.h>


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlPtNode VlPtNode.h
/// @brief verilog のパース木を表示するためのモデル
//////////////////////////////////////////////////////////////////////
class VlPtNode
{
  friend class VlParseTreeModel;
  
public:
  
  /// @brief コンストラクタ
  VlPtNode();

  /// @brief デストラクタ
  virtual
  ~VlPtNode();
  
  
public:

  /// @brief 親のインデックスを返す．
  QModelIndex
  parent_index() const;
  
  /// @brief 子供の数を返す．
  int
  child_num() const;

  /// @brief 子供を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < row_num() )
  VlPtNode*
  child(int pos) const;

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role 
  virtual
  QVariant
  data(int column,
       int role) const = 0;
  
  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const = 0;
  
    
private:

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const = 0;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のノード
  QModelIndex mParentIndex;
  
  // 子供の配列を作っているかを示すフラグ
  mutable
  bool mExpanded;


protected:
  
  // 子供の配列
  mutable
  std::vector<VlPtNode*> mChildren;
  
};


//////////////////////////////////////////////////////////////////////
/// @class VlPtScalarNode VlPtNode_misc.h
/// @brief スカラー値を表すノード
//////////////////////////////////////////////////////////////////////
class VlPtScalarNode :
  public VlPtNode
{
public:

  /// @brief コンストラクタ
  VlPtScalarNode();

  /// @brief デストラクタ
  virtual
  ~VlPtScalarNode();


public:
    
  /// @brief 対象のファイル上での位置を返す．
  /// @note このクラスでは空の FileRegion を返す．
  virtual
  FileRegion
  loc() const;


private:

  /// @brief 子供の配列を作る．
  /// @note このクラスではなにもしない．
  virtual
  void
  expand() const;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 親のインデックスを返す．
inline
QModelIndex
VlPtNode::parent_index() const
{
  return mParentIndex;
}
  
// @brief 子供の数を返す．
inline
int
VlPtNode::child_num() const
{
  if ( !mExpanded ) {
    expand();
    mExpanded = true;
  }
  return mChildren.size();
}

// @brief 子供を返す．
// @param[in] pos 位置番号 ( 0 <= pos < row_num() )
inline
VlPtNode*
VlPtNode::child(int pos) const
{
  if ( !mExpanded ) {
    expand();
    mExpanded = true;
  }
  return mChildren[pos];
}

END_NAMESPACE_YM_VERILOG

#endif // VLPARSETREEMODEL_H
