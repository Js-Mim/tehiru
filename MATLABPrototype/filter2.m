function y = filter2(b,a,x)
%{
==============================================================================

This file contains the function that performs a filtering procedure.
To be used in the main file "recordsimul.m".
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
y(length(x)) = 0;
xr = [0,0];
yr = [0,0];

for indx = 1:length(x);
    y(indx) = b(1)*x(indx) + b(2)*xr(1) + b(3)*xr(2) - ...
        a(2)*yr(1) - a(3)*yr(2);
    xr(2) = xr(1);
    xr(1) = x(indx);
    yr(2) = yr(1);
    yr(1) = y(indx);
end