#ifndef YM_BDN_BDNETWORK_H
#define YM_BDN_BDNETWORK_H

/// @file ym_bdn/BdNetwork.h
/// @brief BdNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BdNetwork.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include <ym_bdn/bdn_nsdef.h>
#include <ym_bdn/BdnNodeHandle.h>
#include <ym_utils/Alloc.h>
#include <ym_utils/ItvlMgr.h>


BEGIN_NAMESPACE_YM_BDN

//////////////////////////////////////////////////////////////////////
/// @class BdNetwork BdNetwork.h <ym_bdn/BdNetwork.h>
/// @brief 2入力ノードのネットワークを表すクラス
///
///
/// ノードは，
///  - 入力ノード
///  - 出力ノード
///  - D-FF ノード
///  - 論理ノード
/// の4種類がある(@sa BdnNode)．
/// 入力ノード，出力ノードは入力 ID 番号，および出力 ID 番号を持って
/// おり，それらの ID 番号からノードを取り出すことができる．
/// ( @sa input(ymuint id), output(ymuint id) )
/// 論理ノードはリストの形で保持される( @sa lnode_list() )．
/// また，すべてのノードに唯一な ID 番号を割り振っており，その ID 番号
/// からノードを取り出すこともできる( @sa node(ymuint id) )．
/// 論理ノードを入力からのトポロジカル順で処理するためには sort()
/// を用いてソートされたノードのベクタを得る．
///
/// その他に記憶素子を表すD-FFがある．D-FFは
///  - 入力に接続する "出力"ノード
///  - 出力に接続する "入力"ノード
///  - リセット値
/// を持つ(@sa BdnLatch)．
/// ラッチはリストの形で保持される(@sa latch_list)
//////////////////////////////////////////////////////////////////////
class BdNetwork
{
public:

  /// @brief コンストラクタ
  /// @note 空のネットワークに初期化される．
  BdNetwork();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  BdNetwork(const BdNetwork& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身への参照を返す．
  const BdNetwork&
  operator=(const BdNetwork& src);

  /// @brief デストラクタ
  ~BdNetwork();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノードとラッチ関連の情報の取得
  /// @{

  /// @brief ノードIDの最大値 + 1 の取得
  /// @return ノードIDの最大値 + 1 を返す．
  ymuint
  max_node_id() const;

  /// @brief 入力ノード数の取得
  /// @return 入力ノード数を返す．
  ymuint
  input_num() const;

  /// @brief 外部入力番号から外部入力ノードを得る．
  /// @param[in] pos 外部入力番号 ( 0 <= pos < input_num() )
  BdnNode*
  input(ymuint pos) const;

  /// @brief 外部入力の名前を得る．
  /// @param[in] pos 外部入力番号 ( 0 <= pos < input_num() )
  string
  input_name(ymuint pos) const;

  /// @brief 出力ノード数を得る．
  ymuint
  output_num() const;

  /// @brief 外部出力番号から外部出力ノードを得る．
  /// @param[in] pos 外部出力番号 ( 0 <= pos < output_num() )
  BdnNode*
  output(ymuint pos) const;

  /// @brief 外部出力の名前を得る．
  /// @param[in] pos 外部出力番号 ( 0 <= pos < output_num() )
  string
  output_name(ymuint pos) const;

  /// @brief DFF数を得る．
  ymuint
  dff_num() const;

  /// @brief D-FFのリストを得る．
  const BdnNodeList&
  dff_list() const;

  /// @brief 論理ノード数を得る．
  ymuint
  lnode_num() const;

  /// @brief 論理ノードのリストを得る．
  const BdnNodeList&
  lnode_list() const;

  /// @brief ソートされた論理ノードのリストを得る．
  /// @param[out] node_list ノードのリストの格納先
  /// @note 入力ノードと出力ノード，ラッチノードは含まない．
  void
  sort(vector<BdnNode*>& node_list) const;

  /// @brief 逆順でソートされた論理ノードのリストを得る．
  /// @param[out] node_list ノードのリストの格納先
  /// @note 入力ノードと出力ノード，ラッチノードは含まない．
  void
  rsort(vector<BdnNode*>& node_list) const;

  /// @brief 最大段数を求める．
  /// @note 段数とは入力ノードから出力ノードへ至る経路中の論理ノードの数
  ymuint
  level() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノードの生成および変更を行う関数
  /// @{

  /// @brief 入力ノードを作る．
  /// @param[in] name 名前
  /// @return 作成したノードを返す．
  BdnNode*
  new_input(const string& name);

  /// @brief 出力ノードを作る．
  /// @param[in] name 名前
  /// @param[in] inode_handle 入力のノード+極性
  /// @return 作成したノードを返す．
  BdnNode*
  new_output(const string& name,
	     BdnNodeHandle inode_handle);

  /// @brief 出力ノードの内容を変更する
  /// @param[in] node 変更対象の出力ノード
  /// @param[in] inode_handle 入力のノード+極性
  void
  change_output(BdnNode* node,
		BdnNodeHandle inode_handle);

  /// @brief D-FF を作る．
  /// @return 生成されたラッチを返す．
  BdnNode*
  new_dff();

  /// @brief D-FF のデータ入力を変更する．
  /// @param[in] dff 変更対象の D-FF
  /// @param[in] input_handle ラッチのデータ入力に接続しているノード+極性
  void
  set_dff_data(BdnNode* dff,
	       BdnNodeHandle input_handle);

  /// @brief D-FF のクロック入力を変更する．
  /// @param[in] dff 変更対象の D-FF
  /// @param[in] input_handle ラッチのクロック入力に接続しているノード+極性
  void
  set_dff_clock(BdnNode* dff,
		BdnNodeHandle input_handle);

  /// @brief D-FF のリセット入力を変更する．
  /// @param[in] dff 変更対象の D-FF
  /// @param[in] input_handle ラッチのリセット入力に接続しているノード+極性
  void
  set_dff_rst(BdnNode* dff,
	      BdnNodeHandle input_handle);

  /// @brief D-FF のセット入力を変更する．
  /// @param[in] dff 変更対象の D-FF
  /// @param[in] input_handle ラッチの入力に接続しているノード+極性
  void
  set_dff_set(BdnNode* dff,
	      BdnNodeHandle input_handle);

  /// @brief 論理ノードを作る．
  /// @param[in] fcode 機能コード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note fcode は2入力関数の真理値表ベクタ
  /// @note fcode の出力極性を正規化する．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_logic(ymuint fcode,
	    BdnNodeHandle inode1_handle,
	    BdnNodeHandle inode2_handle);

  /// @brief AND ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_and(BdnNodeHandle inode1_handle,
	  BdnNodeHandle inode2_handle);

  /// @brief AND ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_and(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief NAND ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note fcode の出力極性を正規化する．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_nand(BdnNodeHandle inode1_handle,
	   BdnNodeHandle inode2_handle);

  /// @brief NAND ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_nand(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief OR ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_or(BdnNodeHandle inode1_handle,
	 BdnNodeHandle inode2_handle);

  /// @brief OR ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_or(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief NOR ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_nor(BdnNodeHandle inode1_handle,
	  BdnNodeHandle inode2_handle);

  /// @brief NOR ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_nor(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief XOR ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_xor(BdnNodeHandle inode1_handle,
	  BdnNodeHandle inode2_handle);

  /// @brief XOR ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_xor(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief XNOR ノードを作る．
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_xnor(BdnNodeHandle inode1_handle,
	   BdnNodeHandle inode2_handle);

  /// @brief XNOR ノードを作る．
  /// @param[in] inode_handle_list 入力ノード+極性のリスト
  /// @return 作成したノードを返す．
  /// @note すでに構造的に同じノードがあればそれを返す．
  BdnNodeHandle
  new_xnor(const vector<BdnNodeHandle>& inode_handle_list);

  /// @brief 論理ノードの内容を変更する．
  /// @param[in] node 変更対象の論理ノード
  /// @param[in] fcode 機能コード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @note fcode の出力極性を正規化する．
  /// @note 実際には新しいノードを作ってそこへのリンクを内部で持つ．
  void
  change_logic(BdnNode* node ,
	       ymuint fcode,
	       BdnNodeHandle inode1_handle,
	       BdnNodeHandle inode2_handle);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他の関数
  /// @{

  /// @brief 名前を得る．
  string
  name() const;

  /// @brief 名前を設定する．
  /// @param[in] name 新しい名前
  void
  set_name(const string& name);

  /// @brief 空にする．
  void
  clear();

  /// @brief どこにもファンアウトしていないノードを削除する．
  void
  clean_up();

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  dump(ostream& s) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーコンストラクタと代入演算子用のサブルーティン
  /// @param[in] src コピー元のオブジェクト
  void
  copy(const BdNetwork& src);

  /// @brief バランス木を作る．
  /// @param[in] fcode 機能コード
  /// @param[in] start 開始位置
  /// @param[in] num 要素数
  /// @param[in] node_list ノードのリスト
  BdnNodeHandle
  make_tree(ymuint fcode,
	    ymuint start,
	    ymuint num,
	    const vector<BdnNodeHandle>& node_list);

  /// @brief 論理ノードの内容を設定する．
  /// @param[in] node 設定するノード
  /// @param[in] fcode 機能コード
  /// @param[in] inode1_handle 1番めの入力ノード+極性
  /// @param[in] inode2_handle 2番めの入力ノード+極性
  /// @return ノード＋極性を返す．
  /// @note fcode の出力極性を正規化する．
  /// @note すでに構造的に同じノードがあればそれを返す．
  /// @note なければ node に設定する．
  /// @note node が NULL の場合，新しいノードを確保する．
  BdnNodeHandle
  set_logic(BdnNode* node,
	    ymuint fcode,
	    BdnNodeHandle inode1_handle,
	    BdnNodeHandle inode2_handle);

  /// @brief ノード間の接続を変更する．
  /// @param[in] from 接続の入力側のノード
  /// @param[in] to 接続の出力側のノード
  /// @param[in] pos to のファンイン番号
  /// @note to の pos 番目にすでに接続があった場合には自動的に削除される．
  void
  connect(BdnNode* from,
	  BdnNode* to,
	  ymuint pos);

  /// @brief 新しいノードを作成し mNodeList に登録する．
  /// @return 作成されたノードを返す．
  BdnNode*
  new_node();

  /// @note node を削除する．
  /// @param[in] node 削除対象のノード
  void
  delete_node(BdnNode* node);

  /// @brief ハッシュ表を確保する．
  /// @param[in] req_size 確保するサイズ
  void
  alloc_table(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードを確保するためのアロケータ
  SimpleAlloc mAlloc;

  // ネットワーク名
  string mName;

  // ID 番号をキーにしたノードの配列
  // すべてのノードが格納される．
  vector<BdnNode*> mNodeArray;

  // ID 番号を管理するためのオブジェクト
  ItvlMgr mItvlMgr;

  // 外部入力ノードの配列
  // 配列上の位置が外部入力ノードの入力番号となる．
  vector<BdnNode*> mInputArray;

  // 外部入力ノードの名前の配列
  vector<string> mInputNameArray;

  // 外部出力ノードの配列
  // 配列上の位置が外部出力ノードの出力番号となる．
  vector<BdnNode*> mOutputArray;

  // 外部出力ノードの名前の配列
  vector<string> mOutputNameArray;

  // D-FFノードのリスト
  BdnNodeList mDffList;

  // 論理ノードのリスト
  BdnNodeList mLnodeList;

  // 論理ノードのハッシュ表
  BdnNode** mHashTable;

  // mHashTable のサイズ
  ymuint32 mHashSize;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

  // 最大レベル (最下位ビットは valid フラグ)
  mutable
  ymuint32 mLevel;

};


/// @relates BdNetwork
/// @brief BdNetwork の内容をダンプする関数
/// @param[in] s 出力先のストリーム
/// @param[in] network 対象のネットワーク
void
dump(ostream& s,
     const BdNetwork& network);


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス BdNetwork
//////////////////////////////////////////////////////////////////////

// @brief 名前を得る．
inline
string
BdNetwork::name() const
{
  return mName;
}

// @brief 名前を設定する．
inline
void
BdNetwork::set_name(const string& name)
{
  mName = name;
}

// ノード番号の最大値 + 1 を返す．
inline
ymuint
BdNetwork::max_node_id() const
{
  return mNodeArray.size();
}

// @brief 入力ノード数の取得
// @return 入力ノード数を返す．
inline
ymuint
BdNetwork::input_num() const
{
  return mInputArray.size();
}

// @brief 外部入力番号から外部入力ノードを得る．
// @param[in] pos 外部入力番号 ( 0 <= pos < input_num() )
inline
BdnNode*
BdNetwork::input(ymuint pos) const
{
  return mInputArray[pos];
}

// @brief 外部入力の名前を得る．
// @param[in] pos 外部入力番号 ( 0 <= pos < input_num() )
inline
string
BdNetwork::input_name(ymuint pos) const
{
  return mInputNameArray[pos];
}

// @brief 出力ノード数を得る．
inline
ymuint
BdNetwork::output_num() const
{
  return mOutputArray.size();
}

// @brief 外部出力番号から外部出力ノードを得る．
// @param[in] pos 外部出力番号 ( 0 <= pos < output_num() )
inline
BdnNode*
BdNetwork::output(ymuint pos) const
{
  return mOutputArray[pos];
}

// @brief 外部出力の名前を得る．
// @param[in] pos 外部出力番号 ( 0 <= pos < output_num() )
inline
string
BdNetwork::output_name(ymuint pos) const
{
  return mOutputNameArray[pos];
}

// @brief D-FF数を得る．
inline
ymuint
BdNetwork::dff_num() const
{
  return mDffList.size();
}

// @brief D-FFのリストを得る．
inline
const BdnNodeList&
BdNetwork::dff_list() const
{
  return mDffList;
}

// 論理ノード数を得る．
inline
ymuint
BdNetwork::lnode_num() const
{
  return mLnodeList.size();
}

// 論理ノードのリストを得る．
inline
const BdnNodeList&
BdNetwork::lnode_list() const
{
  return mLnodeList;
}

// @brief NAND ノードを作る．
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
// @return 作成したノードを返す．
// @note fcode の出力極性を正規化する．
// @note すでに構造的に同じノードがあればそれを返す．
inline
BdnNodeHandle
BdNetwork::new_nand(BdnNodeHandle inode1_handle,
		    BdnNodeHandle inode2_handle)
{
  return ~new_and(inode1_handle, inode2_handle);
}

// @brief NAND ノードを作る．
// @param[in] inode_handle_list 入力ノード+極性のリスト
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
inline
BdnNodeHandle
BdNetwork::new_nand(const vector<BdnNodeHandle>& inode_handle_list)
{
  return ~new_and(inode_handle_list);
}

// @brief NOR ノードを作る．
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
inline
BdnNodeHandle
BdNetwork::new_nor(BdnNodeHandle inode1_handle,
		   BdnNodeHandle inode2_handle)
{
  return ~new_or(inode1_handle, inode2_handle);
}

// @brief NOR ノードを作る．
// @param[in] inode_handle_list 入力ノード+極性のリスト
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
inline
BdnNodeHandle
BdNetwork::new_nor(const vector<BdnNodeHandle>& inode_handle_list)
{
  return ~new_or(inode_handle_list);
}

// @brief XNOR ノードを作る．
// @param[in] inode1_handle 1番めの入力ノード+極性
// @param[in] inode2_handle 2番めの入力ノード+極性
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
inline
BdnNodeHandle
BdNetwork::new_xnor(BdnNodeHandle inode1_handle,
		    BdnNodeHandle inode2_handle)
{
  return ~new_xor(inode1_handle, inode2_handle);
}

// @brief XNOR ノードを作る．
// @param[in] inode_handle_list 入力ノード+極性のリスト
// @return 作成したノードを返す．
// @note すでに構造的に同じノードがあればそれを返す．
inline
BdnNodeHandle
BdNetwork::new_xnor(const vector<BdnNodeHandle>& inode_handle_list)
{
  return ~new_xor(inode_handle_list);
}

END_NAMESPACE_YM_BDN

#endif // YM_BDN_BDNETWORK_H
