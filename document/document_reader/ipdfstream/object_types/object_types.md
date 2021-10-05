# object_types
## オブジェクト
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

## クラス図一覧
- [オブジェクトタイプクラス図](object_types.class.pu)
