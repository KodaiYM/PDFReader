using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace ExportToImage.Commands {
	public class RenderPreviewsCommand: ICommand {
		public void Execute(object parameter) {
			Parallel.For(0, _viewModel.Document.Pages.Count, (int page_number) => {
				var preview =
				    _viewModel.Document.Render(page_number, 7); // for test, dpi is 1
				preview.Freeze();
				// TODO: for debugging
				System.Threading.Thread.Sleep(30);
				_viewModel.Previews[page_number].Source = preview;
			});
		}

		public RenderPreviewsCommand(MainPageViewModel viewModel) {
			_viewModel = viewModel;
		}
		public event EventHandler CanExecuteChanged;

		public bool CanExecute(object parameter) {
			return true;
		}
		private readonly MainPageViewModel _viewModel;
	}
}
