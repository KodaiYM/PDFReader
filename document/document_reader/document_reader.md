# document_reader
## 概要
| クラス名 | 概要 |
| --- | --- |
| pdfparser::document_reader | ドキュメントのページ作成などを管理する |
| pdfparser::document_error  | ドキュメントの構文上のエラーを表す |

## 使用例
- GetPages を呼び出すと、[オブジェクトプール](object_pool/object_pool.md)を使って間接オブジェクトにアクセスしながら、[ファイル解析](document_parser/document_parser.md)を行い、[PDFの各ページを構築する](PDFPage_builder/PDFPage_builder.md)。

## クラス図一覧
- [ドキュメント解析クラス図](document_reader.class.pu)
- [ドキュメントエラークラス図](document_error.class.pu)

## フローチャート
- [ドキュメント解析アクティビティ図](document_reader.activity.pu)
