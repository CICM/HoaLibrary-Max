
// ------------------------------------------ hoa.fx.convolve~ ------------------------------------------ //

/**
 @file      hoa.fx.convolve~.cpp
 @name      hoa.fx.convolve~
 @realname  hoa.fx.convolve~
 @type      abstraction
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A multichannel zero latency convolution reverberation processor.
 
 @description
 <o>hoa.fx.convolve~</o> is a multichannel zero latency convolution reverberation processor.
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @discussion
 <o>hoa.fx.convolve~</o> is a multichannel zero latency convolution reverberation processor.
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @keywords ambisonics, hoa objects, audio, MSP, abstraction
 
 @seealso c.convolve~, hoa.process~, hoa.fx.freeverb~, hoa.fx.decorrelation~, hoa.fx.delay~, hoa.fx.gain~, hoa.fx.grain~, hoa.fx.mirror~, hoa.fx.mixer~
 */

// @method set @digest Set a new impulse response by passing <o>buffer~</o> object name.
// @description The <m>set</m> method sets a new impulse response by passing <o>buffer~</o> object name. An optionnal int can be passed after buffer name to set the buffer channel to be used.
// @marg 0 @name buffer-name @optional 0 @type symbol
// @marg 1 @name channel @optional 1 @type int
class_addmethod(c, 0, "set", A_GIMME, 0);

// @method clear @digest Clear the current reverberation tail
// @description The <m>clear</m> method clear the current reverberation tail (may produce clicks)
class_addmethod(c, 0, "clear", 0);

CLASS_ATTR_SYM                  (c, "buffer", 0, 0, 0);
CLASS_ATTR_LABEL                (c, "buffer", 0, "Buffer~ Object Name");
CLASS_ATTR_DEFAULT          	(c, "buffer", 0, "");
// @description The <b>buffer</b> attribute is the name of the <o>buffer~</o> used to read the impulse response.
	
CLASS_ATTR_LONG                 (c, "channel", 0, 0, 0);
CLASS_ATTR_LABEL                (c, "channel", 0, "Channel");
CLASS_ATTR_DEFAULT          	(c, "channel", 0, "1");
// @description The <b>channel</b> attribute is the channel number of the impulse response buffer.
    
CLASS_ATTR_LONG                 (c, "normalize",  0, 0, 0);
CLASS_ATTR_STYLE_LABEL          (c, "normalize",  0, "onoff", "Normalize buffer content");
CLASS_ATTR_DEFAULT          	(c, "normalize", 0, "0");
// @description If the <b>normalize</b> attribute is checked, <o>c.convolve</o> object will normalize the <o>buffer~</o> content.
    
CLASS_ATTR_LONG                 (c, "fftsize", 0, 0, 0);
CLASS_ATTR_LABEL                (c, "fftsize", 0, "FFT size");
CLASS_ATTR_DEFAULT          	(c, "fftsize", 0, "1024");
// @description The <b>fftsize</b> attribute is the <o>c.convolve</o> FFT size.
    
CLASS_ATTR_ATOM                 (c, "cropsize", 0, 0, 0);
CLASS_ATTR_LABEL                (c, "cropsize", 0, "Crop size (ms)");
CLASS_ATTR_DEFAULT          	(c, "cropsize", 0, "3000.");
// @description Use the <b>cropsize</b> attribute to limit the impulse response at a maximum size. It can be usefull to prevent too big CPU usage when loading big impulse responses.
// the <b>cropsize</b> is set in milliseconds, dont set this if you want to play the entire IR.