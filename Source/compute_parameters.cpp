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
#include "compute_parameters.h"
using namespace std;

tuple<int, float, float, int, float, float> compute_params(int recording_technique, 
	int source_angle, float focus, int fs)
{

	//Variables
	int theta, mic1_angle, mic2_angle, operation, coincident,Delta_T;
	float R0, mic_distance, A1, A2, B1, B2, REF1, REF2, R1, R2, R, 
		R1ang, R2ang, theta1, theta2, VA1, VA2, VMic1, VMic2, Vref1,
		Vref2, VM1HsF, VM2HsF, DeltaL, source_distance;

	// Mic-Directivity coefficients
	float A_s[4] = {1.0f, 0.5f, 0.0f, 0.366f};
	float B_s[4] = {0.0f, 0.5f, 1.0f, 0.634f};

	switch (recording_technique)
	{

	case 1:
		mic_distance = 0.0f;
		mic1_angle = 45;
		mic2_angle = 45;
		A1 = A_s[1];
		B1 = B_s[1];
		A2 = A_s[1];
		B2 = B_s[1];
		operation = 0;
		REF1 = 0.33f;
		REF2 = 0.33f;
		coincident = 1;
		break;

	case 2:
		mic_distance = 0.2f;
		mic1_angle = 0;
		mic2_angle = 0;
		A1 = A_s[1];
		B1 = B_s[1];
		A2 = A_s[1];
		B2 = B_s[1];
		operation = 0;
		REF1 = 0.33f;
		REF2 = 0.33f;
		coincident = 0;
		break;

	case 3:
		mic_distance = 1.95f;
		mic1_angle = 0;
		mic2_angle = 0;
		A1 = A_s[1];
		B1 = B_s[1];
		A2 = A_s[1];
		B2 = B_s[1];
		operation = 0;
		REF1 = 0.33f;
		REF2 = 0.33f;
		coincident = 0;
		break;
	
	case 4:
		mic_distance = 0.17f;
		mic1_angle = 55;
		mic2_angle = 55;
		A1 = A_s[1];
		B1 = B_s[1];
		A2 = A_s[1];
		B2 = B_s[1];
		operation = 0;
		REF1 = 0.33f;
		REF2 = 0.33f;
		coincident = 0;
		break;

	case 5:
		mic_distance = 0.20f;
		mic1_angle = 45;
		mic2_angle = 45;
		A1 = A_s[1];
		B1 = B_s[1];
		A2 = A_s[1];
		B2 = B_s[1];
		operation = 0;
		REF1 = 0.33f;
		REF2 = 0.33f;
		coincident = 0;
		break;

	case 6:
		mic_distance = 0.30f;
		mic1_angle = 45;
		mic2_angle = 45;
		A1 = A_s[1];
		B1 = B_s[1];
		A2 = A_s[1];
		B2 = B_s[1];
		operation = 0;
		REF1 = 0.33f;
		REF2 = 0.33f;
		coincident = 0;
		break;

	case 7:
		mic_distance = 0.0;
		mic1_angle = 0;
		mic2_angle = 90;
		A1 = A_s[1];
		B1 = B_s[1];
		A2 = A_s[2];
		B2 = B_s[2];
		operation = 1;
		REF1 = 0.33f;
		REF2 = 0.33f;
		coincident = 1;
		break;

	default:
		mic_distance = 0.0;
		mic1_angle = 0;
		mic2_angle = 90;
		A1 = A_s[1];
		B1 = B_s[1];
		A2 = A_s[2];
		B2 = B_s[2];
		operation = 1;
		REF1 = 0.33f;
		REF2 = 0.33f;
		coincident = 1;
		break;

	}

	theta = abs(source_angle);
	
	// Near Coincident/Spaced Techniques
	if (coincident == 0)
	{		
		source_distance = 1.00f - focus;
		
		R0 = (source_distance) / (sind( (90.0f - theta) ));

		float const mic_distance_h = (mic_distance/2.0f);
		float const mic_distance_hp = mic_distance_h*mic_distance_h;
		float const mic_distance_p = mic_distance*mic_distance;
		float const R0p = R0 * R0;

		R1 = sqrt(R0p + mic_distance_hp - 2.0f * R0*mic_distance_h*cosd( (90 + theta) ));
		R2 = sqrt(R0p + mic_distance_hp - 2.0f * R0*mic_distance_h*cosd( (90 - theta) ));

		float const comp_angle1 = (R1*R1 + mic_distance_p - R2*R2) / (2.0f * mic_distance*R1);
		float const comp_angle2 = ((R2*R2 + mic_distance_p - R1*R1) / (2.0f * mic_distance*R2));

		R1ang = mic1_angle + 90.0f - acosd( comp_angle1 );
		R2ang = mic2_angle + 90.0f - acosd( comp_angle2 );
	}
	// Coincident Techniques
	else
	{
		
		source_distance = 0.45f + focus;
			
		if (source_distance > 1.0f)
		{
			float const R0a = 1.0f / ( sind( (90-theta) ) );

			float const msd = (1.0f - source_distance);

			float const R0b = sqrt( ( msd*msd + R0a*R0a - 
				(2.0f * abs(msd)*R0a*cosd( (180 - theta) ) ) ) );

			theta2 = acosd( (roundf( ((msd*msd + R0b*R0b - R0a*R0a) / (2.0f * abs(msd)*R0b)) * 100.0f)/100.0f) );

			R1 = R0b;
			R2 = R0b;
			R1ang = (float) (mic1_angle - theta2);
			R2ang = (float) (mic2_angle + theta2);

		}
		else if (source_distance < 1.0f)
		{

			float const R0a = 1.0f / (sind( (90 - theta) ));

			float const msd = (1.0f - source_distance);

			float const R0b = sqrt(msd*msd + R0a*R0a - (2.0f * abs(1.0f - source_distance)*R0a*cosd(theta)));

			theta2 = 180.0f - theta - asind( ((roundf((sind(theta)*abs(msd) / R0b) * 100.0f))/100.0f) );
			R1 = R0b;
			R2 = R0b;
			R1ang = (float) (mic1_angle - theta2);
			R2ang = (float) (mic2_angle + theta2);
		}
		else
		{
			R0 = (sind(90)*source_distance) / (sind( (90 - theta) ));
			R1 = R0;
			R2 = R0;
			R1ang = (float) (mic1_angle - theta);
			R2ang = (float) (mic2_angle + theta);

		}
	}

	theta1 = round(abs((R1ang)));
	theta2 = round(abs(R2ang));

	//Coefficient Computation
	// Absorption
	R = (1000.0f * focus)/(1.0f - focus);

	//ITD
	Delta_T = (int) round(((abs((R1 - R2)) )/343.8f) * fs);

	//ILD
	//Voltage due to angle
	VA1 = (A1 + B1 * abs(cosd(theta1)));
	VA2 = (A2 + B2 * abs(cosd(theta2)));

	//Voltage due to Random Effective Energy
	Vref1 = sqrt(1.0f - REF1);
	Vref2 = sqrt(1.0f - REF2);

	//Sound Pressure Difference
	float const fr1 = ((4.0f / (4.0f * PI * R1*R1)) + 4.0f / R) / ((4.0f / (4.0f * PI * R2*R2)) + 4.0f / R);
	float const fr2 = (Vref1*(1.0f - focus) + VA1) / (Vref2*(1.0f - focus) + VA2);
	DeltaL = abs( (10.0f * log10(fr1) + 20.0f * log10(fr2)) );
	
	// Exporting parameters
	if (source_angle > 0 && operation == 0)
	{

		VMic1 = pow(10.0f, (-DeltaL/20.0f));
		VMic2 = 1.0f;
		//Voltage over frequency bands for the two mics
		VM1HsF = VA1 + 0.158f;
		//VM1LsF = VA1 + 0.3548;
		VM2HsF = VA2 + 0.158f;
		//VM2LsF = VA2 + 0.3548;
	}
	else if (source_angle < 0 && operation == 0)
	{

		VMic1 = 1.0f;
		VMic2 = pow(10.0f, (-DeltaL / 20.0f));
		VM1HsF = VA2 + 0.158f;
		//VM1HpF = VA2 + 0.3548;
		VM2HsF = VA1 + 0.158f;
		//VM2HpF = VA1 + 0.3548;
	}
	else if (source_angle == 0 && operation == 0)
	{
		VMic1 = pow(10.0f, (-DeltaL / 20.0f));
		VMic2 = pow(10.0f, (-DeltaL / 20.0f));
		VM1HsF = VA1 + 0.158f;
		VM2HsF = VA2 + 0.158f;
	}

	else if ( abs(source_angle) <= 35 && operation == 1)
	{
		VMic1 = 1.0f;
		VMic2 = pow(10.0f, (-DeltaL / 20.0f));
		VM1HsF = VA1 + 0.158f;
		VM2HsF = VA2 + 0.158f;
		Delta_T = (int) round((3.8044f / 343.8f) * fs);
	}

	else if ( abs(source_angle) > 35 && operation == 1)
	{
		VMic2 = 1.0f;
		VMic1 = pow(10.0f, (-DeltaL / 20.0f));
		VM2HsF = VA1 + 0.158f;
		VM1HsF = VA2 + 0.158f;
		Delta_T = (int) round((3.8044f/ 343.8f) * fs);
	}

	return make_tuple(Delta_T, VMic1, VMic2, operation, VM1HsF, VM2HsF);
}

tuple<float, float, float, float, float> getHighShelvCoeffs(float V0, int fc, int fs)
{

	float b0, b1, b2, a0, a1, a2;
	float const K = tan(2.0f * PI * fc / fs);
	float const BW = 2.15f;

	if (V0 >= 1.0f)
	{
	
		//Numerators
		b0 = (V0 + BW*sqrt(V0)*K + K*K) / (1 + BW*K + K*K);
		b1 = (2.0f * (K*K - V0)) / (1.0f + BW*K + K*K);
		b2 = (V0 - BW*sqrt(V0)*K + K*K) / (1 + BW*K + K*K);

		//Denumerators
		a0 = 1.0f;
		a1 = (2.0f * (K*K - 1)) / (1 + BW*K + K*K);
		a2 = (1.0f - BW*K + K*K) / (1 + BW*K + K*K);
	}

	else
	{

		V0 = 1.0f/V0;
		//Numerators
		b0 = (1.0f + BW*K + K*K) / (V0 + BW*sqrt(V0)*K + K*K);
		b1 = (2.0f * (K*K - 1.0f)) / (V0 + BW*sqrt(V0)*K + K*K);
		b2 = (1.0f - BW*K + K*K) / (V0 + BW*sqrt(V0)*K + K*K);

		//Denumerators
		a0 = 1.0f;
		a1 = (2.0f * ((K*K)/V0 - 1.0f) ) / (1.0f + BW/sqrt(V0)*K + (K*K)/V0);
		a2 = (1.0f - BW/sqrt(V0)*K + (K*K)/V0) / (1.0f + BW/sqrt(V0)*K + (K*K)/V0);

	}

	return make_tuple(b0, b1, b2, a1, a2);

}

tuple<float, float, float, float, float> getLowShelvCoeffs(float V0, int fc, int fs)
{

	float b0, b1, b2, a0, a1, a2;
	float const K = tan(2.0f * PI * fc / fs);
	float const BW = 2.75f;

	if (V0 >= 1.0f)
	{

		//Numerators
		b0 = (1.0f + BW*sqrt(V0)*K + V0*K*K) / (1.0f + BW*K + K*K);
		b1 = (2.0f * (V0*K*K - 1.0f)) / (1.0f + BW*K + K*K);
		b2 = (1.0f - BW*sqrt(V0)*K + V0*K*K) / (1.0f + BW*K + K*K);

		//Denumerators
		a0 = 1.0f;
		a1 = (2.0f * (K*K - 1.0f)) / (1.0f + BW*K + K*K);
		a2 = (1.0f - BW*K + K*K) / (1.0f + BW*K + K*K);
	}

	else
	{

		V0 = 1.0f / V0;
		//Numerators
		b0 = (1.0f + BW*K + K*K) / (1.0f + BW*sqrt(V0)*K + V0*K*K);
		b1 = (2.0f * (K*K - 1)) / (1.0f + BW*sqrt(V0)*K + V0*K*K);
		b2 = (1.0f - BW*K + K*K) / (1.0f + BW*sqrt(V0)*K + V0*K*K);


		//Denumerators
		a0 = 1.0f;
		a1 = (2.0f * ((V0*K*K - 1.0f))) / (1.0f + BW*K + K*K);
		a2 = (1.0f - BW*K + K*K) / (1.0f + BW*K + K*K);

	}

	return make_tuple(b0, b1, b2, a1, a2);

}

tuple<float, float, float, float, float, float, float, float, float, float> getPrecompAllpasCoeffs()
{
	// Pre-computed coefficients for two all-pass filters with different transition(phase) frequency
	float const b0A = 0.7863f;   
	float const b1A = -1.7831f;
	float const b2A = 1.0000f;
	//float const a0A = 1.0000f;    
	float const a1A = -1.7831f;
	float const a2A = 0.7863f;

	float const b0B = 0.4896f;
	float const b1B = -1.4461f;
	float const b2B = 1.0000f;
	//float const a0B = 1.0000f;
	float const a1B = -1.4461f;
	float const a2B = 0.4896f;

	return make_tuple(b0A, b1A, b2A, a1A, a2A, b0B, b1B, b2B, a1B, a2B);
}