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

#include "DelegateCommand.h"

using namespace Windows::UI::Xaml::Data;
using namespace Windows::Storage;
using namespace Windows::UI::Xaml::Media::Imaging;

namespace ViewModel {

	public enum class NotifyType
	{
		StatusMessage,
		ErrorMessage,
		SuccessMessage
	};

	[Bindable]
	public ref class MainViewModel sealed : INotifyPropertyChanged
	{
		
		public:
			MainViewModel();
			virtual event PropertyChangedEventHandler ^ PropertyChanged;

			property Platform::String^ Status
			{
				Platform::String^ get();
				void set(Platform::String^ value);
			}
			property Windows::UI::Xaml::Media::SolidColorBrush^ StatusForeground
			{
				Windows::UI::Xaml::Media::SolidColorBrush^ get();
				void set(Windows::UI::Xaml::Media::SolidColorBrush^ value);
			}
			property BitmapImage^ OriginalImageSource
			{
				BitmapImage^ get();
				void set(BitmapImage^ value);
			}
			property WriteableBitmap^ ModifiedImageSource
			{
				WriteableBitmap^ get();
				void set(WriteableBitmap^ value);
			}
			property bool ConvertButtonIsEnabled
			{
				bool get();
				void set(bool value);
			}
			property Windows::UI::Xaml::Visibility SaveButtonVisibility {
				Windows::UI::Xaml::Visibility get();
				void set(Windows::UI::Xaml::Visibility value);
			}
			property Windows::UI::Xaml::Visibility ConvertButtonVisibility {
				Windows::UI::Xaml::Visibility get();
				void set(Windows::UI::Xaml::Visibility value);
			}
			property ICommand^ SelectPictureCommand;
			property ICommand^ ConvertPictureCommand;

		private:
			void OnPropertyChanged(Platform::String^ propertyName);

			// Status message.
			
			Platform::String^ m_Status;
			Windows::UI::Xaml::Media::SolidColorBrush^ m_StatusForeground;
			
			// Original image.
			void SelectPicture(Platform::Object^ parameter);
			void GetPicture();
			StorageFile^ SelectedImageFile;
			BitmapImage^ m_OriginalImageSource;
			bool m_ConvertButtonIsEnabled;
			Windows::UI::Xaml::Visibility m_SaveButtonVisibility;
			Windows::UI::Xaml::Visibility m_ConvertButtonVisibility;

			// Convert image.
			void ConvertPicture(Platform::Object^ parameter);
			void ConvPicture();
			WriteableBitmap^ m_ModifiedImageSource;
			byte* DestinationPixels;
			Platform::Array<byte>^ SourcePixels;
			int width;
			int height;
			int xCenter;
			int yCenter;

		internal:
			void NotifyUser(Platform::String^ strMessage, NotifyType type);
	};
}