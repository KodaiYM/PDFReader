using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Media.Imaging;

using MuPDFCore;

namespace ExportToImage.Models {
	public class DocumentModel: IDisposable {
		public void ResetPath(string path) {
			if (_renderers != null) {
				foreach (var renderer in _renderers) {
					renderer?.Dispose();
				}
			}
			_document?.Dispose();

			_document      = new MuPDFDocument(_context, path);
			var page_count = Pages.Count;
			_renderers     = new MuPDFMultiThreadedPageRenderer[page_count];
			for (int i = 0; i < page_count; ++i) {
				// heavy
				_renderers[i] =
				    _document.GetMultiThreadedRenderer(i, RenderThreadCount);
			}
		}
		public MuPDFPageCollection Pages {
			get { return _document.Pages; }
		}

		/// <summary>
		/// This function can be called by multiple threads in parallel.
		/// </summary>
		/// <param name="dpi">Rendered dpi (= pixel per inch)</param>
		/// <returns></returns>
		public BitmapSource Render(int page_index, double dpi = 72.0) {
			// actual bounds
			var bounds = Pages[page_index].Bounds;

			// round to integer bounds
			var roundedBounds = bounds.Round(dpi / 72.0);

			var renderedPageSize =
			    new RoundedSize(roundedBounds.Width, roundedBounds.Height);

			// Determine the boundaries of each tile by splitting the total size of
			// the page by the number of threads.
			var tileBounds = renderedPageSize.Split(RenderThreadCount);

			IntPtr[] destination = new IntPtr[RenderThreadCount];
			for (int i = 0; i < RenderThreadCount; ++i) {
				destination[i] = Marshal.AllocCoTaskMem(tileBounds[i].Height *
				                                        tileBounds[i].Width * 3);
			}

			_renderers[page_index].Render(renderedPageSize, bounds, destination,
			                              MuPDFCore.PixelFormats.RGB);

			// convert to managed array
			byte[] managedBitmapArray =
			    new byte[renderedPageSize.Width * 3 * renderedPageSize.Height];
			System.Diagnostics.Debug.Assert(RenderThreadCount == 1);
			Marshal.Copy(destination[0], managedBitmapArray, 0,
			             renderedPageSize.Width * 3 * renderedPageSize.Height);

			// Release the allocated memory.
			for (int i = 0; i < RenderThreadCount; ++i) {
				Marshal.FreeCoTaskMem(destination[i]);
			}

			// convert bitmap array to bitmap
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
			_context.Dispose();
		}

		private readonly MuPDFContext _context = new MuPDFContext();
		private MuPDFDocument         _document;
		private MuPDFMultiThreadedPageRenderer[] _renderers;

		private const int RenderThreadCount = 1 /*Environment.ProcessorCount*/;
	}
}
