//*****************************************************************************
//
//  Grayscale Converter, a simple color-to-grayscale UWP converter.
//  Copyright(C) 2017 Matei Bogdan Radu
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
#include "Settings.xaml.h"

using namespace Grayscale_Converter;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;

Settings::Settings()
{
	InitializeComponent();
	this->NavigationCacheMode = Navigation::NavigationCacheMode::Enabled;
}

void Settings::OnNavigatedTo(NavigationEventArgs^ e)
{
	using Windows::UI::Core::SystemNavigationManager;
	using Windows::UI::Core::AppViewBackButtonVisibility;

	auto rootFrame = dynamic_cast<Controls::Frame^>(Window::Current->Content);

	if (rootFrame == nullptr)
	{
		return;
	}

	if (rootFrame->CanGoBack)
	{
		// If we have pages in our in-app backstack and have opted in to showing back, do so
		SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Visible;
	}
	else
	{
		// Remove the UI from the title bar if there are no pages in our in-app back stack
		SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Collapsed;
	}
}
