using System;
using System.Collections.Generic;
using System.Reactive.Disposables;
using System.Text;
using System.Threading.Tasks;
using System.Linq;
using Reactive.Bindings;
using Reactive.Bindings.Extensions;
using System.IO;
using System.Windows.Media.Imaging;
using System.ComponentModel;

namespace ExportToImage.Commands {
	public class ExportAndCancelCommandPair: IDisposable {
		public AsyncReactiveCommand Export { get; }
		public AsyncReactiveCommand Cancel { get; }

		public ExportAndCancelCommandPair() {
			Export = new AsyncReactiveCommand(_exporting.Inverse(), null)
			             .AddTo(_disposables);
			Cancel = new AsyncReactiveCommand(_exporting, null).AddTo(_disposables);
		}

		public async Task ExecuteExport(string               outputDirectoryPath,
		                                IEnumerable<Preview> previews) {
			_exporting.Value = true;

			_exportTasks = previews.Select((Preview preview,
			                                int page_index) => Task.Run(async () => {
				Action save_to_file = () => {
					using (var stream =
					           new FileStream(Path.Combine(outputDirectoryPath,
					                                       (page_index + 1) + ".png"),
#if DEBUG
					                          FileMode.CreateNew
#else
					                          FileMode.Create
#endif
					                          )) {
						var encoder = new PngBitmapEncoder();
						encoder.Frames.Add(BitmapFrame.Create(preview.Source));
						encoder.Save(stream);
					}
				};

				object save_to_file_lock = new object();
				bool   saved             = false;

				var                         tcs = new TaskCompletionSource<object>();
				PropertyChangedEventHandler sourceChangedHandler = null;
				sourceChangedHandler = (object sender, PropertyChangedEventArgs e) => {
					if ("Source" == e.PropertyName) {
						var source = preview.Source;
						if (source != null) {
							lock (save_to_file_lock) {
								if (!saved) {
									saved = true;
									save_to_file();
									preview.PropertyChanged -= sourceChangedHandler;
									tcs.TrySetResult(null);
								}
							}
						}
					}
				};
				preview.PropertyChanged += sourceChangedHandler;

				if (!preview.Loading) {
					lock (save_to_file_lock) {
						if (!saved) {
							saved = true;
							save_to_file();
							preview.PropertyChanged -= sourceChangedHandler;
							tcs.TrySetResult(null);
						}
					}
				}

				await tcs.Task;
			}));
			await Task.WhenAll(_exportTasks);

			_exporting.Value = false;
		}
		public async Task ExecuteCancel() {
			_exporting.Value = false;
		}

		private ReactivePropertySlim<bool> _exporting =
		    new ReactivePropertySlim<bool>(false);
		private IEnumerable<Task> _exportTasks;

#region Dispose
		private CompositeDisposable _disposables  = new CompositeDisposable();
		private bool                disposedValue = false;
		public void                 Dispose() {
      if (!disposedValue) {
        _disposables.Dispose();
        _exporting.Dispose();
        disposedValue = true;
      }
		}
#endregion
	}
}
