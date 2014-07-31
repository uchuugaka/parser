#ifndef LIBYM_CEC_CECMGR_H
#define LIBYM_CEC_CECMGR_H

/// @file libym_cec/CecMgr.h
/// @brief CecMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "cec_nsdef.h"
#include "CecHandle.h"
#include "CecHash.h"
#include "YmUtils/SimpleAlloc.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_CEC

class CecNode;

//////////////////////////////////////////////////////////////////////
/// @class CecMgr CecMgr.h "CecMgr.h"
/// @brief 組み合わせ回路の等価検証を行なうクラス
//////////////////////////////////////////////////////////////////////
class CecMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SAT-solver の種類を表す文字列
  /// @param[in] sat_opt SAT-solver に渡すオプション文字列
  /// @param[in] sat_log ログの出力用ストリーム
  CecMgr(const string& sat_type,
	 const string& sat_opt,
	 ostream* sat_log);

  /// @brief デストラクタ
  ~CecMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // FraigMgr とのインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノード数を得る．
  ymuint
  input_num() const;

  /// @brief 入力ノードを取り出す．
  /// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
  CecNode*
  input_node(ymuint pos) const;

  /// @brief ノード数を得る．
  /// @note ANDノードの他に入力ノードも含まれる．
  ymuint
  node_num() const;

  /// @brief ノードを取り出す．
  /// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
  /// @note ANDノードの他に入力ノードも含まれる．
  CecNode*
  node(ymuint pos) const;

  /// @brief 定数0関数をつくる．
  CecHandle
  make_zero();

  /// @brief 定数1関数をつくる．
  CecHandle
  make_one();

  /// @brief 外部入力を作る．
  CecHandle
  make_input();

  /// @brief 2つのノードの AND を取る．
  /// @param[in] handle1, handle2 入力の FRAIG ハンドル
  CecHandle
  make_and(CecHandle handle1,
	   CecHandle handle2);

  /// @brief 2つのノードの OR を取る．
  /// @param[in] handle1, handle2 入力の FRAIG ハンドル
  CecHandle
  make_or(CecHandle handle1,
	  CecHandle handle2);

  /// @brief 2つのハンドルが等価かどうか調べる．
  Bool3
  check_equiv(CecHandle aig1,
	      CecHandle aig2);

  /// @brief sat_sweep() の後始末を行う．
  void
  finish();


public:
  //////////////////////////////////////////////////////////////////////
  // SAT-solver の制御
  //////////////////////////////////////////////////////////////////////

  /// @brief ログレベルを設定する．
  void
  set_loglevel(int level);

  /// @brief ログ出力用ストリームを設定する．
  void
  set_logstream(ostream* out);

  /// @brief 統計情報を出力する．
  void
  dump_stats(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードが定数と等価かどうか調べる．
  /// @param[in] node 対象のノード
  /// @param[in] inv false で 0, true で 1 を表す．
  Bool3
  check_const(CecNode* node,
	      bool inv);

  /// @brief 2つのノードが等価かどうか調べる．
  /// @param[in] node1, node2 対象のノード
  /// @param[in] inv false で同相，true で逆相を表す．
  Bool3
  check_equiv(CecNode* node1,
	      CecNode* node2,
	      bool inv);

  /// @brief lit1 が成り立つか調べる．
  Bool3
  check_condition(Literal lit1);

  /// @brief lit1 & lit2 が成り立つか調べる．
  Bool3
  check_condition(Literal lit1,
		  Literal lit2);

  /// @brief CecHandle に対応するリテラルを返す．
  /// @note 定数の場合の返り値は未定
  Literal
  fraig2literal(CecHandle aig);

  /// @brief 等価候補グループの情報を出力する．
  void
  dump_eqgroup(ostream& s) const;

  /// @brief 新しいノードを生成する．
  CecNode*
  new_node();

  /// @brief CecNode のハッシュ関数
  static
  ymuint
  hash_func(CecHandle handle1,
	    CecHandle handle2);


private:

  // SatSolver の統計情報
  struct SatStat
  {

    // 試行回数
    ymuint32 mTotalCount;

    struct
    {
      // 回数
      ymuint32 mCount;

      // 計算時間の総和
      double mTotalTime;

      // 計算時間の最大値
      double mMaxTime;

      // restart 回数
      ymuint32 mRestart;

      // コンフリクト数
      ymuint64 mConflictNum;

      // decision 数
      ymuint64 mDecisionNum;

      // implication 数
      ymuint64 mPropagationNum;

      // 学習説のリテラル数
      ymuint64 mLearntLitNum;

    } mTimeStat[3];

    // コンストラクタ
    SatStat();

    // 結果をセットする．
    // code = kB3True 検証成功
    //      = kB3False 検証失敗
    //      = kB3X     アボート
    void
    set_result(Bool3 code,
	       double t);

    // 内容をダンプする．
    void
    dump(ostream& s) const;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードを確保するためのアロケータ
  SimpleAlloc mAlloc;

  // ID 番号をキーにしたノードの配列
  vector<CecNode*> mAllNodes;

  // 入力ノードの配列
  vector<CecNode*> mInputNodes;

  // 構造ハッシュ
  CecHash mHashTable1;

  // SATソルバ
  SatSolver mSolver;

  // SAT 用の割り当て格納配列
  vector<Bool3> mModel;

  // sat_sweep 中のシミュレーション回数
  ymuint32 mSimCount;

  // シミュレーションに要した時間
  double mSimTime;

  // check_const の統計情報
  SatStat mCheckConstInfo;

  // check_equiv の統計情報
  SatStat mCheckEquivInfo;

  // recsolver 用のストリーム
  ostream* mOutP;

  // ログレベル
  int mLogLevel;

  // ログ出力用のストリーム
  ostream* mLogStream;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 入力ノード数を得る．
inline
ymuint
CecMgr::input_num() const
{
  return mInputNodes.size();
}

// @brief 入力ノードを取り出す．
// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
inline
CecNode*
CecMgr::input_node(ymuint pos) const
{
  return mInputNodes[pos];
}

// @brief ノード数を得る．
inline
ymuint
CecMgr::node_num() const
{
  return mAllNodes.size();
}

// @brief ノードを取り出す．
// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
// @note ANDノードの他に入力ノードも含まれる．
inline
CecNode*
CecMgr::node(ymuint pos) const
{
  return mAllNodes[pos];
}

// @brief 2つのノードの OR を取る．
// @param[in] handle1, handle2 入力の FRAIG ハンドル
inline
CecHandle
CecMgr::make_or(CecHandle handle1,
		CecHandle handle2)
{
  return ~make_and(~handle1, ~handle2);
}

// CecNode のハッシュ関数
inline
ymuint
CecMgr::hash_func(CecHandle handle1,
		  CecHandle handle2)
{
  return handle1.mPackedData + handle2.mPackedData;
}

END_NAMESPACE_YM_CEC

#endif // LIBYM_CEC_CECMGR_H
