
/// @file libym_bdn/mvnconv/LtConv.cc
/// @brief LtConv の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LtConv.h"
#include "ym_mvn/MvNode.h"
#include "ym_bdn/MvnBdnMap.h"
#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_BDN

// @brief コンストラクタ
LtConv::LtConv()
{
}

// @brief デストラクタ
LtConv::~LtConv()
{
}

// @brief MvNode を BdNetwork に変換する．
// @param[in] node ノード
// @param[in] bdnetwork 変換結果の BdNetwork
// @param[in] nodemap ノードの対応関係を表すマップ
// @retval true このクラスで変換処理を行った．
// @retval false このクラスでは変換処理を行わなかった．
bool
LtConv::operator()(const MvNode* node,
		   BdNetwork& bdnetwork,
		   MvnBdnMap& nodemap)
{
  if ( node->type() == MvNode::kLt ) {
    const MvInputPin* ipin0 = node->input(0);
    const MvOutputPin* src_pin0 = ipin0->src_pin();
    const MvNode* src_node0 = src_pin0->node();

    const MvInputPin* ipin1 = node->input(1);
    const MvOutputPin* src_pin1 = ipin1->src_pin();
    const MvNode* src_node1 = src_pin1->node();

    ymuint bw = src_pin0->bit_width();
    assert_cond( src_pin1->bit_width() == bw, __FILE__, __LINE__);
    BdnNodeHandle lt(BdnNodeHandle::make_zero());
    for (ymuint i = 0; i < bw; ++ i) {
      BdnNodeHandle a = nodemap.get(src_node0, i);
      BdnNodeHandle b = nodemap.get(src_node1, i);
      BdnNodeHandle p = bdnetwork.new_xnor(a, b);
      BdnNodeHandle g = bdnetwork.new_and(~a, b);
      BdnNodeHandle tmp = bdnetwork.new_and(lt, p);
      lt = bdnetwork.new_or(g, tmp);
    }
    nodemap.put(node, 0, lt);

    return true;
  }
  return false;
}

END_NAMESPACE_YM_BDN
