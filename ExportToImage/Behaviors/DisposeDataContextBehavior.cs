using System;
using System.Windows;
using Microsoft.Xaml.Behaviors;

namespace ExportToImage.Behaviors {
	public class DisposeDataContextBehavior: Behavior<FrameworkElement> {
		protected override void OnAttached() {
			base.OnAttached();

			Application.Current.Exit += OnExit;
			AssociatedObject.Unloaded += OnUnloaded;
		}
		private void Behave() {
			AssociatedObject.Unloaded -= OnUnloaded;
			Application.Current.Exit -= OnExit;

			(AssociatedObject.DataContext as IDisposable)?.Dispose();
		}

		private void OnUnloaded(object sender, RoutedEventArgs e) => Behave();
		private void OnExit(object sender, ExitEventArgs e)       => Behave();
	}
}
