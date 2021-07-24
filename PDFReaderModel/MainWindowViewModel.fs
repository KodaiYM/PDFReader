module Core.ViewModels

open Microsoft.WindowsAPICodePack.Dialogs
open System.ComponentModel

type MainWindowViewModel() =
   let mutable _Text = "Hello, PDF Parser Test Program!"
   let PropertyChanged = Event<_, _>()

   interface INotifyPropertyChanged with
      [<CLIEventAttribute>]
      member this.PropertyChanged = PropertyChanged.Publish

   member this.Text
      with get () = _Text
      and set (newText) =
         _Text <- newText
         PropertyChanged.Trigger(this, PropertyChangedEventArgs("Text"))

let ParsedXrefTable () =
   let dialog = new CommonOpenFileDialog()
   dialog.Title <- "test"
   dialog.IsFolderPicker <- false

   if dialog.ShowDialog() = CommonFileDialogResult.Ok then
      pdfparser.parser_tostring.get dialog.FileName
   else
      ""
