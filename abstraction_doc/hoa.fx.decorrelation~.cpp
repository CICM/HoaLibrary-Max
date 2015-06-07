
// ------------------------------------------ hoa.fx.decorrelation~ ------------------------------------------ //

/**
 @file      hoa.fx.decorrelation~.cpp
 @name      hoa.fx.decorrelation~
 @realname  hoa.fx.decorrelation~
 @type      abstraction
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A multichannel decorrelator processor.
 
 @description
 <o>hoa.fx.decorrelation~</o> allows you to generate a diffuse sound field with signal decorrelation.
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @discussion
 <o>hoa.fx.decorrelation~</o> allows you to generate a diffuse sound field with signal decorrelation.
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @keywords ambisonics, hoa objects, audio, MSP, abstraction
 
 @seealso hoa.process~, hoa.syn.decorrelation~, hoa.fx.delay~, hoa.fx.gain~, hoa.fx.grain~, hoa.fx.mirror~, hoa.fx.mixer~, hoa.fx.ringmod~
 */

CLASS_ATTR_LONG             (c, "maxsize", 0, 0, 0);
CLASS_ATTR_LABEL            (c, "maxsize", 0, "Max delay size (samps)");
CLASS_ATTR_DEFAULT          (c, "maxsize", 0, "44100");
// @description Set the maximum delay size in samples

CLASS_ATTR_LONG             (c, "delay", 0, 0, 0);
CLASS_ATTR_LABEL            (c, "delay", 0, "Delay size (samps)");
CLASS_ATTR_DEFAULT          (c, "delay", 0, "22050");
// @description Set the delay size in samples

CLASS_ATTR_FLOAT            (c, "factor", 0, 0, 0);
CLASS_ATTR_LABEL            (c, "factor", 0, "Decorrelation factor");
CLASS_ATTR_DEFAULT          (c, "factor", 0, "0.");
// @description Set the deecorrelation factor (between 0 and 1). 0 means no delay, 1 means delay size.