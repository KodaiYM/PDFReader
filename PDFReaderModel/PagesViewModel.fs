namespace PDFReader

open Microsoft.WindowsAPICodePack.Dialogs
open System.ComponentModel
open System.Collections.ObjectModel
open System.Diagnostics

type PagesViewModel() =
   member val Pages = ObservableCollection<PDFParser.PDFPage>()

   member this.LoadPages() =
      let dialog = new CommonOpenFileDialog()
      dialog.Title <- "test"
      dialog.IsFolderPicker <- false

      if dialog.ShowDialog() = CommonFileDialogResult.Ok then
         let pages =
            PDFParser.PagesModel.GetPages dialog.FileName

         for page in pages do
            this.Pages.Add
            <| PDFParser.PDFPage(
               Width = this.ToDefaultUserSpace page.Width,
               Height = this.ToDefaultUserSpace page.Height
            )

   member this.ToDefaultUserSpace point = point / 72.0 * 96.0
