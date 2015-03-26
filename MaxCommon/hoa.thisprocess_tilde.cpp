/*
 // Copyright (c) 2012-2015 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

// based on dynamic.this~ Copyright 2010 Alex Harker. All rights reserved.

/**
 @file      hoa.thisprocess~.cpp
 @name      hoa.thisprocess~
 @realname  hoa.thisprocess~
 @type      object
 @module    hoa
 @author    Julien Colafrancesco, Pierre Guillot, Eliott Paris.
 
 @digest
 Control hoa.process~ voice allocation, muting, argument and attributes.
 
 @description
 Use the <o>hoa.thisprocess~</o> object to control <o>hoa.process~</o> voice allocation, muting, argument and attributes. When placed inside a patcher loaded by a <o>hoa.process~</o> object, it send and receive messages from the <o>hoa.process~</o> object that loads it.
 
 @discussion
 Use the <o>hoa.thisprocess~</o> object to control <o>hoa.process~</o> voice allocation, muting, argument and attributes. When placed inside a patcher loaded by a <o>hoa.process~</o> object, it send and receive messages from the <o>hoa.process~</o> object that loads it.
 
 @category ambisonics, hoa objects, msp
 
 @seealso hoa.process~, hoa.in~, hoa.in, hoa.out, hoa.out~, thispoly~, poly~, thispatcher, patcher
 */

#include "HoaCommon.max.h"
#include "HoaProcessSuite.h"

typedef struct _hoa_thisprocess
{
    t_object x_obj;
	
	// outlets
	void *out_mute, *out_patcherAttr, *out_patcherArgs, *out_mode, *out_instance_infos;
	
	long index;
	
	void *hoaProcessor_parent;
	
	int                                     f_order;
    
    Processor<Hoa2d, t_sample>::Harmonics*   f_ambi2D;
    Processor<Hoa3d, t_sample>::Harmonics*   f_ambi3D;
	
	t_args_struct object_args;
	long object_nAttrs;
	t_attr_struct *object_attrs;
	
} t_hoa_thisprocess;

t_class *hoa_thisprocess_class;

void hoa_thisprocess_free(t_hoa_thisprocess *x)
{
	free(x->object_args.argv);
	
	if (x->object_nAttrs > 0)
	{
		for (int i = 0; i < x->object_nAttrs; i++)
			free(x->object_attrs[i].argv);
		
		free(x->object_attrs);
	}
}

void hoa_thisprocess_mute(t_hoa_thisprocess *x, t_symbol *msg, short argc, t_atom *argv)
{	
	long arg_val;
	
	if (!argc)
		return;
	
	arg_val = atom_getlong(argv);
	
	HoaProcessor_Set_Patch_On (x->hoaProcessor_parent, x->index, !arg_val);
	outlet_int(x->out_mute, !HoaProcessor_Get_Patch_On (x->hoaProcessor_parent, x->index));
}

void hoa_thisprocess_mutechange(t_hoa_thisprocess *x)
{
    // object must be in a hoa.process~ object
	if (!x->hoaProcessor_parent || x->index <= 0)
		return;
	
	outlet_int(x->out_mute, !HoaProcessor_Get_Patch_On (x->hoaProcessor_parent, x->index));
}

long hoa_get_number_of_attributes(long ac, t_atom *av)
{
	long nAttrs = 0;
	char *attrName = NULL;
	if (ac > 0 && av)
	{
		for (int i=0; i<ac; i++)
		{
			if (atom_gettype(av+i) == A_SYM)
			{
				attrName = atom_string(av + i);
				if (attrName[0] == '@')
					nAttrs++;
			}
		}
	}
	
	return nAttrs;
}

void hoa_process_attrs(t_hoa_thisprocess *x, long patcher_nAttrs, t_attr_struct *attrs_patcher, long *nAttr, t_attr_struct **attrs_processed)
{
	int i, j, k;
	long number_of_attrs = x->object_nAttrs + patcher_nAttrs;
	
	// check the real number of attr (! does not check duplicate symbol in object or patcher)
	for (i = 0; i < patcher_nAttrs; i++)
		for (j = 0; j < x->object_nAttrs; j++)
			if ( attrs_patcher[i].msg == x->object_attrs[j].msg )
				number_of_attrs--;
	
	nAttr[0] = number_of_attrs;
	
	attrs_processed[0] = (t_attr_struct*) malloc( number_of_attrs * sizeof(t_attr_struct));
	
	
	long temp_ac;
	t_symbol *attrSym;
	int common = 0;
	int attr_index = 0, o_attr_index = 0, p_attr_index = 0;
	
	// begin with object attr :
	for (i = 0; i < x->object_nAttrs; i++)
	{
		attrSym = x->object_attrs[i].msg;
		common = 0;
		
		// check if attr is common to object and patcher
		for (j = 0; j < patcher_nAttrs; j++)
		{
			if ( attrSym == attrs_patcher[j].msg )
			{
				common = 1;
				o_attr_index = i;
				p_attr_index = j;
			}
		}
		
		// if attr only exists in object
		if (!common)
		{
			attrs_processed[0][attr_index].msg = attrSym;
			temp_ac = x->object_attrs[i].argc;
			attrs_processed[0][attr_index].argc = temp_ac;
			attrs_processed[0][attr_index].argv = (t_atom *) malloc(temp_ac * sizeof(t_atom));
			for (k = 0; k < temp_ac; k++)
				attrs_processed[0][attr_index].argv[k] = x->object_attrs[i].argv[k];
		}
		
		// attr exists in object and patch (we need to do some overriding)
		else
		{
			attrs_processed[0][attr_index].msg = attrSym;
			temp_ac = MAX(x->object_attrs[i].argc, attrs_patcher[p_attr_index].argc);
			attrs_processed[0][attr_index].argc = temp_ac;
			attrs_processed[0][attr_index].argv = (t_atom *) malloc(temp_ac * sizeof(t_atom));
			for (k = 0; k < temp_ac; k++)
				attrs_processed[0][attr_index].argv[k] = ( k < attrs_patcher[p_attr_index].argc) ? attrs_patcher[p_attr_index].argv[k] : x->object_attrs[i].argv[k];
		}
		
		attr_index++;
	}
	
	// The same of patcher attrs :
	for (i = 0; i < patcher_nAttrs; i++)
	{
		attrSym = attrs_patcher[i].msg;
		common = 0;
		
		// check if attr is common to object and patcher
		for (j = 0; j < x->object_nAttrs; j++)
		{
			if ( attrSym == x->object_attrs[j].msg )
			{
				common = 1;
				o_attr_index = j;
				p_attr_index = i;
			}
		}
		
		// if attr only exists in object
		if (!common)
		{
			attrs_processed[0][attr_index].msg = attrSym;
			temp_ac = attrs_patcher[i].argc;
			attrs_processed[0][attr_index].argc = temp_ac;
			attrs_processed[0][attr_index].argv = (t_atom *) malloc(temp_ac * sizeof(t_atom));
			for (k = 0; k < temp_ac; k++)
				attrs_processed[0][attr_index].argv[k] = attrs_patcher[i].argv[k];
			
			attr_index++;
		}
	}
}

void hoa_thisprocess_assist(t_hoa_thisprocess *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_OUTLET) 
	{
		switch (a)
		{
			case 0:
				sprintf(s,"(list) instance informations");
				break;
			case 1:
				sprintf(s,"(list) process mode");
				break;
			case 2:
				sprintf(s,"(list) normal arguments");
				break;
			case 3:
				sprintf(s,"(list) attributes arguments");
				break;
			case 4:
				sprintf(s,"(int) mute status");
				break;
		}
	}
    else 
	{
		sprintf(s,"(mute) set mute, (bang) for report");
    }
}

void hoa_args_setup(short ac, t_atom *av, long *nAttr, t_args_struct *args, t_attr_struct **attrs)
{
    long attrOffset = 0, attrNumberOfArg = 0;
    attrOffset = attr_args_offset(ac, av);
    char *attrName = NULL;
    t_symbol *tempsym;
    
    nAttr[0] = hoa_get_number_of_attributes(ac, av);
    
    // init arguments
    
    args[0].argc = attrOffset;
    args[0].argv = NULL;
    
    if (attrOffset > 0)
    {
        args[0].argv = (t_atom *) malloc( attrOffset * sizeof(t_atom));
        for (int i = 0; i < attrOffset; i++)
            args[0].argv[i] = av[i];
    }
    
    // init attributes
    
    attrs[0] = NULL;
    
    if (nAttr[0] > 0)
    {
        attrs[0] = (t_attr_struct*) malloc(nAttr[0] * sizeof(t_attr_struct));
        
        av += attrOffset;
        ac -= attrOffset;
        
        for (int i = 0; i < nAttr[0]; i++)
        {
            attrName = atom_string(av);
            
            if (attrName[0] == '@')
            {
                attrName++;						// remove '@' char
                tempsym = gensym(attrName);
                atom_setsym(av, tempsym);		// to check next offset
                attrs[0][i].msg = tempsym;
            }
            
            attrNumberOfArg = attr_args_offset(ac, av);
            
            attrs[0][i].argc = attrNumberOfArg-1;
            attrs[0][i].argv = (t_atom*) malloc( attrNumberOfArg * sizeof(t_atom));
            
            for (int j = 1; j < attrNumberOfArg; j++)
                attrs[0][i].argv[j-1] = av[j];
            
            av += attrNumberOfArg;
            ac -= attrNumberOfArg;
        }
    }
}

void hoa_thisprocess_dobang(t_hoa_thisprocess *x)
{
    // object must be in a hoa.process~ object
    if (!x->hoaProcessor_parent || x->index <= 0)
        return;
    
    t_symbol*	mode = HoaProcessor_Get_Mode(x->hoaProcessor_parent);
    int			is_2D;
    
    // output mute state first
    
    outlet_int(x->out_mute, !HoaProcessor_Get_Patch_On (x->hoaProcessor_parent, x->index));
    
    long ac = 0;
    t_atom *av = NULL;
    t_hoa_err err = HoaProcessor_Get_PatcherArgs(x->hoaProcessor_parent, x->index, &ac, &av);
    
    if (err == HOA_ERR_NONE)
    {
        int i;
        long patcher_nAttrs = 0;
        t_args_struct patcher_args;
        t_attr_struct *patcher_attrs = NULL;
        
        hoa_args_setup(ac, av, &patcher_nAttrs, &patcher_args, &patcher_attrs);
        
        // output patcher attributes
        
        if (patcher_nAttrs > 0 || x->object_nAttrs > 0)
        {
            long nAttrs_processed = 0;
            t_attr_struct *attrs_processed = NULL;
            hoa_process_attrs(x, patcher_nAttrs, patcher_attrs, &nAttrs_processed, &attrs_processed);
            
            if (nAttrs_processed > 0)
                for (i = 0; i < nAttrs_processed; i++)
                    outlet_anything(x->out_patcherAttr, attrs_processed[i].msg, attrs_processed[i].argc, attrs_processed[i].argv);
            
            // output done message to indicate that the attributes have been processed
            
            //outlet_anything(x->out_patcherAttr, hoa_sym_done, 0, NULL);
            
            // free patcher_attrs
            
            for (int i = 0; i < patcher_nAttrs; i++)
                if(patcher_attrs[i].argv)
                    free(patcher_attrs[i].argv);
            if(patcher_attrs)
                free(patcher_attrs);
            
            // free attrs_processed
            if (nAttrs_processed)
            {
                for (int i = 0; i < nAttrs_processed; i++)
                    if(attrs_processed[i].argv)
                        free(attrs_processed[i].argv);
                if(attrs_processed)
                    free(attrs_processed);
            }
            
        }
        
        // output patcher arguments
        
        if (patcher_args.argc > 0 || x->object_args.argc > 0)
        {
            t_args_struct args_processed;
            args_processed.argc = MAX(patcher_args.argc, x->object_args.argc);
            args_processed.argv = (t_atom*) malloc(args_processed.argc * sizeof(t_atom));
            
            for (i = 0; i < args_processed.argc; i++)
                args_processed.argv[i] = ( i < patcher_args.argc) ? patcher_args.argv[i] : x->object_args.argv[i];
            
            if (args_processed.argc == 1)
            {
                if (atom_gettype(args_processed.argv) == A_FLOAT)
                    outlet_float(x->out_patcherArgs, atom_getfloat(args_processed.argv));
                
                else if (atom_gettype(args_processed.argv) == A_LONG)
                    outlet_int(x->out_patcherArgs, atom_getlong(args_processed.argv));
            }
            else
                outlet_list(x->out_patcherArgs, NULL, args_processed.argc, args_processed.argv);
            
            // free patcher_args
            if(patcher_args.argv)
                free(patcher_args.argv);
            
            // free args_processed
            if(args_processed.argv)
                free(args_processed.argv);
        }
        
        if (av) free(av);
    }
    
    mode = HoaProcessor_Get_Mode(x->hoaProcessor_parent);
    is_2D = HoaProcessor_Is_2D(x->hoaProcessor_parent);
    
    // output process mode info (ambisonics/planewave + 2d/3d)
    
    av = new t_atom[2];
    atom_setsym(av, is_2D ? hoa_sym_2d : hoa_sym_3d);
    atom_setsym(av+1, mode);
    outlet_list(x->out_mode, NULL, 2, av);
    delete [] av;
    
    // output process instance info
    
    if (mode == hoa_sym_harmonics)
    {
        if (is_2D)
        {
            av = new t_atom[2];
            atom_setlong( av , x->f_order);									// Ambisonic Order
            atom_setlong(av+1, x->f_ambi2D->getHarmonicOrder(x->index-1));	// Harmonic Order
            outlet_list(x->out_instance_infos, NULL, 2, av);
            delete [] av;
        }
        else
        {
            av = new t_atom[3];
            atom_setlong( av , x->f_order);									// Ambisonic Order
            atom_setlong(av+1, x->f_ambi3D->getHarmonicDegree(x->index-1));	// Harmonic Degree
            atom_setlong(av+2, x->f_ambi3D->getHarmonicOrder(x->index-1));	// Harmonic Order
            outlet_list(x->out_instance_infos, NULL, 3, av);
            delete [] av;
        }
    }
    else if (mode == hoa_sym_planewaves)
    {
        av = new t_atom[2];
        atom_setlong( av , x->f_order);											// Number of Channel
        atom_setlong(av+1, x->index);											// Channel Index
        outlet_list(x->out_instance_infos, NULL, 2, av);
        delete [] av;
    }
    
    
    // output done message to indicate that all values have been sent
    
    outlet_anything(x->out_patcherAttr, hoa_sym_done, 0, NULL);
}

void hoa_thisprocess_bang(t_hoa_thisprocess *x)
{
    defer_low(x, (method)hoa_thisprocess_dobang, NULL, 0, NULL);
}

void hoa_thisprocess_loadbang(t_hoa_thisprocess *x)
{
    hoa_thisprocess_bang(x);
}

void *hoa_thisprocess_new(t_symbol *s, short argc, t_atom *argv)
{
    // @arg 0 @name arguments @optional 1 @type int/float/symbol @digest default arguments
    // @description A list of default arguments
    
    // @arg 1 @name attributes @optional 1 @type int/float/symbol @digest default attributes
    // @description A list of default attributes
    
    t_hoa_thisprocess *x = (t_hoa_thisprocess *) object_alloc(hoa_thisprocess_class);
    
    x->out_mute				= intout(x);
    x->out_patcherAttr		= listout(x);
    x->out_patcherArgs		= listout(x);
    x->out_mode				= listout(x);
    x->out_instance_infos	= listout(x);
    
    x->hoaProcessor_parent = Get_HoaProcessor_Object();
    x->f_order = HoaProcessor_Get_Ambisonic_Order(x->hoaProcessor_parent);
    x->index = Get_HoaProcessor_Patch_Index(x->hoaProcessor_parent);
    
    x->f_ambi2D = new Processor<Hoa2d, t_sample>::Harmonics(x->f_order);
    x->f_ambi3D = new Processor<Hoa3d, t_sample>::Harmonics(x->f_order);
    
    hoa_args_setup(argc, argv, &x->object_nAttrs, &x->object_args, &x->object_attrs);
    
    return (x);
}

#ifdef HOA_PACKED_LIB
int hoa_thisprocess_main(void)
#else
int C74_EXPORT main(void)
#endif
{
    t_class* c;
    c = class_new("hoa.thisprocess~", (method)hoa_thisprocess_new, (method)hoa_thisprocess_free, sizeof(t_hoa_thisprocess), NULL, A_GIMME, 0);
    class_setname((char *)"hoa.thisprocess~", (char *)"hoa.thisprocess~");
    
    hoa_initclass(c, (method)NULL);
    
    class_addmethod(c, (method)hoa_thisprocess_assist,			"assist",	A_CANT, 0);
    //class_addmethod(c, (method)hoa_thisprocess_loadbang,		"loadbang", A_CANT, 0);
    
    // @method mute @digest Disable DSP processing for this patcher instance.
    // @description Turns off signal processing for this specific instance. Argument is the mute state, 0 means unmuted, others value means muted
    // @marg 1 @name on/off-flag @optional 0 @type int
    class_addmethod(c, (method)hoa_thisprocess_mute,			"mute",		A_GIMME, 0);
    
    // @method getmute @digest Report mute state.
    // @description Report mute state only.
    class_addmethod(c, (method)hoa_thisprocess_mutechange,			"getmute",		A_NOTHING, 0);
    
    // @method bang @digest Report the instance informations in a right to left outputting order.
    // @description Output instance informations in a right to left outputting order. <br/><br/>
    // <ul>
    // <li> mute state (0/1). </li>
    // <li> patcher attributes. </li>
    // <li> patcher arguments. </li>
    // <li> process context and mode. ex : "2d harmonics". </li>
    // <li>
    // process instance informations depending on the current mode. <br/><br/>
    // <ul>
    // <li> In <b>planewaves</b> mode (2d/3d). It will be (number-of-channels, instance-channel)</li>
    // <li> In 2d <b>harmonics</b> mode, it will be a list with (ambisonic-order, harmonic-index) </li>
    // <li> In 3d <b>harmonics</b> mode, it will be a list with (ambisonic-order, harmonic-band, harmonic-index) </li>
    // </ul>
    // </li>
    // </ul>
    class_addmethod(c, (method)hoa_thisprocess_bang,			"bang",		0);
    
    class_addmethod(c, (method)hoa_thisprocess_mutechange,		"mutechange", A_CANT, 0);
    
    class_register(CLASS_BOX, c);
    hoa_thisprocess_class = c;
    return 0;
}