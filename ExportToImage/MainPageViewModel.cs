using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Reactive.Disposables;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Linq;
using Reactive.Bindings;
using Reactive.Bindings.Extensions;
using System.IO;
using System.Windows.Media.Imaging;

namespace ExportToImage {
	public sealed class MainPageViewModel: INotifyPropertyChanged, IDisposable {
#region Properties to be Binding
		public string PDFPath {
			get { return _pdfPath; }
			set {
				if (value != _pdfPath) {
					_pdfPath = value;
					NotifyPropertyChanged();
				}
			}
		}
		public string OutputDirectoryPath {
			get { return _outputDirectoryPath; }
			set {
				if (value != _outputDirectoryPath) {
					_outputDirectoryPath = value;
					NotifyPropertyChanged();
				}
			}
		}
		public PreviewCollection Previews { get; } = new PreviewCollection();
		public double            PreviewDisplayMagnification { get; set; } = 0.25;
#endregion

#region Properties used by Commands
		internal Models.DocumentModel Document {
			get;
		} = new Models.DocumentModel();
#endregion

#region Commands to be Binding
		public Commands.OpenPDFCommand            OpenPDF { get; }
		public Commands.SetOutputDirectoryCommand SetOutputDirectory { get; }
		public AsyncReactiveCommand               Export { get; }
		public AsyncReactiveCommand               CancelExport { get; }
#endregion

		public MainPageViewModel() {
			//			_exporting = new
			// ReactivePropertySlim<bool>(false).AddTo(_disposables);

			/* Set Commands */
			OpenPDF            = new Commands.OpenPDFCommand(this);
			SetOutputDirectory = new Commands.SetOutputDirectoryCommand(this);
			Export             = _exportAndCancelCommandPair.Export;
			Export.Subscribe(async () =>
			                     await _exportAndCancelCommandPair.ExecuteExport(
			                         OutputDirectoryPath, Previews));
			CancelExport = _exportAndCancelCommandPair.Cancel;
			CancelExport.Subscribe(
			    async () => await _exportAndCancelCommandPair.ExecuteCancel());

			System.Windows.Data.BindingOperations.EnableCollectionSynchronization(
			    Previews, new object());
		}

#region Backing stores for Binding commands
		private Commands.ExportAndCancelCommandPair _exportAndCancelCommandPair =
		    new Commands.ExportAndCancelCommandPair();
#endregion

#region Backing stores for Binding properties
		private CompositeDisposable _disposables = new CompositeDisposable();
		private string              _pdfPath;
		private string              _outputDirectoryPath;
#endregion

#region Implementation of INotifyPropertyChanged
		public event PropertyChangedEventHandler PropertyChanged;
		private void                             NotifyPropertyChanged([
      System.Runtime.CompilerServices.CallerMemberName
		                            ] string propertyName = "") {
      PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
		}
#endregion

#region Dispose
		private bool _disposedValue = false;
		public void  Dispose() {
      if (!_disposedValue) {
        OpenPDF?.Dispose();
        Document?.Dispose();
        _exportAndCancelCommandPair.Dispose();
        _disposables.Dispose();
        _disposedValue = true;
      }
		}
#endregion
	}
}
