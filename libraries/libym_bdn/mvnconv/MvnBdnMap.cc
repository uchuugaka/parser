
/// @file libym_mvnbdnconv/MvnBdnMap.cc
/// @brief MvnBdnMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/MvnBdnMap.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNode.h"
#include "ym_bdn/BdnNodeHandle.h"
#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvNode.h"


BEGIN_NAMESPACE_YM_BDN

// @brief コンストラクタ
MvnBdnMap::MvnBdnMap(ymuint n) :
  mArray(n)
{
}

// @brief デストラクタ
MvnBdnMap::~MvnBdnMap()
{
}

// @brief 登録する．(1ビット版)
// @param[in] mvnode MvNode
// @param[in] nodehandle BdnNodeHandle
void
MvnBdnMap::put(const MvNode* mvnode,
	       BdnNodeHandle nodehandle)
{
  put(mvnode, 0, nodehandle);
}

// @brief 登録する．(ベクタ版)
// @param[in] mvnode MvNode
// @param[in] index ビット位置
// @param[in] nodehandle BdnNodeHandle
void
MvnBdnMap::put(const MvNode* mvnode,
	       ymuint index,
	       BdnNodeHandle nodehandle)
{
  assert_cond( mArray.size() > mvnode->id(), __FILE__, __LINE__);
  vector<BdnNodeHandle>& array = mArray[mvnode->id()];
  if ( array.size() != mvnode->output(0)->bit_width() ) {
    array.resize(mvnode->output(0)->bit_width());
  }
  array[index] = nodehandle;
}

// @brief 探す．(1ビット版)
// @param[in] mvnode MvNode
BdnNodeHandle
MvnBdnMap::get(const MvNode* mvnode) const
{
  return get(mvnode, 0);
}

// @brief 探す．(ベクタ版)
// @param[in] mvnode MvNode
// @param[in] index ビット位置
BdnNodeHandle
MvnBdnMap::get(const MvNode* mvnode,
	       ymuint index) const
{
  assert_cond( mArray.size() > mvnode->id(), __FILE__, __LINE__);
  const vector<BdnNodeHandle>& array = mArray[mvnode->id()];
  if ( array.empty() ) {
    return BdnNodeHandle(NULL, false);
  }
  assert_cond( array.size() == mvnode->output(0)->bit_width(),
	       __FILE__, __LINE__);
  return array[index];
}


BEGIN_NONAMESPACE

void
dump_nodehandle(ostream& s,
		BdnNodeHandle h)
{
  if ( h.is_zero() ) {
    s << "0";
  }
  else if ( h.is_one() ) {
    s << "1";
  }
  else {
    if ( h.inv() ) {
      s << "~";
    }
    s << h.node()->id_str();
  }
}

END_NONAMESPACE

void
dump_mvnode_map(ostream& s,
		const MvMgr& mvmgr,
		const MvnBdnMap& mvnode_map)
{
  ymuint n = mvmgr.max_node_id();
  for (ymuint i = 0; i < n; ++ i) {
    const MvNode* node = mvmgr.node(i);
    if ( node == NULL ) continue;
    const MvOutputPin* opin = node->output(0);
    ymuint bw = opin->bit_width();
    if ( bw == 1 ) {
      BdnNodeHandle nodehandle = mvnode_map.get(node);
      s << "// node" << node->id() << " : ";
      dump_nodehandle(s, nodehandle);
      s << endl;
    }
    else {
      for (ymuint i = 0; i < bw; ++ i) {
	BdnNodeHandle nodehandle = mvnode_map.get(node, i);
	s << "// node" << node->id() << " [" << i << "] : ";
	//dump_bdnhandle(s, nodehandle);
	s << endl;
      }
    }
  }
}

END_NAMESPACE_YM_BDN
