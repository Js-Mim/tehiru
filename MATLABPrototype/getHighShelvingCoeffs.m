function [b,a] = getHighShelvingCoeffs(V0,fc,fs)
%{
==============================================================================

This file contains the function that compute high-shelving filter coefficients.
To be used in the main file "recordsimul.m"m".
This file serves as a prototype part of Tehiru.

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
K = tan(2*pi*fc/fs);
BW = 2.15;

if V0>=1   
    
     %Numerators
     b(1) = (V0 + BW*sqrt(V0)*K + K*K) / (1 + BW*K + K*K);
     b(2) = (2 * (K*K - V0) ) / (1 + BW*K + K*K);
     b(3) = (V0 - BW*sqrt(V0)*K + K*K) / (1 + BW*K + K*K);
     
     %Denumerators
     a(1) = 1;
     a(2) = (2 * (K*K - 1) ) / (1 + BW*K + K*K);
     a(3) = (1 - BW*K + K*K) / (1 + BW*K + K*K);
  
else
    
    V0 = 1/V0;
    
    %Numerators
    b(1) = (1 + BW*K + K*K) / (V0 + BW*sqrt(V0)*K + K*K);
    b(2) = (2 * (K*K - 1) ) / (V0 + BW*sqrt(V0)*K + K*K);
    b(3) = (1 - BW*K + K*K) / (V0 + BW*sqrt(V0)*K + K*K);
    
    %Denumerators
    a(1) = 1;
    a(2) = (2 * ((K*K)/V0 - 1) ) / (1 + BW/sqrt(V0)*K + (K*K)/V0);
    a(3) = (1 - BW/sqrt(V0)*K + (K*K)/V0) / (1 + BW/sqrt(V0)*K + (K*K)/V0);

end