function [Delta_T, VMic1, VMic2,VM1HsF,VM2HsF,operation] = ...
    get_coefficients(recording_technique, focus, source_angle, fs)
%{
==============================================================================

This file contains the function that compute core-processing coefficients
and parameters. To be used in the main file "recordsimul.m".
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
%Microphone Types Values (Omni, Cardioid, 8-Figure, Hyper-Cardioid)
A_s = [1 , 0.5, 0, 0.366];
B_s = [0 , 0.5, 1, 0.634];

    switch recording_technique
        case 'AB'
            mic_distance = 0.20;
            mic1_angle = 0;
            mic2_angle = 0;
            A1 = A_s(2);
            B1 = B_s(2);
            A2 = A_s(2);
            B2 = B_s(2);
            operation = 0;
            REF1 = 0.33;
            REF2 = 0.33;
            coincident = 0;
            
        case 'AB Omni'
            mic_distance = 0.20;
            mic1_angle = 0;
            mic2_angle = 0;
            A1 = A_s(1);
            B1 = B_s(1);
            A2 = A_s(1);
            B2 = B_s(1);
            operation = 0;
            REF1 = 1;
            REF2 = 1;
            coincident = 0;
            
        case 'AB Spaced'
            mic_distance = 1.95;
            mic1_angle = 0;
            mic2_angle = 0;
            A1 = A_s(2);
            B1 = B_s(2);
            A2 = A_s(2);
            B2 = B_s(2);
            operation = 0;
            REF1 = 0.33;
            REF2 = 0.33;
            coincident = 0;
            
        case 'XY'
            mic_distance = 0;
            mic1_angle = 45;
            mic2_angle = 45;
            A1 = A_s(2);
            B1 = B_s(2);
            A2 = A_s(2);
            B2 = B_s(2);
            operation = 0;
            REF1 = 0.33;
            REF2 = 0.33;
            coincident = 1;
            
        case 'Blumlein'
            mic_distance = 0;
            mic1_angle = 45;
            mic2_angle = 45;
            A1 = A_s(3);
            B1 = B_s(3);
            A2 = A_s(3);
            B2 = B_s(3);
            operation = 0;
            REF1 = 0.33;
            REF2 = 0.33;
            coincident = 1;
            
        case 'M/S'
            mic_distance = 0;
            mic1_angle = 0;
            mic2_angle = 90;
            A1 = A_s(2);
            B1 = B_s(2);
            A2 = A_s(3);
            B2 = B_s(3);
            operation = 1;
            REF1 = 0.33;
            REF2 = 0.33;
            coincident = 1;
            
        case 'ORTF'
            mic_distance = 0.17;
            mic1_angle = 55;
            mic2_angle = 55;
            A1 = A_s(2);
            B1 = B_s(2);
            A2 = A_s(2);
            B2 = B_s(2);
            operation = 0;
            REF1 = 0.33;
            REF2 = 0.33;
            coincident = 0;
            
        case 'DIN'
            mic_distance = 0.20;
            mic1_angle = 45;
            mic2_angle = 45;
            A1 = A_s(2);
            B1 = B_s(2);
            A2 = A_s(2);
            B2 = B_s(2);
            operation = 0;
            REF1 = 0.33;
            REF2 = 0.33;
            coincident = 0;
            
        case 'NOS'
            mic_distance = 0.30;
            mic1_angle = 45;
            mic2_angle = 45;
            A1 = A_s(2);
            B1 = B_s(2);
            A2 = A_s(2);
            B2 = B_s(2);
            operation = 0;
            REF1 = 0.33;
            REF2 = 0.33;
            coincident = 0;
    end
    
% Geometrical Computation of relative distances & angles.
% Solution on the one triangle is needed.
% Convert to radians
theta = abs(source_angle);
% Compute distance from the origin R0 using the law of sines
% Assumption 1 : The source is moving in a circular movement, which is
% useful for binaural. Then :
% R0 = source_distance;
% Assumption 2 : Re-panning with angle will push further away the source.
% Thus, recompute the distance from the origin. Useful for stereo and easy
% visualisation.

if coincident == 0
    
    source_distance = 1 - focus;
    R0 = (sind(90)*source_distance)/(sind(90 - theta));
    % Compute the distances from the microphones
    R1 = sqrt(R0^2 + (mic_distance/2)^2 - 2*R0*(mic_distance/2)*cosd(90 + theta));
    R2 = sqrt(R0^2 + (mic_distance/2)^2 - 2*R0*(mic_distance/2)*cosd(90 - theta));
    
    % Compute source / microphone angle
    R1ang = mic1_angle + 90 - acosd((R1^2 + mic_distance^2 - R2^2)/(2*mic_distance*R1));
    R2ang = mic2_angle + 90 - acosd((R2^2 + mic_distance^2 - R1^2)/(2*mic_distance*R2));

else
    source_distance = 0.45 + focus;
    
    if source_distance > 1
        
        R0a = 1/(sind(90 - theta));
        R0b = sqrt((1-source_distance)*(1-source_distance) + R0a*R0a - ...
            2*abs(1-source_distance)*R0a*cosd(180-theta));
        theta2 = acosd(((1-source_distance)*(1-source_distance) + R0b*R0b - R0a*R0a)/...
            (2*abs(1-source_distance)*R0b));
        
        R1 = R0b;
        R2 = R0b;
        R1ang = mic1_angle - theta2;
        R2ang = mic2_angle + theta2;
        
     elseif source_distance < 1
        
        R0a = 1/(sind(90 - theta));
        R0b = sqrt((1-source_distance)^2 + R0a^2-2*abs(1-source_distance)*R0a*cosd(theta));
        theta2 = 180 - theta - asind((sind(theta)*abs(1-source_distance))/R0b);
        R1 = R0b;
        R2 = R0b;
        R1ang = mic1_angle - theta2;
        R2ang = mic2_angle + theta2;
          
    else
        
        R0 = (sind(90)*source_distance)/(sind(90 - theta));
        R1 = R0;
        R2 = R0;
        R1ang = mic1_angle - theta;
        R2ang = mic2_angle + theta;
                
    end  
end

theta1 = round(abs(R1ang));
theta2 = round(abs(R2ang));

% Room properties
absr_coeff = focus; % Larger = Better Localisation, Smaller = Better Difussion
Surf = 1000;
R = (Surf*absr_coeff)/(1 - absr_coeff);
% Coefficient Computation
% Speed of sound
c = 343.8;   
%ITD
Delta_T = round(((abs(R1-R2))/c) * fs);
%ILD
% Voltage due to angle
VA1 = (A1 + B1 * abs(cosd(theta1)));
VA2 = (A2 + B2 * abs(cosd(theta2)));
% Voltage due to Random Effective Energy
Vref1 = (sqrt(1-REF1));
Vref2 = (sqrt(1-REF2));

% % Sound Pressure Difference
fr1 = ((4.0 / (4.0 * 3.1415927 * R1*R1)) + 4.0 / R) / ((4.0 / (4.0 * 3.1415927 * R2*R2)) + 4.0 / R);
fr2 = (Vref1*(1.0 - focus) + VA1) / (Vref2*(1.0 - focus) + VA2);

DeltaL = abs(10.0 * log10(fr1) + 20.0 * log10(fr2)); 

% Modification of parameters
if (source_angle > 0) && (operation == 0)
    
    VMic1 = 10^(-DeltaL/20);
    VMic2 = 1;
    %Voltage over frequency bands for the two mics
    VM1HsF = VA1 + 0.158;
%    VM1HpF = VA1 + 0.3548;
    VM2HsF = VA2 + 0.158;
%    VM2HpF = VA2 + 0.3548;

elseif (source_angle == 0) && (operation == 0)
    
    VMic1 = 10^(-DeltaL/20);
    VMic2 = 10^(-DeltaL/20);   
    VM1HsF = VA1 + 0.158;
    VM2HsF = VA2 + 0.158;
    
elseif (source_angle < 0) && (operation == 0)
    
    VMic1 = 1;
    VMic2 = 10^(-DeltaL/20);
    VM1HsF = VA2 + 0.158;
%    VM1HpF = VA2 + 0.3548;
    VM2HsF = VA1 + 0.158;
%    VM2HpF = VA1 + 0.3548;

elseif (abs(source_angle) <= 35) && (operation == 1)
    
    VMic1 = 1;
    VMic2 = 10^(-DeltaL/20);
    VM1HsF = VA1 + 0.158;    
    VM2HsF = VA2 + 0.158;
    
elseif (abs(source_angle) > 35) && (operation == 1)
    
    VMic2 = 1;
    VMic1 = 10^(-DeltaL/20);
    VM2HsF = VA1 + 0.158;
    VM1HsF = VA2 + 0.158;
  
end

end