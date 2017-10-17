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
#include "DelegateCommand.h"

using namespace Grayscale_Converter;

DelegateCommand::DelegateCommand(ExecuteDelegate^ execute, CanExecuteDelegate^ canExecute)
	: executeDelegate(execute), canExecuteDelegate(canExecute)
{
}

void DelegateCommand::Execute(Platform::Object^ parameter)
{
	if (executeDelegate != nullptr)
	{
		executeDelegate(parameter);
	}
}

bool DelegateCommand::CanExecute(Platform::Object^ parameter)
{
	if (canExecuteDelegate == nullptr)
	{
		return true;
	}

	bool canExecute = canExecuteDelegate(parameter);

	if (lastCanExecute != canExecute)
	{
		lastCanExecute = canExecute;
		CanExecuteChanged(this, nullptr);
	}

	return lastCanExecute;
}
