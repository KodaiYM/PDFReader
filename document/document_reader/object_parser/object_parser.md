# ipdfstream
## 概要
| クラス名 | 概要 |
| --- | --- |
| pdfparser::ipdfstream | ストリームの読み取り/操作、解析を行う |

## 関連クラス
[トークン単位解析クラス](tokenizer/tokenizer.md)

## 使用例
- take_footer でトレーラー辞書を取得したり、[クロスリファレンステーブル](xref_types/xref_types.md)を構築したりする
- take_object でPDFの[オブジェクト](object_types/object_types.md)を取得する

## クラス図一覧
- [オブジェクト解析クラス図](ipdfstream.class.pu)
- [オブジェクト取得エラークラス図](object_not_found_error.class.pu)
- [オブジェクト解析構文エラークラス図](parse_error.class.pu)

## フローチャート
- [オブジェクト解析アクティビティ図](ipdfstream.activity.pu)
