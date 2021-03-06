using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;

using MuPDFCore;

namespace ExportToImage.Models {
	public class DocumentModel: IDisposable {
		public void ResetPath(string path, CancellationToken cancellationToken) {
			cancellationToken.ThrowIfCancellationRequested();

			if (_renderers != null) {
				foreach (var renderer in _renderers) {
					cancellationToken.ThrowIfCancellationRequested();

					renderer?.Dispose();
				}
				_renderers = null;
			}

			cancellationToken.ThrowIfCancellationRequested();

			_document?.Dispose();
			_document = null;
			_context?.Dispose();
			_context = null;

			cancellationToken.ThrowIfCancellationRequested();

			_context  = new MuPDFContext();
			_document = new MuPDFDocument(_context, path);

			cancellationToken.ThrowIfCancellationRequested();

			var page_count = Pages.Count;
			_renderers     = new MuPDFMultiThreadedPageRenderer[page_count];
		}
		public MuPDFPageCollection Pages {
			get { return _document.Pages; }
		}

		/// <summary>
		/// This function can be called by multiple threads in parallel.
		/// </summary>
		/// <param name="dpi">Rendered dpi (= pixel per inch)</param>
		/// <returns></returns>
		public BitmapSource Render(int page_index, double dpi,
		                           CancellationToken cancellationToken) {
			cancellationToken.ThrowIfCancellationRequested();

			// if renderer is not set
			if (_renderers[page_index] == null) {
				lock (_GetMultiThreadedRenderer_locker) {
					// create renderer
					_renderers[page_index] =
					    _document.GetMultiThreadedRenderer(page_index, RenderThreadCount);
				}
			}

			cancellationToken.ThrowIfCancellationRequested();

			// actual bounds
			var bounds = Pages[page_index].Bounds;

			// round to integer bounds
			var roundedBounds = bounds.Round(dpi / 72.0);

			var renderedPageSize =
			    new RoundedSize(roundedBounds.Width, roundedBounds.Height);

			cancellationToken.ThrowIfCancellationRequested();

			// Determine the boundaries of each tile by splitting the total size of
			// the page by the number of threads.
			var tileBounds = renderedPageSize.Split(RenderThreadCount);

			cancellationToken.ThrowIfCancellationRequested();

			IntPtr[] destination = new IntPtr[RenderThreadCount];
			byte[] managedBitmapArray;
			try {
				for (int i = 0; i < RenderThreadCount; ++i) {
					cancellationToken.ThrowIfCancellationRequested();

					destination[i] = Marshal.AllocCoTaskMem(tileBounds[i].Height *
					                                        tileBounds[i].Width * 3);
				}

				cancellationToken.ThrowIfCancellationRequested();

				// Rendering
				using (cancellationToken.Register(
				    () => { _renderers[page_index].Abort(); })) {
					_renderers[page_index].Render(renderedPageSize, bounds, destination,
					                              MuPDFCore.PixelFormats.RGB);
				}

				cancellationToken.ThrowIfCancellationRequested();

				// convert to managed array
				managedBitmapArray =
				    new byte[renderedPageSize.Width * 3 * renderedPageSize.Height];

				cancellationToken.ThrowIfCancellationRequested();

				System.Diagnostics.Debug.Assert(RenderThreadCount == 1);
				Marshal.Copy(destination[0], managedBitmapArray, 0,
				             renderedPageSize.Width * 3 * renderedPageSize.Height);
			} finally {
				// Release the allocated memory.
				for (int i = 0; i < RenderThreadCount; ++i) {
					Marshal.FreeCoTaskMem(destination[i]);
				}
			}

			cancellationToken.ThrowIfCancellationRequested();

			// convert bitmap array to bitmap
			System.Diagnostics.Debug.Assert(managedBitmapArray != null);
			var image = BitmapSource.Create(
			    renderedPageSize.Width, renderedPageSize.Height, dpi, dpi,
			    System.Windows.Media.PixelFormats.Bgr24, null, managedBitmapArray,
			    renderedPageSize.Width * 3);

			return image;
		}

		public void Dispose() {
			if (_renderers != null) {
				foreach (var renderer in _renderers) {
					renderer?.Dispose();
				}
			}
			_document?.Dispose();
			_context?.Dispose();
		}

		private MuPDFContext  _context;
		private MuPDFDocument _document;
		private MuPDFMultiThreadedPageRenderer[] _renderers;
		private readonly object _GetMultiThreadedRenderer_locker = new object();

		private const int RenderThreadCount = 1 /*Environment.ProcessorCount*/;
	}
}
