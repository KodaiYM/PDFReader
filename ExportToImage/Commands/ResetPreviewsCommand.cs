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

			var        pages        = _viewModel.Document.Pages;
			var        num_of_pages = pages.Count;
			List<Task> RenderTasks  = new List<Task>(num_of_pages);
			for (int i = 0; i < num_of_pages; ++i) {
				if (cancellationToken.IsCancellationRequested) {
					break;
				}

				var bounds = pages[i].Bounds;

				_viewModel.Previews.Add(Preview.CreateLoadingPreview(
				    bounds.Width * 96.0 / 72.0, bounds.Height * 96.0 / 72.0));

				if (cancellationToken.IsCancellationRequested) {
					break;
				}

				// TODO: for debugging
#if DEBUG
				System.Threading.Thread.Sleep(10);
#endif
				// Start Rendering Task
				var page_index = i;
				RenderTasks.Add(Task.Run(() => {
					cancellationToken.ThrowIfCancellationRequested();

					var preview = _viewModel.Document.Render(page_index, dpi: 72,
					                                         cancellationToken);
					preview.Freeze();

					// TODO: for debugging
#if DEBUG
					System.Threading.Thread.Sleep(200);
#endif

					cancellationToken.ThrowIfCancellationRequested();

					_viewModel.Previews[page_index].Source = preview;
				}));
			}

			// wait for complete cancellation or complete all
			Task.WaitAll(RenderTasks.ToArray());
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
