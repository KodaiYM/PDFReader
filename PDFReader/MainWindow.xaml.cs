using System;
using System.Windows;

namespace PDFReader {
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window {
		private PagesViewModel pagesViewModel = new PagesViewModel();
		public MainWindow() {
			InitializeComponent();

			PagesControl.DataContext = pagesViewModel;
		}
		protected override void OnContentRendered(EventArgs e) {
			base.OnContentRendered(e);

			pagesViewModel.LoadPages();
		}
	}
}
