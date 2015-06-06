
// ------------------------------------------ hoa.receive~ ------------------------------------------ //

/**
 @file      hoa.receive~.cpp
 @name      hoa.receive~
 @realname  hoa.receive~
 @type      abstraction
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A multichannel version of the <o>receive~</o> object.
 
 @description
 <o>hoa.receive~</o> is a multichannel version of the <o>receive~</o> object.
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @discussion
 <o>hoa.receive~</o> is a multichannel version of the <o>receive~</o> object.
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @keywords ambisonics, hoa objects, audio, MSP, abstraction
 
 @seealso receive~, hoa.receive~~, hoa.send~, hoa.send
 */

CLASS_ATTR_SYM	            (c, "name", 0, 0, 0);
CLASS_ATTR_LABEL            (c, "name", 0, "Receive~ name");
CLASS_ATTR_DEFAULT          (c, "name", 0, "");
// @description Set the receive~ name to use.