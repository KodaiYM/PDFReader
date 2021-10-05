# document_reader
## 概要
| クラス名 | 概要 |
| --- | --- |
| pdfparser::document_reader | ドキュメントのページ作成などを管理する |
| pdfparser::document_error  | ドキュメントの構文上のエラーを表す |

## 使用例
- get_pages を呼び出すと、[ファイル解析クラス](ipdfstream/ipdfstream.md)や[オブジェクトプールクラス](object_pool/object_pool.md)を使い、[PDFの全ページを解析](page_tree_parser/page_tree_parser.md)する。

## クラス図一覧
- [ドキュメント解析クラス図](document_reader.class.pu)
- [ドキュメントエラークラス図](document_error.class.pu)

## フローチャート
- [ドキュメント解析アクティビティ図](document_reader.activity.pu)
