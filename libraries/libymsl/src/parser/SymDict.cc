
/// @file SymDict.cc
/// @brief SymDict の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SymDict.h"
#include "YmslFunction.h"
#include "YmslVar.h"
#include "YmslSubspace.h"
#include "YmslLabel.h"
#include "SymHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス SymDict
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親
SymDict::SymDict(SymDict* parent) :
  mParent(parent),
  mHashSize(0),
  mHashTable(NULL),
  mHashNum(0)
{
  alloc_table(16);
}

// @brief デストラクタ
SymDict::~SymDict()
{
}

// @brief ラベルを追加する．
void
SymDict::add_label(YmslLabel* label)
{
  SymHandle* handle = new LabelHandle(label);
  put(handle);
}

// @brief 関数を追加する．
// @param[in] item 追加する要素
void
SymDict::add_function(YmslFunction* item)
{
  SymHandle* handle = new FuncHandle(item);
  put(handle);
}

// @brief 変数を追加する．
// @param[in] item 追加する要素
void
SymDict::add_var(YmslVar* item)
{
  SymHandle* handle = new VarHandle(item);
  put(handle);
}

// @brief 名前空間を追加する．
// @param[in] item 追加する要素
void
SymDict::add_namespace(YmslSubspace* item)
{
  SymHandle* handle = new SubspaceHandle(item);
  put(handle);
}

// @brief 名前からラベルを見つける．
// @param[in] name 名前
//
// ここになければ親のブロックを探す．
// それでもなければ NULL を返す．
YmslLabel*
SymDict::find_label(ShString name) const
{
  SymHandle* handle = find(name);
  if ( handle != NULL ) {
    return handle->label();
  }
  return NULL;
}

// @brief 名前から関数を見つける．
// @param[in] name 名前
//
// ここになければ親のブロックを探す．
// それでもなければ NULL を返す．
YmslFunction*
SymDict::find_function(ShString name) const
{
  SymHandle* handle = find(name);
  if ( handle != NULL ) {
    return handle->function();
  }
  return NULL;
}

// @brief 名前から変数を見つける．
// @param[in] name 名前
//
// ここになければ親のブロックを探す．
// それでもなければ NULL を返す．
YmslVar*
SymDict::find_var(ShString name) const
{
  SymHandle* handle = find(name);
  if ( handle != NULL ) {
    return handle->var();
  }
  return NULL;
}

// @brief 名前から名前空間を探す．
// @param[in] name 名前
//
// ここになければ親のブロックを探す．
// それでもなければ NULL を返す．
YmslSubspace*
SymDict::find_subspace(ShString name) const
{
  SymHandle* handle = find(name);
  if ( handle != NULL ) {
    return handle->subspace();
  }
  return NULL;
}

#if 0
// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
SymDict::print(ostream& s,
		ymuint indent) const
{
#if 0
  YmslAstImpl::print_indent(s, indent);
  s << "{" << endl;

  ymuint n = mStatementList.size();
  for (ymuint i = 0; i < n; ++ i) {
    mStatementList[i]->print(s, indent + 1);
  }

  YmslAstImpl::print_indent(s, indent);
  s << "}" << endl;
#endif
}
#endif

// @brief ハッシュ表を確保する．
void
SymDict::alloc_table(ymuint req_size)
{
  ymuint old_size = mHashSize;
  SymHandle** old_table = mHashTable;
  mHashSize = req_size;
  mNextLimit = static_cast<ymuint>(mHashSize * 1.8);
  mHashTable = new SymHandle*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  for (ymuint i = 0; i < old_size; ++ i) {
    for (SymHandle* handle = old_table[i];
	 handle != NULL; ) {
      SymHandle* next = handle->mLink;
      _put(handle);
      handle = next;
    }
  }
  delete [] old_table;
}

// @brief 名前からハンドルを探す．
// @param[in] name 名前
SymHandle*
SymDict::find(ShString name) const
{
  ymuint pos = name.hash() % mHashSize;
  for (SymHandle* handle = mHashTable[pos];
       handle != NULL; handle = handle->mLink) {
    if ( handle->name() == name ) {
      return handle;
    }
  }
  if ( mParent != NULL ) {
    return mParent->find(name);
  }
  return NULL;
}

// @brief ハンドルを登録する．
void
SymDict::put(SymHandle* handle)
{
  if ( mHashNum >= mNextLimit ) {
    alloc_table(mHashSize << 1);
  }

  _put(handle);
}

// @brief ハンドルを登録する．
//
// こちらはサイズチェックなし
void
SymDict::_put(SymHandle* handle)
{
  ymuint pos = handle->name().hash() % mHashSize;
  handle->mLink = mHashTable[pos];
  mHashTable[pos] = handle;
  ++ mHashNum;
}


//////////////////////////////////////////////////////////////////////
// クラス SymHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SymHandle::SymHandle() :
  mLink(NULL)
{
}

// @brief デストラクタ
SymHandle::~SymHandle()
{
}

// @brief 変数を返す．
//
// 他の要素の場合には NULL を返す．
YmslVar*
SymHandle::var() const
{
  return NULL;
}

// @brief 関数を返す．
//
// 他の要素の場合には NULL を返す．
YmslFunction*
SymHandle::function() const
{
  return NULL;
}

// @brief ラベルを返す．
//
// 他の要素の場合には NULL を返す．
YmslLabel*
SymHandle::label() const
{
  return NULL;
}

// @brief 名前空間を返す．
//
// 他の要素の場合には NULL を返す．
YmslSubspace*
SymHandle::subspace() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス VarHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vardecl 変数宣言
VarHandle::VarHandle(YmslVar* var) :
  mVar(var)
{
}

// @brief デストラクタ
VarHandle::~VarHandle()
{
}

// @brief 名前を返す．
ShString
VarHandle::name() const
{
  return mVar->name();
}

// @brief 変数宣言を返す．
//
// 他の要素の場合には NULL を返す．
YmslVar*
VarHandle::var() const
{
  return mVar;
}


//////////////////////////////////////////////////////////////////////
// クラス FuncHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func 関数
FuncHandle::FuncHandle(YmslFunction* func) :
  mFunc(func)
{
}

// @brief デストラクタ
FuncHandle::~FuncHandle()
{
}

// @brief 名前を返す．
ShString
FuncHandle::name() const
{
  return mFunc->name();
}

// @brief 関数を返す．
//
// 他の要素の場合には NULL を返す．
YmslFunction*
FuncHandle::function() const
{
  return mFunc;
}


//////////////////////////////////////////////////////////////////////
// クラス LabelHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
LabelHandle::LabelHandle(YmslLabel* label) :
  mLabel(label)
{
}

// @brief デストラクタ
LabelHandle::~LabelHandle()
{
}

// @brief 名前を返す．
ShString
LabelHandle::name() const
{
  return mLabel->name();
}

// @brief ラベルを返す．
//
// 他の要素の場合には NULL を返す．
YmslLabel*
LabelHandle::label() const
{
  return mLabel;
}


//////////////////////////////////////////////////////////////////////
// クラス SubspaceHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] subspace 名前空間
SubspaceHandle::SubspaceHandle(YmslSubspace* subspace) :
  mSubspace(subspace)
{
}

// @brief デストラクタ
SubspaceHandle::~SubspaceHandle()
{
}

// @brief 名前を返す．
ShString
SubspaceHandle::name() const
{
  return mSubspace->name();
}

// @brief 名前空間を返す．
//
// 他の要素の場合には NULL を返す．
YmslSubspace*
SubspaceHandle::subspace() const
{
  return mSubspace;
}

END_NAMESPACE_YM_YMSL