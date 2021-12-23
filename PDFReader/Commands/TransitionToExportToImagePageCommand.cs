using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Input;

namespace PDFReader.Commands {
	class TransitionToExportToImagePageCommand: ICommand {
		public void Execute(object parameter) {
			_mainWindowViewModel.PageSource =
			    "/ExportToImage;component/MainPage.xaml";
		}

		public TransitionToExportToImagePageCommand(
		    MainWindowViewModel mainWindowViewModel) {
			_mainWindowViewModel = mainWindowViewModel;
		}

		public event EventHandler CanExecuteChanged;

		public bool                          CanExecute(object parameter) => true;
		private readonly MainWindowViewModel _mainWindowViewModel;
	}
}
