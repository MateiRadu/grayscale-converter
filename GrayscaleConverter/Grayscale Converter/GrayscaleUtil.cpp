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
#include "GrayscaleUtil.h"

GrayscaleUtil::GrayscaleUtil()
{
}


GrayscaleUtil::~GrayscaleUtil()
{
}

const double GrayscaleUtil::COEFF_BLUE = 0.0722;
const double GrayscaleUtil::COEFF_GREEN = 0.7152;
const double GrayscaleUtil::COEFF_RED = 0.2126;

double GrayscaleUtil::calculateLumaComponent(double red, double green, double blue)
{
	double componentRed = red * GrayscaleUtil::COEFF_RED;
	double componentGreen = green * GrayscaleUtil::COEFF_GREEN;
	double componentBlue = blue * GrayscaleUtil::COEFF_BLUE;

	return componentBlue + componentGreen + componentRed;
}
