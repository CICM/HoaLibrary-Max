
// ------------------------------------------ hoa.fx.gain~ ------------------------------------------ //

/**
 @file      hoa.fx.gain~.cpp
 @name      hoa.fx.gain~
 @realname  hoa.fx.gain~
 @type      abstraction
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A mulichannel gain manager.
 
 @description
 <o>hoa.fx.gain~</o> is a mulichannel gain manager.
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @discussion
 <o>hoa.fx.gain~</o> is a mulichannel gain manager.
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @keywords ambisonics, hoa objects, audio, MSP, abstraction
 
 @seealso hoa.process~, hoa.gain~, hoa.fx.decorrelation~, hoa.fx.delay~, hoa.fx.grain~, hoa.fx.mirror~, hoa.fx.mixer~, hoa.fx.ringmod~
 */

CLASS_ATTR_FLOAT            (c, "ramp", 0, 0, 0);
CLASS_ATTR_LABEL            (c, "ramp", 0, "Ramp time (ms)");
CLASS_ATTR_DEFAULT          (c, "ramp", 0, "0");
// @description Set the ramp time in ms.

// @arg 0 @name initial-amplitude @optional 1 @type float @digest The initial amplitude value.
// @description First argument is the initial amplitude value.

// @arg 1 @name ramp-time @optional 1 @type float @digest The initial ramp time value.
// @description First argument is the initial ramp time value (in ms).