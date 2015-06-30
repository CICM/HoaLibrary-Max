### Hoa Library (A high order ambisonics library) for <a title="Max" href="http://cycling74.com/products/max/" target="_blank">Max </a>


##### Version :

Release 2.0 for Max 6.1 and higher (32 & 64 bit) on Mac Os and Windows plateforms (<a title="Download" href="http://www.mshparisnord.fr/hoalibrary/en/downloads/" target="_blank">Download</a>).

![Image Max](https://raw.github.com/CICM/HoaLibrary-Max/master/misc/HoaLibrary-v2.2-beta.png "Max Patch")

##### Authors (Hoa for Max) :

Julien Colafrancesco, Pierre Guillot, Eliott Paris

##### Dependencies : 

<a title="HoaLibrary" href="https://github.com/CICM/HoaLibrary-Light">HoaLibrary</a>, <a title="MaxSDK" href="https://github.com/Cycling74/max-sdk">Max-sdk</a>

##### Licence : 

The hoa.library in under the <a title="GNU" href="http://www.gnu.org/copyleft/gpl.html" target="_blank">GNU Public License</a>. If you want to avoid the restrictions of the GPL and use Hoa Library for a closed-source product, please contact the <a title="CICM" href="http://cicm.mshparisnord.org/" target="_blank">CICM</a>.

##### Objects :

- hoa.2d.encoder~ encodes a signal in the circular harmonics domain.
- hoa.3d.encoder~ encodes a signal in the spherical harmonics domain.
- hoa.2d.decoder~ decodes an ambisonic sound field for several loudspeakers configurations or for headphones. (eg. ambisonic | binaural | 5.1 | stereo ..)
- hoa.3d.decoder~ decodes an ambisonic sound field for several loudspeakers configurations or for headphones. (eg. ambisonic | binaural)
- hoa.2d.rotate~ applies a rotation on the ambisonic sound field.
- hoa.process~ make your own ambisonic/planewave processing.
- hoa.2d.scope~ displays the sum of the circular harmonics of an ambisonic sound field on a circle.
- hoa.3d.scope~ displays the sum of the spherical harmonics of an ambisonic sound field on a sphere.
- hoa.2d.projector~ discretizes the ambisonic sound field into an array of plane waves.
- hoa.2d.recomposer~ recomposes a plane waves decomposition into circular harmonics.
- hoa.2d.recomposer a GUI to manipulate virtual microphones and control sound field transformations.
- hoa.2d.meter~ displays the peak levels for a circular array of loudspeakers
- hoa.3d.meter~ displays the peak levels for a spherical array of loudspeakers
- hoa.gain~ a multichannel gain GUI.
- hoa.2d.optim~ applies optimizations to the circular harmonics.
- hoa.3d.optim~ applies optimizations to the spherical harmonics.
- hoa.map a GUI that allows you to spatialize several sources in 2d/3d.
- hoa.2d.map~ a tool that encodes several sources in the circular harmonics domain with distance compensation.
- hoa.2d.map~ a tool that encodes several sources in the spherical harmonics domain with distance compensation.
- hoa.2d.wider~ can be used to wide the diffusion of a localised sound.
- hoa.3d.wider~ can be used to wide the diffusion of a localised sound.
- hoa.2d.space a circular shaped multislider (eg. to draw spatial filtering).
- hoa.2d.vector~ displays the coordinates of the velocity vector and the energy vector that characterize a 2d sound field.
- hoa.3d.vector~ displays the coordinates of the velocity vector and the energy vector that characterize a 3d sound field.
- hoa.connect helps to connect the hoa externals together.
- hoa.dac~ makes easier the setting of the channels.
- hoa.pi is for people that never remember more than 4 decimals or want to initialize a good Pi number. 
- hoa.pi~ is a signal version of the hoa.pi object