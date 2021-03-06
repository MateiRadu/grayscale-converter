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
#include "FileUtil.h"

FileUtil::FileUtil()
{
	FileUtil::setupFilePicker();
}

FileUtil::~FileUtil()
{
}

void FileUtil::setupFilePicker()
{
	using namespace Windows::Storage::Pickers;

	FileUtil::picker = ref new FileOpenPicker();
	FileUtil::picker->ViewMode = PickerViewMode::Thumbnail;
	FileUtil::picker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;

	// Populate filter with the allower file formats list.
	for (Platform::String^ fileFormat : FileUtil::allowedFileFormats) {
		FileUtil::picker->FileTypeFilter->Append(fileFormat);
	}
}

const std::vector<Platform::String^> FileUtil::allowedFileFormats = {
	".jpg",
	".jpeg",
	".png",
};