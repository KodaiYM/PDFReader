using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Windows.Media.Imaging;

using Microsoft.WindowsAPICodePack.Dialogs;

using MuPDFCore;

namespace ExportToImage.Commands {
	public class ResetPreviewsCommand: ICommand {
		public void Execute(object parameter) {
			Task.Run(() => {
				_viewModel.Previews.Clear();

				_viewModel.Document.ResetPath(_viewModel.PDFPath);
				foreach (var page in _viewModel.Document.Pages) {
					var bounds = page.Bounds;

					_viewModel.Previews.Add(Preview.CreateLoadingPreview(
					    bounds.Width * 96 / 72, bounds.Height * 96 / 72));
					// TODO: for debugging
					System.Threading.Thread.Sleep(30);
				}

				_viewModel.RenderPreviews.Execute(null);
			});

			// MuPDF を使ってやろう。
			// Task.Run(async () => {
			//			var stopwatch = new Stopwatch();
			//			stopwatch.Start();

			//			using var renderer = new DocumentRenderer(filename);
			//			var       pages    = renderer.Pages;
			//			var   thumbnails   = new ConcurrentBag<KeyValuePair<int,
			// ImageSource>>(); 			  var generateBitmapSource = new
			// ActionBlock<KeyValuePair<int, Cube.Pdf.Page>>((kv) => {
			// using (var
			// image = renderer.Render(kv.Value, kv.Value.Size)) {
			// using (var
			// image_stream = new MemoryStream()) {
			// image.Save(image_stream,
			// ImageFormat.Png);
			// image_stream.Seek(0, SeekOrigin.Begin);
			// ImageSource
			// bitmap = BitmapFrame.Create(image_stream, BitmapCreateOptions.None,
			// BitmapCacheOption.OnLoad);
			// thumbnails.Add(KeyValuePair.Create(kv.Key, bitmap));
			//		}
			//	}
			//},
			// new ExecutionDataflowBlockOptions {
			// CancellationToken =
			// System.Threading.CancellationToken.None, EnsureOrdered =
			// false, MaxDegreeOfParallelism =
			// System.Threading.Tasks.Dataflow.DataflowBlockOptions.Unbounded
			//}
			//);

			// foreach (var kv in pages.Select((page, index) =>
			//                                    KeyValuePair.Create(index, page))) {
			//	generateBitmapSource.SendAsync(kv).Wait();
			//}
			// generateBitmapSource.Complete();
			// generateBitmapSource.Completion.Wait();
			// foreach (var kv in thumbnails.OrderBy(kv => kv.Key)) {
			//	vm.Thumbnails.Add(kv.Value);
			//}

			// stopwatch.Stop();
			// Debug.WriteLine("Pattern 3: " + stopwatch.ElapsedMilliseconds);
			//});
		}

		public ResetPreviewsCommand(MainPageViewModel viewModel) {
			_viewModel = viewModel;
		}
		public event EventHandler          CanExecuteChanged;
		public bool                        CanExecute(object parameter) => true;
		private readonly MainPageViewModel _viewModel;
	}
}
