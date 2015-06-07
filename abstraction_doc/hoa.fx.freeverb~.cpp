
// ------------------------------------------ hoa.fx.freeverb~ ------------------------------------------ //

/**
 @file      hoa.fx.freeverb~.cpp
 @name      hoa.fx.freeverb~
 @realname  hoa.fx.freeverb~
 @type      abstraction
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A freeverb reverberation for sound fields.
 
 @description
 <o>hoa.fx.freeverb~</o> is a multichannel freeverb reverberation diffuser.
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @discussion
 <o>hoa.fx.freeverb~</o> is a multichannel freeverb reverberation processor.
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @keywords ambisonics, hoa objects, audio, MSP, abstraction
 
 @seealso c.freeverb~, c.convolve~, hoa.process~, hoa.fx.convolve~, hoa.fx.decorrelation~, hoa.fx.delay~, hoa.fx.gain~, hoa.fx.grain~, hoa.fx.mirror~, hoa.fx.mixer~
 */
    
CLASS_ATTR_DOUBLE               (c, "size", 0, 0, 0);
CLASS_ATTR_LABEL                (c, "size", 0, "Size of the room");
CLASS_ATTR_DEFAULT          	(c, "size", 0, "0.5");
// @description The <b>size</b> attribute sets the size of the room (clipped between 0. and 1.)
    
CLASS_ATTR_DOUBLE               (c, "damp", 0, 0, 0);
CLASS_ATTR_LABEL                (c, "damp", 0, "Damp");
CLASS_ATTR_DEFAULT          	(c, "damp", 0, "0.5");
// @description The <b>damp</b> attribute sets the damp of the reverb (clipped between 0. and 1.)

CLASS_ATTR_DOUBLE               (c, "dry", 0, 0, 0);
CLASS_ATTR_LABEL                (c, "dry", 0, "Dry");
CLASS_ATTR_DEFAULT          	(c, "dry", 0, "0.5");
// @description The <b>dry</b> attribute sets dry level of the effect (clipped between 0. and 1.)

CLASS_ATTR_DOUBLE               (c, "wet", 0, 0, 0);
CLASS_ATTR_LABEL                (c, "wet", 0, "Wet");
CLASS_ATTR_DEFAULT          	(c, "wet", 0, "0.5");
// @description The <b>wet</b> attribute sets wet level of the effect (clipped between 0. and 1.)

CLASS_ATTR_LONG               	(c, "freeze", 0, 0, 0);
CLASS_ATTR_STYLE_LABEL          (c, "freeze",  0, "onoff", "Freeze");
CLASS_ATTR_DEFAULT          	(c, "freeze", 0, "0");
// @description The <b>freeze</b> attribute can be used to freeze the reverberation.

CLASS_ATTR_DOUBLE               (c, "diff", 0, 0, 0);
CLASS_ATTR_LABEL                (c, "diff", 0, "Diffusion");
CLASS_ATTR_DEFAULT          	(c, "diff", 0, "0.5");
// @description The <b>diff</b> attribute sets diffusion factor of the reverb (clipped between 0. and 1.), only in "harmonics" mode.

