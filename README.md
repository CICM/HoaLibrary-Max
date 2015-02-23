### Hoa Library (A high order ambisonics library) for <a title="Max" href="http://cycling74.com/products/max/" target="_blank">Max </a>


##### Version :

Release 2.0 for Max 6.1 and higher (32 & 64 bit) on Mac Os and Windows plateforms (<a title="Download" href="http://www.mshparisnord.fr/hoalibrary/en/downloads/" target="_blank">Download</a>).

![Image Max](https://raw.github.com/CICM/HoaLibrary/master/Ressources/HoaLibrary-v2.0-beta.png "Max Patch")

##### Authors (Hoa for Max) :

Julien Colafrancesco, Pierre Guillot, Eliott Paris

##### Dependencies : 

<a title="Cblas" href="http://www.netlib.org/clapack/cblas/" target="_blank">Cblas</a>, <a title="MaxSDK-6.1.4" href="http://cycling74.com/downloads/sdk/" target="_blank">MaxSDK-6.1.4</a>

##### Licence : 

The hoa.library in under the <a title="GNU" href="http://www.gnu.org/copyleft/gpl.html" target="_blank">GNU Public License</a>. If you'd like to avoid the restrictions of the GPL and use Hoa Library for a closed-source product, you contact the <a title="CICM" href="http://cicm.mshparisnord.org/" target="_blank">CICM</a>.

##### Objects :

- hoa.encoder~ encodes a signal in the circular harmonics domain depending on an order of decomposition and an angle.
- hoa.decoder~ decodes an ambisonic sound field for several loudspeakers configurations or for headphones. (eg. ambisonic | binaural | 5.1 | stereo ..)
- hoa.rotate~ applies a rotation on the ambisonic sound field.
- hoa.plug~ helps the modularization of patches for ambisonic processings.
- hoa.connect helps to connect the hoa externals together.
- hoa.dac~ makes easier the setting of the channels. It works like a dac~ object but you can use the matlab syntax to set the loudspeakers routing.
- hoa.pi is for people that never remember more than 4 decimals or want to initialize a good Pi number. 
- hoa.pi~ is a signal version of the hoa.pi object
- hoa.scope~ displays the harmonics of an ambisonic sound field on a circle.
- hoa.projector~ discretizes the ambisonic sound field into an array of virtual microphones.
- hoa.recomposer~ recomposes a plane wave decomposition into circular harmonics and applies sound field transformations.
- hoa.recomposer is a GUI to manipulate virtual microphones and control sound field transformations.
- hoa.meter~ displays the peak levels for a circular array of loudspeakers. It computes and displays the energy and velocity vectors.
- hoa.gain~ is a GUI to control a multichannel gain
- hoa.record~ creates a sfrecord~ object with the best arguments to record of an ambisonic sound field.
- hoa.play~ creates an sfplay~ object with the best arguments to play of an ambisonic sound field.
- hoa.optim~ applies optimizations to the circular harmonics.
- hoa.map allows you to spatialize several sources on a plane.
- hoa.map~ is a tool that encodes several sources in the circular harmonics domain. It's easy to use and works with hoa.map.
- hoa.convolve~ uses time domain and frequency domain filtering to apply reveberation on harmonics dependant signals. The zero latency convolution is optimized with an algorithm based on the Gardner's one.
- hoa.wider~ can be used to wide the diffusion of a localised sound. The order depending signals are weighted and appear in a logarithmic way to have linear changes.
- hoa.freeverb~ is based on Schroeder/Moorer reverberation model that uses eight parallel comb filters and four cascaded allpass filters adapted to ambisonic domain.
- hoa.space allows you to draw spatial filtering and apply gains to the virtual microphones signals. It sould be used with the hoa.space~ object in the plane wave domain.
- hoa.space~ applies gains to the virtual microphones signal.
- hoa.vector~ displays the coordinates of the velocity vector and the energy vector that characterize a sound field. It's an useful tool to analyse restitution quality.

##### Patchs :

- hoa.am~ creates a diffuse soundfield by modulating the amplitude of the circular harmonics signals.
- hoa.delay~ allows you to delay the differents harmonics.
- hoa.decorrelation~ allows you to delay the differents harmonics depending on the harmonic number to decorrelate the harmonics and generate a diffuse sound field.
- hoa.grain~ synthesizes a diffuse sound field. It uses delay lines and amplitude modulation to create streams of grains. The parameters are scaled depending to the orders to generate a granular sound field.
- hoa.mirror~ applies a weight on the negatives harmonics to add the mirror image of a sound field to itself. 
- hoa.mixer~ helps to understand ambisonics and the behavior of the circular harmonics.
- hoa.granular~ synthesizes a diffuse sound field. It uses delay lines and amplitude modulation to create streams of grains. The parameters are scaled depending on the orders to generate a granular sound field.


##### ToDo :

- hoa.convolve~ : Update buffer~ for Max 6 SDK and windows version (low)
- hoa.plug~ : Update dsp method for Max 6 SDK (low)
- hoa.decoder~ : Find a solution for attributes order (low)
- hoa.spectrum~ : Clean (medium)
- hoa.boids : Clean (medium)
- hoa.recomposer : No fisheye preview on window
- hoa.freeverb~ : click on audio on/off
- fftw for window