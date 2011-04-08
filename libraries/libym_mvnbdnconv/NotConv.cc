
/// @file libym_mvnbdnconv/NotConv.cc
/// @brief NotConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "NotConv.h"
#include "ym_mvn/MvnNode.h"
#include "ym_mvnbdnconv/MvnBdnMap.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_MVNBDNCONV

// @brief コンストラクタ
NotConv::NotConv()
{
}

// @brief デストラクタ
NotConv::~NotConv()
{
}

// @brief MvnNode を BdNetwork に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdNetwork
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
NotConv::operator()(const MvnNode* node,
		    BdNetwork& bdnetwork,
		    MvnBdnMap& nodemap)
{
  if ( node->type() == MvnNode::kNot ) {
    const MvnInputPin* ipin = node->input(0);
    const MvnOutputPin* opin = ipin->src_pin();
    const MvnNode* src_node = opin->node();
    ymuint bw = opin->bit_width();
    assert_cond( node->output(0)->bit_width(), __FILE__, __LINE__);
    for (ymuint i = 0; i < bw; ++ i) {
      BdnNodeHandle handle = nodemap.get(src_node, i);
      nodemap.put(node, i, ~handle);
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_MVNBDNCONV
