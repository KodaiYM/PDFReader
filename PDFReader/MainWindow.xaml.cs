using System;
using System.Windows;

namespace PDFReader
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Core.ViewModels.MainWindowViewModel ViewModel = new Core.ViewModels.MainWindowViewModel();

        public MainWindow()
        {
            InitializeComponent();
        }
        protected override void OnInitialized(EventArgs e)
        {
            base.OnInitialized(e);

            this.DataContext = ViewModel;
        }
        protected override void OnContentRendered(EventArgs e)
        {
            base.OnContentRendered(e);

            ViewModel.Text = Core.ViewModels.ParsedXrefTable();
        }
    }
}
