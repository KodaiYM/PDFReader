# parse.cpp
## namespace
pdfparser::concrete_syntax_tree

## 概要
PDFの構文に従って、文字列を親子関係を持つ具象構文木に変換する。

## ノードの種類
| クラス名 | 説明 | 
| -------- | ---- |
| white_space_characters | 空白類文字列 |
| regular_characters | 標準文字列 |
| delimiter_character | 区切り文字(括弧など) |
| eol | 改行 |
| comment | コメント(%～改行まで) |
| literal_string | (文字列) |
| hexadecimal_string | <16進文字列> |
| name | /名前 |
| array | [要素1 要素2 ...] |
| dictionary | \<\</Key1 /Value1 /Key2 /Value2\>\> |
| stream | dictionary <br> "stream" <br> ストリーム <br> "endstream"
| indirect_object | オブジェクト番号(整数) 世代番号(整数) "obj" <br> オブジェクト <br> "endobj"
| indirect_reference | オブジェクト番号(整数) 世代番号(整数) "R" |


## クラス一覧
[クラス図](concrete_syntax_tree.class-diagram.md)

## 関数一覧
| 関数名 | 引数 | 戻り値 |
| ------ | ---- | -----  |
| template\<class InputIterator\> parse | InputIterator first, InputIterator last | [first, last) の範囲を具象構文木に変換して concrete_syntax_tree::root で root を返却 |

### フローチャート
[アクティビティ図](concrete_syntax_tree.activity-diagram.pu)
