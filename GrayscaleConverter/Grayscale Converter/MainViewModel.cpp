#include "pch.h"
#include "MainViewModel.h"

using namespace ViewModel;
using namespace Windows::UI::Xaml::Data;
using namespace RelayCommand;

MainViewModel::MainViewModel()
{
	SetNotifyReadyCommand = ref new DelegateCommand(
		ref new ExecuteDelegate(this, &MainViewModel::SetNotifyReady),
		nullptr);

	SetNotifySelectedCommand = ref new DelegateCommand(
		ref new ExecuteDelegate(this, &MainViewModel::SetNotifySelected),
		nullptr);
	MainViewModel::SetNotifyReady(NULL);
}

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

void MainViewModel::SetNotifyReady(Platform::Object^ Parameter) {
	MainViewModel::Status = "Ready. Pick a picture to modify.";
}

void MainViewModel::SetNotifySelected(Platform::Object^ Parameter) {
	MainViewModel::Status = "Photo selected. Ready to convert.";
}


void MainViewModel::OnPropertyChanged(Platform::String^ propertyName)
{
	PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
}
