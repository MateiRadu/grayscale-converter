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

namespace Grayscale_Converter {

	// Different status message foreground color codes.
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
			
			// Status message.
			property Platform::String^ Status
			{
				Platform::String^ get();
				void set(Platform::String^ value);
			}

			// Status message foreground.
			property Windows::UI::Xaml::Media::SolidColorBrush^ StatusForeground
			{
				Windows::UI::Xaml::Media::SolidColorBrush^ get();
				void set(Windows::UI::Xaml::Media::SolidColorBrush^ value);
			}
			
			// Image selected by user.
			property BitmapImage^ OriginalImageSource
			{
				BitmapImage^ get();
				void set(BitmapImage^ value);
			}

			// Image modified to grayscale.
			property WriteableBitmap^ ModifiedImageSource
			{
				WriteableBitmap^ get();
				void set(WriteableBitmap^ value);
			}
			
			// Covert button enabled toggle.
			property bool ConvertButtonIsEnabled
			{
				bool get();
				void set(bool value);
			}

			// Save button enable toggle.
			property bool SaveButtonIsEnabled
			{
				bool get();
				void set(bool value);
			}
			
			// Commands.
			property ICommand^ SelectPictureCommand;
			property ICommand^ ConvertPictureCommand;
			property ICommand^ SavePictureCommand;

			virtual event PropertyChangedEventHandler ^ PropertyChanged;

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
			bool m_SaveButtonIsEnabled;

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

			// Save picture.
			void SavePicture(Platform::Object^ parameter);
			void SavPicture();

		internal:
			// Update the status message as feedback to the user.
			void NotifyUser(Platform::String^ strMessage, NotifyType type);
	};
}