## You have just discovered Tehiru.

Tehiru provides a method for spatial enhancement of single channel audio recordings. The principle behind it, is a simple architecture of audio filters, which is being derived from electro-acoustic and geometrical motivated computations. It allows simple and effective usage by 
three parameters denoted as the desired recording technique (Effectum), focus on the sound source and the spatial position/angle (Locus) of the source.

Inside you will find the MATLAB prototype and a real-time implementation(VST).
The real-time implementation is based on the JUCE C++ class library. Alongside the source code, pre-compiled files can be found under the 
"Builds" folder. 

## Usage
In case that the real-time implementation is desired, either the pre-compiled version can be directly inserted into a host applications, or it can be rebuilt by acquiring proper SDKs(Steinberg's VST).

On the other hand, the prototype can be used in MATLAB by calling 
the main function "recordsimul.m" i.e.:
my_stereo_output = recordsimul(mono_input, sampling_frequency, desired_technique, desired_focus, desired_source_angle);

## License
This is being distributed under the terms of the Gnu General Public License.

MATLAB® is a registered trademark of Mathworks.

For JUCE please visit : http://www.juce.com