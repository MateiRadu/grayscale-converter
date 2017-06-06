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
			property ICommand^ SetNotifySelectedCommand;
			property ICommand^ SetNotifyReadyCommand;

		private:
			void OnPropertyChanged(Platform::String^ propertyName);

			// Status message.
			void SetNotifyReady(Platform::Object^ parameter);
			Platform::String^ m_Status;
			Windows::UI::Xaml::Media::SolidColorBrush^ m_StatusForeground;
			
			// Original image.
			void SetNotifySelected(Platform::Object^ parameter);
			void GetPicture();
			StorageFile^ SelectedImageFile;
			BitmapImage^ m_OriginalImageSource;
			bool m_ConvertButtonIsEnabled;
			Windows::UI::Xaml::Visibility m_SaveButtonVisibility;
			Windows::UI::Xaml::Visibility m_ConvertButtonVisibility;

		internal:
			void NotifyUser(Platform::String^ strMessage, NotifyType type);
	};
}