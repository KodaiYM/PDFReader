﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Input;

using Microsoft.WindowsAPICodePack.Dialogs;
using MuPDFCore;

namespace ExportToImage.Commands {
	public class OpenPDFCommand: ICommand, IDisposable {
		public bool CanExecute(object parameter) {
			return !_busy;
		}
		public async void Execute(object parameter) {
			var dialog =
			    new Microsoft.Win32.OpenFileDialog { Filter = "PDF ファイル|*.pdf" };
			bool? dialog_result = dialog.ShowDialog();
			if (dialog_result == false) {
				return;
			}
			_viewModel.PDFPath = dialog.FileName;

			_busy = true;
			RaiseCanExecuteChanged();

			await Task.Run(CancelPreviousResetPreviewsTask);
			_resetPreviewsTask = Task.Run(ResetPreviews);

			_busy = false;
			RaiseCanExecuteChanged();
		}
		private async Task CancelPreviousResetPreviewsTask() {
			if (_cancellationTokenSource != null) {
				if (!_resetPreviewsTask.IsCompleted) {
					_cancellationTokenSource.Cancel();

					try {
						// Await cancellation
						await _resetPreviewsTask;
					} catch (OperationCanceledException) {}
					_resetPreviewsTask = null;
				}

				_cancellationTokenSource.Dispose();
			}
			_cancellationTokenSource = new CancellationTokenSource();
		}
		private async Task ResetPreviews() {
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
				// System.Threading.Thread.Sleep(10);
#endif
				// Start Rendering Task
				var page_index = i;
#if DEBUG
				RenderTasks.Add(Task.Run(() => {
#else
				RenderTasks.Add(Task.Run(() => {
#endif
					cancellationToken.ThrowIfCancellationRequested();
					// Debug.WriteLine(page_index + ": start");
					var preview = _viewModel.Document.Render(page_index, dpi: 72,
					                                         cancellationToken);
					preview.Freeze();
					// Debug.WriteLine(page_index + ": end");

					// TODO: for debugging
#if DEBUG
					// System.Threading.Thread.Sleep(200);
#endif

					cancellationToken.ThrowIfCancellationRequested();

					_viewModel.Previews[page_index].Source = preview;
#if DEBUG
				}));
#else
				}));
#endif
			}

			// Await cancel or complete
			await Task.WhenAll(RenderTasks.ToArray());
		}

		public OpenPDFCommand(MainPageViewModel viewModel) {
			_viewModel = viewModel;
		}

		public event EventHandler CanExecuteChanged;
		private void              RaiseCanExecuteChanged() {
      CanExecuteChanged?.Invoke(this, EventArgs.Empty);
		}

		private readonly MainPageViewModel _viewModel;
		private CancellationTokenSource    _cancellationTokenSource;
		private Task                       _resetPreviewsTask;
		private bool                       _busy = false;

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