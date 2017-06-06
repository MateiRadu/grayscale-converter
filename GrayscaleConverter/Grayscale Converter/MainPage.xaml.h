//*****************************************************************************
//
//  Grayscale Converter, a simple color-to-grayscale UWP converter.
//  Copyright(C) 2016, 2017 Matei Bogdan Radu
//
//  This program is free software : you can redistribute it and / or modify
//  it under the terms of the GNU General Public License as published by the
//  Free Software Foundation, either version 3 of the License, or (at your
//  option) any later version.
//
//  This program is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
//  or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General Public License
//  for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program. If not, see <http://www.gnu.org/licenses/>.
//
//*****************************************************************************

#pragma once

#include "MainPage.g.h"
#include "MainViewModel.h"

using namespace ViewModel;
using namespace Windows::Storage;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media::Imaging;

namespace Grayscale_Converter
{
	public enum class NotifyType
	{
		StatusMessage,
		ErrorMessage,
		SuccessMessage
	};

	/// <summary>
	/// Main page of the app.
	/// </summary>
	public ref class MainPage sealed
	{
		public:
			MainPage();
			property MainViewModel^ ViewModel;

		private:
			void Select_Picture_Click(Platform::Object^ sender, RoutedEventArgs^ e);
			void Convert_Click(Platform::Object ^ sender, RoutedEventArgs ^ e);
			void GetPicture();
			void ConvertPicture();

			StorageFile^ SelectedImageFile;
			BitmapImage^ OriginalImageSource;
			WriteableBitmap^ bitmap;
			Platform::Array<byte>^ SourcePixels;
			byte* DestinationPixels;
			int width;
			int height;
			int xCenter;
			int yCenter;

		internal:
			void NotifyUser(Platform::String^ strMessage, NotifyType type);
	};
}