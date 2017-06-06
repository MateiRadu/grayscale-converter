#pragma once

#include "DelegateCommand.h"

using namespace Windows::UI::Xaml::Data;

namespace ViewModel {
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
		property ICommand^ SetNotifySelectedCommand;
		property ICommand^ SetNotifyReadyCommand;

	private:
		Platform::String^ m_Status;
		void OnPropertyChanged(Platform::String^ propertyName);
		void SetNotifySelected(Platform::Object^ parameter);
		void SetNotifyReady(Platform::Object^ parameter);
	};
}