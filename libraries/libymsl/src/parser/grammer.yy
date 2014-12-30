%{

/// @file grammer.yy
/// @brief grammer の構文解析器
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

// C++ 用の定義

#include "YmUtils/FileRegion.h"
#include "YmUtils/MsgMgr.h"

#include "YmslParser.h"

#include "AstBlock.h"
#include "AstExpr.h"
#include "AstVarDecl.h"

#include "../src/parser/AstSymbol.h"

// より詳細なエラー情報を出力させる．
#define YYERROR_VERBOSE 1

// 位置を表す型
// (yylloc の型)
#define YYLTYPE FileRegion

// YYLTYPE を書き換えたので以下のマクロも書き換えが必要
#define YYLLOC_DEFAULT(Current, Rhs, N) Current = fr_merge(Rhs, N);


BEGIN_NAMESPACE_YM_YMSL

// 字句解析関数
int
yylex(YYSTYPE*,
      YYLTYPE*,
      YmslParser&);

// エラー報告関数
int
yyerror(YYLTYPE*,
	YmslParser&,
	const char*);


// fr_array 全体のファイル領域を求める．
// 直感的には FileRegion(fr_array[1], fr_array[n])
// だが(先頭が 0 でなく 1 であることに注意),
// 場合によっては空のトークンで位置がないばあいがあるので
// それをスキップしなければならない．
inline
FileRegion
fr_merge(const FileRegion fr_array[],
	 ymuint n)
{
  if ( n == 0 ) {
    // なんでこんなことがあるのか不明
    return FileRegion();
  }

  // 真の先頭を求める．
  ymuint i;
  for (i = 1; i <= n && !fr_array[i].is_valid(); ++ i) ;
  const FileRegion& first = fr_array[i];

  // 真の末尾を求める．
  ymuint j;
  for (j = n; j >= i && !fr_array[j].is_valid(); -- j) ;
  const FileRegion& last = fr_array[j];

  return FileRegion(first, last);
}

%}

// Yacc 用の定義

// "pure" driver にする．
%define api.pure

// 位置のトラッキングを行う．
%locations

// yyparse の引数
%parse-param {YmslParser& parser}

// yylex の引数
%lex-param {YmslParser& parser}

// 値を表す型
%union {
  TokenType     token_type;
  AstBlock*     block_type;
  AstCaseItem*  caseitem_type;
  AstExpr*      expr_type;
  AstIfBlock*   ifblock_type;
  AstPrimary*   primary_type;
  AstStatement* statement_type;
  AstSymbol*    symbol_type;
  AstVarDecl*   vardecl_type;
  AstValueType* valuetype_type;
}


// 終端記号
%token COLON
%token SEMI
%token COMMA
%token DOT
%token QST
%token PLUSPLUS
%token MINUSMINUS
%token EQ
%token EQPLUS
%token EQMINUS
%token EQMULT
%token EQDIV
%token EQMOD
%token EQLSHIFT
%token EQRSHIFT
%token EQBITAND
%token EQBITOR
%token EQBITXOR
%token LP
%token RP
%token LCB
%token RCB
%token LBK
%token RBK

%token AS
%token BOOLEAN
%token BREAK
%token CASE
%token CONTINUE
%token DEFAULT
%token DO
%token ELIF
%token ELSE
%token FLOAT
%token FOR
%token FUNCTION
%token GLOBAL
%token GOTO
%token IF
%token IMPORT
%token INT
%token RETURN
%token STRING
%token SWITCH
%token VAR
%token VOID
%token WHILE

%token <symbol_type> SYMBOL
%token <expr_type>   INT_VAL
%token <expr_type>   FLOAT_VAL
%token <expr_type>   STRING_VAL

%token DUMMY
%token ERROR

// 演算子の定義
%nonassoc QST
%nonassoc ITE
%left     LOGOR
%left     LOGAND
%nonassoc LSHIFT RSHIFT
%left     BITOR BITXOR
%left     BITAND
%nonassoc EQEQ NOTEQ LT LE GT GE
%left     PLUS MINUS
%left     MULT DIV MOD
%left     LOGNOT BITNEG UOP

// 非終端の型
%type <block_type>     statement_block
%type <caseitem_type>  case_list
%type <expr_type>      expr
%type <expr_type>      expr_list
%type <expr_type>      init_expr
%type <primary_type>   lvalue
%type <ifblock_type>   if_list
%type <ifblock_type>   if_else_list
%type <statement_type> block_statement
%type <statement_type> single_statement
%type <statement_type> statement
%type <symbol_type>    identifier
%type <token_type>     eqop
%type <valuetype_type> type
%type <vardecl_type>   param param_list

%%

// 本体

%start item_list;

// 要素のリスト
item_list
: // 空
| item_list item
;

// トップレベルの要素
// ステートメントか関数定義
item
: statement
{
  parser.add_statement($1);
}
// 関数定義
| func_head SYMBOL LP param_list RP COLON type LCB statement_list RCB
{
  AstFuncDecl* funcdecl = parser.new_FuncDecl($2, $7, $4, @$);
  parser.pop_block();
  parser.add_function(funcdecl);
}
// グローバル変数定義
| GLOBAL SYMBOL COLON type init_expr SEMI
{
  AstVarDecl* vardecl = parser.new_VarDecl($2, $4, $5, true, @$);
  parser.add_global_var(vardecl);
}
// import 文
| import_stmt
;

// import 文
import_stmt
: IMPORT SYMBOL
{
  parser.import($2, NULL);
}
| IMPORT SYMBOL AS SYMBOL
{
  parser.import($2, $4);
}
| import_stmt COMMA SYMBOL
{
  parser.import($3, NULL);
}
| import_stmt COMMA SYMBOL AS SYMBOL
{
  parser.import($3, $5);
}
;

// 関数宣言のヘッダ
func_head
: FUNCTION
{
  parser.push_new_block();
}
;

// ステートメント
statement
// 単一のステートメント
: single_statement SEMI
{
  $$ = $1;
}
// ブロックステートメント
| block_statement
{
  $$ = $1;
}
// ローカル変数定義
| VAR SYMBOL COLON type init_expr SEMI
{
  AstVarDecl* vd = parser.new_VarDecl($2, $4, $5, false, @$);
  parser.add_local_var(vd);
}
// ラベル文
| SYMBOL COLON
{
  $$ = parser.new_Label($1, @$);
}
// エラー回復用のルール
| error SEMI
{
  yyerrok;
}
;

single_statement
// 代入文
: lvalue eqop expr
{
  $$ = parser.new_Assignment($2, $1, $3);
}
// 式文
| expr
{
  $$ = parser.new_ExprStmt($1);
}
// インクリメント文
| lvalue PLUSPLUS
{
}
// デクリメント文
| lvalue MINUSMINUS
{
}
// GOTO 文
| GOTO SYMBOL
{
  $$ = parser.new_Goto($2, @$);
}
// BREAK 文
| BREAK
{
  $$ = parser.new_Break(@$);
}
// CONTINUE 文
| CONTINUE
{
  $$ = parser.new_Continue(@$);
}
// RETURN 文(引数なし)
| RETURN
{
  $$ = parser.new_Return(NULL, @$);
}
// RETURN 文(引数あり)
| RETURN expr
{
  $$ = parser.new_Return($2, @$);
}
;

// 代入文の演算子
// なんかバカみたいなアクション定義
eqop
: EQ       { $$ = EQ; }
| EQPLUS   { $$ = EQPLUS; }
| EQMINUS  { $$ = EQMINUS; }
| EQMULT   { $$ = EQMULT; }
| EQDIV    { $$ = EQDIV; }
| EQMOD    { $$ = EQMOD; }
| EQLSHIFT { $$ = EQLSHIFT; }
| EQRSHIFT { $$ = EQRSHIFT; }
| EQBITAND { $$ = EQBITAND; }
| EQBITOR  { $$ = EQBITOR; }
| EQBITXOR { $$ = EQBITXOR; }
;

// 複合文
// というか末尾にセミコロンがこない文
block_statement
// IF 文
: if_else_list
{
  $$ = parser.new_If($1, @$);
}
// FOR 文
| FOR LP single_statement SEMI expr SEMI single_statement RP statement_block
{
  $$ = parser.new_For($3, $5, $7, $9, @$);
}
// WHILE 文
| WHILE LP expr RP statement_block
{
  $$ = parser.new_While($3, $5, @$);
}
// DO-WHILE 文
| DO statement_block WHILE LP expr RP
{
  $$ = parser.new_DoWhile($2, $5, @$);
}
// SWITCH 文
| SWITCH expr LCB case_list RCB
{
  $$ = parser.new_Switch($2, $4, @$);
}
// ブロック文
| statement_block
{
  $$ = parser.new_BlockStmt($1, @$);
}
;

// ブロック
statement_block
: stmt_begin statement_list RCB
{
  $$ = parser.cur_block();
  parser.pop_block();
}
;

// ブロック文の開始
// わざと非終端ノードを導入したのは
// 本体よりも先にアクションを起こすため．
stmt_begin
: LCB
{
  parser.push_new_block();
}
;

statement_list
: // 空
| statement_list statement
{
  parser.add_statement($2);
}
;

// if 〜 elif 〜 のリスト
if_list
: IF expr statement_block
{
  $$ = parser.new_IfBlock(NULL, $2, $3, @$);
}
| if_list ELIF expr statement_block
{
  $$ = parser.new_IfBlock($1, $3, $4, FileRegion(@2, @4));
}
;

// if_list の末尾にオプションで else 〜 がくるもの
if_else_list
: if_list
{
  $$ = $1;
}
| if_list ELSE statement_block
{
  $$ = parser.new_IfBlock($1, NULL, $3, FileRegion(@2, @3));
}
;

case_list
: // 空
{
  $$ = NULL;
}
| case_list CASE expr COLON statement_block
{
  $$ = parser.new_CaseItem($1, $3, $5, FileRegion(@2, @5));
}
| case_list DEFAULT COLON statement_block
{
  $$ = parser.new_CaseItem($1, NULL, $4, FileRegion(@2, @4));
}
;

// 左辺値
lvalue
: identifier
{
  $$ = parser.new_Primary($1);
  if ( $$ == NULL ) {
    YYERROR;
  }
}
/*
| identifier LBK expr RBK
{
  $$ = parser.new_ArrayRef($1, $3, @$);
}
*/
;

// 関数定義の引数リスト
param_list
: // 空
{
  $$ = NULL;
}
| param
{
  $$ = $1;
}
| param_list COMMA param
{
  $$ = $3;
  $$->set_prev($1);
}
;

// パラメータ宣言
param
: SYMBOL COLON type init_expr
{
  $$ = parser.new_VarDecl($1, $3, $4, false, @$);
}
;

// データ型
type
: VOID
{
  $$ = parser.new_VoidType(@$);
}
| BOOLEAN
{
  $$ = parser.new_BooleanType(@$);
}
| INT
{
  $$ = parser.new_IntType(@$);
}
| FLOAT
{
  $$ = parser.new_FloatType(@$);
}
| STRING
{
  $$ = parser.new_StringType(@$);
}
| SYMBOL
{
  $$ = parser.new_UserType($1);
}
;

init_expr
: // 空
{
  $$ = NULL;
}
| EQ expr
{
  $$ = $2;
}
;

expr
// 単項演算
: PLUS expr %prec UOP
{
  $$ = parser.new_UniOp(PLUS, $2, @$);
}
| MINUS expr %prec UOP
{
  $$ = parser.new_UniOp(MINUS, $2, @$);
}
| BITNEG expr
{
  $$ = parser.new_UniOp(BITNEG, $2, @$);
}
| LOGNOT expr
{
  $$ = parser.new_UniOp(LOGNOT, $2, @$);
}
| INT LP expr RP
{
  $$ = parser.new_UniOp(INT, $3, @$);
}
| BOOLEAN LP expr RP
{
  $$ = parser.new_UniOp(BOOLEAN, $3, @$);
}
| FLOAT LP expr RP
{
  $$ = parser.new_UniOp(FLOAT, $3, @$);
}
// 二項演算
| expr PLUS expr
{
  $$ = parser.new_BinOp(PLUS, $1, $3);
}
| expr MINUS expr
{
  $$ = parser.new_BinOp(MINUS, $1, $3);
}
| expr MULT expr
{
  $$ = parser.new_BinOp(MULT, $1, $3);
}
| expr DIV expr
{
  $$ = parser.new_BinOp(DIV, $1, $3);
}
| expr MOD expr
{
  $$ = parser.new_BinOp(MOD, $1, $3);
}
| expr LSHIFT expr
{
  $$ = parser.new_BinOp(LSHIFT, $1, $3);
}
| expr RSHIFT expr
{
  $$ = parser.new_BinOp(RSHIFT, $1, $3);
}
| expr BITAND expr
{
  $$ = parser.new_BinOp(BITAND, $1, $3);
}
| expr BITOR expr
{
  $$ = parser.new_BinOp(BITOR, $1, $3);
}
| expr BITXOR expr
{
  $$ = parser.new_BinOp(BITXOR, $1, $3);
}
| expr LOGAND expr
{
  $$ = parser.new_BinOp(LOGAND, $1, $3);
}
| expr LOGOR expr
{
  $$ = parser.new_BinOp(LOGOR, $1, $3);
}
| expr EQEQ expr
{
  $$ = parser.new_BinOp(EQEQ, $1, $3);
}
| expr NOTEQ expr
{
  $$ = parser.new_BinOp(NOTEQ, $1, $3);
}
| expr LT expr
{
  $$ = parser.new_BinOp(LT, $1, $3);
}
| expr GT expr
{
  $$ = parser.new_BinOp(GT, $1, $3);
}
| expr LE expr
{
  $$ = parser.new_BinOp(LE, $1, $3);
}
| expr GE expr
{
  $$ = parser.new_BinOp(GE, $1, $3);
}
| expr QST expr COLON expr %prec ITE
{
  $$ = parser.new_IteOp($1, $3, $5);
}
| LP expr RP
{
  $$ = $2;
}
| identifier
{
  $$ = parser.new_VarExpr($1);
  if ( $$ == NULL ) {
    YYERROR;
  }
}
/*
| identifier LBK expr RBK
{
  $$ = parser.new_ArrayRef($1, $3, @$);
}
*/
| identifier LP expr_list RP
{
  $$ = parser.new_FuncCall($1, $3, @$);
}
| INT_VAL
{
  $$ = $1;
}
| FLOAT_VAL
{
  $$ = $1;
}
| STRING_VAL
{
  $$ = $1;
}
;

// 式のリスト
expr_list
: // 空
{
  $$ = NULL;
}
| expr
{
  $$ = $1;
}
| expr_list COMMA expr
{
  $$ = $3;
  $$->set_prev($1);
}
;

// 識別子
identifier
: SYMBOL
{
  $$ = $1;
}
| identifier DOT SYMBOL
{
  $$ = $1;
  $$->set_next($1);
}
;

%%

// C++ の記述

// yacc パーサーが内部で呼び出す関数
// トークンを一つ読み込みトークン番号を返す．
// 同時に意味値(semantic value)を yylval にセットする．
// また位置を yylloc にセットする．
int
yylex(YYSTYPE* lvalp,
      YYLTYPE* llocp,
      YmslParser& parser)
{
  return parser.scan(*lvalp, *llocp);
}

// yacc パーサーが内部で呼び出す関数
// エラーメッセージを出力する．
int
yyerror(YYLTYPE* llocp,
	YmslParser& parser,
	const char* s)
{
  string s2;
  // 好みの問題だけど "parse error" よりは "syntax error" の方が好き．
  if ( !strncmp(s, "parse error", 11) ) {
    s2 ="syntax error";
    s2 += (s + 11);
  }
  else {
    s2 = s;
  }

  MsgMgr::put_msg(__FILE__, __LINE__,
		  *llocp,
		  kMsgError,
		  "PARS",
		  s2);

  return 1;
}

END_NAMESPACE_YM_YMSL
