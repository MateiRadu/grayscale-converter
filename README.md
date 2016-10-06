# Grayscale Converter
A simple color-to-grayscale UWP C++ application.

The application demonstrates these tasks:

1.  **Let the user pick one picture**

    Uses the **FileOpenPicker**.**PickSingleFileAsync** to call a file picker window and let the user pick a single image (jpg, jpeg or png).
2. **Creates a bitmap and displays the image to the user**

    A **BitmapImage** will be created and set using **SetSourceAsync** with the stream provided by the **StorageFile** of the selected image.
    
3. **Decode, modifiy and Encode**

    Upon user command, the image will be decoded, converted and displayed next to the original picture for comparison.

## Screenshot
![Screenshot](https://github.com/MateiRadu/grayscale-converter/blob/master/Screenshots/W10Desktop.png)

## Formula
The specific gray for pixel is calculated by assigning to each color their average:
```
byte OldRed, OldGreen, OldBlue;
byte Average = (OldRed + OldGreen + OldBlue) / 3;

byte NewRed   = Average;
byte NewGreen = Average;
byte NewBlue  = Average;
```

## Future developments
Some features would be nice to implement and I will eventually implement:
- Drag n drop
- Mobile optimized layout
- Elapset time display

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
