#ifndef YM_LUTMAP_LNGRAPH_H
#define YM_LUTMAP_LNGRAPH_H

/// @file ym_lutmap/LnGraph.h 
/// @brief LutNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutNetwork.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/lutmap_nsdef.h"

#include "ym_utils/Alloc.h"
#include "ym_utils/DlList.h"
#include "ym_utils/ItvlMgr.h"
#include "ym_bnet/BNetwork.h"


BEGIN_NAMESPACE_YM_LUTMAP

void
lut2bnet(const LnGraph& src_network,
	 BNetwork& dst_network);


//////////////////////////////////////////////////////////////////////
/// @class LnEdge LnGraph.h "LnGraph.h"
/// @brief LUTネットワークの枝を表すクラス
///
/// - 入力側のノード
/// - 出力側のノード
/// - 出力側のノードの何番目の入力か
/// の情報を持つ．
/// @sa LnNode LnGraph
//////////////////////////////////////////////////////////////////////
class LnEdge :
  public DlElem
{
  friend class LnNode;
  friend class LnGraph;
  friend class LnEnumCut;
  
public:

  //////////////////////////////////////////////////////////////////////
  /// @name 関数処理に関する情報にアクセスする関数
  /// @{
  
  /// @brief 入力側のノードを得る．(const 版)
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  const LnNode*
  from() const;
  
  /// @brief 出力側のノードを得る．(const 版)
  /// @return 出力側のノードを返す．
  const LnNode*
  to() const;
  
  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  LnNode*
  from();
  
  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  LnNode*
  to();
  
  /// @brief 出力側のノードの何番目の入力かを示す．
  ymuint
  pos() const;

  /// @brief 出力ノードに接続している時 true を返す．
  bool
  is_poedge() const;

  /// @}
  //////////////////////////////////////////////////////////////////////
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  // これらは friend class からしか呼ぶことができない．
  //////////////////////////////////////////////////////////////////////
  
  /// @brief コンストラクタ
  LnEdge();
  
  /// @brief デストラクタ
  ~LnEdge();

  /// @brief from ノードをセットする．
  void
  set_from(LnNode* from);

  /// @brief to ノードをセットする．
  void
  set_to(LnNode* to,
	 ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ変数
  //////////////////////////////////////////////////////////////////////
  
  // 入力側のノード
  LnNode* mFrom;

  // 出力側のノード
  LnNode* mTo;
  
  // 入力位置
  ymuint32 mIpos;
  
};


//////////////////////////////////////////////////////////////////////
/// @class LnNode LnGraph.h "LnGraph.h"
/// @brief LUTネットワークを構成するLUTノードを表すクラス
///
/// ノードの種類は
/// - 入力ノード
/// - 出力ノード
/// - LUTノード
/// - DFFノード
/// の 4種類がある．
/// @sa LnEdge LnGraph
//////////////////////////////////////////////////////////////////////
class LnNode :
  public DlElem
{
  friend class LnGraph;
  
public:
  /// @brief ノードの型
  enum tType {
    /// @brief 入力ノード
    kINPUT  = 0,
    /// @brief 出力ノード
    kOUTPUT = 1,
    /// @brief LUTノード
    kLUT = 2,
    /// @brief DFFノード
    kDFF = 3
  };


private:

  /// @brief コンストラクタ
  LnNode();
  
  /// @brief デストラクタ
  ~LnNode();

  
public:

  //////////////////////////////////////////////////////////////////////
  /// @name 構造に関する情報を取り出す関数
  /// @{
  
  /// @brief ID 番号の取得
  /// @return ID 番号を返す．
  /// @note ID 番号はノードの生成時に LnGraph により自動的に割り振られる．
  /// @sa LnGraph
  ymuint
  id() const;

  /// @brief ID を表す文字列の取得
  /// @note デバッグ時にしか意味を持たない
  string
  id_str() const;

  /// @brief 名前を得る．
  /// @note 名前がない場合もある．
  const string&
  name() const;

  /// @brief タイプを得る．
  tType
  type() const;
  
  /// @brief 入力ノードの時に true を返す．
  bool
  is_input() const;
  
  /// @brief 出力ノードの時に true を返す．
  bool
  is_output() const;
  
  /// @brief LUTノードの時に true を返す．
  bool
  is_lut() const;
  
  /// @brief DFFノードの時に true を返す．
  bool
  is_dff() const;

  /// @brief サブID (入力／出力番号)を得る．
  /// @note 入力ノード/出力ノードの場合のみ意味を持つ．
  ymuint
  subid() const;

  /// @brief 入力数を得る．
  ymuint
  ni() const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  const LnNode*
  fanin(ymuint pos) const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  LnNode*
  fanin(ymuint pos);
  
  /// @brief ファンインの枝を得る．(const 版)
  /// @param[in] pos 入力番号
  /// @return pos 番目の入力の枝
  /// @note 該当するファンインの枝がなければ NULL を返す．
  const LnEdge*
  fanin_edge(ymuint pos) const;
  
  /// @brief ファンインの枝を得る．
  /// @param[in] pos 入力番号
  /// @return pos 番目の入力の枝
  /// @note 該当するファンインの枝がなければ NULL を返す．
  LnEdge*
  fanin_edge(ymuint pos);
  
  /// @brief ファンアウト数を得る．
  ymuint
  n_fanout() const;
  
  /// @brief ファンアウトリストを得る．
  const LnEdgeList&
  fanout_list() const;

  /// @brief 出力ノードにファンアウトしているとき true を返す．
  bool
  pomark() const;

  /// @brief LUTノードの場合に真理値ベクタを得る．
  /// @param[out] tv 真理値を格納するベクタ
  void
  tv(vector<int>& tv) const;

  /// @brief レベルを得る．
  ymuint
  level() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief タイプを入力に設定する．
  void
  set_input(ymuint subid = 0);
  
  /// @brief タイプを出力に設定する．
  void
  set_output(ymuint subid = 0);
  
  /// @brief タイプをLUTに設定する．
  void
  set_lut();
  
  /// @brief タイプをDFFに設定する．
  void
  set_dff();
  
  /// @brief ファンアウトに出力が含まれているか調べ pomark をセットする．
  void
  scan_po();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ID 番号
  ymuint32 mId;

  // 名前
  string mName;

  // タイプ (+ 入力／出力番号/PoMask)
  ymuint32 mFlags;

  // 入力数
  ymuint32 mNi;
  
  // ファンインの枝(そのもの)の配列
  LnEdge* mFanins;
  
  // ファンアウトの枝のリスト
  LnEdgeList mFanoutList;
  
  // 真理値ベクタ
  vector<int> mTv;
  
  // レベル
  ymuint32 mLevel;

  
private:
  //////////////////////////////////////////////////////////////////////
  // mFlags 用の定数
  //////////////////////////////////////////////////////////////////////

  static
  const int kTypeShift  = 0;
  static
  const int kPoShift    = 3;
  static
  const int kSubidShift = 4;

  static
  const ymuint32 kTypeMask  = 3U << kTypeShift;
  static
  const ymuint32 kPoMask    = 1U << kPoShift;

};


//////////////////////////////////////////////////////////////////////
/// @class LnGraph LnGraph.h "LnGraph.h"
/// @brief LUTネットワークを表すクラス
///
/// 入力ノード，出力ノード，LUT ノード，DFF ノードを分けてリストで持つ
/// ( @sa input_list(), output_list(), lnode_list(), dff_list() )．
/// また，すべてのノードに唯一な ID 番号を割り振っており，その ID 番号
/// からノードを取り出すこともできる( @sa node(ymuint id) )．
/// 入力ノード，出力ノードは ID 番号とは別に入力 ID 番号，および出力
/// ID 番号を持っており，それらの ID 番号からノードを取り出すこともできる．
/// ( @sa input(ymuint id), output(ymuint id) )
/// LUT ノードを入力からのトポロジカル順で処理するためには sort()
/// を用いてソートされたノードのベクタを得る．
/// @sa LnEdge LnNode
//////////////////////////////////////////////////////////////////////
class LnGraph
{
public:

  /// @brief コンストラクタ
  LnGraph();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  LnGraph(const LnGraph& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身への参照を返す．
  const LnGraph&
  operator=(const LnGraph& src);

  /// @brief デストラクタ
  ~LnGraph();

  
  //////////////////////////////////////////////////////////////////////
  /// @name ノード関連の情報の取得
  /// @{

  /// @brief ノードIDの最大値 + 1 の取得
  /// @return ノードIDの最大値 + 1 を返す．
  /// @note ノードIDは間が抜けている場合がある．
  ymuint
  max_node_id() const;
  
  /// @brief ID 番号によるノードの取得
  /// @param[in] id ID 番号
  /// @return ID 番号が id のノードを返す．
  /// @retrun 該当するノードが無い場合には NULL を返す．
  LnNode*
  node(ymuint id) const;
  
  /// @brief 入力ノード数の取得
  /// @return 入力ノード数を返す．
  ymuint
  n_inputs() const;

  /// @brief 入力 ID 番号による入力ノードの取得
  /// @param[in] id 入力 ID 番号
  /// @return 入力 ID 番号が id のノードを返す．
  LnNode*
  input(ymuint id) const;

  /// @brief 入力ノードのリストを得る．
  const LnNodeList&
  input_list() const;

  /// @brief 出力のノード数を得る．
  ymuint
  n_outputs() const;

  /// @brief 出力 ID 番号による出力ノードの取得
  /// @param[in] id 出力 ID 番号
  /// @return 出力 ID 番号が id のノードを返す．
  LnNode*
  output(ymuint id) const;

  /// @brief 入力ノードのリストを得る．
  const LnNodeList&
  output_list() const;

  /// @brief LUTノード数を得る．
  ymuint
  n_lnodes() const;
  
  /// @brief LUTノードのリストを得る．
  const LnNodeList&
  lnode_list() const;

  /// @brief LUTノード数を得る．
  ymuint
  n_luts() const;
  
  /// @brief LUTノードのリストを得る．
  const LnNodeList&
  lut_list() const;

  /// @brief ソートされたLUTノードのリストを得る．
  /// @param[out] node_list
  void
  sort(vector<LnNode*>& node_list) const;

  /// @brief DFFノード数を得る．
  ymuint
  n_dffs() const;
  
  /// @brief DFFノードのリストを得る．
  const LnNodeList&
  dff_list() const;

  /// @brief 最大段数を求める．
  ymuint
  level() const;

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name ノードの生成／変更
  /// @{

  /// @brief 入力ノードを作る．
  /// @param[in] name 名前
  /// @return 作成したノードを返す．
  LnNode*
  new_input(const string& name);
  
  /// @brief 出力ノードを作る．
  /// @param[in] name 名前
  /// @param[in] inode 入力のノード
  /// @return 作成したノードを返す．
  LnNode*
  new_output(const string& name,
	     LnNode* inode);
  
  /// @brief LUTノードを作る．
  /// @param[in] name 名前
  /// @param[in] inodes 入力ノードのベクタ
  /// @param[in] tv 真理値ベクタ
  /// @return 作成したノードを返す．
  /// @note tv のサイズは inodes のサイズの冪乗
  LnNode*
  new_lut(const string& name,
	  const vector<LnNode*>& inodes,
	  const vector<int>& tv);
  
  /// @brief DFFノードを作る．
  /// @param[in] name 名前
  /// @return 作成したノードを返す．
  LnNode*
  new_dff(const string& name);

  /// @brief DFFノードの入力を設定する．
  /// @param[in] node 対象の DFF ノード
  /// @param[in] inode 入力のノード
  void
  set_dff_input(LnNode* node,
		LnNode* inode);

  /// @}
  //////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////
  /// @name その他の関数
  /// @{

  /// @brief 空にする．
  void
  clear();
  
  /// @brief 内容を s に出力する．
  void
  dump(ostream& s) const;
  
  /// @}
  //////////////////////////////////////////////////////////////////////
  

private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  // 複製する．
  void
  copy(const LnGraph& src,
       vector<LnNode*>& nodemap);
  
  // 新しいノードを作成し mNodeList に登録する．
  // 作成されたノードを返す．
  LnNode*
  new_node(ymuint ni);

  /// @brief 入力ノードの削除
  /// @param[in] node 対象のノード
  void
  delete_input(LnNode* node);

  /// @brief 出力ノードの削除
  /// @param[in] node 対象のノード
  void
  delete_output(LnNode* node);
  
  /// @brief LUTノードを削除する．
  /// @param[in] node 対象のノード
  /// @note node のファンアウトは空でなければならない．
  void
  delete_lut(LnNode* node);
  
  /// @brief DFFノードを削除する．
  /// @param[in] node 対象のノード
  /// @note node のファンアウトは空でなければならない．
  void
  delete_dff(LnNode* node);
  
  // node を削除する．
  void
  delete_node(LnNode* node);
  
  // from を to の pos 番目のファンインとする．
  // to の pos 番目にすでに接続があった場合には自動的に削除される．
  void
  connect(LnNode* from,
	  LnNode* to,
	  ymuint pos);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードを確保するためのアロケータ
  SimpleAlloc mAlloc;

  // ノードのファンインの枝の配列を確保するためのアロケータ
  FragAlloc mAlloc2;
  
  // ID 番号をキーにしたノードの配列
  // すべてのノードが格納される．
  vector<LnNode*> mNodeArray;

  // ID 番号を管理するためのオブジェクト
  ItvlMgr mItvlMgr;

  // 入力番号をキーにした入力ノードの配列
  // 穴はあいていない．
  vector<LnNode*> mInputArray;

  // 入力ノードのリスト
  LnNodeList mInputList;

  // 出力番号をキーにした出力ノードの配列
  // 穴はあいていない．
  vector<LnNode*> mOutputArray;

  // 出力ノードのリスト
  LnNodeList mOutputList;
  
  // LUTノードのリスト
  LnNodeList mLutList;
  
  // DFFノードのリスト
  LnNodeList mDffList;
  
  // 最大レベル
  mutable
  ymuint32 mLevel;

  // mLevel および各 LnNode::mLevel が正しいとき true となるフラグ
  mutable
  bool mLevelValid;
  
};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス LnEdge
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
LnEdge::LnEdge() :
  mFrom(NULL),
  mTo(NULL),
  mIpos(0)
{
}

// デストラクタ
inline
LnEdge::~LnEdge()
{
}

// 入力側のノードを得る．
inline
const LnNode*
LnEdge::from() const
{
  return mFrom;
}

// 出力側のノードを得る．
inline
const LnNode*
LnEdge::to() const
{
  return mTo;
}

// 入力側のノードを得る．
inline
LnNode*
LnEdge::from()
{
  return mFrom;
}

// 出力側のノードを得る．
inline
LnNode*
LnEdge::to()
{
  return mTo;
}

// 出力側のノードの何番目の入力かを示す．
inline
ymuint
LnEdge::pos() const
{
  return mIpos;
}

// @brief from ノードをセットする．
inline
void
LnEdge::set_from(LnNode* from)
{
  mFrom = from;
}

// @brief to ノードをセットする．
inline
void
LnEdge::set_to(LnNode* to,
	       ymuint pos)
{
  mTo = to;
  mIpos = pos;
}

// @brief 出力ノードに接続している時 true を返す．
inline
bool
LnEdge::is_poedge() const
{
  return to()->is_output();
}


//////////////////////////////////////////////////////////////////////
// クラス LnNode
//////////////////////////////////////////////////////////////////////

// ID 番号を得る．
inline
ymuint
LnNode::id() const
{
  return mId;
}

// @brief 名前を得る．
// @note 名前がない場合もある．
inline
const string&
LnNode::name() const
{
  return mName;
}

// @brief LUTノードの場合に真理値ベクタを得る．
// @param[out] tv 真理値を格納するベクタ
inline
void
LnNode::tv(vector<int>& tv) const
{
  tv = mTv;
}

// タイプを入力に設定する．
inline
void
LnNode::set_input(ymuint subid)
{
  mFlags = static_cast<ymuint>(kINPUT) | (subid << kSubidShift);
}

// タイプを出力に設定する．
inline
void
LnNode::set_output(ymuint subid)
{
  mFlags = static_cast<ymuint>(kOUTPUT) | (subid << kSubidShift);
}

// タイプをLUTに設定する．
inline
void
LnNode::set_lut()
{
  mFlags = static_cast<ymuint>(kLUT);
}

// @brief タイプをDFFに設定する．
inline
void
LnNode::set_dff()
{
  mFlags = static_cast<ymuint>(kDFF);
}

// タイプを得る．
inline
LnNode::tType
LnNode::type() const
{
  return static_cast<tType>((mFlags & kTypeMask) >> kTypeShift);
}

// 入力ノードの時に true を返す．
inline
bool
LnNode::is_input() const
{
  return type() == kINPUT;
}

// 出力ノードの時に true を返す．
inline
bool
LnNode::is_output() const
{
  return type() == kOUTPUT;
}

// LUTノードの時に true を返す．
inline
bool
LnNode::is_lut() const
{
  return type() == kLUT;
}

// @brief DFFノードの時に true を返す．
inline
bool
LnNode::is_dff() const
{
  return type() == kDFF;
}

// @brief サブID (入力／出力番号)を得る．
inline
ymuint
LnNode::subid() const
{
  return (mFlags >> kSubidShift);
}

// @brief 入力数を得る．
inline
ymuint
LnNode::ni() const
{
  return mNi;
}

// @brief ファンインのノードを得る．
inline
const LnNode*
LnNode::fanin(ymuint pos) const
{
  // pos の範囲チェックはしていない！！！
  return mFanins[pos].from();
}

// @brief ファンインのノードを得る．
inline
LnNode*
LnNode::fanin(ymuint pos)
{
  // pos の範囲チェックはしていない！！！
  return mFanins[pos].from();
}

// ファンインの枝を得る．
inline
const LnEdge*
LnNode::fanin_edge(ymuint pos) const
{
  // pos の範囲チェックはしていない！！！
  return &mFanins[pos];
}

// ファンインの枝を得る．
inline
LnEdge*
LnNode::fanin_edge(ymuint pos)
{
  // pos の範囲チェックはしていない！！！
  return &mFanins[pos];
}

// ファンアウトリストを得る．
inline
const LnEdgeList&
LnNode::fanout_list() const
{
  return mFanoutList;
}

// ファンアウト数を得る．
inline
ymuint
LnNode::n_fanout() const
{
  return mFanoutList.size();
}

// @brief 出力ノードにファンアウトしているとき true を返す．
inline
bool
LnNode::pomark() const
{
  return static_cast<bool>((mFlags >> kPoShift) & 1U);
}

// @brief レベルを得る．
inline
ymuint
LnNode::level() const
{
  return mLevel;
}


//////////////////////////////////////////////////////////////////////
// クラス LnGraph
//////////////////////////////////////////////////////////////////////

// ノード番号の最大値 + 1 を返す．
inline
ymuint
LnGraph::max_node_id() const
{
  return mNodeArray.size();
}

// ID 番号が id のノードを取り出す．
// 該当するノードが無い場合には NULL を返す．
inline
LnNode*
LnGraph::node(ymuint id) const
{
  return mNodeArray[id];
}

// 入力ノード数を得る．
inline
ymuint
LnGraph::n_inputs() const
{
  return mInputArray.size();
}

// ID 番号が id の入力ノードを取り出す．
inline
LnNode*
LnGraph::input(ymuint id) const
{
  return mInputArray[id];
}

// @brief 入力ノードのリストを得る．
inline
const LnNodeList&
LnGraph::input_list() const
{
  return mInputList;
}

// 出力のノード数を得る．
inline
ymuint
LnGraph::n_outputs() const
{
  return mOutputArray.size();
}

// ID 番号が id の出力ノードを取り出す．
inline
LnNode*
LnGraph::output(ymuint id) const
{
  return mOutputArray[id];
}

// @brief 入力ノードのリストを得る．
inline
const LnNodeList&
LnGraph::output_list() const
{
  return mOutputList;
}

// LUTノード数を得る．
inline
ymuint
LnGraph::n_lnodes() const
{
  return mLutList.size();
}

// LUTノードのリストを得る．
inline
const LnNodeList&
LnGraph::lnode_list() const
{
  return mLutList;
}

// @brief LUTノード数を得る．
inline
ymuint
LnGraph::n_luts() const
{
  return mLutList.size();
}

// @brief LUTノードのリストを得る．
inline
const LnNodeList&
LnGraph::lut_list() const
{
  return mLutList;
}

// @brief DFFノード数を得る．
inline
ymuint
LnGraph::n_dffs() const
{
  return mDffList.size();
}

// @brief DFFノードのリストを得る．
inline
const LnNodeList&
LnGraph::dff_list() const
{
  return mDffList;
}

END_NAMESPACE_YM_LUTMAP

BEGIN_NAMESPACE_YM

using nsLutmap::lut2bnet;

END_NAMESPACE_YM

#endif // YM_LUTMAP_LUTNETWORK_H
