# Grayscale Converter
A simple color-to-grayscale UWP C++ application.

The application demonstrates these tasks:

1.  **Let the user pick one picture**

    Uses the **FileOpenPicker**.**PickSingleFileAsync** to call a file picker window and let the user pick a single image (jpg, jpeg or png).
2. **Creates a bitmap and displays the image to the user**

    A **BitmapImage** will be created and set using **SetSourceAsync** with the stream provided by the **StorageFile** of the selected image.
    
3. **Decode, modifiy and Encode**

    Upon user command, the image will be decoded, converted and re-encoded to a new BitmapImage that will be displayed next to the original picture for comparison.

4. **Save results**

    User can save the grayscale image to disk.

## Formula
The formula used for the color conversion is the following, as suggested by the [International Commission on Illumination](http://www.cie.co.at/index.php).
```
Luminance  =  0.2126 × Red  +  0.7152 × Green  +  0.0722 × Blue
```

## Requirements
In order to build and deploy the app, the following requirements must be met:
- Microsoft Visual Studio 2015 with Universal Windows App Development Tools
- Target platform version: 10.0.14393
- Target platform min. version: 10.0.10586
- A Windows 10 device

## License

Copyright(C) 2016 Matei Bogdan Radu

This program is free software : you can redistribute it and / or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
