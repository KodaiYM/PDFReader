using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Input;

namespace ExportToImage.Commands {
	public class OpenPDFCommand: ICommand {
		void ICommand.Execute(object parameter) {
			var dialog =
			    new Microsoft.Win32.OpenFileDialog { Filter = "PDF ファイル|*.pdf" };
			bool? dialog_result = dialog.ShowDialog();
			if (dialog_result == false) {
				return;
			}

			_viewModel.PDFPath = dialog.FileName;
			((ICommand)_viewModel.ResetPreviews).Execute(null);
		}

		public OpenPDFCommand(MainPageViewModel viewModel) {
			_viewModel = viewModel;
		}
		public event EventHandler CanExecuteChanged;
		bool                      ICommand.CanExecute(object parameter) {
      return true;
		}

		private readonly MainPageViewModel _viewModel;
	}
}
