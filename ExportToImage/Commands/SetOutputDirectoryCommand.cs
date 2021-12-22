using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Input;
using Microsoft.WindowsAPICodePack.Dialogs;

namespace ExportToImage.Commands {
	public class SetOutputDirectoryCommand: ICommand {
		void ICommand.Execute(object parameter) {
			using var dialog = new CommonOpenFileDialog { IsFolderPicker = true };
			var       result = dialog.ShowDialog();
			if (result != CommonFileDialogResult.Ok) {
				return;
			}

			vm.OutputDirectoryPath = dialog.FileName;
		}

		public SetOutputDirectoryCommand(MainPageViewModel viewModel) {
			vm = viewModel;
		}
		public event EventHandler CanExecuteChanged;
		bool                      ICommand.CanExecute(object parameter) {
      return true;
		}

		private readonly MainPageViewModel vm;
	}
}
