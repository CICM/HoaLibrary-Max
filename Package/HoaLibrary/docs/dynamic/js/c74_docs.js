/*
	Copyright (c) 2012 Cycling '74

	Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
	and associated documentation files (the "Software"), to deal in the Software without restriction, 
	including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
	and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
	subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies 
	or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
	OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 function Console() {
 this.log = function(s ) {
   var d = document.getElementById("console");
   d.innerHTML += "<br>"+s;
 }
}
var console = new Console();
*/

// returns a string with object properties
function getObjectSrc(obj, indent) {
  var result = "";
  if (indent == null) indent = "";

  for (var property in obj) {
    var value = obj[property];
    if (typeof value == 'string')
      value = "'" + value + "'";
    else if (typeof value == 'object') {
      if (value instanceof Array){
        // Just let JS convert the Array to a string!
        value = "[ " + value + " ]";
      } else {
        // Recursive dump
        // (replace "  " by "\t" or something else if you prefer)
        var od = getObjectSrc(value, indent + "  ");
        // If you like { on the same line as the key
        //value = "{\n" + od + "\n" + indent + "}";
        // If you prefer { and } to be aligned
        value = "\n" + indent + "{\n" + od + "\n" + indent + "}";
      }
    }
    result += indent + "'" + property + "' : " + value + ",\n";
  }
  return result.replace(/,\n$/, "");
}

// takes a jquery xml node object and returns all of its children
// as a string
function getContents( jqnode ) {
	var tmp ="";
	$(jqnode).contents().each( function () {
		var nname = this.nodeName.toLowerCase();
		if (this.nodeType==3) {
			tmp += this.nodeValue;
		} else if (this.nodeType==1) {
		  tmp +=  "<" + nname;
			if (this.attributes != null && this.attributes != undefined) {
			for (var i=0; i < this.attributes.length;i++) {
				var a = this.attributes[i];
				tmp += " " + a.name + '="' + a.value +  '" ';
			}
			}
			tmp +=">";
			tmp += getContents( $(this) );
			if ( nname != "br")
		  tmp +=  "</" +nname + ">";
		}
	});
	return tmp;
}

// see: http://reference.sitepoint.com/javascript/Node/nodeName 
// takes a real XML node object ,not a jquery xml object
function parseXmlToText ( node ) {
	var tmp = "";
	// check if this is a text node 
	if (node.nodeType == 3) { // we have a text
		var text = node.nodeValue;
		text = $.trim( text );
		if (text != "") {
			tmp += text;
			return tmp;
		}
		return "";  // no text node has children
	} else if (node.nodeType ==1 ) {  // we have a valid sub node element
		tmp += "<";
		tmp += node.nodeName;
		if (node.attributes != null && node.attributes != undefined) {
			for (var i=0; i < node.attributes.length;i++) {
				var a = node.attributes[i];
				tmp += " " + a.name + '="' + a.value +  '" ';
			}
		}
		tmp += ">";
	}
	// recurse through this nodes children (if any) 
	if( node.childNodes) {
		for (var j=0; j < node.childNodes.length;j++) {
			tmp += parseXmlToText( node.childNodes[j] );
		}
	}

		// put the end tag on
	if (node.nodeType == 1) {
		tmp +=  "</";
		tmp+=node.nodeName;
		tmp += ">";
	} 
	return tmp;
}



(function() {
escape_re = /[#;&,\.\+\*~':"!\^\$\[\]\(\)=>|\/\\]/;
jQuery.escape = function jQuery$escape(s) {
  var left = s.split(escape_re, 1)[0];
  if (left == s) return s;
  return left + '\\' + 
    s.substr(left.length, 1) + 
    jQuery.escape(s.substr(left.length+1));
};
})();

if(!Array.prototype.indexOf) {
    Array.prototype.indexOf = function(needle) {
        for(var i = 0; i < this.length; i++) {
            if(this[i] === needle) {
                return i;
            }
        }
        return -1;
    };
}


$(document).ready(function () {
	
	//Check whether page is running locally or online.
	var ONLINE;
	switch(window.location.protocol) {
	   case 'http:':
	   case 'https:':
		 ONLINE = true;
		 break;
	   case 'file:':
		 ONLINE = false;
		 break;
	   default:
	   	 ONLINE = false;
		 break;
	}
	//ONLINE = true;  //can be set for testing.
	
	//Some globals
	//------------
	
	var USING_ENUMLIST = true;
	
	var activePanel = 'content';
	
	
	//Use Max's indexed db for search (true), or use js (false)
	if(ONLINE) {
		var USING_DB = false;
	} else {
		var USING_DB = true;
	}
	
	//temporary...if IE don't use db
   if (!document.createEvent) {
		USING_DB = false;
	}
	
	//Add debugging console to dom (for loggin within max)
	var DEBUGGING = false;
	
	
	//for intradocument scrolling, take this many ms
	var SCROLL_SPEED = 300;
	
	//store results of qlookup.json files
	var refpages = {};
	var tutorials = {};
	var vignettes = {};
	
	// these all have to be global
	var INTERFACES_PREFIX = "../../../Cycling \'74/interfaces";
	//var SEARCH_PREFIX = "max:db.simpledocsearch/~:%2FLibrary%2FCaches%2Fc74search.json/";
	//var SEARCH_RESULT_URL = "~:/Library/Caches/c74search.json";
	var SEARCH_PREFIX = "max:db.refwindowsearch/";
	var SEARCH_RESULT_URL = "../../c74search.json";

	//default to show objects as categories or a-z	
	var activeObjNavId = "#catNav";
	
	//var lastTitle;
	
	var URL_PREFIX = "../";
	var objects = [];
	var objectsLookup = [];
	
	var topCats = ['Max', 'MSP', 'Jitter', 'M4L'];
	
	// jb: removing M4L category, as there are currently no docs there. We should consider
	// moving the "core" module M4L docs to their own folder.
	var vigCats = ['Intro', 'Core', 'Gen', 'JS','Licenses', 'Lua', /*'M4L',*/ 'Thesaurus'];
	
	var hash = location.hash;
	
	//get parameters from url;
	var params = {};
	var hashIndex = location.href.indexOf('#');
	var qIndex = location.href.indexOf('?')+1;
	if(hashIndex==-1) {
		var paramstring = location.href.substring(qIndex);
	} else {
		var paramstring = location.href.substring(qIndex, hashIndex);
	}
	paramArray = paramstring.split('&');
	for(var i=0;i<paramArray.length;i++) {
		var pair = paramArray[i];
		var eIndex = pair.indexOf('=');
		var key = pair.substring(0, eIndex);
		var val = pair.substring(eIndex+1);
		params[key]=val;
	}
	
	
	var searchCats = ['text', 'name', 'tags'];
	searchCats['text'] = [];
	searchCats['name'] = [];
	searchCats['tags'] = [];
	
	var maxSubCats = [];
	var mspSubCats = [];
	var jitterSubCats = [];
	var m4lSubCats = [];
	
	var subcats = [];
	subcats['Max'] = maxSubCats;
	subcats['MSP'] = mspSubCats;
	subcats['Jitter'] = jitterSubCats;
	subcats['M4L'] = m4lSubCats;
	
	var ordinals = ['left', 'second', 'third', 'fourth', 'fifth', 'sixth', 'seventh', 'eighth', 'ninth', 'tenth', 'eleventh', 'twelfth', 'thirteenth', 'fourteenth', 'fifteenth'];
	var jbox = [];
	var ob3dMsgs = [];
	var ob3dAttrs = [];
	var jmopMsgs = [];
	var jmopAttrs = [];
	
	//STRING UTIITY
	if (typeof (String.prototype.trim) === "undefined") {
		String.prototype.trim = function () {
			return String(this).replace(/^\s+|\s+$/g, '');
		}; 
	}
	
	//TO GET HIDDEN LINK TO RESPOND TO FIRED CLICK EVENT
	//necessary to generate message to max to create dbsearch
	//---------------------------------------------------------
	function fireEvent(element,event) {

		if(event=="click") {
		   if (document.createEvent) {
			   // dispatch for firefox + others
			   var evt = document.createEvent("MouseEvents");
			   evt.initMouseEvent(event, true, true, window,0,0,0,0,0,false,false,false,false,0,null ); // event type,bubbling,cancelable
		   //myLog('firefox click '+$('#simplesearchlink').attr('href'));
			   return !document.getElementById("simplesearchlink").dispatchEvent(evt);
		   } else {
			   // dispatch for IE
		   //myLog('ie click'+$('#simplesearchlink').attr('href'));
			   var evt = document.createEventObject();
			   return document.getElementById("simplesearchlink").fireEvent('on'+event,evt)
		   }

		} else {
			   if (document.createEvent) {
			   // dispatch for firefox + others
			   var evt = document.createEvent("HTMLEvents");
			   evt.initEvent(event, true, true ); // event type,bubbling,cancelable
		   //myLog('firefox '+$('#simplesearchlink').attr('href'));
			   return !element.dispatchEvent(evt);
		   } else {
			   // dispatch for IE
			   var evt = document.createEventObject();
		   //myLog('ie '+$('#simplesearchlink').attr('href'));
			   return element.fireEvent('on'+event,evt);
		   }
		}
	}
	
	$(window).hashchange(function () {
		//myLog('hash changed');
		//$('#content').prepend('\n'+location.hash);
		var qIndex = location.hash.indexOf("?");
		var queryIndex = location.hash.indexOf('query=')+6;
		if(qIndex != -1) {
			var contentName = nameReplace(location.hash.substring(1, qIndex));
			//myLog('url after hashchange: '+location);
			//myLog('queryIndex '+queryIndex);
			//myLog('query after hashchange: '+location.hash.substring(queryIndex));
			var query = unescape(location.hash.substring(queryIndex));
			//myLog('query: '+query);
			$('#searchBox').attr('value', query);
			genSearchNav();
		} else {
			//myLog('hash changed with no query');
			var contentName = nameReplace(location.hash.substring(1));
			genContent(contentName);
		}
		/*
		if(tutorials[contentName]) {
			val = tutorials[contentName];
			tutName = val.digest;
			//document.title = tutName;
		} else if (vignettes[contentName]) {
			val = vignettes[contentName];
			vigName = val.digest;
			//document.title = vigName;
		} else {
			//document.title = contentName+" Reference";
		}
		*/
	});
	
	window.onresize = function () {
		var navMaxHeight = document.documentElement.clientHeight - 20;
		$('#nav').css('max-height', navMaxHeight);
	
		var objNavListMaxHeight = document.documentElement.clientHeight - 95;
		var tutsNavListMaxHeight = document.documentElement.clientHeight -70;
		var vigsNavListMaxHeight = document.documentElement.clientHeight - 70;
		var searchNavListMaxHeight = document.documentElement.clientHeight - 95;
		$('#tutsNav').css('max-height', tutsNavListMaxHeight);
		$('#searchNav').css('max-height', searchNavListMaxHeight);
		$('#vigsNav').css('max-height', vigsNavListMaxHeight);
		$('#catNav, #abcNav ').css('max-height', objNavListMaxHeight);
		
		
		var refMaxHeight = document.documentElement.clientHeight - $('#mainHeader').height() - 80;
		$('#reflist').css('max-height', refMaxHeight);

		var vigMaxHeight = document.documentElement.clientHeight - $('#mainHeader').height() - 100;
		$('#viglist').css('max-height', vigMaxHeight);

		if($('#tutButton').length) {
		var tutsMaxHeight = document.documentElement.clientHeight - $('#mainHeader').height() - 110;
		} else {
			var tutsMaxHeight = document.documentElement.clientHeight - $('#mainHeader').height() - 70;
		}			
		$('#tutlist').css('max-height', tutsMaxHeight);
		var contentHeight = document.documentElement.clientHeight - 20;
		$('#content').css('height', contentHeight);

	}
	
	//FIRST in a series of AJAX calls to gather data
	//----------------------------------------------
	function findInterfaces() {
		// where's the interfaces folder?
		$.ajax({
			url: '../../../Cycling \'74/interfaces/obj-qlookup.json',
			dataType: 'json',
			success: function () {
				INTERFACES_PREFIX = '../../../Cycling \'74/interfaces';
				gatherData();
			},
			error: function () {
				$.ajax({
					url: '../../../interfaces/obj-qlookup.json', // first fallback attempt (maybe we're using aliases in a dev build)
					dataType: 'json',
					success: function () {
						INTERFACES_PREFIX = '../../../interfaces';
						gatherData();
					},
					error: function () {
						$.ajax({
							url: '../../../../../Cycling \'74/interfaces/obj-qlookup.json', // second fallback attempt (translations...)
							dataType: 'json',
							success: function () {
								INTERFACES_PREFIX = '../../../../../Cycling \'74/interfaces';
								gatherData();
							},
							error: function () {
								INTERFACES_PREFIX = './interfaces'; // third fallback (maybe we're on the webserver)
								gatherData();
							}
						});
					}
				});
			}
		});
	}
	
	//				ALTERNATE_INTERFACES_PREFIX = '../../../../Cycling \'74/interfaces';

	
	
	function gatherDocs(val, obj) {
		$.each(val, function (key2, val2) {
			$.each(val2, function (key3, val3) {
				if (key3.match(/(.*?)\.(maxref|maxtut|maxvig)\.xml/)) {
					var name = RegExp.$1;
					var myCat = val3["category"];
					
					if(obj=="vignettes") {
						vignettes[name] = {
							"name":name,
							"file": key3,
							"module": key2,
							"category": myCat ? myCat : "Miscellaneous",
							"digest": val3["digest"] ? val3["digest"] : val3["name"],
							"order": val3["order"] ? val3["order"] : 10000
						};
					} else if (obj=="tutorials") {
						tutorials[name] = {
							"name": name,
							"file": key3,
							"module": key2,
							"category": myCat ? myCat : "Miscellaneous",
							"digest": val3["digest"] ? val3["digest"] : val3["name"],
							"order": val3["order"] ? val3["order"] : 10000
						};
					}
				}
			});
		});
		
	}
	
	// Replaces all instances of the given substring.
	String.prototype.replaceAll = function (strTarget, strSubString)  {
		var strText = this;
		var intIndexOfMatch = strText.indexOf(strTarget);
	
		// Keep looping while an instance of the target string
		// still exists in the string.
		while (intIndexOfMatch != -1) {
			// Relace out the current instance.
			strText = strText.replace(strTarget, strSubString)
	
			// Get the index of any next matching substring.
			intIndexOfMatch = strText.indexOf(strTarget);
		}
	
		// Return the updated string with ALL the target strings
		// replaced out with the new substring.
		return (strText);
	}
	
	function xmlGatherAttrs(xml, into) {
		// this is the way to do a non-recursive iteration of the elements
		// would be a good idea to do this for everything, in fact, but
		// it's only really a problem for attributes at the moment, which
		// may contain attr attrs

		var attributelist = $(xml).find('attributelist');

		if (attributelist) {
			//alert("got list:" + attributelist.length );
			attributelist.children('attribute').each( function() {
				// ATTENTION below 
				//if($(this).html()!="") {
				if (getContents( $(this) ) != "") {
					var attr = {
						"name": $(this).attr('name'),
						"desc": getBestDescription(this),
						"disc": getChildHtml($(this), 'discussion'),
						"type": $(this).attr('type'),
						"size": $(this).attr('size')
					}
					if (USING_ENUMLIST) {
						var enumlist = $(this).find("enum");
						if(enumlist.length) {
							attr.desc+="<ul class='enumlist'>";
						}
						$.each(enumlist, function (key, val) {
							attr.desc += "<li> " + ($(val).attr('name'));
							var enumDesc = getBestDescription(val);
							if(enumDesc) {
								attr.desc += ": "+enumDesc+"</li>"
							}
						});
						if(enumlist.length) {
							attr.desc+="</ul>";
						}
					}
					into.push(attr);
				}
			});
		} else {
		}
		//alert( getObjectSrc(into) );
		//end non-recursive attr iteration
	}
	
	
	genNavs = function () {
		$('#nav').prepend('<ul>' + '<li><a id="objTabLink" href="#objTab" class="tabLink">Objects</a></li>' +
				'<li><a id="tutsTabLink" href="#tutsTab" class="tabLink">Tutorials</a></li>' +
				'<li><a id="vigsTabLink" href="#vigsTab" class="tabLink">Vignettes</a></li>'+
				'<li><a id="searchTabLink" href="#searchTab" class="tabLink"><img src="images/search-icon.png" alt="search"></a></li></ul>');
	
		$('#nav').prepend('<a id="simplesearchlink" href=""></a>');
	
		//tab-ify
		//-------
		$("#nav").tabs().tabs('select', 3);
		
		//add searchbars
		$('#objTab').append('<div id="objNavTop">'+
							'<button id="listByCatButton" class="sortButton">List by Category</button>'+
							'<button id="listByAZButton" class="sortButton">List by A-Z</button></div>');
		$('#tutsTab').append('<div id="tutsNavTop"></div>');
		$('#vigsTab').append('<div id="vigsNavTop"></div>');
		$('#searchTab').append('<div id="searchNavTop"><input id="searchBox" class="searchbar" value="" type="search" autosave="cycling74" results=20 placeholder="Search..."  /><button id="searchButton" class="searchButton" /></div>');
		
	
		$("#searchButton").button({
			icons: {
				primary: "ui-icon-search"
			},
			text: false
		});
		
		
		//CSS changed by javascript to avoid having to alter the jQueryUI css file...
		//when the button() function is called, it adds the jQueryUI classes, but
		//after the class I ascribe, so the rules overwrite mine.  This is a work around.
		//-------------------------------------------------------------------------------
		$(".sortButton").button().css('display', 'inline').css('font-size', '.6em');
		$("#listByAZButton").css("padding-left", "1.4em").css("padding-right", "1.4em");
	
		//list button functions
		//---------------------
		$("#listByAZButton").click(function() {
			activeObjNavId = "#abcNav";
			$('#catNav').css('display', 'none');
			$('#abcNav').css('display', 'block');
		});
		
		$("#listByCatButton").click(function() {
			activeObjNavId = "#catNav";
			$('#catNav').css('display', 'block');
			$('#abcNav').css('display', 'none');
		});
	
	
		// keypress works in IE, but not keyup or keydown
		$('#searchBox').bind("keypress", function (event) {
			if(USING_DB) {			
				var mySearchString = escape($(this).val());
				var searchUrl = SEARCH_PREFIX+mySearchString;
				//lastTitle = document.title;
				$('#simplesearchlink').attr('href', searchUrl);
				//windows ie8 check for event.keyCode (jquery should fill the e.keyCode
				// for us, but we check here just in case.
				var keyCode = (window.event) ? event.which : event.keyCode;
				if (keyCode == 13)
					fireEvent($('#simplesearchlink'), 'click');
			} else {
				//do js search
				// since this is slow in IE, we only fire on 'enter'
				var keyCode = (window.event) ? event.which : event.keyCode;
				if (keyCode == 13)
				genSearchNav();
			}
		})
		
		//$('#simplesearchlink').click(function() {
			//myLog('simple search link: '+$(this).attr('href'));
		//});
	
		$('#searchButton').bind('click', function() {
			//lastTitle = document.title;
			if(USING_DB) {
				fireEvent($('#simplesearchlink'), 'click');
			} else {
				//do js search
				genSearchNav();
			}
/*			if(location.hash.indexOf("?")) {
				location.hash+=location.hash.substring(0, location.hash.indexOf("?"))+("?search_results="+SEARCH_RESULT_URL);
			} else {
				location.hash+=("?search_results="+SEARCH_RESULT_URL);
			} */
			//genSearchNav();
		});
	
		$('#objTab').append('<ul id="catNav" class="accordion navlist"></ul><ul id="abcNav" class="accordion navlist hidden"></ul>')
		$('#tutsTab').append('<ul id="tutsNav" class="accordion navlist"></ul>');
		$('#vigsTab').append('<ul id="vigsNav" class="accordion navlist"></ul>');
		$('#searchTab').append('<ul id="searchNav" class="accordion navlist">'+
									'<li class="searchNav"><a id="objSearchLink" href="#">Objects</a><ul id="objSearch" class="shown"></ul></li>' + 
									'<li class="searchNav"><a id="tutSearchLink" href="#">Tutorials</a><ul id="tutSearch" class="shown"></ul></li>' + 
									'<li class="searchNav"><a id="vigSearchLink" href="#">Vignettes</a><ul id="vigSearch" class="shown"></ul></li>'+
							   '</ul>');
		
	
		$("#searchButton").button({
			icons: {
				primary: "ui-icon-search"
			},
			text: false
		});
		$.each(topCats, function (key, val) {
			var myName = val.replace(/ /g, "_");
			val.replace("/", "-");
			$('#catNav').append('<li class="nav1"><a href="#">' + myName + '</a><ul id="' + myName + '_cat_navList"></ul></li>');
			$.each(subcats[val], function (key2, val2) {
				var selector = '#' + val.replace(/ /g, "-");
				selector.replace("/", "-");
				selector += '_cat_navList';
				$(selector).append('<li class="nav2"><a href="#">' + val2 + '</a><ul id="' + val.replace(/ /g, "-").replace("/", "-") + '_' + val2.replace(/ /g, "-").replace("/", "-") + '_cat_navList"></ul></li>');
			});
		});
	
		//place objects into appropriate lists
		
		$.each(objects, function (key, val) {
			if(!val.alias && !(val['private'] == 1) && !(val.subCats.indexOf('Unlisted') >= 0)) {
				if (val.digest == "TEXT_HERE") {
					val.digest = "...";
				} //strip broken listings
				//generate subcats list and add objects to appropriate category
				$.each(val.subCats, function (key2, val2) {
					var myListSelector = val.topCat.replace(/ /g, "-").replace("/", "-") + '_' + val2.replace(/ /g, "-").replace("/", "-") + '_cat_navList';
					myListSelector = '#' + $.escape(myListSelector);
					$(myListSelector).append('<li class="nav3"><a href="#' + val.link + '" onclick="genContent(\'' + val.link + '\')">' + val.name + '</a><div class="digest">' + val.digest + '</div></li>');
				});
				//generate abcNav
				//---------------
				//var myLetter = String(val.name[0]);
				var myLetter = val.name.charAt(0);
				var myCat ="";
				var myDisplayCat;
				var alphaExp = /^[a-zA-Z]/;
				if (myLetter.match(alphaExp)) {
					myCat = myLetter.charAt(0).toUpperCase();
				} else {
					myCat = "symbol";
				}
				if ($("#abcNav_" + myCat).length == 0) {
					$('#abcNav').append('<li class="abcNav"><a href="#">' + myCat + '</a><ul id="abcNav_' + myCat + '"></ul></li>');
				}
				$("#abcNav_" + myCat).append('<li class="nav3"><a href="#' + val.link + '" class="objLink"  onclick="genContent(\'' + val.link + '\')">' + val.name + '</a><div class="digest">' + val.digest + '</div></li>');
			}
		});

		
		//generate tutsNav
		//----------------
		
		$('#tutsNav').append('<li class="nav1"><a href="#">Max Tutorials</a><ul id="maxTutsList"></ul></li>');
		$('#tutsNav').append('<li class="nav1"><a href="#">MSP Tutorials</a><ul id="mspTutsList"></ul></li>');
		$('#tutsNav').append('<li class="nav1"><a href="#">Jitter Tutorials</a><ul id="jitTutsList"></ul></li>');
		
		var tutsArray = [];
		$.each(tutorials, function(key, val) {
			tutsArray.push(val);
		});
		
		tutsArray.sort(function(a, b) {
			return (parseInt(a.order) - parseInt(b.order));
		});
		
		$.each(tutsArray, function(key, val) {
			if (val.digest == "TEXT_HERE") {
				val.digest = "...";
			}
			var colonIndex = val.digest.indexOf(':');
			if(colonIndex>0) {
				var tutName = val.digest.substring(0, colonIndex);
				var tutDigest = val.digest.substring(colonIndex+2);
			} else {
				var tutName = val.digest;
				var tutDigest = "";
			}
			var dotIndex = val.file.indexOf('.');
			var fileName = val.file.substring(0, dotIndex);
			
			
			if (val.module=="max-tut") {
				var mySubcat = val.category.replace(/ /g, "_");
				var mySubcatId = mySubcat+'_maxTutSubcat';
				if($('#'+mySubcatId).length==0) {
					$('#maxTutsList').append('<li class="nav2"><a href="#">' + val.category + '</a><ul id="' + mySubcatId + '"</ul></li>');
				}
				//$('#'+mySubcatId).append('<li class="nav3"><a href="#' + fileName + '" class="objLink"  title="' + val.digest + '" onclick="genContent(\'' + val.file + '\', tutorials, '+val.module+')">' + tutName + '</a><div class="digest">' + tutDigest + '</div></li>');
				$('#'+mySubcatId).append('<li class="nav3"><a href="#' + fileName + '" class="objLink"  onclick="genContent(\''+val.file+'\')">' + tutName + '</a><div class="digest">' + tutDigest + '</div></li>');
			} else if (val.module=="msp-tut") {
				var mySubcat = val.category.replace(/ /g, "_");
				var mySubcatId = mySubcat+'_mspTutSubcat';
				if($('#'+mySubcatId).length==0) {
					$('#mspTutsList').append('<li class="nav2"><a href="#">' + val.category + '</a><ul id="' + mySubcatId + '"</ul></li>');
				}
				//$('#'+mySubcatId).append('<li class="nav3"><a href="#' + fileName + '" class="objLink"  title="' + val.digest + '" onclick="genContent(\'' + val.file + '\', tutorials, '+val.module+')">' + tutName + '</a><div class="digest">' + tutDigest + '</div></li>');
				$('#'+mySubcatId).append('<li class="nav3"><a href="#' + fileName + '" class="objLink"  onclick="genContent(\''+val.file+'\')">' + tutName + '</a><div class="digest">' + tutDigest + '</div></li>');
			} else if (val.module=="jit-tut") {
				var mySubcat = val.category.replace(/ /g, "_");
				var mySubcatId = mySubcat+'_jitTutSubcat';
				if($('#'+mySubcatId).length==0) {
					$('#jitTutsList').append('<li class="nav2"><a href="#">' + val.category + '</a><ul id="' + mySubcatId + '"</ul></li>');
				}
				var jitterPrefix = "";
				if(val.category=="Tutorials" && val.digest!="Jitter Tutorials" && val.digest!="Jitter Bibliography") {
					jitterPrefix = "Jitter ";
				}
				//$('#'+mySubcatId).append('<li class="nav3"><a href="#' + fileName + '" class="objLink"  title="' + val.digest + '" onclick="genContent(\'' + val.file + '\', tutorials, '+val.module+')">' + tutName + '</a><div class="digest">' + tutDigest + '</div></li>');
				$('#'+mySubcatId).append('<li class="nav3"><a href="#' + fileName + '" class="objLink"  onclick="genContent(\''+val.file+'\')">' + jitterPrefix+tutName + '</a><div class="digest">' + tutDigest + '</div></li>');
			}
		});
		
		
		//generate vigsNav
		//----------------
		//$('#vigsNav').append('<li class="nav1"><a href="#">Vignettes</a><ul id="maxTutsList"></ul></li>');
		$.each(vigCats, function(key, val) {
			$('#vigsNav').append('<li class="nav1"><a href="#">'+val+'</a><ul id="'+val.toLowerCase()+'VigsList"></ul></li>');
		});
		
		var vigArray = [];
		$.each(vignettes, function(key, val) {
			vigArray.push(val);
		});
		vigArray.sort(function(a, b) {
		
			 var A = a.digest.toLowerCase();
			 var B = b.digest.toLowerCase();
			 if (A < B){
				return -1;
			 }else if (A > B){
			   return  1;
			 }else{
			   return 0;
			 }
			
		});
		
		vigArray.sort(function(a, b) {
			return (parseInt(a.order) - parseInt(b.order));
		});
		
		
		$(vigArray).each(function(key, val) {
			if (val.digest == "TEXT_HERE") {
				val.digest = "...";
			}
			var dotIndex = val.file.indexOf('.');
			var fileName = val.file.substring(0, dotIndex);
			$('#'+val.module.toLowerCase()+'VigsList').append('<li class="vigNav"><a href="#' + fileName + '" class="objLink"  onclick="genContent(\''+fileName+'\')">' + val.digest + '</a></li>');
		});
							
		//add collapse/expand functionality
		$(".navTab").nestedAccordion({
			//initShow : "#current"
			standardExpansible: true,
			showSpeed: 150,
			hideSpeed: 150,
			activeLink : ((ONLINE == true) ? true : false),
			initShow: ".shown"
		});

		//zebra-ize
		$(".nav3:even, .tutNav:even, .vigNav:even").css("background-color", "#ffffff");
		$(".nav3:odd, .tutNav:odd, .vigNav:odd").css("background-color", "#f9f9fe");
		$(".nav3:first, .tutNav:first, .vigNav:first").css("border-top", "1px #cccccc");
		
	
		//add genContent function to all .nav3 a links
		$('.nav3 a').each(function (index) {
			$(this).bind('click', function () {
				genContent($(this).attr('href').substring(1));
			});
		});
		
		
		var navMaxHeight = document.documentElement.clientHeight - 20;
		$('#nav').css('max-height', navMaxHeight);
	
		var objNavListMaxHeight = document.documentElement.clientHeight - 95;
		var tutsNavListMaxHeight = document.documentElement.clientHeight -70;
		var searchNavListMaxHeight = document.documentElement.clientHeight -95;
		var vigsNavListMaxHeight = document.documentElement.clientHeight - 70;
		$('#tutsNav').css('max-height', tutsNavListMaxHeight);
		$('#searchNav').css('max-height', searchNavListMaxHeight);
		$('#vigsNav').css('max-height', vigsNavListMaxHeight);
		$('#catNav, #abcNav ').css('max-height', objNavListMaxHeight);
				
	}
	
	function subCatMatch(searchTerm, object) {
		mySubcats = object.subCats;
		searchTerm = searchTerm.toLowerCase();
		var foundIt = false;
		$.each(mySubcats, function(key, val) {
			if(val.toLowerCase().indexOf(searchTerm) >= 0) {
				foundIt = true;
				return true;
			}
		});
		return foundIt;
	}
	
	function genSearchNav() {
	
		//myLog('genSearchNav() entered');
		//get search string
		var searchString = $('#searchBox').val().toLowerCase().trim();
		//myLog('searchString: '+searchString);
	
		//show search nav
		$('#searchNav').css('display', 'block');
		
		//list elements
		var searchResultLists = [$('#objSearch'), $('#tutSearch'), $('#vigSearch')];
		
		var jsSearch = [];
		
		//reset count			
		var nObs = 0;
		var nTuts = 0;
		var nVigs = 0;
		
		$(searchResultLists).each(function(key, val) {
			
			val.empty();		//remove old search results (html)
			
			var myID = (val.attr('id'));
			var myCount;
			
			var nameMatches = [];
			var Matches = [];
			var nameMatches = [];
			
			
			switch (myID) {
				case 'objSearch':
					var nameComplete = [];
					var nameSearch = [];
					var digestSearch = [];
					var digestComplete = [];
					var catSearch = [];
					var searches = [nameComplete, nameSearch, digestComplete, digestSearch, catSearch];

					$(objects).each(function(objIndex, obj) {
						if(!obj.alias && !obj.private) {  //don't show aliased stuff or private stuff
							
							var myName = obj.name.toLowerCase();
							var myLink = obj.link.toLowerCase();
							var myDigest = obj.digest.toLowerCase();
							
							var nameIndex = myName.indexOf(searchString);
							var linkIndex = myLink.indexOf(searchString);
							
							/*
							//handle quotes
							//maybe for another day...
							//------------------------

							///quoteIndex = searchString.indexOf('\"');

							var searchTerms = [];
							
							while(true) {
								quoteIndex = searchString.indexOf('\"');
								if(quoteIndex<0) {
									break;
								}
								quoteIndex2 = searchString.indexOf('\"', quoteIndex+1);
								if(quoteIndex2>-1) {
									//extract quoted string, add to searchTerms
									var quotedString = searchString.substring(quoteIndex, quoteIndex2);
									//and remove it from the searchString
									searchString.replace(quotedString, '');
									//and strip quotes
									quotedString = quotedString.replace('"', '');
									//and add it to the searchTerms array
									searchTerms.push(quotedString);
									break;
								} else {
									//remove the stray quote
									searchString = searchString.replace('\"', '');
									break;
								}
							}
							
							
							//end handle quotes
							//-----------------
							*/
							
							
							var foundMatchInDigest = false;
							
							//searching for each word in search string
							searchTerms = searchString.split(' ');
							$.each(searchTerms, function(key, val) {
								var myDigestIndex = myDigest.indexOf(val.trim());
								if(myDigestIndex>-1) {
									foundMatchInDigest = true;
								}
							});
							
							var digestIndex = myDigest.indexOf(searchString);
							var sLength = searchString.length;
							
							var catString = obj.subCats.join()+', '+obj.topCat;
							catString = catString.toLowerCase();
							var catsIndex = catString.indexOf(searchString);
							
							
							if(nameIndex===0 || linkIndex===0) {
								nameComplete.push(myName);
							} else if (nameIndex>0 || linkIndex>0 && sLength>1) { //only do other searches if search string is longer than one char
								nameSearch.push(myName);
							} else if (sLength>1 && digestIndex>-1) {
								digestComplete.push(myName);
							} else if (foundMatchInDigest && sLength>1) {
								digestSearch.push(myName);
							} else if (catsIndex>-1 && sLength>1) {
								catSearch.push(myName);
							}
						}
					});
					
					$(searches).each(function(key, search) {
						$(search).sort();
						$(search).each(function(i, objName) {
							var myObj = objectsLookup[nameReplace(objName)];
							if(myObj) {
								jsSearch.push(objName);
								$("#objSearch").append('<li class="nav3 searchresult " id="navLink_'+objName+'"><a href="#' + myObj.link + '" class="objLink">' + myObj.name + '</a><div class="digest">' + myObj.digest + '</div></li>');
								nObs++;
							}
						});
					});
					break;
				case 'tutSearch':
					var nameComplete = [];
					var nameSearch = [];
					var digestComplete = [];
					var digestSearch = [];
					var catSearch = [];
					var searches = [nameComplete, nameSearch, digestComplete, digestSearch, catSearch];

					
					$.each(tutorials, function(tutKey, tut) {
						
						
						var myName = tutKey.toLowerCase();
						var myLink = tut.file.toLowerCase();
						var myDigest = tut.digest.toLowerCase();
						var myCat = tut.category.toLowerCase();
						
						var nameIndex = myName.indexOf(searchString);
						var linkIndex = myLink.indexOf(searchString);
						var digestIndex = myDigest.indexOf(searchString);
						var catString = myCat+', '+tut.module.substring(0,2);
						catString = catString.toLowerCase();
						var catsIndex = catString.indexOf(searchString);
						
						
						if(nameIndex===0 || linkIndex===0) {
							nameComplete.push(myName);
						} else if (nameIndex>0 || linkIndex>0 && searchString.length>1) { //only do other searches if search string is longer than one char
							nameSearch.push(myName);
						} else if (digestIndex>-1 && searchString.length>1) {
							digestSearch.push(myName);
						} else if (catsIndex>-1 && searchString.length>1) {
							catSearch.push(myName);
						}
						
					});
					
					
					
					$(searches).each(function(key, search) {
						$(search).sort();
						$(search).each(function(i, tutName) {
							var myTut = tutorials[tutName];
							if(myTut) {
								var myDigest = myTut.digest;
								var colonIndex = myTut.digest.indexOf(':');
								var tutDisplayTitle = myDigest.substring(0, colonIndex);
								var tutDisplayDigest = myDigest.substring(colonIndex+1);
								if(myTut.module=='jit-tut' && tutDisplayTitle!="Jitter Tutorials" && tutDisplayTitle!="Jitter Bibliography") {
									tutDisplayTitle = 'Jitter '+tutDisplayTitle;
								}
																
								jsSearch.push(tutName);

								$("#tutSearch").append('<li class="nav3 searchresult" id="navLink_'+tutName+'"><a href="#' + tutName + '" class="objLink" onclick="genContent(\'' + myTut.file + '\', \'tutorials\', \''+myTut.module+'\')">' + tutDisplayTitle + '</a><div class="digest">' + tutDisplayDigest + '</div></li>');
								nTuts++;
							}
						});
					});
					

					break;
				case 'vigSearch':
					var nameComplete = [];
					var nameSearch = [];
					var digestCompete = [];
					var digestSearch = [];
					var catSearch = [];
					var searches = [nameComplete, nameSearch, digestComplete, digestSearch, catSearch];

					//go through vignettes json
					
					$.each(vignettes, function(vigKey, vig) {
						var myName = vigKey.toLowerCase();
						var myLink = vig.file.toLowerCase();
						var myDigest = vig.digest.toLowerCase();
						var myCat = vig.category.toLowerCase();
						
						var nameIndex = myName.indexOf(searchString);
						var linkIndex = myLink.indexOf(searchString);
						var digestIndex = myDigest.indexOf(searchString);
						var catString = myCat+', '+vig.module.substring(0,2);
						catString = catString.toLowerCase();
						var catsIndex = catString.indexOf(searchString);
						
						if (myCat === "unlisted") return true; // continue
						if(nameIndex===0 || linkIndex===0) {
							nameComplete.push(myName);
						} else if (nameIndex>0 || linkIndex>0 && searchString.length>1) { //only do other searches if search string is longer than one char
							nameSearch.push(myName);
						} else if (digestIndex>-1 && searchString.length>1) {
							digestSearch.push(myName);
						} else if (catsIndex>-1 && searchString.length>1) {
							catSearch.push(myName);
						}
					});
					
					$(searches).each(function(key, search) {
						$(search).sort();
						$(search).each(function(i, vigName) {
							var myVig = vignettes[vigName];
							if(myVig) {
								var myDigest = myVig.digest;
								//var colonIndex = myTut.digest.indexOf(':');
								//var tutDisplayTitle = myDigest.substring(0, colonIndex);
								//var tutDisplayDigest = myDigest.substring(colonIndex+1);
								//if(myTut.module=='jit-tut') {
								//	tutDisplayTitle = 'Jitter '+tutDisplayTitle;
								//}
								jsSearch.push(vigName);
								$("#vigSearch").append('<li class="nav3 searchresult"><a href="#' + vigName + '" class="objLink" onclick="genContent(\'' + myVig.file + '\', \'vignettes\', \''+myVig.module+'\')">' + myVig.digest + '</a></li>');
								nVigs++;
							}
						});
					});
					
					
					break;
				default:
					break;
			}				
		});
		
		
		//show number of search results
		//$('#objSearchLink').html('<span>Objects ('+nObs+' results)</span>');
		//$('#tutSearchLink').html('<span>Tutorials ('+nTuts+' results)</span>');
		//$('#vigSearchLink').html('<span>Vignettes ('+nVigs+' results)</span>');
		
		
		
		if(USING_DB) { //use Max indexed DB (USING_DB==true)
			var mySearchResultLocationIndex = location.hash.indexOf("=");
			var ampIndex = location.hash.indexOf("&");
			if(ampIndex) {
				var mySearchResultUrl = location.hash.substring(mySearchResultLocationIndex+1, ampIndex);
			} else {
				var mySearchResultUrl = location.hash.substring(mySearchResultLocationIndex+1);
			}
			//myLog("search result url: "+mySearchResultUrl);

			if(DEBUGGING) {
				var params = {};
				var hashIndex = location.href.indexOf('#');
				var qIndex = location.href.indexOf('?')+1;
				var paramstring = location.href.substring(qIndex);
				paramArray = paramstring.split('&');
				for(var i=0;i<paramArray.length;i++) {
					var pair = paramArray[i];
					var eIndex = pair.indexOf('=');
					var key = pair.substring(0, eIndex);
					var val = pair.substring(eIndex+1);
					params[key]=val;
				}
			}
			
			
			$.ajax({
				url: mySearchResultUrl,
				dataType: "json",
				success: function (json) {
					//myLog("myserachloc: "+params['search_results']);
					//myLog("myquery: "+params['query']);
					//myLog(location.href);
					//$('#searchNav').css('display', 'block');
					//$('#objSearch').empty();
					//$('#tutSearch').empty();
					//$('#vigSearch').empty();
	
					//var nObs = 0;
					//var nTuts = 0;
					//var nVigs = 0;
					if (json.results.reffile != null) {
						//myLog('adding reffiles from json');
						var nParsed = 0;
						var nReffiles = json.results.reffile.length;
						$.each(json.results.reffile, function(key, val) {
							nParsed++;
							var dotIndex = val.name.indexOf('.maxref.xml');
							var myObjName = val.name.substring(0, dotIndex).toLowerCase();
							//myLog('parsing '+myObjName);
							if(objectsLookup[myObjName]) {
								var myObj = objectsLookup[myObjName];
								//myLog('found '+myObj.name+' in objectsLookup table');
								//myLog(jsSearch);
								var isDup = false;
								for(dup in jsSearch) {
									//myLog(dup+' : '+myObj.name);
									if(myObj.name==jsSearch[dup]) {
										isDup=true;
									}
								}
								if(!isDup) {
									//myLog('adding '+myObjName+' to search results');
									if(myObj!=null) {
										if(!myObj.alias && !(myObj['private'] == 1) && !(myObj.subCats.indexOf('Unlisted') >= 0)) {
											
											nObs++;
											$("#objSearch").append('<li class="nav3 searchresult" id="navLink_'+myObjName+'"><a href="#' + myObj.link + '" class="objLink">' + myObj.name + '</a><div class="digest">' + myObj.digest + '</div></li>');
											$('#objSearchLink').html('<span>Objects ('+nObs+' results)</span>');
											
										} else {
											//myLog(myObj.name+' is unlisted');
										}
									} else if (myObjName == 'parameters') {
										//myLog("parsing parameters");
									}
									//myLog('done parsing: '+myObjName);
								} else {
									//myLog('already have a '+myObjName);
								}
							}
							//myLog('done parsing '+myObjName+' ('+nParsed+'/'+nReffiles+')');
						});
						//myLog('done parsing reffiles from json');
					}
	
					if (json.results.tutorial != null) {
						//myLog('adding tutorials from json');
						$.each(json.results.tutorial, function(key, val) {
							var myName = val.name;
							//myLog("search result: tutorial - "+myName);
							var myTut = tutorials[myName];
							var isDup = false;
							for(dup in jsSearch) {
								//myLog(dup+' : '+myName);
								if(myName==jsSearch[dup]) {
									isDup=true;
								}
							}
							if(!isDup) {
								var colonIndex = myTut.digest.indexOf(':');
								if(colonIndex>0) {
									var tutName = myTut.digest.substring(0, colonIndex);
									var tutDigest = myTut.digest.substring(colonIndex+2);
								} else {
									var tutName = myTut.digest;
									var tutDigest = "";
								}
								var dotIndex = myTut.file.indexOf('.');
								var fileName = myTut.file.substring(0, dotIndex);
	
								if($('#navLink_'+myName).length==0) {
									$("#tutSearch").append('<li class="nav3 searchresult" id="navLink_'+myName+'"><a href="#' + myName + '" class="objLink"  onclick="genContent(\'' + myTut.file + '\', tutorials, '+myTut.module+')">' + tutName + '</a><div class="digest">' + tutDigest + '</div></li>');
									nTuts++;
									$('#tutSearchLink').html('<span>Tutorials ('+nTuts+' results)</span>');
								}
							}
							
						});
						//$('#nav').append(nTuts);
					} else {
						//myLog('found no tutorials');
					}
					//myLog('done adding reffiles from json');
					if (json.results.vignette != null) {
						//myLog('adding vignettes from json');
						$.each(json.results.vignette, function(key, val) {
							var myName = val.name;
							//myLog("search result: vignette - "+myName);
							var myVig = vignettes[myName];
							var isDup = false;
							for(dup in jsSearch) {
								//myLog(dup+' : '+myName);
								if(myName==jsSearch[dup]) {
									isDup=true;
								}
							}
							if(!isDup) {
								nVigs++;
								$("#vigSearch").append('<li class="nav3 searchresult" id="navLink_'+myName+'"><a href="#' + myName + '" class="objLink"  onclick="genContent(\'' + myVig.file + '\', "vignettes", '+myVig.module+')">' + myVig.digest + '</a></li>');
								$('#vigSearchLink').html('<span>Vignettes ('+nVigs+' results)</span>');
							}
						});
					}
					


				},
				error: function (obj, t) {
					//myLog("error loading data with location: "+mySearchResultUrl+" <br \>"+obj[0]+': '+ t);
				}
			});
			$('#objSearchLink').html('<span>Objects ('+nObs+' results)</span>');
			$('#tutSearchLink').html('<span>Tutorials ('+nTuts+' results)</span>');
			$('#vigSearchLink').html('<span>Vignettes ('+nVigs+' results)</span>');
			//myLog("nObs: "+nObs+", nTuts: "+nTuts+", nVigs: "+nVigs);
		}
		//Presentation Stuff
		//------------------
		$('.searchNav').css('display', 'block');
	
		/*slowed things down a LOT and was completely unnecessary
		$(".navTab").nestedAccordion({
			//initShow : "#current"
			standardExpansible: true,
			showSpeed: 150,
			hideSpeed: 150,
			initShow: ".shown"
		});
		*/
	
		//add genContent function to all .nav3 a links - make links work!
		$('.nav3 a').each(function (index) {
			$(this).bind('click', function () {
				genContent($(this).attr('href').substring(1));
			});
		});
		//zebra-ize
		$(".searchresult:even").css("background-color", "#ffffff");
		$(".searchresult:odd").css("background-color", "#f9f9fe");
		$(".searchNav").css("border-top", "1px #cccccc");
		$(".searchNav:first").css("border-top", "1px solid #ddd");
	
		var searchNavListMaxHeight = document.documentElement.clientHeight - 95;
		$('#searchNav').css('max-height', searchNavListMaxHeight);
		//End presentation stuff
		//----------------------
	}
	
	myScrollTo = function (container, anchor) {
		//alert("scroll "+container+" to "+anchor);
		//first try identify element by id, then by name
		var myAnchor = $('#'+anchor);
		if(myAnchor.length==0) {
			myAnchor = $('*[name='+anchor+']');
		}
		var myContainer = $('#'+container);
		if(!myContainer) {
			$('*[name='+container+']');
		}
		if (myAnchor && myContainer) {
			myContainer.scrollTo(myAnchor, SCROLL_SPEED, {easing:'easeInOutExpo'});	
		}
	}
	
	myLog = function (string) {
		if(DEBUGGING) {
			$('#debug').prepend(string+'<br \>');
		}
	}
	
	genContent = function (objName, type, module) {
		myLog('url: '+location.href);
		//get parameters from url;
		var params = {};
		var hashIndex = location.href.indexOf('#');
		var qIndex = location.href.indexOf('?')+1;
		var paramstring = location.href.substring(qIndex);
		paramArray = paramstring.split('&');
		for(var i=0;i<paramArray.length;i++) {
			var pair = paramArray[i];
			var eIndex = pair.indexOf('=');
			var key = pair.substring(0, eIndex);
			var val = pair.substring(eIndex+1);
			params[key]=val;
		}
		
		var myCatFolder;
		objName = nameReplace(objName).toLowerCase();
		lastTite = objName;
		if (type) {
		}
		if (!objName) {
			objName = "docintro";
		}
		var obj = objectsLookup[objName];
		if (!obj) {
			// look up in the vignettes and tuturials dicts
			if (objName in tutorials) {
				document.title = tutorials[objName]['digest'];
				type = "tutorials";
				myCatFolder = tutorials[objName]['module'] + "/";
				genTutorial(objName, tutorials[objName], myCatFolder);
			} else if (objName in vignettes) {

				document.title = vignettes[objName]['digest'];
				type = "vignettes";
				myCatFolder = vignettes[objName]['module'] + "/";
				genVignette(objName, vignettes[objName], myCatFolder);
			} else {
				return;
			}
		} else {
			document.title = objName+" Reference";
			type = "refpages";
			myCatFolder = obj.module + "-ref/";
			genRefpage(objName, obj, myCatFolder);
		}
		
		
		
		var contentHeight = document.documentElement.clientHeight * .98-5;	
		$('#content').css('height', contentHeight);
		
		
		//keyboard shortcuts
		$(window).keydown(function (e){
			if(e.metaKey) {
				if(e.keyCode==40) {
					if(activePanel=='content') {
						$('.contentScroll').scrollTo('100000px', 0, {easing:'easeInOutExpo', axis:'y'});
					} else {
						$('.navlist').scrollTo('10000px', 0, {easing:'easeInOutExpo', axis:'y'});
					}
				} else if(e.keyCode==38) {
					if(activePanel=='content') {
						$('.contentScroll').scrollTo('0px', 0, {easing:'easeInOutExpo', axis:'y'});
					} else {
						$('.navlist').scrollTo('0px', 0, {easing:'easeInOutExpo', axis:'y'});
					}
				}
			}
		});
		
		$('#content').mouseover(function() {
			activePanel = 'content';
		});
		
		$('#nav').mouseover(function() {
			activePanel = 'nav';
		});
		
		
		
	}
	
	genTutorial = function (objName, obj, myCatFolder) {
		$.ajax({
			url: URL_PREFIX + 'tutorials/' + myCatFolder + objName + '.maxtut.xml',
			dataType: 'text',
			success: function (xml) {
				var html = "";
				var openfileurl = "";
	
				xml = convertTags(xml);
				xml = convertTutorialTags(xml);
				
				
				xml = xml.replace(new RegExp( "<o>", "g" ), '<a class="o">');
				xml = xml.replace(new RegExp( "</o>", "g" ), '</a>');
	
				var xmlObj = $.parseXML(xml);			
				$('a[href="#mainTab"]').html("Documentation");
				$('#mainTab').html('<div id="mainHeader"></div>');
				if(obj['digest']) {
					$('#mainHeader').append('<h1 id="pageTitle" class="objTitle">' + obj['digest'] + '</h1>');
				} else {
					$('#mainHeader').append('<h1 id="pageTitle" class="objTitle">' + objName + '</h1>');
				}
				$('#pageTitle').prepend('<a class="homelink" href="#docintro"><img class="titleLogo" src="images/max6.png" alt="home"/></a>');
				$('#mainTab').append('<div id="tutlist" class="contentScroll"></div>');
				
				
				
				//$(xml).children().each(function(key, val) {
				$(xmlObj).find(":first").children().each(function(key, val) {
					//alert(val.tagName.toLowerCase());
					switch(val.tagName.toLowerCase()) {
						
						case "div":
							var tmp = getContents( val );
							tmp = tmp.replace(new RegExp( "<o", "g" ), '<a class="o"');
							tmp = tmp.replace(new RegExp( "</o>", "g" ), '</a>');
							$('#tutlist').append(tmp);
							break;
						case "body":
							var myCode = getContents($(val));
							$('#tutlist').append('<div class="tutDiv">'+myCode+'</div>');
							break;
						case "openfile":
							if(!ONLINE) {
								openfileurl = 'max:openfilefront/' + $(val).attr('name') + '/' + $(val).attr('patch');
								var onclick = "document.location.href='" +openfileurl + "';";
								$('<button id="tutButton" class="linkbutton" onclick="'+onclick+'">Open the tutorial patcher</button>').insertBefore($('#tutlist'));
							}
							break;
						case "header":
							break;
						case "openpatchlink":
							// it would be nice to have a 2nd link in the text, but I couldn't get this to work for some reason
							//$('#tutlist').append('<a href="' + openfileurl + '">Open the tutorial.</a>');
							break;
						case "subhead":
							//var myTitle = $(this).html();
							var myTitle = getContents( $(this) );
							$('#tutlist').append('<h2 class="tutTitle">'+myTitle+'</h2>');
							break;
						case "subsubhead":
							//var myTitle = $(this).html();
							var myTitle = getContents( $(this) );
							$('#tutlist').append('<h2 class="tutTitle subsubhead">'+myTitle+'</h2>');
							break;
						case "h2":
							$('#tutlist').append($(this));
						case "summary":
							//var myTitle = $(this).html();
							var myTitle = getContents( $(this) );
							$('#tutlist').append('<h2 class="tutTitle">'+myTitle+'</h2>');
							break;
						case "seealsolist":
							var myTitle = "See Also";
							var seealsos = $(this).children();
							$('#tutlist').append('<h2 class="tutTitle">'+myTitle+'</h2>');
							$('#tutlist').append('<table id="seealsos"></table>');
							$('#seealsos').append('<tr><td class="refData argName refHeader">Name</td><td class="refData argDesc refHeader">Description</td></tr>');
							$.each(seealsos, function(key, val) {
								var myName = $(this).attr('name');
								//var myDesc = $(this).html().trim();
								var myDesc = $.trim(getContents( $(this) ));
								$('#seealsos').append('<tr><td class="refData argName"><a class="o" href="#"'+myName+'">'+myName+'</a></td><td class="refData seealsoDesc">'+myDesc+'</td></tr>');										
							});
							//zebra-ize seealsos
							$("#seealsos tr:even").css("background-color", "#ffffff");
							$("#seealsos tr:odd").css("background-color", "#f7f7fe");
							$("#seealsos tr:last").css("border", "none");
							break;
						case "i":
							//$('#tutlist').append('<span class="italic">'+$(this).html()+'</span>');
							break;
						case "o":
							//var myObj = $(this).html().trim();
							var myObj = $.trim( getContents($(val)));
							$('#tutlist').append('<a class="o" href="#' + myObj + '">' + myObj + '</a>');
							break;
						case "illustration":

							if(myCatFolder=="msp-tut/") {
								var mySrc = '../tutorials/'+myCatFolder+$(this).find('img').attr('src');
							} else {
								var mySrc = '../tutorials/'+myCatFolder+$(this).find('example').attr('name');
							}
							if($(this).find('example')[0] || $(this).find('img')[0]) {
								$('#tutlist').append('<table><tr><td><img class="illustration" src="'+mySrc+'"></td></tr><tr><td class="tutCaption" alt=""></td></tr></table>');

							} else {
								$('#tutlist').append('<div class="textIllContainer"><div class="textIllustration" alt="">'+$(this).text()+'</div></div>');
							}
							break;
						case "bullet":
							//old, "try it" style
							//$('#tutlist').append('<div class="tryIt"><p><span class="italic">Try it in the tutorial patch:</span></p><p>'+$(this).html()+'</p></div>');
							//new "bullet" style
							//$('#tutlist').append('<ul><li>'+$(this).html()+'</li></ul>');
							$('#tutlist').append('<ul><li>'+ getContents( $(this))+'</li></ul>');
							break;
						case "ul":
							$('#tutlist').append('<ul>'+getContents( $(this))+'</ul>'); // TODO
							break;
						case "techdetail": // TODO
							break;
						case "codeexample": // TODO
							//var myCode = $(val).html();
							var myCode = getContents($(val));
							$('#tutlist').append('<pre><code><span class="code">'+myCode+'</pre></code></span>');
							break;
						case "code":
							//var myCode = $(val).html();
							var myCode = getContents($(val));
							$('#tutlist').append('<pre><code><span class="code">'+myCode+'</pre></code></span>');
							break;
						default:
							break;
					}
				});
				
				
				//replace ltp_standin with <p
				var s = $('#mainTab').html();
				s = s.replace(new RegExp("LTP_STANDIN", "g"), '&lt;p');
				$('#mainTab').html(s);
				
				
				$("#tutButton").button({
					icons: {
						secondary: "ui-icon-newwin"
					}
				}).bind('click', function () {
					//open the patch
				});
				
				$('.patchlink').attr('href', openfileurl);
				
				//$('.tut_openpatchlink').parent().css('display', 'none');
				
				/*
				var refMaxHeight = document.documentElement.clientHeight * .88 - 80;
				var vigMaxHeight = document.documentElement.clientHeight * .88 - 30;
	
				$('#tutlist').css('max-height', refMaxHeight);
				$('#viglist').css('max-height', vigMaxHeight);
				*/
					
				// ATTENTION: We have problems here.  We can't do $('o').each. IE7 doesn't like this.
				// THis make sense since 'o' isnt an html element.  So, what I do is regex it in the xml
				// string above and search on 'a.o' here.  However, we seem to have some <o> data that really
				// isn't <o> ..such as "Max Basic Tutorial 11:Procedural drawing"
				//$('o').each(function () {
				$('a.o').each(function () {
					//var myObj = ($(this).html().trim());
					var myObj = $.trim( getContents( $(this) ) );
					//$(this).replaceWith('<a class="o" href="#' + myObj + '">' + myObj + '</a>');
					$(this).attr('href', '#' + myObj );
				});
					
				if($('#tutButton').length) {
				var tutsMaxHeight = document.documentElement.clientHeight - $('#mainHeader').height() - 110;
				} else {
					var tutsMaxHeight = document.documentElement.clientHeight - $('#mainHeader').height() - 70;
				}			
				$('#tutlist').css('max-height', tutsMaxHeight);
				
				/*
				//set title
				var qIndex = location.hash.indexOf("?");
				if(qIndex != -1) {
					var contentName = nameReplace(location.hash.substring(1, qIndex));
				} else {
					var contentName = nameReplace(location.hash.substring(1));
				}
				val = tutorials[contentName];
				tutName = val.digest;
				//document.title = tutName;
				*/
				
				//add anchorlink functionality
				$('a.anchorlink').each(function (key, val) {
					var myAnchor = $(val).attr('href');
					if(myAnchor[0]=="#") {
						myAnchor = myAnchor.substring(1);
					}
					$(val).attr('href', 'javascript:myScrollTo("tutlist", "'+myAnchor+'")');
				});
				
				
				
				
				//scroll to anchor if specified in url
				if(params.anchor) {
					myScrollTo('tutlist', params.anchor);
				}
		
			}
		});
	}
		

	function convertXmlToVig(key, val) {
		switch(val.tagName.toLowerCase()) {
			
			case "digest":
				$('#viglist').append('<div class="bodytext">' + getContents(val) + '</div><br />');
				break;
			case "description":
				$('#viglist').append('<div class="bodytext">' + getContents(val) + '</div><br />');
				break;
			case "intro":
				var myIntro = getContents(val);
				myIntro = myIntro.replace(/'/g, "\"");
				$('#viglist').append('<h2 id="Introduction" class="vigTitle2">Introduction</h2>');
				$('#viglist').append('<div class="bodytext">'+myIntro+'</div><br>');
				break;
			case "chapter":
				var myChapter = getContents(val);
				myChapter = myChapter.replace(/'/g, "\"");
				myName = $(val).attr('name');
				if(myName=="Object Model Overview") {
					myChapter = myChapter.replace(/<table>[^"]*<\/table>/g, '');
				}
				$('#viglist').append('<h2 id="'+myName+'" class="vigTitle2">'+myName+'</h2>');
				$('#viglist').append(myChapter);
				break;
			case "liveapi_object":
				
				var myObj = getContents(val);
				myObj = myObj.replace(/'/g, "\"");
				myName = $(val).attr('name');
				myId = myName.replace(/\./g, '-');
				$('#viglist').append('<h2 id="'+myId+'" name="'+myName+'"class="vigTitle2 lomTitle">'+myName+'</h2>');
				myObj= lomParseChildren(myObj);
				$('#viglist').append(myObj);
				break;
			case "bodytext":
				if($(this).attr('id')=='docintroaccordion') {
					$('#viglist').append('<div id="docintroaccordion" class="vigBody">'+getContents( val )+'</div><br>');
				} else {
					$('#viglist').append('<div class="bodytext">'+getContents(val)+'</div><br>');
				}
				break;
			case "header1":
				var myTitle = getContents(val);
				$('#viglist').append('<h2 class="vigTitle1">'+myTitle+'</h2>');
				break;
			case "header2":
				var myTitle = getContents(val);
				$('#viglist').append('<h2 class="vigTitle2">'+myTitle+'</h2>');
				break;
			case "header3":
				var myTitle = getContents(val);
				$('#viglist').append('<h3 class="vigTitle">'+myTitle+'</h3>');
				break;
			case "bluebox":
				$('#viglist').append('<div class="vigBlueBox">'+getContents(val)+'</div><br>');
				break;
			case "ul":
				$('#viglist').append('<ul>'+getContents(val)+'</ul>');
				break;
			case "a":
				$(this).replaceWith('<a name="'+$(val).attr('name')+'">'+getContents(val)+'</a>');
				break;
			case "i":
				//$('#tutlist').append('<span class="italic">'+$(this).html()+'</span>');
				break;
			case "o":
				var myObj = $.trim( getContents(val));
				$('#viglist').append('<a class="o" href="#' + myObj + '">' + myObj + '</a>');
				break;
			case "villustration":
				var myClass = $(val).attr('class');
				var mySrc = '../vignettes/'+myCatFolder+$(val).attr('src');
				$('#vigist').append('<table><tr><td><img class="illustration '+myClass+'" src="'+mySrc+'"/></td></tr><tr><td class="tutCaption"></td></tr></table>');
				break;
			case "code":
				var myCode = getContents(val);
				$('#viglist').append('<pre><code><span class="code">'+myCode+'</pre></code></span>');
				break;
			case "table":
				if($(val).attr('id')=="thesaurus") {
					$('#viglist').append('<table id="thesaurus">'+getContents(val)+'</table>');
				}
				break;
			case "div":
				if($(val).attr('id')=="docintroaccordion") {
					
					var myHTML = getContents(val);
					myHTML = myHTML.replace(/<header2>/g, "<h2 class='vigTitle3'>");
					myHTML = myHTML.replace(/<\/header2>/g, "</h2>");

					$('#viglist').append('<div id="docintroaccordion">'+myHTML+'</div>');
				} else {
				
					var myHTML = getContents(val);
					myHTML = myHTML.replace(/<header2>/g, "<h2 class='vigTitle3'>");
					myHTML = myHTML.replace(/<\/header2>/g, "</h2>");

					$('#viglist').append('<div>'+myHTML+'</div>');
				}
				break;
			default:
				break;
		}
	}
	
	lomParseChildren = function(objString) {
		objString = objString.replace(/<description>/g, '<div class="bodytext">');
		objString = objString.replace(/<\/description>/g, '</div>');
		objString = objString.replace(/<metadatalist>/g, '<div class="metadatalist">');
		objString = objString.replace(/<\/metadatalist>/g, '</div>');
		objString = objString.replace(/<inletlist>/g, '<div class="inletlist">');
		objString = objString.replace(/<\/inletlist>/g, '</div>');
		objString = objString.replace(/<outletlist>/g, '<div class="outletlist">');
		objString = objString.replace(/<\/outletlist>/g, '</div>');
		objString = objString.replace(/<attributelist>/g, '<div class="attributelist">');
		objString = objString.replace(/<\/attributelist>/g, '</div>');
		objString = objString.replace(/<methodlist>/g, '<div class="methodlist">');
		objString = objString.replace(/<\/methodlist>/g, '</div>');
		objString = objString.replace(/<liveapi_path path="/g, '<table class="lomTable"><tr><td class="lomHeader">Canonical Path</td></tr><tr><td class="lomName">');
		objString = objString.replace(/" ><\/liveapi_path>/g, '</td></tr></table>');
		objString = objString.replace(/"><\/liveapi_path>/g, '</td></tr></table>');


		
		//parse children table
		//remove if empty:
		objString = objString.replace(/<liveapi_children>[ ]*<\/liveapi_children>/g, '');
		objString = objString.replace(/<liveapi_children>/g, '<table class="lomTable"><tr><td class="lomHeader">Children</td><td class="lomHeader">&nbsp;</td><td class="lomHeader">&nbsp;</td><td class="lomHeader">&nbsp;</td></tr><tr><td class="italic lomKey lomName">Name</td><td class="italic lomKey lomType">Type</td><td class="italic lomKey lomAccess">Access</td><td class="italic lomKey lomDesc">Description</td></tr>');
		objString = objString.replace(/<\/liveapi_children>/g, '</table>');
		objString = objString.replace(/<liveapi_child[ ]+name="([^"]*)"[ ]+gso="([^"]*)"[ ]+type="([^"]*)"[ ]*>/g, function(wholematch, myName, myAccess, myType) {
			var accessArray = []
			var myAccessChars = myAccess.split(''); // Array-like character access is only in ecmascript 5, IE 7 doesn't like it, so we force an array here
			for(var i=0; i<myAccessChars.length; i++) {
				switch (myAccessChars[i]) {
					case "g":
						accessArray.push('get');
						break;
					case "s":
						accessArray.push('set');
						break;
					case "o":
						accessArray.push('observe');
						break;
					default:
						break;
				}
			}
			myAccess = accessArray.join(', ');
			
			return '<tr class="lomRow"><td class="lomName">'+myName+'</td><td class="lomType">'+myType+'</td><td class="lomAccess">'+myAccess+'</td><td class="lomDesc">';
		});
		objString = objString.replace(/<\/liveapi_child>/g, '</td></tr>');
		
		//parse functions table
		objString = objString.replace(/<liveapi_functions>/g, '<table class="lomTable"><tr><td class="lomHeader">Functions</td><td class="lomHeader">&nbsp;</td></tr><tr><td class="italic lomKey lomName">Name</td><td class="italic lomKey lomDesc">Description</td></tr>');
		objString = objString.replace(/<\/liveapi_functions>/g, '</table>');
		objString = objString.replace(/<liveapi_function[ ]+name="([^"]*)"[ ]*>/g, function(wholematch, myName) {
			return '<tr class="lomRow"><td class="lomName">'+myName+'</td><td class="lomDesc">';
		});
		objString = objString.replace(/<liveapi_function[ ]+name="([^"]*)"[ ]+type="([^"]*)"[ ]*>/g, function(wholematch, myName, myType) {
			return '<tr class="lomRow"><td class="lomName">'+myName+'</td><td class="lomDesc">';
		});
		objString = objString.replace(/<\/liveapi_function>/g, '</td></tr>');
		
		//parse properties table
		//remove if empty:
		objString = objString.replace(/<liveapi_properties>[ ]*<\/liveapi_properties>/g, '');
		objString = objString.replace(/<liveapi_properties>/g, '<table class="lomTable"><tr><td class="lomHeader">Properties</td><td class="lomHeader">&nbsp;</td><td class="lomHeader">&nbsp;</td><td class="lomHeader">&nbsp;</td></tr><tr><td class="italic lomKey lomName">Name</td><td class="italic lomKey lomType">Type</td><td class="italic lomKey lomAccess">Access</td><td class="italic lomKey lomDesc">Description</td></tr>');
		objString = objString.replace(/<\/liveapi_properties>/g, '</table>');
		objString = objString.replace(/<liveapi_property[ ]+name="([^"]*)"[ ]+gso="([^"]*)"[ ]+type="([^"]*)"[ ]*>/g, function(wholematch, myName, myAccess, myType) {
			var accessArray = [];
			var myAccessChars = myAccess.split(''); // Array-like character access is only in ecmascript 5, IE 7 doesn't like it, so we force an array here
			for(var i=0; i<myAccessChars.length; i++) {
				switch (myAccessChars[i]) {
					case "g":
						accessArray.push('get');
						break;
					case "s":
						accessArray.push('set');
						break;
					case "o":
						accessArray.push('observe');
						break;
					default:
						break;
				}
			}
			myAccess = accessArray.join(', ');
			
			return '<tr class="lomRow"><td class="lomName">'+myName+'</td><td class="lomType">'+myType+'</td><td class="lomAccess">'+myAccess+'</td><td class="lomDesc">';
		});
		objString = objString.replace(/<\/liveapi_property>/g, '</td></tr>');
		
		
		
		return objString;
	}
	
	genVignette = function (objName, obj, myCatFolder) {
		var myUrl = URL_PREFIX + 'vignettes/' + myCatFolder + objName + '.maxvig.xml';

		$.ajax({
			url: myUrl,
			dataType: 'text',
			success: function (xml) {
				var html = "";
				var openfile = "";
	
				xml = convertTags(xml);
				xml = xml.replace(/<bodytext class="introbody"/g, "<div");
				xml = xml.replace(/<bodytext/g, "<div class='bodytext'");
				xml = xml.replace(/<\/bodytext>/g, "</div>");
				xml = xml.replace(/<img/g, "<villustration");
				xml = xml.replace(/<\/img/g, "</villustration");
				//xml = xml.replace(/<header1/g, "<h2 class='vigTitle1'");
				//xml = xml.replace(/<\/header1/g, "</h2");
				//xml = xml.replace(/<header2/g, "<h2 class='vigTitle'");
				//xml = xml.replace(/<\/header2/g, "</h2");
				xml = xml.replace(/<body class="introbody"/g, "<div class='vigDiv'");
				xml = xml.replace(/<body/g, "<div class='vigDiv'");
				xml = xml.replace(/<\/body>/g, "</div>");
				//convert thesaurus tags to table
				
				xml = xml.replace(/<thesaurus>/g, '<table id="thesaurus">');
				xml = xml.replace(/<\/thesaurus>/g, '</table>');


				xml = xml.replace(/<thesaurusentrygroup>/g, '<tr><td><table class="thesaurusentrygroup">');
				xml = xml.replace(/<\/thesaurusentrygroup>/g, '</table></td></tr>');
				xml = xml.replace(/<thesaurusheader>/g, '<tr class="thesaurusheaderrow"><td class="thesaurusheader">');
				xml = xml.replace(/<\/thesaurusheader>/g, '</td><td></td></tr>');
				xml = xml.replace(/<thesaurusentry>/g, '<tr class="thesaurusentryrow">');
				xml = xml.replace(/<\/thesaurusentry>/g, '</tr>');
				
				xml = xml.replace(/<thesauruskey \/>/g, '<td class="thesauruskey"></td>');
				xml = xml.replace(/<thesauruskey>/g, '<td class="thesauruskey">');
				xml = xml.replace(/<\/thesauruskey>/g, '</td>');
				
				xml = xml.replace(/<thesaurusobjectlist>/g, '<td class="thesaurusobjectlist">');
				xml = xml.replace(/<\/thesaurusobjectlist>/g, '</td>');
				xml = xml.replace(/<thesaurusobject>/g, '');
				xml = xml.replace(/<\/thesaurusobject>/g, '');
				//if(objName=="thesaurus") {
				//	xml = xml.replace(/<a name/g, '<a id');
				//}

				var xmlObj = $.parseXML(xml);
				$('a[href="#mainTab"]').html("Documentation");
				$('#mainTab').html('<div id="mainHeader"></div>');
				if(obj['digest']) {
					$('#mainTab').html('<h1 id="pageTitle" class="objTitle">' + obj['digest'] + '</h1>');
				} else {
					$('#mainTab').html('<h1 id="pageTitle" class="objTitle">' + objName + '</h1>');
				}				
				$('#pageTitle').prepend('<a class="homelink" href="#docintro"><img class="titleLogo" src="images/max6.png" alt="home"/></a>');
				$('#mainTab').append('<div id="viglist" class="contentScroll"></div>');
				
				//$(xml).children().each(function(key, val) {	
				
				
				$(xmlObj).find("vignette").children().each(function(key, val) {
					//alert(val.tagName.toLowerCase() + " : " + val.nodeName.toLowerCase());
					convertXmlToVig(key, val);
				});
				
				$("#vigButton").button({
					icons: {
						secondary: "ui-icon-newwin"
					}
				}).bind('click', function () {
					//open the patch
				});
				
	
				
				$('o').each(function (index) {
					//var myObj = ($(this).html().trim());
					var myObj = $.trim( getContents( $(this) ));
					$(this).replaceWith('<a class="o" href="#' + myObj + '">' + myObj + '</a>');
					
				});


				$('.o').each(function(index) {
					if(objName.indexOf('alphabetical')>-1 || objName.indexOf('functional')>-1) {
						$(this).wrap('<span>&nbsp;&nbsp;</span>');
					}
				});

				$('area').each(function(index) {
					$(this).addClass('anchorlink');
				});
	
				$('villustration').each(function (index) {
					var myClass=$(this).attr('class');
					var mySrc = '../vignettes/'+myCatFolder+$(this).attr('src');
					if(objName == "live_object_model" || objName == "live_api_overview") {
						//we're not going to use the image map until we figure out intra-document linking
						$(this).replaceWith('<img class="illustration" style="max-width: 1078px; padding: 0px;" src="'+mySrc+'" usemap="#LOMIndex"/>');
						//$(this).replaceWith('<img class="illustration" style="max-width: 1078px;" src="'+mySrc+'"/>');
					} else {
						$(this).replaceWith('<table><tr><td><img class="illustration '+myClass+'" style="max-width: 1000px;" src="'+mySrc+'"/></td></tr><tr><td class="tutCaption"></td></tr></table>');
					}
				});
				
				
				//fix area tags
				$('area').each(function (key, val) {
					myHref = $(val).attr('href');
					myHref = myHref.replace(/\./g, '-');
					$(val).attr('href', myHref);
				});
	
				var vigMaxHeight = document.documentElement.clientHeight - $('#mainHeader').height() - 100;
				$('#viglist').css('max-height', vigMaxHeight);
				
				
				/*
				//set title
				contentName = nameReplace(hash.substring(1))			
				val = vignettes[contentName];
				if(val) {
					vigName = val.digest;
				} else {
					vigName = "Welcome";
				}
				*/
				//document.title = vigName;
				
				$('a').bind('click', function(e) {
					if($(this).attr('anchor')) {
						var myAnchorName = $(this).attr('anchor');
						var myAnchor = $('a[name*="'+myAnchorName+'"]');
						//get the top offset of the target anchor
						//var target_offset = myAnchor.offset();
						//var target_top = target_offset.top;
				
						//goto that anchor by setting the body scroll top to anchor top
						//$('html, body').animate({scrollTop:target_top}, 500);
						return false;
					}
					return true;
				});
				
				
				//zebra-ize
				$(".lomRow:even").css("background-color", "#ffffff");
				$(".lomRow:odd").css("background-color", "#f9f9fe");
				
				//add anchorlink functionality
				$('a, .anchor').each(function (key, val) {
					var myAnchor = $(val).attr('anchor');
					if(myAnchor&&myAnchor[0]=="#") {
						myAnchor = myAnchor.substring(1);
					}
					if(myAnchor) {
						$(val).attr('href', 'javascript:myScrollTo("viglist", "'+myAnchor+'")');
					}
					$(val).removeAttr('anchor');
				});
				
				//add anchorlink functionality
				$('.anchorlink').each(function (key, val) {
					var myAnchor = $(val).attr('href');
					if(myAnchor[0]=="#") {
						myAnchor = myAnchor.substring(1);
					}
					$(val).attr('href', 'javascript:myScrollTo("viglist", "'+myAnchor+'")');
				});
				
				
				
				//scroll to anchor if specified in url
				if(params.anchor) {
					myScrollTo('viglist', params.anchor);
				}
				
				
				//replace ltp_standin with <p
				var s = $('#mainTab').html();
				s = s.replace(new RegExp("LTP_STANDIN", "g"), '&lt;p');
				$('#mainTab').html(s);

				$('#docintroaccordion').accordion();	

			}
		});
	}
	
	genRefpage = function (objName, obj, myCatFolder) {
		$.ajax({
			url: URL_PREFIX + 'refpages/' + myCatFolder + objName + '.maxref.xml',
			dataType: "text",
			success: function (xml) {
				xml=convertTags(xml);
				xml = xml.replace(new RegExp( "<o>", "g" ), '<a class="o">');
				xml = xml.replace(new RegExp( "</o>", "g" ), '</a>');
				xml = xml.replace(new RegExp( "<m>", "g" ), '<a class="m">');
				xml = xml.replace(new RegExp( "</m>", "g" ), '</a>');
	
				var xmlObj = $.parseXML( xml );	
				//set tab title
				//WITH OBJ NAME$('a[href="#mainTab"]').html("<span class='bold'>"+objName+"</span>ocumentation");
				$('a[href="#mainTab"]').html("Documentation");
				//add title and subtitle
				$('#mainTab').html('<div id="mainHeader"></div>');
				objName = objName.toLowerCase();
				$('#mainHeader').append('<h1 id="pageTitle" class="objTitle">' + nameReplace(objectsLookup[objName].name) + '</h1>');
				$('#pageTitle').prepend('<a class="homelink" href="#docintro"><img class="titleLogo" src="images/max6.png" alt="home"/></a>');

				//$('#mainHeader').append('<h2 class="objSubtitle">' + $(xml).find("digest:first").html() + '</h2>');
				$('#mainHeader').append('<h2 class="objSubtitle">' + getContents( $(xmlObj).find("digest:first") ) + '</h2>');
				if(objName!='Max Object Reference' && objName!="max" && objName!="jit.group-gl" && objName!="jbox" && objName!="jit.group-mop" && ONLINE == false) {
					openfileurl = 'max:openhelp/' + objName;
					var onclick = "document.location.href='" +openfileurl + "';";
					$('<button id="tutButton" class="linkbutton" onclick="'+ onclick + '">Open the tutorial patcher</button>').insertBefore($('#tutlist'));
					$('#mainHeader').append('<button id="'+objName+'Button" class="patcherButton" onclick="'+onclick+'">Open <b>'+objName+'</b> help patcher</button>');
				}
				
				$('.patcherButton').button({
						icons: {
							secondary: "ui-icon-newwin"
						}
					}).bind('click', function () {
						//open max patcher
					}).css('font-size', '.8em').css('line-height', '.8em').css('margin-top', '.5em');
	
				var outputs = [];
				var args = [];
				var msgs = [];
				var attrs = [];
				var seealsos = [];
				var mops = [];
	
	/*
				$.each($(xml).find("inlet digest").contents(), function(key, val) {
					inlets.push($(val).text());
				});
				$.each($(xml).find("outlet digest").contents(), function(key, val) {
					outlets.push($(val).text());
				});
				*/
				//$.each($(xml).find("seealso"), function (key, val) {
				$.each($(xmlObj).find("seealso"), function (key, val) {
					var myName = ($(val).attr('name')).toLowerCase();
					var myType = ($(val).attr('type'));
					var myModule = ($(val).attr('module'));
					var myLink = "";
					var dict = null;
	
					if (!myType) {
						myType = "refpage";
					}
					var myDesc = "";
					switch (myType) {
					case "refpage":
						dict = refpages;
						if(objectsLookup[nameReplace(myName)]) {
							myDesc = objectsLookup[nameReplace(myName)].digest;
						} else {
							myDesc = '';
						}
						break;
					case "tutorial":
						dict = tutorials;
						myLink = myName;
						break;
					case "vignette":
						dict = vignettes;
						break;
					default:
						break;
					}
					if (dict) {
						if (nameReplace(myName) in dict) {
							myLink = "#" + nameReplace(myName);
							//myLink = URL_PREFIX + myType + "s/" + dict[myName]['module'] + "/" + dict[myName]['file'];
							myDesc = dict[nameReplace(myName)]["digest"];
							if (myType != "refpage") {
								myName = myDesc;
							}
						}
					}
	
					var seealso = {
						"name": myName,
						"type": myType,
						"link": myLink,
						"desc": myDesc
					}
					seealsos.push(seealso);
				});
	
				//$.each($(xml).find("entry"), function (key, val) {
				$.each($(xmlObj).find("entry"), function (key, val) {
					var output = {
						"name": ($(val).attr('name')),
						"desc": getBestDescription(val),
						"disc": getChildHtml($(val), 'discussion')
					}
					outputs.push(output);
				});
				
				
				//gather mops
				//sometimes the number of input and output matrixes shown in the <mop> tag's
				//attribute list does not match that of the actual <matrixinput> and <matrixoutput>
				$.each($(xmlObj).find("mop"), function (key, val) {
					
					var myMop = {
						"nInputs": $(val).attr("matrixinputcount"),
						"nOutputs":  $(val).attr("matrixoutputcount"),
						"matrixInputs": [],
						"matrixOutputs": []
					};
					
					//get input matrixes
					$.each($(val).find("matrixinput"), function (key2, val2) {
						var myMatrixInput = {
							"name": $(val2).attr('name'),
							"planelink": $(val2).attr('planelink'),
							"typelink": $(val2).attr('typelink'),
							"dimlink": $(val2).attr('dimlink'),
							"minplanecount": $(val2).attr('minplanecount'),
							"maxplanecount": $(val2).attr('maxplanecount'),
							"mindimcount": $(val2).attr('mindimcount'),
							"maxdimcount": $(val2).attr('maxdimcount'),
							"ioproc": ($(val2).attr('ioproc') ? $(val2).attr('ioproc') : 'n/a')
						};
						
						$.each($(val).find("types"), function (key3, val3) {
							var myTypes = $(val3).text();
							myMatrixInput.types = myTypes;
						});
						
						myMop.matrixInputs.push(myMatrixInput);
						
					});
					$.each($(val).find("matrixoutput"), function (key2, val2) {
						var myMatrixOutput = {
							"name": $(val2).attr('name'),
							"planelink": $(val2).attr('planelink'),
							"typelink": $(val2).attr('typelink'),
							"dimlink": $(val2).attr('dimlink'),
							"minplanecount": $(val2).attr('minplanecount'),
							"maxplanecount": $(val2).attr('maxplanecount'),
							"mindimcount": $(val2).attr('mindimcount'),
							"maxdimcount": $(val2).attr('maxdimcount'),
							"ioproc": ($(val2).attr('ioproc') ? $(val2).attr('ioproc') : 'n/a')
						};
						
						$.each($(val).find("types"), function (key3, val3) {
							var myTypes = $(val3).text();
							myMatrixOutput.types = myTypes;
						});
						
						myMop.matrixOutputs.push(myMatrixOutput);
					});
					
					mops.push(myMop);
					//myLog('nMops: '+mops.length);
					//myLog("nInputs: "+myMop.nInputs);
					//myLog("nOutputs: "+myMop.nOutputs);
					//myLog("matrixoutput 1  name: "+myMop.matrixOutputs[0].name);
					//myLog("matrixoutput 1  types: "+myMop.matrixOutputs[0].types);
				});

	
				//$.each($(xml).find("objarg"), function (key, val) {
				$.each($(xmlObj).find("objarg"), function (key, val) {
					var myUnits = $(val).attr('units');
					if(myUnits) {
						myUnits = '('+myUnits+')';
					} else {
						myUnits = '';
					}
					var arg = {
						"name": ($(val).attr('name')),
						"opt": ($(val).attr('optional') == '1') ? "optional" : "required",
						"type": ($(val).attr('type') + ' '+myUnits),
						"desc": getBestDescription(val),
						"disc": getChildHtml($(val), 'discussion')
					}
					if (arg.name != "OBJARG_NAME") {
						args.push(arg);
					}
				});
	
				//$.each($(xml).find("method"), function (key, val) {
				$.each($(xmlObj).find("method"), function (key, val) {
				
					var myArgsList = $(val).find('arg');
					var myArgs = '';
					$(myArgsList).each(function(key, val) {
						var myUnits = $(val).attr('units');
						if(myUnits) {
							myUnits = '('+myUnits+')';
						} else {
							myUnits = '';
						}
						
						myArgs+=($(val).attr('name')+' '+myUnits+' ['+ $(val).attr('type')+']<br />');
					});
					
				
					var myDesc = getBestDescription(val);
					var method = {
						"name": ($(val).attr('name')),
						"desc": (myDesc),
						"type": ($(val).find("method arglist arg").attr('type')),
						"disc": getChildHtml($(val), 'discussion'),
						"args": myArgs
					}
					
					msgs.push(method);
				
				});
	
				//sort msgs
				msgs.sort(function (a, b) {
					return (a.name > b.name ? 1 : -1)
				});
				
				//xmlGatherAttrs(xml, attrs);
				xmlGatherAttrs(xmlObj, attrs);
	
				//var myDesc = convertTags($(xml).find("description:first").html());
				var myDesc = convertTags( getContents( $(xmlObj).find("description:first")));

				// myOutput is never used ?
				//var myOutput = ($(xml).find("entry").html());
				
	
				$('#mainTab, #quickrefTab').append('<ul id="reflist" class="contentScroll accordion"></ul>');
	
				$('#mainTab #reflist').append('<li class="reflistTitle">Description' + '<ul id="descriptionReflist" class="shown">' + '<li>' + '<table id="descriptionTable"><tr><td class="description">' + myDesc + '</td></tr>' + '</table>' + '</li>' + '</ul>' + '</li>');
	
				
				//get discussion
				var myDiscussion = convertTags( getContents( $(xmlObj).find("discussion:first")));
				if(myDiscussion) {
					$('#mainTab #reflist').append('<li class="reflistTitle">Discussion' + '<ul id="discussionReflist">' + '<li>' + '<table id="discussionTable"><tr><td class="discussion">' + myDiscussion + '</td></tr>' + '</table>' + '</li>' + '</ul>' + '</li>');
				}
	
				if (objName != "Max Object Reference" && objName != "jit.group-gl" && objName != "jit.group-mop" && objName != "jbox") {
					//example image will not render unless the example tag's img attribute is set.
					myName = $(xmlObj).find("example").attr('img');
					if (!myName) {
						myName = nameReplace(objName)+'.png';
					}
					if (myName) {
						var myCaption = $(xmlObj).find("example").attr('caption');
						if(!myCaption) { 
							myCaption = ' '; 
						}
						var tmp = '<tr><td class="centered"><img class="illustration" src="' + URL_PREFIX + 'refpages/';
						tmp += myCatFolder + 'images/' + objName + '.png" title="' + myCaption + '"><div class="caption">';
						tmp += myCaption + '</div></td></tr>'//</table>';
						$('#descriptionTable').append(tmp);
					}
				}
				
				if (outputs.length) {
					$('#reflist').append('<li class="reflistTitle">Output<ul id="outputsReflist" class="shown">\</ul>' + '</li>');
					$('#outputsReflist').append('<li class="reflistItem"><table><tr><td class="refData argName refHeader">Type</td><td class="refData argDesc refHeader">Description</td></tr></table></li>');
					$.each(outputs, function (key, val) {
						if (val) {
							$('#outputsReflist').append('<li class="reflistItem"><table><tr><td class="refData refKey">' + val.name + '</td><td class="refData">' + val.desc + genDiscussion(val) + '</td></tr></table></li>');
						}
					});
				}
	
				if (args.length) {
					$('#reflist, #quickrefTab #reflist').append('<li class="reflistTitle">Arguments' + '<ul id="argumentsReflist" class="shown"></ul>' + '</li>');
	
					//generate names for numbered inlets
					$('#argumentsReflist, #quickrefTab #argumentsReflist').append('<li class="reflistItem"><table><tr><td class="refData argName refHeader">Name</td><td class="refData argOpt refHeader">Optional</td><td class="refData argType refHeader">Type (units)</td><td class="refData argDesc refHeader">Description</td></tr></table></li>');
					$.each(args, function (key, val) {
						$('#argumentsReflist, #quickrefTab #argumentsReflist').append('<li class="reflistItem"><table><tr><td class="refData argName">' + val.name + '</td><td class="refData argOpt">' + val.opt + '</td><td class="refData argType">' + val.type + '</td><td class="refData argDesc">' + val.desc + genDiscussion(val) + '</td></tr></table></li>');
					});
				}
	
				if (msgs.length) {
					$('#reflist, #quickrefTab #reflist').append('<li class="reflistTitle">Messages' + '<ul id="messagesReflist" class="shown"></ul>' + '</li>');
	
					//generate names for numbered inlets
					$('#messagesReflist, #quickrefTab #messagesReflist').append('<li class="reflistItem"><table><tr><td class="refData argName refHeader">Message</td><td class="refData argName refHeader typeUnits">Argument (units) [type]</td><td class="refData argDesc refHeader">Description</td></tr></table></li>');
					$.each(msgs, function (key, val) {
						$('#messagesReflist, #quickrefTab #messagesReflist').append('<li class="reflistItem"><table><tr><td class="refData argName" name="msg_anchor_'+val.name+'">' + val.name + '</td><td class="refData argName typeUnits">' + val.args + '</td><td class="refData argDesc">' + val.desc + genDiscussion(val) + '</td></tr></table></li>');
					});
				}
				
				
				
				if (attrs.length) {
					$('#reflist, #quickrefTab #reflist').append('<li class="reflistTitle">Attributes' + '<ul id="attributesReflist" class="shown"></ul>' + '</li>');
	
					//generate names for numbered inlets
					$('#attributesReflist, #quickrefTab #attributesReflist').append('<li class="reflistItem"><table><tr><td class="refData argName refHeader">Name</td><td class="refData argName refHeader">Length/Type</td><td class="refData argDesc refHeader">Description</td></tr></table></li>');
					$.each(attrs, function (key, val) {
						if (val.size > 1) {
							plural = "s";
						} else {
							plural = "";
						}
						val.desc=convertTags(val.desc);
						if(val.name!='enumvals') {
							$('#attributesReflist, #quickrefTab #attributesReflist').append('<li class="reflistItem"><table><tr>' + '<td class="refData argName" name="msg_anchor_'+val.name+'">' + val.name + '</td>' + '<td class="refData argName">(' + val.size + ' ' + val.type + plural + ')</td>' + '<td class="refData argDesc">' + val.desc + genDiscussion(val) + '</td></tr></table></li>');
						}
					});
				}
				
				
				//render mops
				if(mops.length) {
					$('#reflist, #quickrefTab #reflist').append('<li class="reflistTitle">Matrix Operator' + '<ul id="matrixOperatorReflist" class="shown"></ul>' + '</li>');
					
					$('#matrixOperatorReflist').append('<li class="reflistItem"><table><tr><td class="refData mopTD refHeader">Name</td><td class="refData mopTD refHeader">IOProc</td><td class="refData mopTD refHeader">Planelink</td><td class="refData mopTD refHeader">Typelink</td><td class="refData mopTD refHeader">Dimlink</td><td class="refData mopTD refHeader">Min Plane Count</td><td class="refData mopTD refHeader">Max Plane Count</td><td class="refData mopTD refHeader">Min Dim Count</td><td class="refData mopTD refHeader">Max Dim Count</td><td class="refData mopTD refHeader">Types</td></tr></table></li>');

					$.each(mops, function (key, val) {
						var myInputs = val.matrixInputs;
						var myOutputs = val.matrixOutputs;
						for (i = 0; i<myInputs.length; i++) {
							$('#matrixOperatorReflist').append('<li class="reflistItem"><table><tr>' + '<td class="refData mopTD">' + myInputs[i].name + '</td>' + '<td class="refData mopTD">' + myInputs[i].ioproc + '</td>' + '<td class="refData mopTD">' + myInputs[i].planelink + '</td>' + '<td class="refData mopTD">' + myInputs[i].typelink + '</td>' + '<td class="refData mopTD">' + myInputs[i].dimlink + '</td>' + '<td class="refData mopTD">' + myInputs[i].minplanecount + '</td>' + '<td class="refData mopTD">' + myInputs[i].maxplanecount + '</td>' + '<td class="refData mopTD">' + myInputs[i].mindimcount + '</td>' + '<td class="refData mopTD">' + myInputs[i].maxdimcount + '</td>' + '<td class="refData mopTD">' + myInputs[i].types + '</td>' + '</tr></table></li>');
						}
						for (i = 0; i<myOutputs.length; i++) {
							$('#matrixOperatorReflist').append('<li class="reflistItem"><table><tr>' + '<td class="refData mopTD">' + myOutputs[i].name + '</td>' + '<td class="refData mopTD">' + myOutputs[i].ioproc + '</td>' + '<td class="refData mopTD">' + myOutputs[i].planelink + '</td>' + '<td class="refData mopTD">' + myOutputs[i].typelink + '</td>' + '<td class="refData mopTD">' + myOutputs[i].dimlink + '</td>' + '<td class="refData mopTD">' + myOutputs[i].minplanecount + '</td>' + '<td class="refData mopTD">' + myOutputs[i].maxplanecount + '</td>' + '<td class="refData mopTD">' + myOutputs[i].mindimcount + '</td>' + '<td class="refData mopTD">' + myOutputs[i].maxdimcount + '</td>' + '<td class="refData mopTD">' + myOutputs[i].types + '</td>' + '</tr></table></li>');
						}
						//$.each(matrixInputs), function (key2, val2) {
						//	myLog(val2.name);
						//});
						//$('#matrixOperatorReflist').append('<li class="reflistItem"><table><tr>' + '<td class="refData argName">' + val.name + '</td>' + '<td class="refData argName">' + val.planelink + '</td>' + '<td class="refData argName"></tr></table></li>');
					});
				}
	
				if (objName!="Max Object Reference" && jbox.length && ($.inArray(objName, ["jbox", "jit.group-gl", "jit.group-mop"])) < 0) {
					$('#reflist').append('<li id="jboxReflistTitle" class="reflistTitle">Common Box Attributes (jbox)' + '<ul id="jboxReflist"></ul>' + '</li>');
	
					$('#jboxReflist').append('<li class="reflistItem"><table><tr><td class="refData argDesc refHeader linkTD"><a href="#jbox"><button id="jboxButton" class="linkbutton">Open jbox documentation</button></a></td></tr></table></li>');
					$('#jboxReflist').append('<li class="reflistItem"><table><tr><td class="refData argName refHeader">Name</td><td class="refData argName refHeader">Size/Type</td><td class="refData argDesc refHeader">Description</td></tr></table></li>');
					$.each(jbox, function (key, val) {
						if (val.size > 1) {
							plural = "s";
						} else {
							plural = "";
						}
						if(val.name!="enumvals") {
							$('#jboxReflist').append('<li class="reflistItem"><table><tr>' + '<td class="refData argName">' + val.name + '</td>' + '<td class="refData argName">(' + val.size + ' ' + val.type + plural + ')</td>' + '<td class="refData argDesc">' + val.desc + genDiscussion(val) + '</td></tr></table></li>');
						}
					});
	
					$("#jboxButton").button({
						icons: {
							secondary: "ui-icon-document-b"
						}
					}).bind('click', function () {
						genContent('jbox');
					});
	
					$("#ob3dButton").button({
						icons: {
							secondary: "ui-icon-document-b"
						}
					}).bind('click', function () {
						genContent('jit.group-gl');
					});
					
					
				}
	
				if (objName.indexOf(".gl.") > 0 && $.inArray(objName, ["jbox", "jit.group-gl", "jit.group-mop"]) < 0) {
					if (ob3dMsgs.length) {
						$('#reflist').append('<li class="reflistTitle">Jitter GL Messages (OB3D)' + '<ul id="ob3dMsgReflist"></ul>' + '</li>');
	
						//generate names for numbered inlets
						$('#ob3dMsgReflist').append('<li class="reflistItem"><table><tr><td class="refData argDesc refHeader linkTD"><a href="#jit.group-gl"><button id="ob3dMsgsButton" class="linkbutton">Open Jitter GL (OB3D) documentation</button></a></td></tr></table></li>');
						$('#ob3dMsgReflist').append('<li class="reflistItem"><table><tr><td class="refData argName refHeader">Type</td><td class="refData argDesc refHeader">Description</td></tr></table></li>');
						$.each(ob3dMsgs, function (key, val) {
							$('#ob3dMsgReflist').append('<li class="reflistItem"><table><tr><td class="refData argName">' + val.name + '</td><td class="refData argDesc">' + val.desc + genDiscussion(val) + '</td></tr></table></li>');
						});
					}
	
					if (ob3dAttrs.length) {
						$('#reflist').append('<li class="reflistTitle">Jitter GL Attributes (OB3D)' + '<ul id="ob3dAttrsReflist"></ul>' + '</li>');
	
						//generate names for numbered inlets
						$('#ob3dAttrsReflist').append('<li class="reflistItem"><table><tr><td class="refData argDesc refHeader linkTD"><a href="#jit.group-gl"><button id="ob3dAttrsButton" class="linkbutton">Open Jitter GL (OB3D) documentation</button></a></td></tr></table></li>');
						$('#ob3dAttrsReflist').append('<li class="reflistItem"><table><tr><td class="refData argName refHeader">Name</td><td class="refData argName refHeader">Size/Type</td><td class="refData argDesc refHeader">Description</td></tr></table></li>');
						$.each(ob3dAttrs, function (key, val) {
							if (val.size > 1) {
								plural = "s";
							} else {
								plural = "";
							}
							$('#ob3dAttrsReflist').append('<li class="reflistItem"><table><tr>' + '<td class="refData argName">' + val.name + '</td>' + '<td class="refData argName">(' + val.size + ' ' + val.type + plural + ')</td>' + '<td class="refData argDesc">' + val.desc + genDiscussion(val) + '</td></tr></table></li>');
						});
					}
	
					$("#ob3dMsgsButton, #ob3dAttrsButton").button({
						icons: {
							secondary: "ui-icon-document-b"
						}
					}).bind('click', function () {
						genContent('jit.group-gl');
					});
				} else if (objName.indexOf("jit.") === 0 && $.inArray(objName, ["jbox", "jit.group-gl", "jit.group-mop"]) < 0) {
					if (jmopMsgs.length) {
						$('#reflist').append('<li class="reflistTitle">Jitter Matrix Operator Messages' + '<ul id="jmopMsgsReflist"></ul>' + '</li>');
	
						//generate names for numbered inlets
						$('#jmopMsgsReflist').append('<li class="reflistItem"><table><tr><td class="refData argDesc refHeader linkTD"><a href="#jit.group-mop"><button id="jmopMsgsButton" class="linkbutton">Open Jitter Matrix Operator documentation</button></a></td></tr></table></li>');
						$('#jmopMsgsReflist').append('<li class="reflistItem"><table><tr><td class="refData argName refHeader">Type</td><td class="refData argDesc refHeader">Description</td></tr></table></li>');
						$.each(jmopMsgs, function (key, val) {
							$('#jmopMsgsReflist').append('<li class="reflistItem"><table><tr><td class="refData argName">' + val.name + '</td><td class="refData argDesc">' + val.desc + genDiscussion(val) + '</td></tr></table></li>');
						});
					}
	
					if (jmopAttrs.length) {
						$('#reflist').append('<li class="reflistTitle">Jitter Matrix Operator Attributes' + '<ul id="jmopAttrsReflist"></ul>' + '</li>');
	
						//generate names for numbered inlets
						$('#jmopAttrsReflist').append('<li class="reflistItem"><table><tr><td class="refData argDesc refHeader linkTD"><a href="#jit.group-mop"><button id="jmopAttrsButton" class="linkbutton">Open Jitter Matrix Operator documentation</button></a></td></tr></table></li>');
						$('#jmopAttrsReflist').append('<li class="reflistItem"><table><tr><td class="refData argName refHeader">Name</td><td class="refData argName refHeader">Size/Type</td><td class="refData argDesc refHeader">Description</td></tr></table></li>');
						$.each(jmopAttrs, function (key, val) {
							if (val.size > 1) {
								plural = "s";
							} else {
								plural = "";
							}
							$('#jmopAttrsReflist').append('<li class="reflistItem"><table><tr>' + '<td class="refData argName">' + val.name + '</td>' + '<td class="refData argName">(' + val.size + ' ' + val.type + plural + ')</td>' + '<td class="refData argDesc">' + val.desc + genDiscussion(val) + '</td></tr></table></li>');
						});
					}
	
					$("#jmopAttrsButton, #jmopMsgsButton").button({
						icons: {
							secondary: "ui-icon-document-b"
						}
					}).bind('click', function () {
						genContent('jit.group-gl');
					});
				}
	
				//draw seealsos
				//-------------
				if (seealsos.length) {
					$('#reflist').append('<li class="reflistTitle">See also' + '<ul id="seealsoReflist" class="shown"></ul>' + '</li>');
	
					$('#seealsoReflist').append('<li class="reflistItem"><table><tr><td class="refData argName refHeader">Name</td><td class="refData argDesc refHeader">Description</td></tr></table></li>');
					$.each(seealsos, function (key, val) {
						if (val.link) {
							$('#seealsoReflist').append('<li class="reflistItem"><table><tr><td class="refData argName"><a class="o seealsotutoriallink" href="' + val.link + '">' + val.name + '</a></td><td class="refData argDesc">' + val.desc + genDiscussion(val) + '</td></tr></table></li>');
						} else {
							$('#seealsoReflist').append('<li class="reflistItem"><table><tr><td class="refData argName"><a class="o" href="#' + val.name + '">' + val.name + '</a></td><td class="refData argDesc">' + val.desc + genDiscussion(val) + '</td></tr></table></li>');
						}
					});
				}
				
				
				
				//replace ltp_standin with <p
				var s = $('#mainTab').html();
				s = s.replace(new RegExp("LTP_STANDIN", "g"), '&lt;p');
				$('#mainTab').html(s);
				
	
				$("#mainTab").nestedAccordion({
					//initShow : "#current"
					standardExpansible: true,
					showSpeed: 150,
					hideSpeed: 150,
					activeLink : ((ONLINE == true) ? true : false),
					initShow: ".shown"
				});
	
				$(".reflistItem:even").css("background-color", "#ffffff");
				$(".reflistItem:odd").css("background-color", "#f9f9fe");
	
				//add genContent function to all .descLink links

				//$('o').each(function (key,val) {
				// ATTENTION: We have problems here.  We can't do $('o').each. IE7 doesn't like this.
				// THis make sense since 'o' isnt an html element.  So, what I do is regex it in the xml
				// string above and search on 'a.o' here.  However, we seem to have some <o> data that really
				// isn't <o> ..such as "Max Basic Tutorial 11:Procedural drawing"
				$('a.o').each(function () {
					//var myObj =  ($(this).html().trim());
					var myObj = $.trim( getContents($(this) ) );
					//$(this).replaceWith('<a class="o" href="#' + myObj + '">' + myObj + '</a>');
					if(!$(this).hasClass('seealsotutoriallink')) {
						$(this).attr('href', '#' + myObj );
					}
				});
				
				$('link').each(function (index, link) {
					var linkText = link.innerHTML;
					var lName = $(this).attr('name');
					if(lName) {
						lName=lName.toLowerCase();
					}
					if (lName) {
						var lObj = null;
						var lType = $(this).attr('type');
						var lModule = $(this).attr('module');
	
						lName = lName.trim();
						if (!lType) lType = 'refpage';
						switch(lType) {
							case "refpage":
								lObj = objectsLookup[lName];
								break;
							case "tutorial":
								lObj = tutorials[lName];
								break;
							case "vignette":
								lObj = vignettes[lName];
								break;
							default:
								break;
						}
						if (lObj !== null) {
							//$(this).replaceWith('<a class="o" href="#' + lName + '">' + linkText + '</a>');
						}
					}
				});
				
				//add link functionality to message links (<a class="m">)
				$('a.m').each(function (key, val) {
					var myLink = 'msg_anchor_'+$(val).html();
					$(val).attr('href', 'javascript:myScrollTo("reflist", "'+myLink+'")');
					$(val).attr('style', 'color:black');
				});
	
				var refMaxHeight = document.documentElement.clientHeight - $('#mainHeader').height() - 80;
				$('#reflist').css('max-height', refMaxHeight);

				//set title
				//contentName = nameReplace(location.hash.substring(1, location.hash.indexOf("?")))
				//document.title = objName+" Reference";
				//add anchorlink functionality
				$('a.anchorlink').each(function (key, val) {
					var myAnchor = $(val).attr('href');
					if(myAnchor[0]=="#") {
						myAnchor = myAnchor.substring(1);
					}
					$(val).attr('href', 'javascript:myScrollTo("reflist", "'+myAnchor+'")');
				});
				
				
				//fix img links that aren't example images
				//if (ONLINE) {
					$('img[class!="illustration"]').each(function (key, val) {
						var mySrc = $(val).attr('src');
						// on IE, the src attribute comes in as file:// 
						if(mySrc!="images/search-icon.png" && mySrc!="images/max6.png" && mySrc.slice(0,5) != "file:" ) {
							var newSrc = '../refpages/'+myCatFolder+mySrc;
							$(val).attr('src', newSrc);
						}
					});
				//}
				//scroll to anchor if specified in url
				if(params.anchor) {
					myScrollTo('reflist', params.anchor);
				}
				
			},
	
			error: function (obj, t) {
				var obj = objectsLookup[objName.toLowerCase()];
				if (obj && obj.module != "kernel") {
					obj.module = "kernel";
					genContent(objName);
				}
			}
		});
	}
	
	convertTutorialTags = function (newStr) {
		if(ONLINE) {
			newStr = newStr.replace(/<openpatchlink>/g, '');
			newStr = newStr.replace(/<\/openpatchlink>/g, '');
		} else {
			newStr = newStr.replace(/<openpatchlink>/g, '<a class="patchlink" href="">');
			newStr = newStr.replace(/<\/openpatchlink>/g, '</a>');
		}
		//doing this in genTutorial now
		//newStr = newStr.replace(/<body>/g, '<div class="tutDiv">');
		//newStr = newStr.replace(/<\/body>/g, '</div>');
		return newStr;
	}
	//utility for objects with special characters
	
	function nameReplace(name) {
		switch (name) {
		case "ggate":
			newName = "gswitch2";
			return newName;
			break;
		case ">=":
			newName = "greaterthaneq";
			return newName;
			break;
		case ">=~":
			newName = "greaterthaneq~";
			return newName;
			break;
		case "<=":
			newName = "lessthaneq";
			return newName;
			break;
		case "<=~":
			newName = "lessthaneq~";
			return newName;
			break;
		case "==":
			newName = "equals";
			return newName;
			break;
		case "==~":
			newName = "equals~";
			return newName;
			break;
		case "!=":
			newName = "notequals";
			return newName;
			break;
		case "!=~":
			newName = "notequals~";
			return newName;
			break;
		case "!-":
			newName = "rminus";
			return newName;
			break;
		case "!/":
			newName = "rdiv";
			return newName;
			break;
		case "!/~":
			newName = "rdiv~";
			return newName;
			break;
		case "&&":
			newName = "logand";
			return newName;
			break;
		case "||":
			newName = "logor";
			return newName;
			break;
		case "<<":
			newName = "shiftleft";
			return newName;
			break;
		case ">>":
			newName = "shiftright";
			return newName;
			break;
		case "+=":
			newName = "plusequals";
			return newName;
			break;
		case "+":
			newName = "plus";
			return newName;
			break;
		case "+~":
			newName = "plus~";
			return newName;
			break;
		case "-":
			newName = "minus";
			return newName;
			break;
		case "-~":
			newName = "minus~";
			return newName;
			break;
		case "!-~":
			newName = "rminus~";
			return newName;
			break;
		case "*":
			newName = "times";
			return newName;
			break;
		case "*~":
			newName = "times~";
			return newName;
			break;
		case "/":
			newName = "div";
			return newName;
			break;
		case "/~":
			newName = "div~";
			return newName;
			break;
		case ">":
			newName = "greaterthan";
			return newName;
			break;
		case ">~":
			newName = "greaterthan~";
			return newName;
			break;
		case "<":
			newName = "lessthan";
			return newName;
			break;
		case "<~":
			newName = "lessthan~";
			return newName;
			break;
		case "&":
			newName = "bitand";
			return newName;
			break;
		case "|":
			newName = "bitor";
			return newName;
			break;
		case "%":
			newName = "modulo";
			return newName;
			break;
		case "%~":
			newName = "modulo~";
			return newName;
			break;
		case "t":
			newName = "trigger";
			return newName;
			break;
		case "b":
			newName = "bangbang";
			return newName;
			break;
		case "s":
			newName = "send";
			return newName;
			break;
		case "r":
			newName = "receive";
			return newName;
			break;
		case "v":
			newName = "value";
			return newName;
			break;
		case "+=~":
			newName = "plusequals~"
			return newName;
			break;
		default:
			return name;
			break;
		}
	}
				
	function getBestDigest(node) {
		var dig = $(node).find("digest:first");	
		if (!dig) { 
			return "";
		} else {
			var digText = $.trim( getContents( dig ) );
			if (  digText== "" || digText == "TEXT_HERE") {
				return "";
			} else {
				return digText;
			}
		}
		return "";
	}
		
	function getBestDescription(node) {
		// the get() func translates the jQuery object into a regular xml obj
		var desc = $(node).contents("description:first");	
		if (!desc) {
			return getBestDigest(node);
		} else {
			var descText = $.trim( getContents( desc ) );
			if (  descText== "" || descText == "TEXT_HERE") {
				return getBestDigest(node);
			} else {
				return descText;
			}
		}
		return "";
	}


	function genDiscussion(val) {
		var disc = "";
		if (val.disc && val.disc.trim() !== "") {
			// furiously lame, but just so it's displayed for now
			var discussionTag = '<discussion>';
			var discussionClosingTag = '</discussion>';
			discStartIndex = val.disc.indexOf(discussionTag)+discussionTag.length;
			discEndIndex = val.disc.indexOf(discussionClosingTag);
			disc = val.disc.substring(discStartIndex, discEndIndex);
			disc = '<br/><br/><span class="italic">' + disc +'</span>';
		}
		return disc;
	}
		
	function getChildHtml(val, name) {
		var children = $(val).children(name);
		if (children && children.length > 0) {
			return getContents( val );
		}
		return null;
	}
	
	function convertTags(oldStr) {
		var newStr;
		//newStr = oldStr.replace(new RegExp( "<o>", "g" ), '<a class="o">');
		//newStr = newStr.replace(new RegExp( "</o>", "g" ), '</a>');
		newStr = oldStr.replace(new RegExp("<m>", "g"), '<a class="m" href="">');
		newStr = newStr.replace(new RegExp("</m>", "g"), '</a>');
		newStr = oldStr.replace(new RegExp("&lt;p", "g"), 'LTP_STANDIN');
		
		// We don't really need this since documents are currently found by name, not by module
		// Leaving in, but there was a nasty little bug with single v. double quotes that was breaking
		// links in 'bodytext' sections in vignettes. Anyhow, fixed.
		while (newStr.match(/<link.*?name=['"](.*?)['"]>/)) { // we should really make this more selective based on module, but it's ok for now
			var lName = RegExp.$1.trim();
			newStr = newStr.replace(/'/g, "\'");
			newStr = newStr.replace(/<link .*?>/, '<a href="#' + lName + '" class="reflink">');
		}
		newStr = newStr.replace(/<\/link>/g, '</a>');
		newStr = newStr.replace(/\s<\/span>\s\./g, '</span>.');
		newStr = newStr.replace(/\s<\/a>\s\./g, '</a>.');
		newStr = newStr.replace(/\s<\/span>\s,/g, '</span>,');
		newStr = newStr.replace(/\s<\/a>\s,/g, '</a>,');
		newStr = newStr.replace(/\s<\/span>\s;/g, '</span>;');
		newStr = newStr.replace(/\s<\/a>\s;/g, '</a>;');
		newStr = newStr.replace(/\s<\/span>\s:/g, '</span>:');
		newStr = newStr.replace(/\s<\/a>\s:/g, '</a>:');
		newStr = newStr.replace(/\s*<\/a>/g, '</a>');
		return newStr;
	}
	
	
	
	
	
	function gatherData() {
		//BEGIN:
		//GET DATA.  All AJAX calls must be daisy chained.
		//------------------------------------------------
		$.ajax({
			url: URL_PREFIX + 'refpages/max-ref/jbox.maxref.xml',
			dataType: "xml",
			success: function (xml) {
				xmlGatherAttrs(xml, jbox);
				getGLData();
			},
			error: function (obj, t) {
				$.ajax({
					url: URL_PREFIX + 'refpages/kernel-ref/jbox.maxref.xml',
					dataType: "xml",
					success: function (xml) {
						xmlGatherAttrs(xml, jbox);
						getGLData();
					},
					error: function (obj, t) {
						alert(t);
					}
				});
			}
		});
		
	}
	
	function getGLData() {
		$.ajax({
			url: URL_PREFIX + 'refpages/jit-ref/jit.group-gl.maxref.xml',
			dataType: "xml",
			success: function (xml) {
				$(xml).find("attribute").each( function (key, val) {
					var attr = {
						"name": ($(val).attr('name')),
						"desc": getBestDescription(val),
						"disc": getChildHtml($(val), 'discussion'),
						"type": ($(val).attr('type')),
						"size": ($(val).attr('size'))
					}
					if (attr.desc != null) {
						ob3dAttrs.push(attr);
					}
				});
	
				$.each($(xml).find("method"), function (key, val) {
					var myDesc = getBestDescription(val);
					var method = {
						"name": ($(val).attr('name')),
						"desc": (myDesc),
						"disc": getChildHtml($(val), 'discussion'),
						"type": ($(val).find("method arglist arg").attr('type'))
					}
					ob3dMsgs.push(method);
				});
				getMopData();
			},
			error: function (obj, t) {
				alert(t);
			}
		});
	}
	
	function getMopData() {
		$.ajax({
			url: URL_PREFIX + 'refpages/jit-ref/jit.group-mop.maxref.xml',
			dataType: "xml",
			success: function (xml) {
				//	$.each($(text).find("inlet digest").contents(), function(key, val) {
				//					inlets.push($(val).text());
				//				});
				$.each($(xml).find("attribute"), function (key, val) {
					var attr = {
						"name": ($(val).attr('name')),
						"desc": getBestDescription(val),
						"disc": getChildHtml($(val), 'discussion'),
						"type": ($(val).attr('type')),
						"size": ($(val).attr('size'))
					}
					if (attr.desc != null) {
						jmopAttrs.push(attr);
					}
	
				});
				$.each($(xml).find("method"), function (key, val) {
					var myDesc = getBestDescription(val);
					var method = {
						"name": ($(val).attr('name')),
						"desc": (myDesc),
						"disc": getChildHtml($(val), 'discussion'),
						"type": ($(val).find("method arglist arg").attr('type'))
					}
					jmopMsgs.push(method);
				});
				getDocData();
			},
			error: function (obj, t) {
			}
		});
	}
	function getDocData() {
		$.ajax({
			url: INTERFACES_PREFIX + '/doc-qlookup.json',
			dataType: "json",
			success: function (data) {
				$.each(data, function (key, val) {
					//gatherDocs(val, window[key]);  //window[key] returning null.  with vignettes moved inside doc/onready, perhaps it is no longer within the "window" scope?  
					gatherDocs(val, key);
				});
				
				
				getObjData();
				
			},
			error: function () {
				//alert('could not find doc-qlookup.json at: '+INTERFACES_PREFIX + '/doc-qlookup.json');
			}
		});
	}
	
	function getObjData() {
		$.ajax({
			url: INTERFACES_PREFIX + '/obj-qlookup.json',
			dataType: "json",
			success: function (data) {
				var mySubArray;
	
				$.each(data, function (key, val) {
					if(key) {
						switch (val["module"]) {
						case "msp":
							val['topCat'] = "MSP";
							mySubArray = mspSubCats;
							break;
						case "jit":
							val['topCat'] = "Jitter";
							mySubArray = jitterSubCats;
							break;
						case "m4l":
							val['topCat'] = "M4L";
							mySubArray = m4lSubCats;
							break;
						default:
						case "max":
						case "kernel":
							val['topCat'] = "Max";
							mySubArray = maxSubCats;
							break;
						}
		
						
		
						//map symbol names to xml filename
						//TODO: should read from objectmappings.txt
						//-----------------------------------------
						var myLink = nameReplace(key).toLowerCase();
						switch (myLink) {
						case "jitter gl object (ob3d) messages":
							myLink = "jit.group-gl";
							break;
						case "jitter matrix operators":
							myLink = "jit.group-mop";
							break;
						default:
							break;
						}
						



						//create subcats uniquely into appropriate top-level cat
						var mySubCats = val["category"];
						$.each(mySubCats, function () {
							if (this.toString() != "MissingCategory" && this.toString() != "Unlisted") {
								if (jQuery.inArray(this.toString(), mySubArray) == -1) {
									mySubArray.push(this.toString());
									mySubArray.sort();
								}
							}
						});
		
						//save json objects data
						var hash = {
							"name": key.toLowerCase(),
							"topCat": val.topCat,
							"subCats": val["category"],
							"digest": val["digest"],
							"link": myLink,
							"module": val["module"],
							"alias":val['alias'],
							"private":val['private']
						}
						objects.push(hash);
						objectsLookup[myLink] = hash;
					}
				});
						
	
				
	
				//abc order all objects
				objects.sort(function (a, b) {
					return (a.name > b.name ? 1 : -1)
				});
				
				
				$("#content").css('display', 'block').tabs().tabs("select", 0);
				
				$('.mainLoader').css('display', 'none');
	
				
				if(USING_DB) {
					var qIndex = location.hash.indexOf("?");
					if(qIndex != -1) {
						var contentName = nameReplace(location.hash.substring(1, qIndex));
						genSearchNav();
					} else {
						var contentName = nameReplace(location.hash.substring(1));
					}
				} else {
					var contentName = nameReplace(location.hash.substring(1));
				}
				genContent(contentName);
				genNavs();
			},
			error: function (obj, t) {
				alert('couldnt load obj-qlookup.json'+t);
			}
		});
	}
	
	//BEGIN
	//-----
	
	if(DEBUGGING) {
		//add debugging console.
		$('#content').append('<div id="debug"></div>');
		$('#debug').css('position', 'absolute');
		$('#debug').css('bottom', '0');
		$('#debug').css('height', '100px');
		$('#debug').css('color', 'red');
		$('#debug').css('background-color', 'white');
		$('#debug').css('overflow', 'auto');
		$('#debug').css('width', '100%');
		$('#debug').after('<button id="debugClear">clear console</button>');
		$('#debug').after('<button id="printUrl">print url</button>');
		$('#debugClear, #printUrl').css('position', 'absolute');
		$('#debugClear').css('right', '0');
		$('#debugClear, #printUrl').css('top', '0');
		$('#printUrl').css('right', '140px');
		$('#debugClear').click(function() {
			$('#debug').empty();			
		});
		$('#printUrl').click(function() {
			//myLog('current location: '+location);			
		});

		
	}
	
	findInterfaces();
});
