function [b,a] = getAllPassCoeffs(Fc, Fs, Q)
%{
==============================================================================

This file contains the function that compute all-pass filter coefficients.
To be used in the main file "recordsimul.m"
This file serves as a prototype, part of Tehiru.

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
%}
K = tan(pi*(Fc/Fs));
BW  = 1/Q;
k  = 1/(1 + BW*K + K^2);

%Numerators
b(1) = (1 - K*BW + K^2) * k;
b(2) = 2 * (K^2 - 1) * k;
b(3) = 1;

%Denumerators
a(1) = 1;
a(2) = b(2);
a(3) = b(1);