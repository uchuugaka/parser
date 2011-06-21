#ifndef LIBYM_NETWORKS_TGBLIFHANDLER_H
#define LIBYM_NETWORKS_TGBLIFHANDLER_H

/// @file libym_networks/TgBlifHandler.h
/// @brief TgBlifHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TgBlifHandler.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/tgnet.h"
#include "ym_blif/BlifHandler.h"
#include "ym_logic/LogExpr.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class TgBlifHandler TgBlifHandler.h
/// @brief TgNetwork 用の BlifHandler
//////////////////////////////////////////////////////////////////////
class TgBlifHandler :
  public BlifHandler
{
public:

  /// @brief コンストラクタ
  TgBlifHandler();

  /// @brief デストラクタ
  virtual
  ~TgBlifHandler();


public:

  /// @brief ネットワークをセットする．
  void
  set_network(TgNetwork& network);

  /// @brief 初期化
  virtual
  bool
  init();

  /// @brief .model 文の処理
  /// @param[in] loc1 .model の位置情報
  /// @param[in] loc2 文字列の位置情報
  /// @param[in] name 文字列
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  model(const FileRegion& loc1,
	const FileRegion& loc2,
	const char* name);

  /// @brief .inputs 文中の文字列の処理
  /// @param[in] name_id 文字列のID番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  inputs_elem(ymuint32 name_id);

  /// @brief .outputs 文中の文字列の処理
  /// @param[in] name_id 文字列のID番号
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  outputs_elem(ymuint32 name_id);

  /// @brief .names 文の処理
  /// @param[in] name_id_array 各識別子のID番号の配列
  /// @param[in] nc キューブ数
  /// @param[in] cover_pat 入力カバーを表す文字列
  /// @param[in] opat 出力の極性
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  /// @note cover_pat は ni 個ごとに1行のパタンを表す．
  /// 各要素のとりうる値は '0', '1', '-' を表す．
  /// @note opat は '0' か '1' のどちらか
  virtual
  bool
  names(const vector<ymuint32>& name_id_array,
	ymuint32 nc,
	const char* cover_pat,
	char opat);

  /// @brief .gate 文の開始
  /// @param[in] loc1 .gate の位置情報
  /// @param[in] loc2 セル名の位置情報
  /// @param[in] name セル名
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  gate_begin(const FileRegion& loc1,
	     const FileRegion& loc2,
	     const char* name);

  /// @brief .gate 文中のピン割り当ての処理
  /// @param[in] loc1 ピン名の位置情報
  /// @param[in] f_name ピン名
  /// @param[in] loc2 ノード名の位置情報
  /// @param[in] a_name ノード名
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  gate_assign(const FileRegion& loc1,
	      const char* f_name,
	      const FileRegion& loc2,
	      const char* a_name);

  /// @brief .gate 文の終了
  virtual
  bool
  gate_end();

  /// @brief .latch 文の処理
  /// @param[in] name1_id 最初の識別子のID番号
  /// @param[in] name2_id 次の識別子のID番号
  /// @param[in] loc4 リセット値の位置情報
  /// @param[in] rval リセット時の値('0'/'1') 未定義なら ' '
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  latch(ymuint32 name1_id,
	ymuint32 name2_id,
	const FileRegion& loc4,
	char rval);

  /// @brief .end 文の処理
  /// @param[in] loc 位置情報
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  end(const FileRegion& loc);

  /// @brief 通常終了時の処理
  virtual
  void
  normal_exit();

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit();


private:

  // 可能なら接続する．
  // できれなれば mConList に積む
  void
  connect(TgNode* from,
	  TgNode* to,
	  size_t pos);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Connection
  {
    TgNode* mFrom;
    TgNode* mTo;
    size_t mPos;

    Connection(TgNode* from,
	       TgNode* to,
	       size_t pos) :
      mFrom(from),
      mTo(to),
      mPos(pos) { }
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の TgNetwork
  TgNetwork* mNetwork;

  // cubepat2expr で用いる作業領域
  vector<LogExpr> mChd1;
  vector<LogExpr> mChd2;

  // 後で接続する要素のリスト
  vector<Connection> mConList;

  // .names 文の処理で用いる作業領域
  vector<TgNode*> mCurFanins;

};

END_NAMESPACE_YM_NETWORKS

#endif // LIBYM_NETWORKS_TGBLIFHANDLER_H
