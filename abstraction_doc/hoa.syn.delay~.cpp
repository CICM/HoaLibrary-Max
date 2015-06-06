
// ------------------------------------------ hoa.syn.delay~ ------------------------------------------ //

/**
 @file      hoa.syn.delay~.cpp
 @name      hoa.syn.delay~
 @realname  hoa.syn.delay~
 @type      abstraction
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A delay sound field synthesizer.
 
 @description
 <o>hoa.syn.delay~</o> allows you to synthesize a sound field by delaying a single signal.
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @discussion
 <o>hoa.syn.delay~</o> allows you to synthesize a sound field by delaying a single signal.
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @keywords ambisonics, hoa objects, audio, MSP, abstraction
 
 @seealso hoa.process~, hoa.fx.delay~, hoa.syn.decorrelation~, hoa.syn.grain~, hoa.syn.ringmod~
 */

CLASS_ATTR_FLOAT            (c, "window", 0, 0, 0);
CLASS_ATTR_LABEL            (c, "window", 0, "Max delay size (samps)");
CLASS_ATTR_DEFAULT          (c, "window", 0, "100");
// @description Set the delay windowing size in ms.

CLASS_ATTR_FLOAT            (c, "deltime", 0, 0, 0);
CLASS_ATTR_LABEL            (c, "deltime", 0, "Delay size (ms)");
CLASS_ATTR_DEFAULT          (c, "deltime", 0, "0");
// @description Set the delay size in ms.

CLASS_ATTR_FLOAT            (c, "feedback", 0, 0, 0);
CLASS_ATTR_LABEL            (c, "feedback", 0, "Feedback");
CLASS_ATTR_DEFAULT          (c, "feedback", 0, "0.");
// @description Set the feedback value (between 0 and 1).


