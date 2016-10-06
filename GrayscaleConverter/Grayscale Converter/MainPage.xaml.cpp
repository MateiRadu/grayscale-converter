//*****************************************************************************
//
//  Grayscale Converter, a simple color-to-grayscale UWP converter.
//  Copyright(C) 2016 Matei Bogdan Radu
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

#include "pch.h"
#include "MainPage.xaml.h"

using namespace Grayscale_Converter;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Media;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
	NotifyUser("Ready. Pick a picture to modify.", NotifyType::StatusMessage);
}


void MainPage::Select_Picture_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	GetPicture();
}

void MainPage::Convert_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	ConvertPicture();
}

void MainPage::GetPicture()
{
	FileOpenPicker^ FilePicker = ref new FileOpenPicker();
	FilePicker->ViewMode = PickerViewMode::Thumbnail;
	FilePicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;
	FilePicker->FileTypeFilter->Append(".jpg");
	FilePicker->FileTypeFilter->Append(".jpeg");
	FilePicker->FileTypeFilter->Append(".png");

	create_task(FilePicker->PickSingleFileAsync())
		.then([this](StorageFile^ file)
	{
		if (file)
		{
			create_task(file->OpenAsync(Windows::Storage::FileAccessMode::Read))
				.then([this](IRandomAccessStream^ Stream)
			{
				OriginalImageSource = ref new BitmapImage();
				OriginalBitmap->CreateAsync(Stream);
				OriginalImageSource->SetSourceAsync(Stream);
				OriginalImage->Source = OriginalImageSource;
				//ModifiedImage->Source = OriginalImageSource; //Layout testing
				ConvertButton->IsEnabled = true;
				NotifyUser("Photo selected. Ready to convert.", NotifyType::StatusMessage);
		});
		}
		else
		{
			NotifyUser("Something went wrong. Retry", NotifyType::ErrorMessage);
		}
	});
}

void MainPage::ConvertPicture()
{
	Array<unsigned char>^ NewPixels = ref new Array<unsigned char>{};
	Array<unsigned char>^ Pixels;
	create_task(OriginalBitmap->GetPixelDataAsync())
		.then([&](Windows::Graphics::Imaging::PixelDataProvider^ PixelData)
	{
		if (PixelData) {
			Pixels = PixelData->DetachPixelData();
			for (int i = 0; i < Pixels->Length; i++) {
				// ???
			}
			//NewPixels to PixelDataProvider;
			//PixelDataProvider to BitmapEncoder;
		}
	});
}

void MainPage::NotifyUser(String^ strMessage, NotifyType type)
{
	switch (type)
	{
		case NotifyType::StatusMessage:
			StatusLabel->Foreground = ref new SolidColorBrush(Windows::UI::Colors::Gray);
			break;
		case NotifyType::ErrorMessage:
			StatusLabel->Foreground = ref new SolidColorBrush(Windows::UI::Colors::Red);
			break;
		case NotifyType::SuccessMessage:
			StatusLabel->Foreground = ref new SolidColorBrush(Windows::UI::Colors::Green);
			break;
		default:
			break;
	}
	StatusLabel->Text = strMessage;
}