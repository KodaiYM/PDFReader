using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Windows.Media.Imaging;

using Microsoft.WindowsAPICodePack.Dialogs;

using MuPDFCore;

namespace ExportToImage.Commands {
	public class ResetPreviewsCommand: ICommand, IDisposable {
		public void Execute(object parameter) {
			if (_cancellationTokenSource != null) {
				if (!_resetPreviewsTask.IsCompleted) {
					_cancellationTokenSource.Cancel();

					// Wait for complete cancellation
					try {
						_resetPreviewsTask.Wait();
					} catch (AggregateException) {}
				}

				_cancellationTokenSource.Dispose();
			}

			_cancellationTokenSource = new CancellationTokenSource();
			_resetPreviewsTask       = Task.Run(ResetPreviews);
		}
		public void ResetPreviews() {
			var cancellationToken = _cancellationTokenSource.Token;

			cancellationToken.ThrowIfCancellationRequested();

			_viewModel.Previews.Clear();

			cancellationToken.ThrowIfCancellationRequested();

			_viewModel.Document.ResetPath(_viewModel.PDFPath, cancellationToken);

			cancellationToken.ThrowIfCancellationRequested();

			foreach (var page in _viewModel.Document.Pages) {
				cancellationToken.ThrowIfCancellationRequested();

				var bounds = page.Bounds;

				// TODO: for debugging
				System.Threading.Thread.Sleep(30);
				_viewModel.Previews.Add(Preview.CreateLoadingPreview(
				    bounds.Width * 96.0 / 72.0, bounds.Height * 96.0 / 72.0));
			}

			cancellationToken.ThrowIfCancellationRequested();

			Parallel.For(0, _viewModel.Document.Pages.Count, (int page_number) => {
				cancellationToken.ThrowIfCancellationRequested();

				var preview =
				    _viewModel.Document.Render(page_number, 72,
				                               cancellationToken); // for test, dpi is 1
				preview.Freeze();

				cancellationToken.ThrowIfCancellationRequested();

				// TODO: for debugging
				System.Threading.Thread.Sleep(30);
				_viewModel.Previews[page_number].Source = preview;
			});
		}

		public ResetPreviewsCommand(MainPageViewModel viewModel) {
			_viewModel = viewModel;
		}
		public event EventHandler CanExecuteChanged;
		public bool               CanExecute(object parameter) => true;

		private readonly MainPageViewModel _viewModel;
		private CancellationTokenSource    _cancellationTokenSource;
		private Task                       _resetPreviewsTask;

#region Dispose
		private bool disposedValue;
		public void  Dispose() {
      if (!disposedValue) {
        _cancellationTokenSource?.Dispose();
        disposedValue = true;
      }
		}
#endregion
	}
}
