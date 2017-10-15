# Grayscale Converter
A simple color-to-grayscale UWP C++ application.

## Screenshot
![Screenshot](https://github.com/MateiRadu/grayscale-converter/blob/master/Screenshots/W10.png)

## Formula
The gray color for each pixel is calculated according to the [ITU-R Recommendation BT.709](https://en.wikipedia.org/wiki/Grayscale#Luma_coding_in_video_systems):
```cpp
const double BLUE = 0.0722;
const double GREEN = 0.7152;
const double RED = 0.2126;

// Calculate color components.
double componentBlue = sourceBlue * BLUE;
double componentGreen = sourceGreen * GREEN;
double componentRed = sourceRed * RED;

// Calculate the Luma component (gray).
byte componentLuma = (componentBlue + componentGreen + componentRed);

// Assign the Luma component to each primary.
byte newBlue = componentLuma;
byte newGreen = componentLuma;
byte newRed = componentLuma;
```

## Future developments
Some features would be nice to implement and I will eventually do:
- Drag n drop

## Requirements
In order to build and deploy the app, the following requirements must be met:
- Microsoft Visual Studio 2017 with Universal Windows App Development Tools
- Target platform version: 10.0.15063.0
- Target platform min. version: 10.0.10586.0
- A Windows 10 device

## License

Copyright(C) 2016, 2017 Matei Bogdan Radu

This program is free software : you can redistribute it and / or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.
