using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace ExportToImage {
	public class Preview: INotifyPropertyChanged {
		public bool Loading {
			get { return _loading; }
		private
			set {
				if (value != _loading) {
					_loading = value;
					NotifyPropertyChanged();
				}
			}
		}
		// width in 96dpi
		public double Width {
			get { return _width; }
			set {
				if (value != _width) {
					_width = value;
					NotifyPropertyChanged();
				}
			}
		}
		// height in 96dpi
		public double Height {
			get { return _height; }
			set {
				if (value != _height) {
					_height = value;
					NotifyPropertyChanged();
				}
			}
		}
		public BitmapSource Source {
			get { return _source; }
			set {
				if (value == null) {
					throw new ArgumentException("value cannot be null");
				}

				if (value != _source) {
					_source = value;
					Loading = false;
					NotifyPropertyChanged();
				}
			}
		}

		public Preview(BitmapSource source) {
			Source = source;
		}
		public static Preview CreateLoadingPreview(double width, double height) {
			return new Preview { _width = width, _height = height, _loading = true };
		}

		public void SetStateToLoading() {
			_source = null;
			Loading = true;
		}

		private Preview() {}

#region Backing stores
		private bool         _loading;
		private double       _width;
		private double       _height;
		private BitmapSource _source;
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
