# document_parser.cpp
## 概要
| クラス名 | 概要 |
| --- | --- |
| pdfparser::document_parser | ストリームの読み取り/操作、解析を行う |
| pdfparser::object_pool | 間接オブジェクトを保持し、オブジェクトへのアクセスを管理する |

## 用語
### 文字
| 用語 | 意味 |
| ---- | ---- |
| 空白類文字 | null(00), 水平タブ(09), 改行(0A), 改ページ(0C), 行頭復帰(0D), 20(空白) |
| 区切り文字 | (, ), <, >, [, ], {, }, /, % |
| 標準文字 | その他の文字 |

### 文字列
| 用語 | 意味 |
| ---- | ---- |
| 改行(EOL) | 0C0Aの2バイト, または, 0C または 0A |
| コメント | %から改行まで(改行を除く) |
| オブジェクト | PDFにおいて特定の意味をもつ文字列 |

### オブジェクト
| オブジェクト名 | 構文 | 意味 |
| -------------- | ---- | ---- |
| Literal String | (文字列) | そのまま文字列を表す |
| Hexadecimal String | <16進文字列> | 16進法で文字列を表す |
| Name | /名前 | 名前 |
| Array | [オブジェクト1 オブジェクト2 ...] | オブジェクトの配列 |
| Dictionary | \<\</Key1 /Value1 /Key2 /Value2\>\> | Key-Value store 型の順序のない辞書 |
| Stream | dictionary <br> "stream" <br> ストリーム <br> "endstream" | 画像などのバイナリデータをストリームに表現できる。dictionary はストリームに関する情報を保持する。 |
| Indirec Object | オブジェクト番号(整数) 世代番号(整数) "obj" <br> オブジェクト <br> "endobj" | 任意のオブジェクトに対して、(オブジェクト番号, 世代番号)の組で一意な識別を可能にする |
| Indirect Reference | オブジェクト番号(整数) 世代番号(整数) "R" | (オブジェクト番号, 世代番号)の組に対応する Indirect オブジェクトへの参照 |

## 公開クラス一覧
- [オブジェクト解析クラス図](document_parser.class.pu)
- [トークン解析クラス図](tokenizer.class.pu)
- [ストリーム機能拡張クラス図](istream_extended.class.pu)
- [オブジェクトプールクラス図](object_pool.class.pu)

## 主要フローチャート
- [オブジェクト解析クラス図](document_parser.activity.pu)
- [トークン解析クラス図](tokenizer.activity.pu)
- [ストリーム機能拡張クラス図](istream_extended.activity.pu)
- [オブジェクトプールクラス図](object_pool.activity.pu)
