# これは誰のためのドキュメントか
この git リポジトリ上で開発する開発者のためのドキュメント

# 使用ツール
- Visual Studio
- Git
- Github
- PlantUML

# 使用言語(予定)
- C#
- F#
- C++/CLI

# API
- WPF on .NET Core 3.1

# 開発手順
1. Github の Issues に長くても1～2週間でできそうな要件定義(やりたいこと)を書く
2. Issue に応じて作業用ブランチを作成する
	- git checkout -b \*/*
	- コア機能の実装の場合: core/*
	- 機能の実装の場合: features/*
	- バグ修正の場合: bugfix/*
	- プロジェクト管理の場合: management/*
3. [バージョン](/version)更新
	- x.y.z の形式
	- x: 安定したリリース
	- y: 細かい機能の追加、修正
	- z: バグ修正
4. [基本設計書](/document/outline.md)作成
5. [詳細設計書](/document/)作成
6. [結合テストプログラム](/test/IntegrationTest/)作成
7. [単体テストプログラム](/test/UnitTest/)作成
8. プログラム作成（後のことは考えず、今必要な機能のみを考えて作る）
9. 6 のテスト
10. 4 のテスト
11. プルリクエスト&マージ
12. ブランチを削除
