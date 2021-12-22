using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Text;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;

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
		public Commands.ResetPreviewsCommand      ResetPreviews { get; }
		public Commands.UpdatePreviewsCommand     UpdatePreviews { get; }
		public Commands.RenderPreviewsCommand     RenderPreviews { get; }
#endregion

		public MainPageViewModel() {
			/* Set Commands */
			OpenPDF            = new Commands.OpenPDFCommand(this);
			SetOutputDirectory = new Commands.SetOutputDirectoryCommand(this);
			ResetPreviews      = new Commands.ResetPreviewsCommand(this);
			UpdatePreviews     = new Commands.UpdatePreviewsCommand(this);
			RenderPreviews     = new Commands.RenderPreviewsCommand(this);

			System.Windows.Data.BindingOperations.EnableCollectionSynchronization(
			    Previews, new object());
		}

#region Backing stores for Binding properties
		private string _pdfPath;
		private string _outputDirectoryPath;
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
        Document.Dispose();
        _disposedValue = true;
      }
		}
#endregion
	}
}
