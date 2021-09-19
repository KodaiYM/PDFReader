namespace PDFReader

open Microsoft.WindowsAPICodePack.Dialogs
open System.ComponentModel
open System.Collections.ObjectModel
open System.Diagnostics

type PagesViewModel() =
   //   let mutable _Text = "Hello, PDF Parser Test Program!"
   //   let PropertyChanged = Event<_, _>()

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

      this.Pages.Add(PDFParser.PDFPage(Width = 200.0, Height = 400.0))
      this.Pages.Add(PDFParser.PDFPage(Width = 595.0, Height = 842.0))

   member this.ToDefaultUserSpace point = point / 72.0 * 96.0
(*
   interface INotifyPropertyChanged with
      [<CLIEventAttribute>]
      member this.PropertyChanged = PropertyChanged.Publish

   member this.Text
      with get () = _Text
      and set (newText) =
         _Text <- newText
         PropertyChanged.Trigger(this, PropertyChangedEventArgs("Text"))
         *)
