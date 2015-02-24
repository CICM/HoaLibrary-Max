{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 7,
			"minor" : 0,
			"revision" : 0,
			"architecture" : "x86",
			"modernui" : 1
		}
,
		"rect" : [ 169.0, 79.0, 925.0, 655.0 ],
		"bgcolor" : [ 0.93, 0.93, 0.92, 1.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 11.595187,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 1,
		"objectsnaponopen" : 1,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"lefttoolbarpinned" : 0,
		"toptoolbarpinned" : 0,
		"righttoolbarpinned" : 0,
		"bottomtoolbarpinned" : 0,
		"toolbars_unpinned_last_save" : 0,
		"tallnewobj" : 0,
		"boxanimatetime" : 200,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"style" : "",
		"subpatcher_template" : "",
		"showrootpatcherontab" : 0,
		"showontab" : 0,
		"boxes" : [ 			{
				"box" : 				{
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"color" : [ 0.734694, 0.734694, 0.734694, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"hidden" : 1,
					"id" : "obj-1",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 0,
							"revision" : 0,
							"architecture" : "x86",
							"modernui" : 1
						}
,
						"rect" : [ 169.0, 105.0, 925.0, 629.0 ],
						"bgcolor" : [ 0.93, 0.93, 0.92, 1.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 1,
						"gridsize" : [ 20.0, 20.0 ],
						"gridsnaponopen" : 1,
						"objectsnaponopen" : 1,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"lefttoolbarpinned" : 0,
						"toptoolbarpinned" : 0,
						"righttoolbarpinned" : 0,
						"bottomtoolbarpinned" : 0,
						"toolbars_unpinned_last_save" : 0,
						"tallnewobj" : 0,
						"boxanimatetime" : 200,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"style" : "",
						"subpatcher_template" : "",
						"showontab" : 1,
						"boxes" : [ 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"hidden" : 1,
									"id" : "obj-17",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 396.0, 246.0, 85.0, 22.0 ],
									"style" : "",
									"text" : "loadmess 200"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"hidden" : 1,
									"id" : "obj-13",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 304.6875, 111.0, 82.0, 22.0 ],
									"style" : "",
									"text" : "loadmess 0.1"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"format" : 6,
									"id" : "obj-11",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 304.6875, 138.0, 50.0, 21.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-9",
									"linecount" : 2,
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 304.6875, 191.0, 99.0, 35.0 ],
									"style" : "",
									"text" : "triangle~ 0.5 @lo -0.5 @hi 0.5"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"id" : "obj-7",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 304.6875, 234.0, 49.0, 21.0 ],
									"style" : "",
									"text" : "hoa.pi~"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"id" : "obj-8",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 304.6875, 164.0, 72.0, 21.0 ],
									"style" : "",
									"text" : "phasor~ 0.1"
								}

							}
, 							{
								"box" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"gain" : 50.0,
									"id" : "obj-6",
									"maxclass" : "hoa.3d.scope~",
									"numinlets" : 16,
									"numoutlets" : 0,
									"order" : 3,
									"patching_rect" : [ 608.0, 383.0, 223.0, 223.0 ]
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-5",
									"maxclass" : "newobj",
									"numinlets" : 20,
									"numoutlets" : 0,
									"patching_rect" : [ 30.0, 539.0, 312.0, 22.0 ],
									"style" : "",
									"text" : "hoa.dac~ 1:20"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-3",
									"maxclass" : "newobj",
									"numinlets" : 16,
									"numoutlets" : 20,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal" ],
									"patching_rect" : [ 30.0, 431.0, 312.0, 22.0 ],
									"style" : "",
									"text" : "hoa.3d.decoder~ 3 20"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-23",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 339.5, 293.0, 71.0, 33.0 ],
									"style" : "",
									"text" : "set delays with a float",
									"textcolor" : [ 0.30292, 0.30292, 0.30292, 1.0 ],
									"varname" : "autohelp_top_description[2]"
								}

							}
, 							{
								"box" : 								{
									"angle" : 0.0,
									"bgcolor" : [ 0.94324, 0.954082, 0.954082, 1.0 ],
									"border" : 1,
									"bordercolor" : [ 0.75, 0.75, 0.75, 1.0 ],
									"id" : "obj-24",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 339.5, 293.0, 71.0, 33.0 ],
									"proportion" : 0.39,
									"rounded" : 0,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"format" : 6,
									"id" : "obj-12",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 323.0, 263.0, 50.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"args" : [ "@size", 0 ],
									"bgmode" : 0,
									"border" : 0,
									"clickthrough" : 0,
									"enablehscroll" : 0,
									"enablevscroll" : 0,
									"id" : "obj-25",
									"lockeddragscroll" : 0,
									"maxclass" : "bpatcher",
									"name" : "hoa.helpcredit.maxpat",
									"numinlets" : 1,
									"numoutlets" : 0,
									"offset" : [ 0.0, 0.0 ],
									"patching_rect" : [ 30.0, 583.0, 548.0, 23.0 ],
									"viewvisibility" : 1
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 0.853045, 0.853045, 0.853045, 1.0 ],
									"color" : [ 0.92549, 0.364706, 0.341176, 1.0 ],
									"id" : "obj-2",
									"local" : 1,
									"maxclass" : "ezdac~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 383.5, 514.0, 45.0, 45.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"args" : [ "@module", 0, "@file", "cello-f2.aif", "@loop", 1 ],
									"bgmode" : 0,
									"border" : 0,
									"clickthrough" : 0,
									"enablehscroll" : 0,
									"enablevscroll" : 0,
									"id" : "obj-1",
									"lockeddragscroll" : 0,
									"maxclass" : "bpatcher",
									"name" : "demosound.maxpat",
									"numinlets" : 0,
									"numoutlets" : 1,
									"offset" : [ 0.0, 0.0 ],
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 30.0, 104.0, 221.0, 93.0 ],
									"viewvisibility" : 1
								}

							}
, 							{
								"box" : 								{
									"args" : [ "@obj-name", "hoa.fx.delay~", "@obj-desc", "A multichannel delay processor" ],
									"bgmode" : 0,
									"border" : 0,
									"clickthrough" : 0,
									"enablehscroll" : 0,
									"enablevscroll" : 0,
									"id" : "obj-18",
									"lockeddragscroll" : 0,
									"maxclass" : "bpatcher",
									"name" : "hoa.helpheader.maxpat",
									"numinlets" : 1,
									"numoutlets" : 0,
									"offset" : [ 0.0, 0.0 ],
									"patching_rect" : [ 7.0, 6.0, 483.0, 56.0 ],
									"viewvisibility" : 1
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 7.180877,
									"hidden" : 1,
									"id" : "obj-19",
									"linecount" : 2,
									"maxclass" : "newobj",
									"numinlets" : 4,
									"numoutlets" : 0,
									"patching_rect" : [ 406.0, 22.5, 52.0, 25.0 ],
									"style" : "",
									"text" : "bgcolor 0.93 0.93 0.92 1."
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-21",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 7.0, 72.0, 321.0, 20.0 ],
									"style" : "",
									"text" : "hoa.fx.delay~ allows you to delay the differents harmonics.",
									"textcolor" : [ 0.30292, 0.30292, 0.30292, 1.0 ],
									"varname" : "autohelp_top_description[1]"
								}

							}
, 							{
								"box" : 								{
									"angle" : 0.0,
									"bgcolor" : [ 0.94324, 0.954082, 0.954082, 1.0 ],
									"border" : 1,
									"bordercolor" : [ 0.75, 0.75, 0.75, 1.0 ],
									"id" : "obj-22",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 7.0, 72.0, 480.0, 20.0 ],
									"proportion" : 0.39,
									"rounded" : 0,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"hidden" : 1,
									"id" : "obj-46",
									"maxclass" : "newobj",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 7,
											"minor" : 0,
											"revision" : 0,
											"architecture" : "x86",
											"modernui" : 1
										}
,
										"rect" : [ 0.0, 0.0, 640.0, 480.0 ],
										"bglocked" : 0,
										"openinpresentation" : 0,
										"default_fontsize" : 12.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 1,
										"gridsize" : [ 15.0, 15.0 ],
										"gridsnaponopen" : 1,
										"objectsnaponopen" : 1,
										"statusbarvisible" : 2,
										"toolbarvisible" : 1,
										"lefttoolbarpinned" : 0,
										"toptoolbarpinned" : 0,
										"righttoolbarpinned" : 0,
										"bottomtoolbarpinned" : 0,
										"toolbars_unpinned_last_save" : 0,
										"tallnewobj" : 0,
										"boxanimatetime" : 200,
										"enablehscroll" : 1,
										"enablevscroll" : 1,
										"devicewidth" : 0.0,
										"description" : "",
										"digest" : "",
										"tags" : "",
										"style" : "",
										"subpatcher_template" : "",
										"boxes" : [ 											{
												"box" : 												{
													"id" : "obj-10",
													"maxclass" : "button",
													"numinlets" : 1,
													"numoutlets" : 1,
													"outlettype" : [ "bang" ],
													"patching_rect" : [ 50.0, 160.0, 20.0, 20.0 ],
													"style" : ""
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 11.595187,
													"id" : "obj-23",
													"maxclass" : "newobj",
													"numinlets" : 2,
													"numoutlets" : 2,
													"outlettype" : [ "bang", "" ],
													"patching_rect" : [ 50.0, 130.0, 41.0, 19.0 ],
													"style" : "",
													"text" : "sel 32"
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 11.595187,
													"id" : "obj-17",
													"maxclass" : "newobj",
													"numinlets" : 0,
													"numoutlets" : 4,
													"outlettype" : [ "int", "int", "int", "int" ],
													"patching_rect" : [ 50.0, 100.0, 59.5, 19.0 ],
													"style" : "",
													"text" : "key"
												}

											}
, 											{
												"box" : 												{
													"comment" : "",
													"id" : "obj-45",
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 50.0, 240.0, 25.0, 25.0 ],
													"style" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"destination" : [ "obj-45", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-10", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-23", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"midpoints" : [ 59.5, 121.0, 59.5, 121.0 ],
													"source" : [ "obj-17", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-10", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"midpoints" : [ 59.5, 151.0, 59.5, 151.0 ],
													"source" : [ "obj-23", 0 ]
												}

											}
 ],
										"styles" : [ 											{
												"name" : "newobjBlue-1",
												"default" : 												{
													"accentcolor" : [ 0.317647, 0.654902, 0.976471, 1.0 ]
												}
,
												"parentstyle" : "",
												"multi" : 0
											}
, 											{
												"name" : "newobjGreen-1",
												"default" : 												{
													"accentcolor" : [ 0.0, 0.533333, 0.168627, 1.0 ]
												}
,
												"parentstyle" : "",
												"multi" : 0
											}
, 											{
												"name" : "newobjYellow-1",
												"default" : 												{
													"fontsize" : [ 12.059008 ],
													"accentcolor" : [ 0.82517, 0.78181, 0.059545, 1.0 ]
												}
,
												"parentstyle" : "",
												"multi" : 0
											}
 ]
									}
,
									"patching_rect" : [ 501.0, 13.0, 51.0, 21.0 ],
									"saved_object_attributes" : 									{
										"description" : "",
										"digest" : "",
										"globalpatchername" : "",
										"style" : "",
										"tags" : ""
									}
,
									"style" : "",
									"text" : "p key32"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"hidden" : 1,
									"id" : "obj-31",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 501.0, 43.0, 74.0, 21.0 ],
									"saved_object_attributes" : 									{
										"nhcolor" : [ 0.0, 0.0, 1.0, 1.0 ],
										"phcolor" : [ 1.0, 0.0, 0.0, 1.0 ],
										"planecolor" : [ 1.0, 1.0, 1.0, 1.0 ],
										"zhcolor" : [ 0.2, 0.2, 0.2, 1.0 ]
									}
,
									"style" : "",
									"text" : "hoa.connect"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"id" : "obj-4",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 167.0, 263.0, 62.0, 21.0 ],
									"style" : "",
									"text" : "hoa.pi~ 2."
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"format" : 6,
									"id" : "obj-10",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 167.0, 203.0, 50.0, 21.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"hidden" : 1,
									"id" : "obj-16",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 167.0, 172.0, 79.0, 21.0 ],
									"style" : "",
									"text" : "loadmess 0.2"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"id" : "obj-29",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 167.0, 234.0, 72.0, 21.0 ],
									"style" : "",
									"text" : "phasor~ 0.2"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"id" : "obj-15",
									"maxclass" : "newobj",
									"numinlets" : 3,
									"numoutlets" : 16,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal" ],
									"patching_rect" : [ 30.0, 293.0, 293.6875, 21.0 ],
									"style" : "",
									"text" : "hoa.3d.encoder~ 3"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"id" : "obj-14",
									"maxclass" : "newobj",
									"numinlets" : 17,
									"numoutlets" : 16,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal" ],
									"patching_rect" : [ 30.0, 335.0, 312.0, 21.0 ],
									"style" : "",
									"text" : "hoa.3d.process~ 3 hoa.fx.delay~ harmonics @window 400"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-1", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-29", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-10", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-8", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 16 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-11", 0 ],
									"disabled" : 0,
									"hidden" : 1,
									"source" : [ "obj-13", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 15 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 15 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 14 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 14 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 13 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 13 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.2, 0.2, 0.2, 1.0 ],
									"destination" : [ "obj-3", 12 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 12 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 11 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 11 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 10 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 10 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 9 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 9 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 8 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 8 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 7 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 7 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.2, 0.2, 0.2, 1.0 ],
									"destination" : [ "obj-3", 6 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 6 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 5 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 4 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 4 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 3 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.2, 0.2, 0.2, 1.0 ],
									"destination" : [ "obj-3", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.2, 0.2, 0.2, 1.0 ],
									"destination" : [ "obj-3", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-6", 15 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 332.5, 368.0, 821.5, 368.0 ],
									"source" : [ "obj-14", 15 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-6", 14 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 312.966675, 368.0, 807.900024, 368.0 ],
									"source" : [ "obj-14", 14 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-6", 13 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 293.433319, 368.0, 794.299988, 368.0 ],
									"source" : [ "obj-14", 13 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.2, 0.2, 0.2, 1.0 ],
									"destination" : [ "obj-6", 12 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 273.899994, 368.0, 780.700012, 368.0 ],
									"source" : [ "obj-14", 12 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-6", 11 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 254.366669, 368.0, 767.099976, 368.0 ],
									"source" : [ "obj-14", 11 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-6", 10 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 234.833328, 368.0, 753.5, 368.0 ],
									"source" : [ "obj-14", 10 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-6", 9 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 215.300003, 368.0, 739.900024, 368.0 ],
									"source" : [ "obj-14", 9 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-6", 8 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 195.766663, 368.0, 726.299988, 368.0 ],
									"source" : [ "obj-14", 8 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-6", 7 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 176.233337, 368.0, 712.700012, 368.0 ],
									"source" : [ "obj-14", 7 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.2, 0.2, 0.2, 1.0 ],
									"destination" : [ "obj-6", 6 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 156.699997, 368.0, 699.099976, 368.0 ],
									"source" : [ "obj-14", 6 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-6", 5 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 137.166672, 368.0, 685.5, 368.0 ],
									"source" : [ "obj-14", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-6", 4 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 117.633331, 368.0, 671.900024, 368.0 ],
									"source" : [ "obj-14", 4 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-6", 3 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 98.099998, 368.0, 658.299988, 368.0 ],
									"source" : [ "obj-14", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.2, 0.2, 0.2, 1.0 ],
									"destination" : [ "obj-6", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 78.566666, 368.0, 644.700012, 368.0 ],
									"source" : [ "obj-14", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-6", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 59.033333, 368.0, 631.099976, 368.0 ],
									"source" : [ "obj-14", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.2, 0.2, 0.2, 1.0 ],
									"destination" : [ "obj-6", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 39.5, 368.0, 617.5, 368.0 ],
									"source" : [ "obj-14", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-14", 15 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 15 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-14", 14 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 14 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-14", 13 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 13 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.2, 0.2, 0.2, 1.0 ],
									"destination" : [ "obj-14", 12 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 12 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 11 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 11 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 10 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 10 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 9 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 9 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-14", 8 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 8 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-14", 7 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 7 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.2, 0.2, 0.2, 1.0 ],
									"destination" : [ "obj-14", 6 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 6 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 5 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 4 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 4 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-14", 3 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.2, 0.2, 0.2, 1.0 ],
									"destination" : [ "obj-14", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.2, 0.2, 0.2, 1.0 ],
									"destination" : [ "obj-14", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"disabled" : 0,
									"hidden" : 1,
									"source" : [ "obj-16", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-12", 0 ],
									"disabled" : 0,
									"hidden" : 1,
									"source" : [ "obj-17", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-4", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-29", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 19 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 19 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 18 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 18 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 17 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 17 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 16 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 16 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 15 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 15 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 14 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 14 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 13 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 13 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 12 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 12 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 11 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 11 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 10 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 10 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 9 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 9 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 8 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 8 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 7 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 7 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 6 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 6 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 5 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 4 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 4 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 3 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-3", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-4", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-31", 0 ],
									"disabled" : 0,
									"hidden" : 1,
									"midpoints" : [ 510.5, 34.0, 510.5, 34.0 ],
									"source" : [ "obj-46", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-9", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-8", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-9", 0 ]
								}

							}
 ],
						"styles" : [ 							{
								"name" : "newobjBlue-1",
								"default" : 								{
									"accentcolor" : [ 0.317647, 0.654902, 0.976471, 1.0 ]
								}
,
								"parentstyle" : "",
								"multi" : 0
							}
, 							{
								"name" : "newobjGreen-1",
								"default" : 								{
									"accentcolor" : [ 0.0, 0.533333, 0.168627, 1.0 ]
								}
,
								"parentstyle" : "",
								"multi" : 0
							}
, 							{
								"name" : "newobjYellow-1",
								"default" : 								{
									"fontsize" : [ 12.059008 ],
									"accentcolor" : [ 0.82517, 0.78181, 0.059545, 1.0 ]
								}
,
								"parentstyle" : "",
								"multi" : 0
							}
 ]
					}
,
					"patching_rect" : [ 16.0, 92.0, 101.0, 20.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p \"3d harmonics\"",
					"textcolor" : [ 0.209184, 0.209184, 0.209184, 1.0 ],
					"varname" : "q_tab[3]"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"color" : [ 0.734694, 0.734694, 0.734694, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"hidden" : 1,
					"id" : "obj-19",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 0,
							"revision" : 0,
							"architecture" : "x86",
							"modernui" : 1
						}
,
						"rect" : [ 0.0, 26.0, 925.0, 629.0 ],
						"bgcolor" : [ 0.93, 0.93, 0.92, 1.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 1,
						"gridsize" : [ 20.0, 20.0 ],
						"gridsnaponopen" : 1,
						"objectsnaponopen" : 1,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"lefttoolbarpinned" : 0,
						"toptoolbarpinned" : 0,
						"righttoolbarpinned" : 0,
						"bottomtoolbarpinned" : 0,
						"toolbars_unpinned_last_save" : 0,
						"tallnewobj" : 0,
						"boxanimatetime" : 200,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"style" : "",
						"subpatcher_template" : "",
						"showontab" : 1,
						"boxes" : [ 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"hidden" : 1,
									"id" : "obj-17",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 666.5, 216.0, 60.0, 22.0 ],
									"style" : "",
									"text" : "loadbang"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-8",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 666.5, 177.0, 139.0, 20.0 ],
									"style" : "",
									"text" : "A list of feedback values",
									"textcolor" : [ 0.30292, 0.30292, 0.30292, 1.0 ],
									"varname" : "autohelp_top_description[2]"
								}

							}
, 							{
								"box" : 								{
									"angle" : 0.0,
									"bgcolor" : [ 0.94324, 0.954082, 0.954082, 1.0 ],
									"border" : 1,
									"bordercolor" : [ 0.75, 0.75, 0.75, 1.0 ],
									"id" : "obj-13",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 666.5, 177.0, 139.0, 20.0 ],
									"proportion" : 0.39,
									"rounded" : 0,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-27",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 425.5, 177.0, 71.0, 33.0 ],
									"style" : "",
									"text" : "A list of delay times",
									"textcolor" : [ 0.30292, 0.30292, 0.30292, 1.0 ],
									"varname" : "autohelp_top_description[3]"
								}

							}
, 							{
								"box" : 								{
									"angle" : 0.0,
									"bgcolor" : [ 0.94324, 0.954082, 0.954082, 1.0 ],
									"border" : 1,
									"bordercolor" : [ 0.75, 0.75, 0.75, 1.0 ],
									"id" : "obj-28",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 425.5, 177.0, 71.0, 33.0 ],
									"proportion" : 0.39,
									"rounded" : 0,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"attr" : "minmax",
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-1",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 666.5, 216.0, 188.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-6",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 666.5, 484.0, 107.0, 22.0 ],
									"style" : "",
									"text" : "prepend feedback"
								}

							}
, 							{
								"box" : 								{
									"channels" : 16,
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"hoa_2d_space_saved_state" : [ 0.207675, 0.22914, 0.209018, 0.194483, 0.126501, 0.123033, 0.132806, 0.196326, 0.326935, 0.439389, 0.486401, 0.466822, 0.535652, 0.758852, 0.588544, 0.400281 ],
									"id" : "obj-5",
									"maxclass" : "hoa.2d.space",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "list", "int" ],
									"parameter_enable" : 1,
									"patching_rect" : [ 666.5, 246.0, 225.0, 225.0 ],
									"saved_attribute_attributes" : 									{
										"valueof" : 										{
											"parameter_longname" : "hoa.2d.space[1]",
											"parameter_shortname" : "hoa.2d.space[1]",
											"parameter_type" : 3,
											"parameter_initial_enable" : 1,
											"parameter_initial" : [ 0.207675, 0.22914, 0.209018, 0.194483, 0.126501, 0.123033, 0.132806, 0.196326, 0.326935, 0.439389, 0.486401, 0.466822, 0.535652, 0.758852, 0.588544, 0.400281 ],
											"parameter_invisible" : 1
										}

									}
,
									"spcolor" : [ 0.347599, 0.347599, 0.347599, 0.5 ],
									"varname" : "hoa.2d.space[1]"
								}

							}
, 							{
								"box" : 								{
									"args" : [ "@module", 0, "@file", "cello-f2.aif", "@loop", 1 ],
									"bgmode" : 0,
									"border" : 0,
									"clickthrough" : 0,
									"enablehscroll" : 0,
									"enablevscroll" : 0,
									"id" : "obj-55",
									"lockeddragscroll" : 0,
									"maxclass" : "bpatcher",
									"name" : "demosound.maxpat",
									"numinlets" : 0,
									"numoutlets" : 1,
									"offset" : [ 0.0, 0.0 ],
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 30.0, 104.0, 221.0, 93.0 ],
									"viewvisibility" : 1
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-12",
									"maxclass" : "newobj",
									"numinlets" : 16,
									"numoutlets" : 15,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal" ],
									"patching_rect" : [ 30.0, 389.5, 329.0, 22.0 ],
									"style" : "",
									"text" : "hoa.2d.recomposer~ 7 16 fixe"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-11",
									"maxclass" : "newobj",
									"numinlets" : 15,
									"numoutlets" : 16,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal" ],
									"patching_rect" : [ 30.0, 268.0, 309.625, 22.0 ],
									"style" : "",
									"text" : "hoa.2d.projector~ 7 16"
								}

							}
, 							{
								"box" : 								{
									"channels" : 16,
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"hoa_2d_space_saved_state" : [ 1851.342651, 1843.300415, 1851.807983, 1872.309326, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1949.922485, 1940.075562, 1907.340332 ],
									"id" : "obj-7",
									"maxclass" : "hoa.2d.space",
									"minmax" : [ 0.0, 2000.0 ],
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "list", "int" ],
									"parameter_enable" : 1,
									"patching_rect" : [ 425.5, 246.0, 225.0, 225.0 ],
									"saved_attribute_attributes" : 									{
										"valueof" : 										{
											"parameter_longname" : "hoa.2d.space",
											"parameter_shortname" : "hoa.2d.space",
											"parameter_type" : 3,
											"parameter_initial_enable" : 1,
											"parameter_initial" : [ 1851.342651, 1843.300415, 1851.807983, 1872.309326, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1949.922485, 1940.075562, 1907.340332 ],
											"parameter_invisible" : 1
										}

									}
,
									"spcolor" : [ 0.347599, 0.347599, 0.347599, 0.5 ],
									"varname" : "hoa.2d.space"
								}

							}
, 							{
								"box" : 								{
									"args" : [ "@size", 0 ],
									"bgmode" : 0,
									"border" : 0,
									"clickthrough" : 0,
									"enablehscroll" : 0,
									"enablevscroll" : 0,
									"id" : "obj-25",
									"lockeddragscroll" : 0,
									"maxclass" : "bpatcher",
									"name" : "hoa.helpcredit.maxpat",
									"numinlets" : 1,
									"numoutlets" : 0,
									"offset" : [ 0.0, 0.0 ],
									"patching_rect" : [ 30.0, 578.0, 548.0, 23.0 ],
									"viewvisibility" : 1
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 0.853045, 0.853045, 0.853045, 1.0 ],
									"color" : [ 0.92549, 0.364706, 0.341176, 1.0 ],
									"id" : "obj-2",
									"local" : 1,
									"maxclass" : "ezdac~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 40.0, 507.0, 45.0, 45.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"bgmode" : 0,
									"border" : 1,
									"clickthrough" : 0,
									"enablehscroll" : 0,
									"enablevscroll" : 0,
									"id" : "obj-3",
									"lockeddragscroll" : 0,
									"maxclass" : "bpatcher",
									"name" : "hoa.helpout_7.maxpat",
									"numinlets" : 15,
									"numoutlets" : 0,
									"offset" : [ 0.0, 0.0 ],
									"patching_rect" : [ 30.0, 427.0, 329.0, 132.0 ],
									"viewvisibility" : 1
								}

							}
, 							{
								"box" : 								{
									"args" : [ "@obj-name", "hoa.fx.delay~", "@obj-desc", "A multichannel delay processor" ],
									"bgmode" : 0,
									"border" : 0,
									"clickthrough" : 0,
									"enablehscroll" : 0,
									"enablevscroll" : 0,
									"id" : "obj-18",
									"lockeddragscroll" : 0,
									"maxclass" : "bpatcher",
									"name" : "hoa.helpheader.maxpat",
									"numinlets" : 1,
									"numoutlets" : 0,
									"offset" : [ 0.0, 0.0 ],
									"patching_rect" : [ 7.0, 6.0, 483.0, 56.0 ],
									"viewvisibility" : 1
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 7.180877,
									"hidden" : 1,
									"id" : "obj-19",
									"linecount" : 2,
									"maxclass" : "newobj",
									"numinlets" : 4,
									"numoutlets" : 0,
									"patching_rect" : [ 406.0, 22.5, 52.0, 25.0 ],
									"style" : "",
									"text" : "bgcolor 0.93 0.93 0.92 1."
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-21",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 7.0, 72.0, 351.0, 20.0 ],
									"style" : "",
									"text" : "hoa.fx.delay~ allows you to delay different planewaves channels",
									"textcolor" : [ 0.30292, 0.30292, 0.30292, 1.0 ],
									"varname" : "autohelp_top_description[1]"
								}

							}
, 							{
								"box" : 								{
									"angle" : 0.0,
									"bgcolor" : [ 0.94324, 0.954082, 0.954082, 1.0 ],
									"border" : 1,
									"bordercolor" : [ 0.75, 0.75, 0.75, 1.0 ],
									"id" : "obj-22",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 7.0, 72.0, 480.0, 20.0 ],
									"proportion" : 0.39,
									"rounded" : 0,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"hidden" : 1,
									"id" : "obj-46",
									"maxclass" : "newobj",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 7,
											"minor" : 0,
											"revision" : 0,
											"architecture" : "x86",
											"modernui" : 1
										}
,
										"rect" : [ 0.0, 0.0, 640.0, 480.0 ],
										"bglocked" : 0,
										"openinpresentation" : 0,
										"default_fontsize" : 12.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 1,
										"gridsize" : [ 15.0, 15.0 ],
										"gridsnaponopen" : 1,
										"objectsnaponopen" : 1,
										"statusbarvisible" : 2,
										"toolbarvisible" : 1,
										"lefttoolbarpinned" : 0,
										"toptoolbarpinned" : 0,
										"righttoolbarpinned" : 0,
										"bottomtoolbarpinned" : 0,
										"toolbars_unpinned_last_save" : 0,
										"tallnewobj" : 0,
										"boxanimatetime" : 200,
										"enablehscroll" : 1,
										"enablevscroll" : 1,
										"devicewidth" : 0.0,
										"description" : "",
										"digest" : "",
										"tags" : "",
										"style" : "",
										"subpatcher_template" : "",
										"boxes" : [ 											{
												"box" : 												{
													"id" : "obj-10",
													"maxclass" : "button",
													"numinlets" : 1,
													"numoutlets" : 1,
													"outlettype" : [ "bang" ],
													"patching_rect" : [ 50.0, 160.0, 20.0, 20.0 ],
													"style" : ""
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 11.595187,
													"id" : "obj-23",
													"maxclass" : "newobj",
													"numinlets" : 2,
													"numoutlets" : 2,
													"outlettype" : [ "bang", "" ],
													"patching_rect" : [ 50.0, 130.0, 41.0, 19.0 ],
													"style" : "",
													"text" : "sel 32"
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 11.595187,
													"id" : "obj-17",
													"maxclass" : "newobj",
													"numinlets" : 0,
													"numoutlets" : 4,
													"outlettype" : [ "int", "int", "int", "int" ],
													"patching_rect" : [ 50.0, 100.0, 59.5, 19.0 ],
													"style" : "",
													"text" : "key"
												}

											}
, 											{
												"box" : 												{
													"comment" : "",
													"id" : "obj-45",
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 50.0, 240.0, 25.0, 25.0 ],
													"style" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"destination" : [ "obj-45", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-10", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-23", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"midpoints" : [ 59.5, 121.0, 59.5, 121.0 ],
													"source" : [ "obj-17", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-10", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"midpoints" : [ 59.5, 151.0, 59.5, 151.0 ],
													"source" : [ "obj-23", 0 ]
												}

											}
 ]
									}
,
									"patching_rect" : [ 501.0, 13.0, 51.0, 21.0 ],
									"saved_object_attributes" : 									{
										"description" : "",
										"digest" : "",
										"globalpatchername" : "",
										"style" : "",
										"tags" : ""
									}
,
									"style" : "",
									"text" : "p key32"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"hidden" : 1,
									"id" : "obj-31",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 501.0, 43.0, 74.0, 21.0 ],
									"saved_object_attributes" : 									{
										"nhcolor" : [ 0.0, 0.0, 1.0, 1.0 ],
										"phcolor" : [ 1.0, 0.0, 0.0, 1.0 ],
										"planecolor" : [ 1.0, 1.0, 1.0, 1.0 ],
										"zhcolor" : [ 0.2, 0.2, 0.2, 1.0 ]
									}
,
									"style" : "",
									"text" : "hoa.connect"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"id" : "obj-4",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 320.625, 191.0, 62.0, 21.0 ],
									"style" : "",
									"text" : "hoa.pi~ 2."
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"format" : 6,
									"id" : "obj-10",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 320.625, 131.0, 50.0, 21.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"hidden" : 1,
									"id" : "obj-16",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 321.0, 104.0, 79.0, 21.0 ],
									"style" : "",
									"text" : "loadmess 0.5"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"id" : "obj-29",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 320.625, 162.0, 72.0, 21.0 ],
									"style" : "",
									"text" : "phasor~ 0.5"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"id" : "obj-15",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 15,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal" ],
									"patching_rect" : [ 30.0, 224.0, 309.625, 21.0 ],
									"style" : "",
									"text" : "hoa.2d.encoder~ 7"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"id" : "obj-14",
									"maxclass" : "newobj",
									"numinlets" : 17,
									"numoutlets" : 16,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal" ],
									"patching_rect" : [ 30.0, 325.0, 329.0, 21.0 ],
									"style" : "",
									"text" : "hoa.2d.process~ 16 hoa.fx.delay~ planewaves @window 200"
								}

							}
, 							{
								"box" : 								{
									"attr" : "minmax",
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-9",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 425.5, 216.0, 188.0, 22.0 ],
									"style" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-1", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-29", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-10", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 15 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 15 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 14 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 14 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 13 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 13 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 12 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 12 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 11 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 11 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 10 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 10 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 9 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 9 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 8 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 8 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 7 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 7 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 6 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 6 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 5 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 4 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 4 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 3 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 14 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 14 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 13 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 13 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 12 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 12 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 11 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 11 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 10 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 10 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 9 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 9 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 8 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 8 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 7 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 7 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 6 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 6 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 5 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 4 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 4 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 3 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.2, 0.2, 0.2, 1.0 ],
									"destination" : [ "obj-3", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-12", 15 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 15 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-12", 14 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 14 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-12", 13 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 13 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-12", 12 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 12 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-12", 11 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 11 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-12", 10 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 10 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-12", 9 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 9 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-12", 8 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 8 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-12", 7 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 7 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-12", 6 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 6 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-12", 5 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-12", 4 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 4 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-12", 3 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-12", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-12", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 1.0, 1.0, 1.0 ],
									"destination" : [ "obj-12", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-11", 14 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 14 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-11", 13 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 13 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-11", 12 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 12 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-11", 11 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 11 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-11", 10 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 10 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-11", 9 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 9 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-11", 8 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 8 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-11", 7 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 7 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-11", 6 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 6 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-11", 5 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-11", 4 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 4 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-11", 3 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-11", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-11", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.2, 0.2, 0.2, 1.0 ],
									"destination" : [ "obj-11", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"disabled" : 0,
									"hidden" : 1,
									"source" : [ "obj-16", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"disabled" : 0,
									"hidden" : 1,
									"source" : [ "obj-17", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"disabled" : 0,
									"hidden" : 1,
									"source" : [ "obj-17", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-4", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-29", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-4", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-31", 0 ],
									"disabled" : 0,
									"hidden" : 1,
									"midpoints" : [ 510.5, 34.0, 510.5, 34.0 ],
									"source" : [ "obj-46", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-5", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-55", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 16 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 676.0, 513.0, 392.75, 513.0, 392.75, 292.0, 349.5, 292.0 ],
									"source" : [ "obj-6", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 16 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 435.0, 480.0, 392.25, 480.0, 392.25, 292.0, 349.5, 292.0 ],
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-9", 0 ]
								}

							}
 ],
						"styles" : [ 							{
								"name" : "AudioStatus_Menu",
								"default" : 								{
									"bgfillcolor" : 									{
										"type" : "color",
										"color" : [ 0.294118, 0.313726, 0.337255, 1 ],
										"color1" : [ 0.454902, 0.462745, 0.482353, 0.0 ],
										"color2" : [ 0.290196, 0.309804, 0.301961, 1.0 ],
										"angle" : 270.0,
										"proportion" : 0.39,
										"autogradient" : 0
									}

								}
,
								"parentstyle" : "",
								"multi" : 0
							}
, 							{
								"name" : "newobjBlue-1",
								"default" : 								{
									"accentcolor" : [ 0.317647, 0.654902, 0.976471, 1.0 ]
								}
,
								"parentstyle" : "",
								"multi" : 0
							}
, 							{
								"name" : "newobjGreen-1",
								"default" : 								{
									"accentcolor" : [ 0.0, 0.533333, 0.168627, 1.0 ]
								}
,
								"parentstyle" : "",
								"multi" : 0
							}
, 							{
								"name" : "newobjYellow-1",
								"default" : 								{
									"fontsize" : [ 12.059008 ],
									"accentcolor" : [ 0.82517, 0.78181, 0.059545, 1.0 ]
								}
,
								"parentstyle" : "",
								"multi" : 0
							}
 ]
					}
,
					"patching_rect" : [ 16.0, 57.0, 109.0, 20.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p \"2d planewaves\"",
					"textcolor" : [ 0.209184, 0.209184, 0.209184, 1.0 ],
					"varname" : "q_tab[2]"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"color" : [ 0.734694, 0.734694, 0.734694, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"hidden" : 1,
					"id" : "obj-18",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 0,
							"revision" : 0,
							"architecture" : "x86",
							"modernui" : 1
						}
,
						"rect" : [ 0.0, 26.0, 925.0, 629.0 ],
						"bgcolor" : [ 0.93, 0.93, 0.92, 1.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 12.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 1,
						"gridsize" : [ 20.0, 20.0 ],
						"gridsnaponopen" : 1,
						"objectsnaponopen" : 1,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"lefttoolbarpinned" : 0,
						"toptoolbarpinned" : 0,
						"righttoolbarpinned" : 0,
						"bottomtoolbarpinned" : 0,
						"toolbars_unpinned_last_save" : 0,
						"tallnewobj" : 0,
						"boxanimatetime" : 200,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"style" : "",
						"subpatcher_template" : "",
						"showontab" : 1,
						"boxes" : [ 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-27",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 448.0, 335.0, 239.0, 20.0 ],
									"style" : "",
									"text" : "set time delay of each harmonics with a list",
									"textcolor" : [ 0.30292, 0.30292, 0.30292, 1.0 ],
									"varname" : "autohelp_top_description[3]"
								}

							}
, 							{
								"box" : 								{
									"angle" : 0.0,
									"bgcolor" : [ 0.94324, 0.954082, 0.954082, 1.0 ],
									"border" : 1,
									"bordercolor" : [ 0.75, 0.75, 0.75, 1.0 ],
									"id" : "obj-28",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 448.0, 335.0, 239.0, 20.0 ],
									"proportion" : 0.39,
									"rounded" : 0,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-23",
									"linecount" : 2,
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 356.5, 277.0, 71.0, 33.0 ],
									"style" : "",
									"text" : "set delays with a float",
									"textcolor" : [ 0.30292, 0.30292, 0.30292, 1.0 ],
									"varname" : "autohelp_top_description[2]"
								}

							}
, 							{
								"box" : 								{
									"angle" : 0.0,
									"bgcolor" : [ 0.94324, 0.954082, 0.954082, 1.0 ],
									"border" : 1,
									"bordercolor" : [ 0.75, 0.75, 0.75, 1.0 ],
									"id" : "obj-24",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 356.5, 277.0, 71.0, 33.0 ],
									"proportion" : 0.39,
									"rounded" : 0,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"format" : 6,
									"id" : "obj-12",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 340.0, 247.0, 50.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"attr" : "setminmax",
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-7",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 448.0, 107.0, 201.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 1.0, 1.0, 1.0, 0.5 ],
									"contdata" : 1,
									"id" : "obj-9",
									"maxclass" : "multislider",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 448.0, 142.0, 394.0, 168.0 ],
									"setminmax" : [ 0.0, 2000.0 ],
									"size" : 15,
									"slidercolor" : [ 0.0, 0.0, 0.0, 1.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"args" : [ "@size", 0 ],
									"bgmode" : 0,
									"border" : 0,
									"clickthrough" : 0,
									"enablehscroll" : 0,
									"enablevscroll" : 0,
									"id" : "obj-25",
									"lockeddragscroll" : 0,
									"maxclass" : "bpatcher",
									"name" : "hoa.helpcredit.maxpat",
									"numinlets" : 1,
									"numoutlets" : 0,
									"offset" : [ 0.0, 0.0 ],
									"patching_rect" : [ 30.0, 583.0, 548.0, 23.0 ],
									"viewvisibility" : 1
								}

							}
, 							{
								"box" : 								{
									"bgcolor" : [ 0.853045, 0.853045, 0.853045, 1.0 ],
									"color" : [ 0.92549, 0.364706, 0.341176, 1.0 ],
									"id" : "obj-2",
									"local" : 1,
									"maxclass" : "ezdac~",
									"numinlets" : 2,
									"numoutlets" : 0,
									"patching_rect" : [ 40.0, 512.0, 45.0, 45.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"bgmode" : 0,
									"border" : 1,
									"clickthrough" : 0,
									"enablehscroll" : 0,
									"enablevscroll" : 0,
									"id" : "obj-3",
									"lockeddragscroll" : 0,
									"maxclass" : "bpatcher",
									"name" : "hoa.helpout_7.maxpat",
									"numinlets" : 15,
									"numoutlets" : 0,
									"offset" : [ 0.0, 0.0 ],
									"patching_rect" : [ 30.0, 432.0, 329.0, 132.0 ],
									"viewvisibility" : 1
								}

							}
, 							{
								"box" : 								{
									"args" : [ "@module", 0, "@file", "cello-f2.aif", "@loop", 1 ],
									"bgmode" : 0,
									"border" : 0,
									"clickthrough" : 0,
									"enablehscroll" : 0,
									"enablevscroll" : 0,
									"id" : "obj-1",
									"lockeddragscroll" : 0,
									"maxclass" : "bpatcher",
									"name" : "demosound.maxpat",
									"numinlets" : 0,
									"numoutlets" : 1,
									"offset" : [ 0.0, 0.0 ],
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 30.0, 104.0, 221.0, 93.0 ],
									"viewvisibility" : 1
								}

							}
, 							{
								"box" : 								{
									"args" : [ "@obj-name", "hoa.fx.delay~", "@obj-desc", "A multichannel delay processor" ],
									"bgmode" : 0,
									"border" : 0,
									"clickthrough" : 0,
									"enablehscroll" : 0,
									"enablevscroll" : 0,
									"id" : "obj-18",
									"lockeddragscroll" : 0,
									"maxclass" : "bpatcher",
									"name" : "hoa.helpheader.maxpat",
									"numinlets" : 1,
									"numoutlets" : 0,
									"offset" : [ 0.0, 0.0 ],
									"patching_rect" : [ 7.0, 6.0, 483.0, 56.0 ],
									"viewvisibility" : 1
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 7.180877,
									"hidden" : 1,
									"id" : "obj-19",
									"linecount" : 2,
									"maxclass" : "newobj",
									"numinlets" : 4,
									"numoutlets" : 0,
									"patching_rect" : [ 406.0, 22.5, 52.0, 25.0 ],
									"style" : "",
									"text" : "bgcolor 0.93 0.93 0.92 1."
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-21",
									"maxclass" : "comment",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 7.0, 72.0, 321.0, 20.0 ],
									"style" : "",
									"text" : "hoa.fx.delay~ allows you to delay the differents harmonics.",
									"textcolor" : [ 0.30292, 0.30292, 0.30292, 1.0 ],
									"varname" : "autohelp_top_description[1]"
								}

							}
, 							{
								"box" : 								{
									"angle" : 0.0,
									"bgcolor" : [ 0.94324, 0.954082, 0.954082, 1.0 ],
									"border" : 1,
									"bordercolor" : [ 0.75, 0.75, 0.75, 1.0 ],
									"id" : "obj-22",
									"maxclass" : "panel",
									"mode" : 0,
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 7.0, 72.0, 480.0, 20.0 ],
									"proportion" : 0.39,
									"rounded" : 0,
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"hidden" : 1,
									"id" : "obj-46",
									"maxclass" : "newobj",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patcher" : 									{
										"fileversion" : 1,
										"appversion" : 										{
											"major" : 7,
											"minor" : 0,
											"revision" : 0,
											"architecture" : "x86",
											"modernui" : 1
										}
,
										"rect" : [ 0.0, 0.0, 640.0, 480.0 ],
										"bglocked" : 0,
										"openinpresentation" : 0,
										"default_fontsize" : 12.0,
										"default_fontface" : 0,
										"default_fontname" : "Arial",
										"gridonopen" : 1,
										"gridsize" : [ 15.0, 15.0 ],
										"gridsnaponopen" : 1,
										"objectsnaponopen" : 1,
										"statusbarvisible" : 2,
										"toolbarvisible" : 1,
										"lefttoolbarpinned" : 0,
										"toptoolbarpinned" : 0,
										"righttoolbarpinned" : 0,
										"bottomtoolbarpinned" : 0,
										"toolbars_unpinned_last_save" : 0,
										"tallnewobj" : 0,
										"boxanimatetime" : 200,
										"enablehscroll" : 1,
										"enablevscroll" : 1,
										"devicewidth" : 0.0,
										"description" : "",
										"digest" : "",
										"tags" : "",
										"style" : "",
										"subpatcher_template" : "",
										"boxes" : [ 											{
												"box" : 												{
													"id" : "obj-10",
													"maxclass" : "button",
													"numinlets" : 1,
													"numoutlets" : 1,
													"outlettype" : [ "bang" ],
													"patching_rect" : [ 50.0, 160.0, 20.0, 20.0 ],
													"style" : ""
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 11.595187,
													"id" : "obj-23",
													"maxclass" : "newobj",
													"numinlets" : 2,
													"numoutlets" : 2,
													"outlettype" : [ "bang", "" ],
													"patching_rect" : [ 50.0, 130.0, 41.0, 19.0 ],
													"style" : "",
													"text" : "sel 32"
												}

											}
, 											{
												"box" : 												{
													"fontname" : "Arial",
													"fontsize" : 11.595187,
													"id" : "obj-17",
													"maxclass" : "newobj",
													"numinlets" : 0,
													"numoutlets" : 4,
													"outlettype" : [ "int", "int", "int", "int" ],
													"patching_rect" : [ 50.0, 100.0, 59.5, 19.0 ],
													"style" : "",
													"text" : "key"
												}

											}
, 											{
												"box" : 												{
													"comment" : "",
													"id" : "obj-45",
													"maxclass" : "outlet",
													"numinlets" : 1,
													"numoutlets" : 0,
													"patching_rect" : [ 50.0, 240.0, 25.0, 25.0 ],
													"style" : ""
												}

											}
 ],
										"lines" : [ 											{
												"patchline" : 												{
													"destination" : [ "obj-45", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"source" : [ "obj-10", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-23", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"midpoints" : [ 59.5, 121.0, 59.5, 121.0 ],
													"source" : [ "obj-17", 0 ]
												}

											}
, 											{
												"patchline" : 												{
													"destination" : [ "obj-10", 0 ],
													"disabled" : 0,
													"hidden" : 0,
													"midpoints" : [ 59.5, 151.0, 59.5, 151.0 ],
													"source" : [ "obj-23", 0 ]
												}

											}
 ]
									}
,
									"patching_rect" : [ 501.0, 13.0, 51.0, 21.0 ],
									"saved_object_attributes" : 									{
										"description" : "",
										"digest" : "",
										"globalpatchername" : "",
										"style" : "",
										"tags" : ""
									}
,
									"style" : "",
									"text" : "p key32"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"hidden" : 1,
									"id" : "obj-31",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 501.0, 43.0, 74.0, 21.0 ],
									"saved_object_attributes" : 									{
										"nhcolor" : [ 0.0, 0.0, 1.0, 1.0 ],
										"phcolor" : [ 1.0, 0.0, 0.0, 1.0 ],
										"planecolor" : [ 1.0, 1.0, 1.0, 1.0 ],
										"zhcolor" : [ 0.2, 0.2, 0.2, 1.0 ]
									}
,
									"style" : "",
									"text" : "hoa.connect"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"id" : "obj-4",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 297.0, 164.0, 62.0, 21.0 ],
									"style" : "",
									"text" : "hoa.pi~ 2."
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"format" : 6,
									"id" : "obj-10",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 297.0, 104.0, 50.0, 21.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"hidden" : 1,
									"id" : "obj-16",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 297.0, 73.0, 79.0, 21.0 ],
									"style" : "",
									"text" : "loadmess 0.5"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"id" : "obj-29",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "signal" ],
									"patching_rect" : [ 297.0, 135.0, 72.0, 21.0 ],
									"style" : "",
									"text" : "phasor~ 0.5"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"id" : "obj-15",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 15,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal" ],
									"patching_rect" : [ 30.0, 210.0, 308.333344, 21.0 ],
									"style" : "",
									"text" : "hoa.2d.encoder~ 7"
								}

							}
, 							{
								"box" : 								{
									"fontname" : "Arial",
									"fontsize" : 11.595187,
									"id" : "obj-14",
									"maxclass" : "newobj",
									"numinlets" : 16,
									"numoutlets" : 15,
									"outlettype" : [ "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal", "signal" ],
									"patching_rect" : [ 30.0, 335.0, 329.0, 21.0 ],
									"style" : "",
									"text" : "hoa.2d.process~ 7 hoa.fx.delay~ harmonics @window 400"
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-1", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-29", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-10", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 15 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-12", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 14 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 14 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 13 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 13 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 12 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 12 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 11 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 11 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 10 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 10 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 9 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 9 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 8 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 8 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 7 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 7 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 6 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 6 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 5 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 4 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 4 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 3 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-3", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-3", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.2, 0.2, 0.2, 1.0 ],
									"destination" : [ "obj-3", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-14", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-14", 14 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 14 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 13 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 13 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-14", 12 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 12 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 11 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 11 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-14", 10 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 10 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 9 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 9 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-14", 8 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 8 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 7 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 7 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-14", 6 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 6 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 5 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 5 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-14", 4 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 4 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 3 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 3 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 1.0, 0.0, 0.0, 1.0 ],
									"destination" : [ "obj-14", 2 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.0, 0.0, 1.0, 1.0 ],
									"destination" : [ "obj-14", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"color" : [ 0.2, 0.2, 0.2, 1.0 ],
									"destination" : [ "obj-14", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"disabled" : 0,
									"hidden" : 1,
									"source" : [ "obj-16", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-4", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-29", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 306.5, 197.0, 328.833344, 197.0 ],
									"source" : [ "obj-4", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-31", 0 ],
									"disabled" : 0,
									"hidden" : 1,
									"midpoints" : [ 510.5, 34.0, 510.5, 34.0 ],
									"source" : [ "obj-46", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-9", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 15 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 457.5, 325.5, 349.5, 325.5 ],
									"source" : [ "obj-9", 0 ]
								}

							}
 ],
						"styles" : [ 							{
								"name" : "newobjBlue-1",
								"default" : 								{
									"accentcolor" : [ 0.317647, 0.654902, 0.976471, 1.0 ]
								}
,
								"parentstyle" : "",
								"multi" : 0
							}
, 							{
								"name" : "newobjGreen-1",
								"default" : 								{
									"accentcolor" : [ 0.0, 0.533333, 0.168627, 1.0 ]
								}
,
								"parentstyle" : "",
								"multi" : 0
							}
, 							{
								"name" : "newobjYellow-1",
								"default" : 								{
									"fontsize" : [ 12.059008 ],
									"accentcolor" : [ 0.82517, 0.78181, 0.059545, 1.0 ]
								}
,
								"parentstyle" : "",
								"multi" : 0
							}
 ]
					}
,
					"patching_rect" : [ 16.0, 27.0, 101.0, 20.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p \"2d harmonics\"",
					"textcolor" : [ 0.209184, 0.209184, 0.209184, 1.0 ],
					"varname" : "q_tab[1]"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 1.0, 1.0, 1.0, 1.0 ],
					"color" : [ 0.734694, 0.734694, 0.734694, 1.0 ],
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"hidden" : 1,
					"id" : "obj-24",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 0,
							"revision" : 0,
							"architecture" : "x86",
							"modernui" : 1
						}
,
						"rect" : [ 0.0, 26.0, 925.0, 629.0 ],
						"bglocked" : 0,
						"openinpresentation" : 0,
						"default_fontsize" : 13.0,
						"default_fontface" : 0,
						"default_fontname" : "Arial",
						"gridonopen" : 1,
						"gridsize" : [ 5.0, 5.0 ],
						"gridsnaponopen" : 1,
						"objectsnaponopen" : 1,
						"statusbarvisible" : 2,
						"toolbarvisible" : 1,
						"lefttoolbarpinned" : 0,
						"toptoolbarpinned" : 0,
						"righttoolbarpinned" : 0,
						"bottomtoolbarpinned" : 0,
						"toolbars_unpinned_last_save" : 0,
						"tallnewobj" : 0,
						"boxanimatetime" : 200,
						"enablehscroll" : 1,
						"enablevscroll" : 1,
						"devicewidth" : 0.0,
						"description" : "",
						"digest" : "",
						"tags" : "",
						"style" : "",
						"subpatcher_template" : "",
						"showontab" : 1,
						"boxes" : [  ],
						"lines" : [  ]
					}
,
					"patching_rect" : [ 128.0, 27.0, 50.0, 20.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"fontsize" : 13.0,
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p ?",
					"textcolor" : [ 0.209184, 0.209184, 0.209184, 1.0 ],
					"varname" : "q_tab"
				}

			}
 ],
		"lines" : [  ],
		"parameters" : 		{
			"obj-1::obj-1::obj-21::obj-6" : [ "live.tab[2]", "live.tab[1]", 0 ],
			"obj-18::obj-1::obj-35" : [ "[5]", "Level", 0 ],
			"obj-19::obj-5" : [ "hoa.2d.space[1]", "hoa.2d.space[1]", 0 ],
			"obj-18::obj-1::obj-21::obj-6" : [ "live.tab[3]", "live.tab[1]", 0 ],
			"obj-1::obj-1::obj-35" : [ "[6]", "Level", 0 ],
			"obj-19::obj-7" : [ "hoa.2d.space", "hoa.2d.space", 0 ],
			"obj-19::obj-55::obj-35" : [ "[1]", "Level", 0 ],
			"obj-19::obj-55::obj-21::obj-6" : [ "live.tab[1]", "live.tab[1]", 0 ]
		}
,
		"dependency_cache" : [ 			{
				"name" : "hoa.fx.delay~.maxpat",
				"bootpath" : "~/Documents/programmation/CICM/sourceTree/HoaLibrary/Max/Package/HoaLibrary/patchers/effects",
				"patcherrelativepath" : "../../patchers/effects",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "hoa.help.process.infos.maxpat",
				"bootpath" : "~/Documents/programmation/CICM/sourceTree/HoaLibrary/Max/Package/HoaLibrary/patchers/helputility",
				"patcherrelativepath" : "../../patchers/helputility",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "hoa.args.map.maxpat",
				"bootpath" : "~/Documents/programmation/CICM/sourceTree/HoaLibrary/Max/Package/HoaLibrary/misc/others",
				"patcherrelativepath" : "../../misc/others",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "hoa.mode.sel.maxpat",
				"bootpath" : "~/Documents/programmation/CICM/sourceTree/HoaLibrary/Max/Package/HoaLibrary/misc/others",
				"patcherrelativepath" : "../../misc/others",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "hoa.helpheader.maxpat",
				"bootpath" : "~/Documents/programmation/CICM/sourceTree/HoaLibrary/Max/Package/HoaLibrary/misc/others",
				"patcherrelativepath" : "../../misc/others",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "hoa.2d.scope~.svg",
				"bootpath" : "~/Documents/programmation/CICM/sourceTree/HoaLibrary/Max/Package/HoaLibrary/misc/object-palettes",
				"patcherrelativepath" : "../../misc/object-palettes",
				"type" : "svg ",
				"implicit" : 1
			}
, 			{
				"name" : "demosound.maxpat",
				"bootpath" : "/Applications/Max.app/Contents/Resources/C74/help/msp",
				"patcherrelativepath" : "../../../../../../../../../../../../Applications/Max.app/Contents/Resources/C74/help/msp",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "sine.svg",
				"bootpath" : "/Applications/Max.app/Contents/Resources/C74/media/max/picts/m4l-picts",
				"patcherrelativepath" : "../../../../../../../../../../../../Applications/Max.app/Contents/Resources/C74/media/max/picts/m4l-picts",
				"type" : "svg ",
				"implicit" : 1
			}
, 			{
				"name" : "saw.svg",
				"bootpath" : "/Applications/Max.app/Contents/Resources/C74/media/max/picts/m4l-picts",
				"patcherrelativepath" : "../../../../../../../../../../../../Applications/Max.app/Contents/Resources/C74/media/max/picts/m4l-picts",
				"type" : "svg ",
				"implicit" : 1
			}
, 			{
				"name" : "square.svg",
				"bootpath" : "/Applications/Max.app/Contents/Resources/C74/media/max/picts/m4l-picts",
				"patcherrelativepath" : "../../../../../../../../../../../../Applications/Max.app/Contents/Resources/C74/media/max/picts/m4l-picts",
				"type" : "svg ",
				"implicit" : 1
			}
, 			{
				"name" : "random.svg",
				"bootpath" : "/Applications/Max.app/Contents/Resources/C74/media/max/picts/m4l-picts",
				"patcherrelativepath" : "../../../../../../../../../../../../Applications/Max.app/Contents/Resources/C74/media/max/picts/m4l-picts",
				"type" : "svg ",
				"implicit" : 1
			}
, 			{
				"name" : "hoa.helpout_7.maxpat",
				"bootpath" : "~/Documents/programmation/CICM/sourceTree/HoaLibrary/Max/Package/HoaLibrary/misc/others/hoa.helpout",
				"patcherrelativepath" : "../../misc/others/hoa.helpout",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "hoa.helpcredit.maxpat",
				"bootpath" : "~/Documents/programmation/CICM/sourceTree/HoaLibrary/Max/Package/HoaLibrary/misc/others",
				"patcherrelativepath" : "../../misc/others",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "hoa.process~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.in.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.in~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.out~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.thisprocess~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.2d.encoder~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.pi~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.connect.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.2d.scope~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.gain~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.2d.meter~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.2d.decoder~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.2d.space.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.2d.projector~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.2d.recomposer~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.3d.encoder~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.3d.decoder~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "hoa.3d.scope~.mxo",
				"type" : "iLaX"
			}
 ],
		"embedsnapshot" : 0
	}

}
