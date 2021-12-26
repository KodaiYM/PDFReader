using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Windows.Media.Imaging;

namespace ExportToImage.Commands {
	public class ExportCommand: ICommand {
		public bool CanExecute(object parameter) {
			return !_executing;
		}
		public async void Execute(object parameter) {
			_executing = true;
			RaiseCanExecuteChanged();

			await Task.Run(Export);

			_executing = false;
			RaiseCanExecuteChanged();
		}

		public async Task Export() {
			var previews = _viewModel.Previews;

			var save_tasks = previews.Select(
			    (Preview preview, int page_index) => Task.Run(async () => {
				    Action save_to_file = () => {
					    using (var stream = new FileStream(
					               Path.Combine(_viewModel.OutputDirectoryPath,
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

				    var tcs = new TaskCompletionSource<object>();
				    PropertyChangedEventHandler sourceChangedHandler = null;
				    sourceChangedHandler =
				        (object sender, PropertyChangedEventArgs e) => {
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

			await Task.WhenAll(save_tasks);
		}

		public ExportCommand(MainPageViewModel viewModel) {
			_viewModel = viewModel;
		}
		public event EventHandler CanExecuteChanged;
		private void              RaiseCanExecuteChanged() {
      CanExecuteChanged?.Invoke(this, EventArgs.Empty);
		}

		private readonly MainPageViewModel _viewModel;
		private bool                       _executing = false;
	}
}
