#include "pch.h"
#include "MainViewModel.h"

using namespace concurrency;
using namespace ViewModel;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace RelayCommand;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Streams;

MainViewModel::MainViewModel()
{
	SetNotifyReadyCommand = ref new DelegateCommand(
		ref new ExecuteDelegate(this, &MainViewModel::SetNotifyReady),
		nullptr);

	SetNotifySelectedCommand = ref new DelegateCommand(
		ref new ExecuteDelegate(this, &MainViewModel::SetNotifySelected),
		nullptr);
	MainViewModel::SetNotifyReady(nullptr);
	MainViewModel::ConvertButtonIsEnabled = false;
	MainViewModel::SaveButtonVisibility = Windows::UI::Xaml::Visibility::Collapsed;
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

Windows::UI::Xaml::Visibility MainViewModel::SaveButtonVisibility::get()
{
	return m_SaveButtonVisibility;
}

void MainViewModel::SaveButtonVisibility::set(Windows::UI::Xaml::Visibility value)
{
	if (m_SaveButtonVisibility != value)
	{
		m_SaveButtonVisibility = value;
		OnPropertyChanged("SaveButtonVisibility");
	}
}

Windows::UI::Xaml::Visibility MainViewModel::ConvertButtonVisibility::get()
{
	return m_ConvertButtonVisibility;
}

void MainViewModel::ConvertButtonVisibility::set(Windows::UI::Xaml::Visibility value)
{
	if (m_ConvertButtonVisibility != value)
	{
		m_ConvertButtonVisibility = value;
		OnPropertyChanged("ConvertButtonVisibility");
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

// Functions.

void MainViewModel::SetNotifyReady(Platform::Object^ Parameter) {
	NotifyUser("Ready. Pick a picture to modify.", NotifyType::StatusMessage);
}

void MainViewModel::SetNotifySelected(Platform::Object^ Parameter) {
	GetPicture();
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

				// Enable the conversion button and notify the user.
				ConvertButtonIsEnabled = true;
				NotifyUser("Photo selected. Ready to convert.", NotifyType::StatusMessage);

				// Invalidate Modified Image if previously displayed.
				BitmapImage^ EmptyImageSource = ref new BitmapImage();
				//ModifiedImage->Source = EmptyImageSource;
				SaveButtonVisibility = Windows::UI::Xaml::Visibility::Collapsed;
				ConvertButtonVisibility = Windows::UI::Xaml::Visibility::Visible;
			});
		}
		else {
			NotifyUser("Something went wrong. Retry", NotifyType::ErrorMessage);
		}
	});
}


void MainViewModel::OnPropertyChanged(Platform::String^ propertyName)
{
	PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
}
