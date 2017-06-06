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

using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;

namespace Delegate {

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

