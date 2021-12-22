using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Drawing.Imaging;
using System.IO;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Linq;
using System.Threading.Tasks.Dataflow;
using System.ComponentModel;

namespace PDFReader {
	public class MainWindowViewModel: INotifyPropertyChanged {
#region Properties to be Binding
		public string PageSource {
			get { return _pageSource; }
			set {
				if (value != _pageSource) {
					_pageSource = value;
					NotifyPropertyChanged();
				}
			}
		}
		public bool ExportToImageButtonIsChecked {
			get { return _exportToImageButtonIsChecked; }
			set {
				if (value != _exportToImageButtonIsChecked) {
					_exportToImageButtonIsChecked = value;
					NotifyPropertyChanged();
				}
			}
		}
#endregion

#region Commands to be Binding
		public ICommand TransitionToExportToImagePage { get; }
#endregion

		public MainWindowViewModel() {
#region Set Commands
			TransitionToExportToImagePage =
			    new Commands.TransitionToExportToImagePageCommand(this);
#endregion

#region Set Default Menu
			ExportToImageButtonIsChecked = true;
			TransitionToExportToImagePage.Execute(null);
#endregion
		}

#region Backing stores for Binding properties
		private string _pageSource;
		private bool   _exportToImageButtonIsChecked;
#endregion
#region Implementation of INotifyPropertyChanged
		public event PropertyChangedEventHandler PropertyChanged;
		private void                             NotifyPropertyChanged([
      System.Runtime.CompilerServices.CallerMemberName
		                            ] string propertyName = "") {
      PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
		}
#endregion
	}
}
