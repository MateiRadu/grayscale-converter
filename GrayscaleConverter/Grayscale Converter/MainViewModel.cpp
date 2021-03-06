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
#include "MainViewModel.h"
#include "Robuffer.h"
#include "GrayscaleUtil.h"
#include "FileUtil.h"
#include <chrono>

using namespace Grayscale_Converter;
using namespace concurrency;
using namespace Microsoft::WRL;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Streams;

MainViewModel::MainViewModel()
{
	// Init the commands.
	ConvertPictureCommand = ref new DelegateCommand(
		ref new ExecuteDelegate(this, &MainViewModel::ConvertPicture),
		nullptr);

	SelectPictureCommand = ref new DelegateCommand(
		ref new ExecuteDelegate(this, &MainViewModel::SelectPicture),
		nullptr);

	SavePictureCommand = ref new DelegateCommand(
		ref new ExecuteDelegate(this, &MainViewModel::SavePicture),
		nullptr);

	// Init the UI components.
	MainViewModel::NotifyUser("Ready. Pick a picture to modify.", NotifyType::StatusMessage);
	MainViewModel::ConvertButtonIsEnabled = false;
	MainViewModel::SaveButtonIsEnabled = false;
}

// Properties.

Platform::String^ MainViewModel::Status::get()
{
	return m_Status;
}

void MainViewModel::Status::set(Platform::String^ value)
{
	if (m_Status != value)
	{
		m_Status = value;
		OnPropertyChanged("Status");
	}
}

bool MainViewModel::ConvertButtonIsEnabled::get()
{
	return m_ConvertButtonIsEnabled;
}

void MainViewModel::ConvertButtonIsEnabled::set(bool value)
{
	if (m_ConvertButtonIsEnabled != value)
	{
		m_ConvertButtonIsEnabled = value;
		OnPropertyChanged("ConvertButtonIsEnabled");
	}
}

Windows::UI::Xaml::Media::SolidColorBrush^ MainViewModel::StatusForeground::get()
{
	return m_StatusForeground;
}

void MainViewModel::StatusForeground::set(Windows::UI::Xaml::Media::SolidColorBrush^ value)
{
	if (m_StatusForeground != value)
	{
		m_StatusForeground = value;
		OnPropertyChanged("StatusForeground");
	}
}

bool MainViewModel::SaveButtonIsEnabled::get()
{
	return m_SaveButtonIsEnabled;
}

void MainViewModel::SaveButtonIsEnabled::set(bool value)
{
	if (m_SaveButtonIsEnabled != value)
	{
		m_SaveButtonIsEnabled = value;
		OnPropertyChanged("SaveButtonIsEnabled");
	}
}

BitmapImage^ MainViewModel::OriginalImageSource::get()
{
	return m_OriginalImageSource;
}

void MainViewModel::OriginalImageSource::set(BitmapImage^ value)
{
	if (m_OriginalImageSource != value)
	{
		m_OriginalImageSource = value;
		OnPropertyChanged("OriginalImageSource");
	}
}

WriteableBitmap^ MainViewModel::ModifiedImageSource::get()
{
	return m_ModifiedImageSource;
}

void MainViewModel::ModifiedImageSource::set(WriteableBitmap^ value)
{
	if (m_ModifiedImageSource != value)
	{
		m_ModifiedImageSource = value;
		OnPropertyChanged("ModifiedImageSource");
	}
}

// Functions.

void MainViewModel::ConvertPicture(Platform::Object^ Parameter) {
	ConvPicture();
}

void MainViewModel::SelectPicture(Platform::Object^ Parameter) {
	GetPicture();
}

void MainViewModel::SavePicture(Platform::Object^ Parameter) {
	SavPicture();
}

void MainViewModel::NotifyUser(Platform::String^ strMessage, NotifyType type)
{
	switch (type)
	{
	case NotifyType::StatusMessage:
		StatusForeground = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Colors::Gray);
		break;
	case NotifyType::ErrorMessage:
		StatusForeground = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Colors::Red);
		break;
	case NotifyType::SuccessMessage:
		StatusForeground = ref new Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Colors::Green);
		break;
	default:
		break;
	}
	Status = strMessage;
}

void MainViewModel::GetPicture()
{
	// Notifiy the user that processing has begun.
	NotifyUser("Selecting...", NotifyType::StatusMessage);

	auto fileUtil = new FileUtil();

	create_task(fileUtil->picker->PickSingleFileAsync()).then([this](StorageFile^ file)
	{
		if (file)
		{
			// Save the file for grayscale conversion.
			SelectedImageFile = file;

			create_task(file->OpenAsync(Windows::Storage::FileAccessMode::Read)).then([this](IRandomAccessStream^ Stream)
			{
				// Create a new BitmapImage and set the file stream as source.
				OriginalImageSource = ref new BitmapImage();
				OriginalImageSource->SetSourceAsync(Stream);

				// Enable the conversion button and notify the user.
				ConvertButtonIsEnabled = true;
				NotifyUser("Photo selected. Ready to convert.", NotifyType::StatusMessage);

				// Invalidate Modified Image if previously displayed.
				ModifiedImageSource = ref new WriteableBitmap(1, 1);
				SaveButtonIsEnabled = false;
			});
		}
		else {
			NotifyUser("Something went wrong. Retry", NotifyType::ErrorMessage);
		}
	});
}

void MainViewModel::ConvPicture()
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
			.then([this](BitmapDecoder^ decoder) {
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
					ModifiedImageSource = ref new WriteableBitmap(width, height);

					// Get access to the pixels.
					IBuffer^ buffer = ModifiedImageSource->PixelBuffer;

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

							// Get RGB values.
							double blue = (double)(SourcePixels[iSrc++]);
							double green = (double)(SourcePixels[iSrc++]);
							double red = (double)(SourcePixels[iSrc++]);

							// Calculate gray value.
							byte componentLuma = GrayscaleUtil::calculateLumaComponent(red, green, blue);

							// Transfer the pixel bytes.
							// Alpha channel is unchanged.
							DestinationPixels[iDst++] = componentLuma;		// Blue
							DestinationPixels[iDst++] = componentLuma;		// Green
							DestinationPixels[iDst++] = componentLuma;		// Red
							DestinationPixels[iDst] = SourcePixels[iSrc];	// Alpha channel
						}
					}
					// Get end timestamp and calculate difference.
					auto finish = std::chrono::high_resolution_clock::now();
					auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);

					// Convert microseconds to seconds.
					double seconds = (double)microseconds.count() / 1000000;

					NotifyUser("Done! Elapsed time: " + seconds.ToString() + " seconds.", NotifyType::SuccessMessage);
					SaveButtonIsEnabled = true;
				});
			});
		});
	});
}

void MainViewModel::SavPicture() 
{
	// Notify the user that the process has begun.
	NotifyUser("Saving image...", NotifyType::StatusMessage);

	// Prepare the SavePicker with available extensions.
	FileSavePicker^ picker = ref new FileSavePicker();
	auto imgExtensions = ref new Platform::Collections::Vector<Platform::String^>();
	imgExtensions->Append(".jpg");
	picker->FileTypeChoices->Insert("JPG File", imgExtensions);

	create_task(picker->PickSaveFileAsync()).then([this](StorageFile^ saveFile)
	{
		if (saveFile == nullptr)
		{
			return;
		}
		create_task(saveFile->OpenAsync(FileAccessMode::ReadWrite)).then([this](IRandomAccessStream^ stream)
		{
			create_task(BitmapEncoder::CreateAsync(BitmapEncoder::JpegEncoderId, stream)).then([this](BitmapEncoder^ encoder)
			{
				IBuffer^ buffer = ModifiedImageSource->PixelBuffer;
				DataReader^ dataReader = DataReader::FromBuffer(buffer);
				Platform::Array<unsigned char, 1>^ pixels = ref new Platform::Array<unsigned char, 1>(buffer->Length);
				dataReader->ReadBytes(pixels);
				encoder->SetPixelData(BitmapPixelFormat::Bgra8, BitmapAlphaMode::Ignore, ModifiedImageSource->PixelWidth, ModifiedImageSource->PixelHeight, 96.0, 86.0, pixels);
				create_task(encoder->FlushAsync());
				NotifyUser("Image saved.", NotifyType::SuccessMessage);
			});
		});
	});
}

void MainViewModel::OnPropertyChanged(Platform::String^ propertyName)
{
	PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
}
