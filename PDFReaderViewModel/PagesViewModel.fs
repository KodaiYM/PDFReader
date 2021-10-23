namespace PDFReader

open Microsoft.WindowsAPICodePack.Dialogs
open System.ComponentModel
open System.Collections.ObjectModel
open System.Diagnostics
open System.Windows

type PagesViewModel() =
   member val Pages = ObservableCollection<PDFReader.PDFPage>()

   member this.LoadPages() =
      let dialog = new CommonOpenFileDialog()
      dialog.Title <- "test"
      dialog.IsFolderPicker <- false

      if dialog.ShowDialog() = CommonFileDialogResult.Ok then
         let pages =
            try
               PDFReader.PagesModel.GetPages dialog.FileName
            with
            | :? PDFReader.PDFParserError as e ->
               let caption = "エラー"

               let message =
                  "ドキュメントの読み込み中にエラーが発生しました。\n\n" + e.Message

               let button = MessageBoxButton.OK
               let icon = MessageBoxImage.Error

               MessageBox.Show(message, caption, button, icon)
               |> ignore

               exit (1)

         for page in pages do
            this.Pages.Add
            <| PDFReader.PDFPage(
               Width = this.ToDefaultUserSpace page.Width,
               Height = this.ToDefaultUserSpace page.Height
            )

   member this.ToDefaultUserSpace point = point / 72.0 * 96.0
