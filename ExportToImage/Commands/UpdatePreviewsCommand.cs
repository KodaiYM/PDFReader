using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Input;

using MuPDFCore;

namespace ExportToImage.Commands {
	public class UpdatePreviewsCommand: ICommand {
		public void Execute(object parameter) {
			// 1. 全要素ロード中に変更する
			foreach (var preview in _viewModel.Previews) {
				preview.SetStateToLoading();
			}

			// 2. 各ページレンダリングして、完了したらその画像を表示する
			_viewModel.RenderPreviews.Execute(null);

			// System.Threading.Tasks.Task.Run(() => {
			//	// Open Document
			//	using var context = new MuPDFContext();
			//	// 日本語ダメかも！
			//	using var document = new MuPDFDocument(context, vm.PDFPath);

			//	foreach (var page in document.Pages) {
			//		var roundedBounds = page.Bounds.Round();

			//		vm.Previews.Add(new LoadingPreview { Width  = roundedBounds.Width,
			//			                                   Height = roundedBounds.Height
			//}); 		System.Threading.Thread.Sleep(20);
			//	}
			//});
		}

		public UpdatePreviewsCommand(MainPageViewModel viewModel) {
			_viewModel = viewModel;
		}
		public event EventHandler CanExecuteChanged;

		public bool CanExecute(object parameter) {
			throw new NotImplementedException();
		}
		private readonly MainPageViewModel _viewModel;
	}
}
