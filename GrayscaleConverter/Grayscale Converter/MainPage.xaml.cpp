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

#include "pch.h"
#include "MainPage.xaml.h"
#include "Robuffer.h"
#include <chrono>

using namespace Grayscale_Converter;

using namespace concurrency;
using namespace Microsoft::WRL;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Imaging;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Streams;

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
	// Prepare the file picker and the file type filters.
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
			// Save the file for grayscale conversion.
			SelectedImageFile = file;

			create_task(file->OpenAsync(Windows::Storage::FileAccessMode::Read))
				.then([this](IRandomAccessStream^ Stream)
			{
				// Create a new BitmapImage and set the file stream as source.
				OriginalImageSource = ref new BitmapImage();
				OriginalImageSource->SetSourceAsync(Stream);
				OriginalImage->Source = OriginalImageSource;

				// Enable the conversion button and notify the user.
				ConvertButton->IsEnabled = true;
				NotifyUser("Photo selected. Ready to convert.", NotifyType::StatusMessage);
				
				// Invalidate Modified Image if previously displayed.
				BitmapImage^ EmptyImageSource = ref new BitmapImage();
				ModifiedImage->Source = EmptyImageSource;
				SaveButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
				ConvertButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
			});
		} else {
			NotifyUser("Something went wrong. Retry", NotifyType::ErrorMessage);
		}
	});
}

void MainPage::ConvertPicture()
{
	// Notifiy the user that processing has begun.
	NotifyUser("Processing...", NotifyType::StatusMessage);
	RandomAccessStreamReference^ StreamFromFile = 
		RandomAccessStreamReference::CreateFromFile(SelectedImageFile);

	// From Stream create BitmapDecoder.
	create_task(StreamFromFile->OpenReadAsync())
		.then([this](IRandomAccessStreamWithContentType^ fileStream)
	{
		BitmapDecoder^ ImageDecoder;
		// From BitmapDecoder create BitmapFrame.
		create_task(ImageDecoder->CreateAsync(fileStream))
			.then([this](BitmapDecoder^ decoder){
			create_task(decoder->GetFrameAsync(0)).then([this](BitmapFrame^ BitFrame) {
				// Get Width, Height and Center coordinates of the image.
				width = BitFrame->OrientedPixelWidth;
				height = BitFrame->OrientedPixelHeight;
				xCenter = width / 2;
				yCenter = height / 2;
				// From BitmapFrame create PixelDataProvider.
				create_task(BitFrame->GetPixelDataAsync(
					BitmapPixelFormat::Rgba8,
					BitmapAlphaMode::Straight,
					ref new BitmapTransform(),
					ExifOrientationMode::RespectExifOrientation,
					ColorManagementMode::ColorManageToSRgb)).then([this](PixelDataProvider^ pixelProvider)
				{
					// Get start timestamp.
					auto start = std::chrono::high_resolution_clock::now();

					// Get array of pixels.
					SourcePixels = pixelProvider->DetachPixelData();

					// Create the WriteableBitmap. 
					bitmap = ref new WriteableBitmap(width, height);
					// Set the bitmap to the Image element.
					ModifiedImage->Source = bitmap;

					// Get access to the pixels.
					IBuffer^ buffer = bitmap->PixelBuffer;

					// Obtain IBufferByteAccess.
					ComPtr<IBufferByteAccess> pBufferByteAccess;
					ComPtr<IUnknown> pBuffer((IUnknown*)buffer);
					pBuffer.As(&pBufferByteAccess);

					// Get pointer to pixel bytes.
					pBufferByteAccess->Buffer(&DestinationPixels);

					for (int yDestination = 0; yDestination < height; yDestination++)
					{
						for (int xDestination = 0; xDestination < width; xDestination++)
						{
							// Calculate the indexes.
							// Since pixels won't be moved they share the same
							// origins yDestination and xDestination.
							int iDst = 4 * (yDestination * width + xDestination);
							int iSrc = 4 * (yDestination * width + xDestination);

							// Generate grayscale pixel color based on average RGB.
							byte AveragePixel = (byte)((SourcePixels[iSrc++] + 
								SourcePixels[iSrc++] + SourcePixels[iSrc++]) / 3);
							
							// Transfer the pixel bytes.
							// Alpha channel is unchanged.
							DestinationPixels[iDst++] = AveragePixel;		// Blue
							DestinationPixels[iDst++] = AveragePixel;		// Green
							DestinationPixels[iDst++] = AveragePixel;		// Red
							DestinationPixels[iDst] = SourcePixels[iSrc];	// Alpha channel
						}
					}
					// Get end timestamp and calculate difference.
					auto finish = std::chrono::high_resolution_clock::now();
					auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);

					// Convert microseconds to seconds.
					double seconds = (double)microseconds.count() / 1000000;

					NotifyUser("Done! Elapsed time: " + seconds.ToString() + " seconds.", NotifyType::SuccessMessage);
					SaveButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
					ConvertButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
				});
			});
		});
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