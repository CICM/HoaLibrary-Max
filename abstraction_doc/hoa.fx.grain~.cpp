
// ------------------------------------------ hoa.fx.grain~ ------------------------------------------ //

/**
 @file      hoa.fx.grain~.cpp
 @name      hoa.fx.grain~
 @realname  hoa.fx.grain~
 @type      abstraction
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A multichannel granulator processor.
 
 @description
 <o>hoa.fx.grain~</o> create a diffuse sound field. It uses delay lines and amplitude modulation to create streams of grains.
 In "harmonics" mode, parameters are scaled depending to the orders to generate a granular sound field.
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @discussion
 <o>hoa.fx.grain~</o> create a diffuse sound field. It uses delay lines and amplitude modulation to create streams of grains.
 In "harmonics" mode, parameters are scaled depending to the orders to generate a granular sound field.
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @keywords ambisonics, hoa objects, audio, MSP, abstraction
 
 @seealso hoa.process~, hoa.syn.grain~, hoa.fx.decorrelation~, hoa.fx.gain~, hoa.fx.mirror~, hoa.fx.mixer~, hoa.fx.ringmod~
 */

CLASS_ATTR_SYM	            (c, "name", 0, 0, 0);
CLASS_ATTR_LABEL            (c, "name", 0, "Buffer name");
CLASS_ATTR_DEFAULT          (c, "name", 0, "livegrain");
// @description Set the name of the buffer to be used.

CLASS_ATTR_FLOAT            (c, "size", 0, 0, 0);
CLASS_ATTR_LABEL            (c, "size", 0, "Grain size (ms)");
CLASS_ATTR_DEFAULT          (c, "size", 0, "0");
// @description Set grain size in ms (can not be greather than the <m>delay<m> time).

CLASS_ATTR_FLOAT            (c, "delay", 0, 0, 0);
CLASS_ATTR_LABEL            (c, "delay", 0, "Delay time (ms)");
CLASS_ATTR_DEFAULT          (c, "delay", 0, "0.");
// @description Set the delay size in ms.

CLASS_ATTR_FLOAT            (c, "feedback", 0, 0, 0);
CLASS_ATTR_LABEL            (c, "feedback", 0, "Feedback");
CLASS_ATTR_DEFAULT          (c, "feedback", 0, "0.");
// @description Set the feedback value (between 0 and 1).

CLASS_ATTR_FLOAT            (c, "rarefaction", 0, 0, 0);
CLASS_ATTR_LABEL            (c, "rarefaction", 0, "Rarefaction factor");
CLASS_ATTR_DEFAULT          (c, "rarefaction", 0, "0.");
// @description Set the rarefaction factor (between 0 and 1).