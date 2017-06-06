#pragma once

using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;

namespace RelayCommand {

	public delegate void ExecuteDelegate(Platform::Object^ parameter);
	public delegate bool CanExecuteDelegate(Platform::Object^ parameter);

	public ref class DelegateCommand sealed : public ICommand
	{
	private:
		ExecuteDelegate^ executeDelegate;
		CanExecuteDelegate^ canExecuteDelegate;
		bool lastCanExecute;

	public:
		DelegateCommand(ExecuteDelegate^ execute, CanExecuteDelegate^ canExecute);

		virtual event EventHandler<Platform::Object^>^ CanExecuteChanged;
		virtual void Execute(Platform::Object^ parameter);
		virtual bool CanExecute(Platform::Object^ parameter);
	};
}

