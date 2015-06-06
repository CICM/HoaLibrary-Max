
// ------------------------------------------ hoa.send~ ------------------------------------------ //

/**
 @file      hoa.send~.cpp
 @name      hoa.send~
 @realname  hoa.send~
 @type      abstraction
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 A multichannel version of the <o>send~</o> object.
 
 @description
 <o>hoa.send~</o> is a multichannel version of the <o>send~</o> object.
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @discussion
 <o>hoa.send~</o> is a multichannel version of the <o>send~</o> object.
 This patcher requires to be loaded by a <o>hoa.process~</o> object to work.
 
 @keywords ambisonics, hoa objects, audio, MSP, abstraction
 
 @seealso send~, hoa.send~~, hoa.send~, hoa.send
 */

CLASS_ATTR_SYM	            (c, "name", 0, 0, 0);
CLASS_ATTR_LABEL            (c, "name", 0, "Send~ name");
CLASS_ATTR_DEFAULT          (c, "name", 0, "");
// @description Set the send~ name to use.