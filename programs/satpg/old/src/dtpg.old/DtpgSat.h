#ifndef DTPGSAT_H
#define DTPGSAT_H

/// @file DtpgSat.h
/// @brief DtpgSat のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"
#include "Dtpg.h"
#include "DtpgFault.h"
#include "ym_YmNetworks/tgnet.h"
#include "ym_YmLogic/Literal.h"
#include "ym_YmLogic/Bool3.h"
#include "ym_YmLogic/sat_nsdef.h"
#include "ym_YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgSat DtpgSat.h "DtpgSat.h"
/// @brief SAT を用いたパタン生成器
//////////////////////////////////////////////////////////////////////
class DtpgSat :
  public Dtpg
{
public:

  /// @brief コンストラクタ
  DtpgSat();

  /// @brief デストラクタ
  virtual
  ~DtpgSat();


public:
  //////////////////////////////////////////////////////////////////////
  // Dtpg の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用する SAT エンジンを指定する．
  virtual
  void
  set_mode(const string& type = string(),
	   const string& option = string(),
	   ostream* outp = NULL);

  /// @brief get_pat フラグを設定する．
  virtual
  void
  set_get_pat(ymuint val);

  /// @brief dry-run フラグを設定する．
  virtual
  void
  set_dry_run(bool flag);

  /// @brief 回路と故障リストを設定する．
  /// @param[in] tgnetwork 対象のネットワーク
  /// @param[in] fault_list 故障リスト
  virtual
  void
  set_network(const TgNetwork& tgnetwork,
	      const vector<SaFault*>& fault_list);

  /// @brief モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  /// @param[in] option オプション文字列
  virtual
  void
  run(DtpgOperator& op,
      const string& option = string());

  /// @brief 統計情報をクリアする．
  virtual
  void
  clear_stats();

  /// @brief 統計情報を得る．
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  get_stats(DtpgStats& stats) const;

  /// @breif 時間計測を制御する．
  virtual
  void
  timer_enable(bool enable);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief single モードの共通処理
  void
  single_sub(DtpgOperator& op);

  /// @brief dual モードの共通処理
  void
  dual_sub(DtpgOperator& op);

  /// @brief ffr モードの共通処理
  void
  ffr_sub(DtpgOperator& op);

  /// @brief mffc モードの共通処理
  void
  mffc_sub(DtpgOperator& op);

  /// @brief all モードの共通処理
  void
  all_sub(DtpgOperator& op);

  /// @brief 一つの FFR に対してテストパタン生成を行う．
  /// @param[in] ffr 対象の FFR
  void
  ffr_mode(DtpgFFR* ffr,
	   DtpgOperator& op);

  /// @brief 一つの故障に対してテストパタン生成を行う．
  /// @param[in] f 故障
  /// @param[in] op テスト生成の結果を処理するファンクター
  void
  dtpg_single(DtpgFault* f,
	      DtpgOperator& op);

  /// @brief 同じ位置の2つの出力故障に対してテストパタン生成を行なう．
  /// @param[in] f0 0縮退故障
  /// @param[in] f1 1縮退故障
  /// @param[in] op テスト生成の結果を処理するファンクター
  void
  dtpg_dual(DtpgFault* f0,
	    DtpgFault* f1,
	    DtpgOperator& op);

  void
  dfs_ffr(DtpgNode* node);

  /// @brief ノードの故障を追加する．
  void
  add_node_faults(DtpgNode* node);

  /// @brief 故障を追加する．
  void
  add_fault(DtpgFault* f);

  /// @brief 複数の故障に対してテストパタン生成を行なう．
  /// @param[in] op テスト生成の結果を処理するファンクター
  void
  do_dtpg(DtpgOperator& op);

  /// @brief 複数の故障に対してテストパタン生成を行なう．
  /// @param[in] op テスト生成の結果を処理するファンクター
  void
  dtpg_group(DtpgOperator& op);

  /// @brief 統計情報を得る．
  /// @param[in] solver SatSolver
  void
  update_stats(SatSolver& solver,
	       ymuint n);

  /// @brief 一つの SAT問題を解く．
  void
  solve(SatSolver& solver,
	DtpgFault* f,
	DtpgOperator& op);

  /// @brief テストパタンを求める．
  /// @param[in] fnode 故障のあるノード
  /// @note 結果は mValList に格納される．
  void
  get_pat(DtpgNode* fnode);

  /// @brief テストパタンを求める．
  /// @param[in] fnode 故障のあるノード
  /// @note 結果は mValList に格納される．
  void
  get_pat2(DtpgNode* fnode);

  /// @brief solve 中で故障差を持つノードをたどる．
  /// @param[in] node 対象のノード
  /// @retval true node を通って外部出力まで故障差が伝搬している．
  /// @retval false 故障差が伝搬していない．
  /// @note 故障差の伝搬経路上のノードは mDiffNodeList に格納される．
  /// @note 一旦調べたノードはすべて mark1 がつく．
  /// @note 故障差が伝搬しているノードは mark2 がつく．
  /// @note マークがついたノードは mBwdNodeList に格納される．
  bool
  fwd_dfs(DtpgNode* node);

  /// @brief solve 中で変数割り当ての正当化を行なう．
  /// @param[in] node 対象のノード
  /// @note node の値割り当てを正当化する．
  /// @note 正当化に用いられているノードには mark3 がつく．
  /// @note mark3 がついたノードは mBwdNodeList に格納される．
  void
  justify(DtpgNode* node);

  /// @brief すべてのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  void
  just_sub1(DtpgNode* node);

  /// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] val 値
  void
  just_sub2(DtpgNode* node,
	    Bool3 val);

  /// @brief justify の下請け関数
  /// @param[in] prim 対象のプリミティブ
  /// @param[in] node 対象のノード
  /// @note node の値割り当てを正当化する．
  /// @note 正当化に用いられているノードには mark3 がつく．
  /// @note mark3 がついたノードは mBwdNodeList に格納される．
  void
  justify_primitive(DtpgPrimitive* prim,
		    DtpgNode* node);

  /// @brief すべてのファンインに対して justify_primitive() を呼ぶ．
  /// @param[in] prim 対象のプリミティブ
  /// @param[in] node 対象のノード
  void
  jp_sub1(DtpgPrimitive* prim,
	  DtpgNode* node);

  /// @brief 指定した値を持つファンインに対して justify_primitive() を呼ぶ．
  /// @param[in] prim 対象のプリミティブ
  /// @param[in] node 対象のノード
  /// @param[in] val 値
  void
  jp_sub2(DtpgPrimitive* prim,
	  DtpgNode* node,
	  Bool3 val);

  /// @brief 入力ノードの値を記録する．
  /// @param[in] node 対象の外部入力ノード
  /// @note node の値を mValList に記録する．
  /// @note 単純だが mModel 上のインデックスと mValList の符号化は異なる．
  void
  record_value(DtpgNode* node);

  /// @brief ノードの変数割り当てフラグを消す．
  void
  clear_node_mark();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT solver のタイプ
  string mType;

  // SAT solver のオプション
  string mOption;

  // SAT solver の記録用ストリーム
  ostream* mOutP;

  // get_pat フラグ
  ymuint32 mGetPatFlag;

  // SAT 用の assumption を格納するベクタ
  vector<Literal> mAssumptions;

  // SAT 用の model を格納するベクタ
  vector<Bool3> mModel;

  // SAT 用の割り当てリスト
  vector<ymuint> mValList;

  // 対象の回路
  DtpgNetwork* mNetwork;

  // 対象の故障リスト
  vector<DtpgFault*> mFaultList;

  // 変数を割り当てたノードを格納するリスト
  vector<DtpgNode*> mUsedNodeList;

  // 現在の故障に関係のありそうな外部入力のリスト
  vector<DtpgNode*> mInputList;

  // 現在の故障に関係ありそうな外部出力のリスト
  vector<DtpgNode*> mOutputList;

  // 故障差が伝搬しているノードを格納するリスト
  vector<DtpgNode*> mDiffNodeList;

  // バックトレースに用いたノードを格納するリスト
  vector<DtpgNode*> mBwdNodeList;

  // skip フラグ
  bool mSkip;

  // skip マークのついた故障のリスト
  vector<DtpgFault*> mSkippedFaults;

  // dry-run フラグ
  bool mDryRun;

  // CNF の生成回数
  ymuint32 mRunCount;

  // SAT の実行回数
  ymuint32 mSatCount;

  // restart の回数の総和
  ymuint64 mRestart;

  // 変数の数の総和
  ymuint64 mVarNum;

  // 制約節の数の総和
  ymuint64 mConstrClauseNum;

  // 制約節のリテラル数の総和
  ymuint64 mConstrLitNum;

  // 学習節の数の総和
  ymuint64 mLearntClauseNum;

  // 学習節のリテラル数の総和
  ymuint64 mLearntLitNum;

  // コンフリクト数の総和
  ymuint64 mConflictNum;

  // decision 数の総和
  ymuint64 mDecisionNum;

  // implication数の総和
  ymuint64 mPropagationNum;

  // 時間計測を行なうかどうかの制御フラグ
  bool mTimerEnable;

  // 時間計測用のタイマー
  StopWatch mTimer;

  // CNF 式を生成した回数
  ymuint32 mCnfCount;

  // CNF 式を生成する時の時間
  USTime mCnfTime;

  // テスト生成が成功した回数
  ymuint32 mDetCount;

  // テスト生成が成功した場合の実行時間
  USTime mDetTime;

  // 冗長故障の判定回数
  ymuint32 mUndetCount;

  // 冗長故障の場合の実行時間
  USTime mUndetTime;

  // アボート回数
  ymuint32 mAbortCount;

  // アボートの場合の実行時間
  USTime mAbortTime;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////


// @brief 一つの故障に対してテストパタン生成を行う．
// @param[in] f 故障
// @param[in] op テスト生成の結果を処理するファンクター
inline
void
DtpgSat::dtpg_single(DtpgFault* f,
		     DtpgOperator& op)
{
  mFaultList.clear();

  add_fault(f);

  do_dtpg(op);
}

// @brief 同じ位置の2つの出力故障に対してテストパタン生成を行なう．
// @param[in] f0 0縮退故障
// @param[in] f1 1縮退故障
// @param[in] op テスト生成の結果を処理するファンクター
inline
void
DtpgSat::dtpg_dual(DtpgFault* f0,
		   DtpgFault* f1,
		   DtpgOperator& op)
{
  mFaultList.clear();

  add_fault(f0);
  add_fault(f1);

  do_dtpg(op);
}

// @brief 故障を追加する．
inline
void
DtpgSat::add_fault(DtpgFault* f)
{
  if ( f != NULL && !f->is_skip() ) {
    mFaultList.push_back(f);
  }
}

// @brief 複数の故障に対してテストパタン生成を行なう．
// @param[in] op テスト生成の結果を処理するファンクター
inline
void
DtpgSat::do_dtpg(DtpgOperator& op)
{
  if ( !mFaultList.empty() ) {
    dtpg_group(op);
  }
}

END_NAMESPACE_YM_SATPG_DTPG

#endif // DTPGSAT_H
