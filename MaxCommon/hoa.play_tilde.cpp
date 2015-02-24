/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

/**
 @file      hoa.play~.cpp
 @name      hoa.play~
 @realname  hoa.play~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 an ambisonic sfplay~ object
 
 @description
 <o>hoa.play~</o> is a wrapped <o>sfplay~</o> object
 
 @discussion
 <o>hoa.play~</o> is a wrapped <o>sfplay~</o> object
 
 @category ambisonics, hoa objects, audio, msp
 
 @seealso hoa.record~, sfplay~, sfrecord~
 */

#include "HoaCommon.max.h"

t_class *play_class;

void *play_new(t_symbol *s, int argc, t_atom *argv);

#ifdef HOA_PACKED_LIB
int hoa_play_main(void)
#else
int C74_EXPORT main(void)
#endif
{
	t_class *c;

	c = class_new("hoa.play~", (method)play_new, (method)NULL, (short)sizeof(0), 0L, A_GIMME, 0);
    class_setname((char *)"hoa.play~", (char *)"hoa.play~");
    class_setname((char *)"hoa.2d.play~", (char *)"hoa.play~");
    class_setname((char *)"hoa.3d.play~", (char *)"hoa.play~");
    
	hoa_initclass(c, NULL);
	class_register(CLASS_BOX, c);
    class_alias(c, gensym("hoa.2d.play~"));
    class_alias(c, gensym("hoa.3d.play~"));
	play_class = c;
    return 0;
}

void *play_new(t_symbol *s, int argc, t_atom *argv)
{
    // @arg 0 @ambisonic-order @optional 0 @type int @digest The ambisonic order
    // @description The ambisonic order, must be at least equal to 1
    
	t_object *x;
	t_atom arguments[1];
	int order = 1;
	long channels = 1;
	if(atom_gettype(argv) == A_LONG || atom_gettype(argv) == A_FLOAT)
		order = atom_getlong(argv);

	if(order < 1)
		order = 1;
	
    
	if (s == gensym("hoa.play~") || s == gensym("hoa.2d.play~"))
		channels = order * 2 + 1;
	else if (s == gensym("hoa.3d.play~"))
		channels = (order+1) * (order+1);
    
	atom_setlong(arguments, channels);
	x = (t_object *)object_new_typed(CLASS_BOX, gensym("sfplay~"), 1, arguments);
	
	return x;
}


