
// ------------------------------------------ hoa.fx.hiss.convolve~ ------------------------------------------ //

/**
 @file      hoa.fx.hiss.convolve~.cpp
 @name      hoa.fx.hiss.convolve~
 @realname  hoa.fx.hiss.convolve~
 @type      abstraction
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A multichannel zero latency and high optimized convolution processor (thanks to the use of the multiconvolve~ object by Alex Harker and Pierre Alexandre Tremblay).
 
 @description
 <o>hoa.fx.hiss.convolve~</o> is a multichannel zero latency and high optimized convolution processor (thanks to the use of the multiconvolve~ object by Alex Harker and Pierre Alexandre Tremblay).
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @discussion
 <o>hoa.fx.hiss.convolve~</o> is a multichannel zero latency and high optimized convolution processor (thanks to the use of the multiconvolve~ object by Alex Harker and Pierre Alexandre Tremblay).
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @category Process, MSP, abstraction
 
 @seealso multiconvolve~, hoa.fx.convolve~ c.convolve~, hoa.process~, hoa.fx.freeverb~, hoa.fx.decorrelation~, hoa.fx.delay~, hoa.fx.gain~, hoa.fx.grain~, hoa.fx.mirror~, hoa.fx.mixer~
 */

// @method set @digest Set a new impulse response by passing <o>buffer~</o> object name.
// @description The <m>set</m> method sets a new impulse response by passing <o>buffer~</o> object name.
// @marg 0 @name buffer-name @optional 0 @type symbol
class_addmethod(c, 0, "set", A_GIMME, 0);

// @method clear @digest Clear the current impulse response.
// @description The <m>clear</m> method clear the current impulse response.
class_addmethod(c, 0, "clear", 0);

// @method clear @digest Clear the current impulse response, then set the last IR again.
// @description The <m>reset</m> method clear the current impulse response, then set the last IR again. it can be usefull to kill reverberation tail.
class_addmethod(c, 0, "reset", 0);

CLASS_ATTR_SYM                  (c, "buffer", 0, 0, 0);
CLASS_ATTR_LABEL                (c, "buffer", 0, "Buffer~ Object Name");
CLASS_ATTR_DEFAULT          	(c, "buffer", 0, "");
// @description The <b>buffer</b> attribute is the name of the <o>buffer~</o> used to read the impulse response.

CLASS_ATTR_SYM                  (c, "wet", 0, 0, 0);
CLASS_ATTR_LABEL                (c, "wet", 0, "Wet value");
CLASS_ATTR_DEFAULT          	(c, "wet", 0, "0.5");
// @description The <b>wet</b> attribute set the wet value (between 0. and 1.).

CLASS_ATTR_SYM                  (c, "dry", 0, 0, 0);
CLASS_ATTR_LABEL                (c, "dry", 0, "Dry value");
CLASS_ATTR_DEFAULT          	(c, "dry", 0, "0.5");
// @description The <b>dry</b> attribute set the dry value (between 0. and 1.).

CLASS_ATTR_SYM                  (c, "input", 0, 0, 0);
CLASS_ATTR_LABEL                (c, "input", 0, "Input gain");
CLASS_ATTR_DEFAULT          	(c, "input", 0, "1.");
// @description The <b>input</b> attribute set the gain to be applied to the signal before been convolved.