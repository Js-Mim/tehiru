function [stereo_audio] = recordsimul(in, fs, recording_technique, focus, source_angle)
%{
==============================================================================

This file contains the function that loads and process the input monophonic
audio file.
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

% Initialization
if size(in,2) > 1;
    disp 'Stereo audio file detected, summing into mono...'
    x = sum(in,2);
else
    x = in;
end

[Delta_T, VMic1, VMic2, VM1HsF, VM2HsF,operation] = ...
    get_coefficients(recording_technique, focus, source_angle, fs); 

 [bL,aL] = getHighShelvingCoeffs(VM1HsF,8110,fs);
 [bR,aR] = getHighShelvingCoeffs(VM2HsF,8110,fs);
 
 xL = filter2(bL,aL,x);
 xR = filter2(bR,aR,x);
 
 [bL,aL] = getLowShelvingCoeffs(VM1HsF,88,fs);
 [bR,aR] = getLowShelvingCoeffs(VM2HsF,88,fs);
 
 xL = filter2(bL,aL,xL);
 xR = filter2(bR,aR,xR);
 
%Preallocation of the output
stereo_audio(length(x)+Delta_T , 2) = 0;

if operation == 0
    if source_angle < 0
        stereo_audio(1:length(x),1) = xL .* VMic1;
        stereo_audio(Delta_T+1:end,2) = xR .* VMic2;
    elseif source_angle > 0
        stereo_audio(Delta_T+1:end,1) = xL.* VMic1;
        stereo_audio(1:length(x),2) = xR .* VMic2;
    else
        stereo_audio(1+Delta_T:Delta_T + length(x),1) = xL .* VMic1;
        stereo_audio(1+Delta_T:Delta_T + length(x),2) = xR .* VMic2;
    end
else
    
    [b1,a1] = getAllPassCoeffs(420, 44100, 0.25);
    amb1 = filter2(b1,a1,xR)';
    [b2,a2] = getAllPassCoeffs(1700, 44100, 0.35);
    amb2 = filter2(b2,a2,xR)';
    
    Delta_T = round(((3.8044)/343.8) * fs);
    ps_amb(length(x)+Delta_T , 1) = 0;
    ps_amb(1+Delta_T:Delta_T + length(x)) = ((x-amb1) + (x-amb2)).*0.25;
    VMic2 = VMic2*(0.95-focus);
    
    if (source_angle ~= 0)
        
        g(2) = 1;
        g(1) = - (tand(source_angle)-tand(80)) / (tand(source_angle)+tand(80)+eps);
        g = g./sqrt(sum(g.^2));
        
        stereo_audio(1:length(x),1) = VMic2.*ps_amb(1:length(xL))' + xL.*(VMic1.*g(1));
        stereo_audio(1:length(x),2) = -VMic2.*ps_amb(1:length(xL))' + xL.*(VMic1.*g(2));
        
    else
        
        stereo_audio(1:length(x),1) = (1.05-focus).*ps_amb(1:length(xL))' + xL.*VMic1;
        stereo_audio(1:length(x),2) = -(1.05-focus).*ps_amb(1:length(xL))' + xL.*VMic1;
        
    end
end
end