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

#pragma once

class GrayscaleUtil
{
	public:
		GrayscaleUtil();
		~GrayscaleUtil();

		// Primary coefficients according to ITU-R Recommendation BT.709.
		static const double COEFF_BLUE;
		static const double COEFF_GREEN;
		static const double COEFF_RED;

		// Calculate the Luma component (gray) based on the RGB colors.
		static double calculateLumaComponent(double red, double green, double blue);
};

