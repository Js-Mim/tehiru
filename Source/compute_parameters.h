/*
==============================================================================

This file contains the functions for computing coefficients and parameters,
useful for further processing audio signals.

Copyright (c) 2015 Stylianos Ioannis Mimilakis
Distributed under the terms of the Gnu General Public License

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
==============================================================================
*/

#ifndef COMPUTE_PARAMETERS_H_INCLUDED
#define COMPUTE_PARAMETERS_H_INCLUDED

#include <cmath>
#include <tuple>

// Helpful constants
#define PI (3.1415927f)
#define eps (0.00001f)

// Helpful functions
#define sind(x) (sinf( x * (PI / 180.0f) ))
#define cosd(x) (cosf( x * (PI / 180.0f) ))
#define tand(x) (tanf( x * (PI / 180.0f) ))
#define asind(x) (asinf(x) * (180.0f / PI))
#define acosd(x) (acosf(x) * (180.0f / PI))

// ITD, ILD computation
std::tuple<int, float, float, int, float, float> compute_params(int recording_technique,
	int source_angle, float focus, int fs);

// Filter coefficients computation & acquisition
std::tuple<float, float, float, float, float> getHighShelvCoeffs(float V0, int fc, int fs);
std::tuple<float, float, float, float, float> getLowShelvCoeffs(float V0, int fc, int fs);
std::tuple<float, float, float, float, float, float, float, float, float, float> getPrecompAllpasCoeffs();

#endif