#ifndef TCLPP_TCLOBJMSGHANDLER_H
#define TCLPP_TCLOBJMSGHANDLER_H

/// @file tclpp/TclObjMsgHandler.h
/// @brief TclObjMsgHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "tclpp/tclpp_nsdef.h"
#include "utils/MsgHandler.h"
#include "tclpp/TclObj.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
/// @class TclObjMsgHandler TclObjMsgHandler.h "tclpp/TclObjMsgHandler.h"
/// @brief TclObj に出力するメッセージハンドラ
//////////////////////////////////////////////////////////////////////
class TclObjMsgHandler :
  public MsgHandler
{
public:

  /// @brief コンストラクタ
  TclObjMsgHandler();

  /// @brief デストラクタ
  virtual
  ~TclObjMsgHandler();


public:

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] loc ファイル位置
  /// @param[in] type メッセージタイプ
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& loc,
	  tMsgType type,
	  const char* label,
	  const char* body);

  /// @brief 内容をクリアする．
  void
  clear();


public:

  /// @brief メッセージオブジェクトを取り出す．
  TclObj
  msg_obj() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  TclObj mMsg;

};

END_NAMESPACE_YM_TCLPP

#endif //  TCLPP_TCLOBJMSGHANDLER_H