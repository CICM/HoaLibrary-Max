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

/* ---------------------------------------------
Nested Accordion v.1.4.7.3
Script to create 'accordion' functionality on a hierarchically structured content.
http://www.adipalaz.com/experiments/jquery/nested_accordion.html
Requires: jQuery v1.4.2+
Copyright (c) 2009 Adriana Palazova
Dual licensed under the MIT (http://www.adipalaz.com/docs/mit-license.txt) and GPL (http://www.adipalaz.com/docs/gpl-license.txt) licenses.
------------------------------------------------ */
(function($) {
//$.fn.orphans - http://www.mail-archive.com/jquery-en@googlegroups.com/msg43851.html
$.fn.orphans = function(){
var txt = [];
this.each(function(){$.each(this.childNodes, function() {
  if (this.nodeType == 3 && $.trim(this.nodeValue)) txt.push(this)})}); return $(txt);};
  
$.fn.nestedAccordion = function(options) {
	//console.log("accordionizing with options: "+options);
    var o = $.extend({}, $.fn.nestedAccordion.defaults, options);
    
    return this.each(function() {
      var containerID = o.container ? '#' + this.id : '', objID = o.objID ? o.objID : o.obj + o.objClass,
        Obj = o.container ? containerID + ' ' + objID : '#' + this.id,
        El = Obj + ' ' + o.el,
        hTimeout = null; 

      // build
      if (o.head) $(Obj).find(o.head).addClass('h');
      if (o.head) {
        if ($(El).next('div:not(.outer)').length) {$(El).next('div:not(.outer)').wrap('<div class="outer" />');} 
        $(Obj + ' .h').each(function(){
            var $this = $(this);
            if (o.wrapper == 'div' && !$this.parent('div.new').length) {$this.add( $this.next('div.outer') ).wrapAll('<div class="new"></div>');}
        }); 
      }
      $(El).each(function(){
          var $node = $(this);
          if ($node.find(o.next).length || $node.next(o.next).length) {
            if ($node.find('> a').length) {
                $node.find('> a').addClass("trigger").css('display', "block");
            } else {
                var anchor = '<a class="trigger" style="display:block" href="#" />'
                if (o.elToWrap) {
                  var $t = $node.orphans(), $s = $node.find(o.elToWrap);
                  $t.add($s).wrapAll(anchor);
                } else {
                  $node.orphans().wrap(anchor);
                }
            }
          } else {
            $node.addClass('last-child');
            if (o.lastChild && $node.find('> a').length) {$node.find('> a').addClass("trigger").css('display', "block");}
          }
      });
      // init state
      $(El + ' a.trigger').closest(o.wrapper).find('> ' + o.next).not('.shown').hide().closest(o.wrapper).find('a.open').removeClass('open').data('state', 0);
      if (o.activeLink) {
          var loc,
              fullURL = window.location.href,
              path = window.location.pathname.split( '/' ),
              page = path[path.length-1];
              (o.uri == 'full') ? loc = fullURL : loc = page;
          $(Obj + ' a:not([href $= "#"])[href$="' + loc + '"]').addClass('active').parent().attr('id', 'current').closest(o.obj).addClass('current');
          if (o.shift && $(Obj + ' a.active').closest(o.wrapper).prev(o.wrapper).length) {
            var $currentWrap = $(Obj + ' a.active').closest(o.wrapper),
                $curentStack = $currentWrap.nextAll().andSelf(),
                $siblings = $currentWrap.siblings(o.wrapper),
                $first = $siblings.filter(":first");
            if (o.shift == 'clicked' || (o.shift == 'all' && $siblings.length)) {
                $currentWrap.insertBefore($first).addClass('shown').siblings(o.wrapper).removeClass('shown');
            }
            if (o.shift == 'all' && $siblings.length > 1) {$curentStack.insertBefore($first);}
          }
      }
      if (o.initShow) {
        $(Obj).find(o.initShow).show().addClass('shown')
          .parents(Obj + ' ' + o.next).show().addClass('shown').end()
          .parents(o.wrapper).find('> a.trigger, > ' + o.el + ' a.trigger').addClass('open').data('state', 1);
          if (o.expandSub) {$(Obj + ' ' + o.initShow).children(o.next).show().end().find('> a').addClass('open').data('state', 1 );}
      }
      // event
      if (o.event == 'click') {
          var ev = 'click';
      } else  {
          if (o.focus) {var f = ' focus';} else {var f = '';}
          var ev = 'mouseenter' + f;
      }
      var scrollElem;
      (typeof scrollableElement == 'function') ? (scrollElem = scrollableElement('html', 'body')) : (scrollElem = 'html, body');

      // The event handler is bound to the "accordion" element
      // The event is filtered to only fire when an <a class="trigger"> was clicked on.
      $(Obj).delegate('a.trigger', ev, function(ev) {
          var $thislink = $(this),
              $thisWrapper = $thislink.closest(o.wrapper),
              $nextEl = $thisWrapper.find('> ' + o.next),
              $siblings = $thisWrapper.siblings(o.wrapper),
              $trigger = $(El + ' a.trigger'),
              $shownEl = $thisWrapper.siblings(o.wrapper).find('>' + o.next + ':visible'),
              shownElOffset;
              $shownEl.length ? shownElOffset = $shownEl.offset().top : shownElOffset = false;
              
          function action(obj) {
             if (($nextEl).length && $thislink.data('state') && (o.collapsible)) {
                  $thislink.removeClass('open');
                  $nextEl.filter(':visible')[o.hideMethod](o.hideSpeed, function() {$thislink.data('state', 0);});
              }
              if (($nextEl.length && !$thislink.data('state')) || (!($nextEl).length && $thislink.closest(o.wrapper).not('.shown'))) {
                  if (!o.standardExpansible) {
                    $siblings.find('> a.open, >'+ o.el + ' a.open').removeClass('open').data('state', 0).end()
                    .find('> ' + o.next + ':visible')[o.hideMethod](o.hideSpeed);
                    if (shownElOffset && shownElOffset < $(window).scrollTop()) {$(scrollElem).animate({scrollTop: shownElOffset}, o.scrollSpeed);}
                  }
                  $thislink.addClass('open');
                  $nextEl.filter(':hidden')[o.showMethod](o.showSpeed, function() {$thislink.data('state', 1);});
              }
              if (o.shift && $thisWrapper.prev(o.wrapper).length) {
                var $thisStack = $thisWrapper.nextAll().andSelf(),
                    $first = $siblings.filter(":first");
                if (o.shift == 'clicked' || (o.shift == 'all' && $siblings.length)) {
                  $thisWrapper.insertBefore($first).addClass('shown').siblings(o.wrapper).removeClass('shown');
                }
                if (o.shift == 'all' && $siblings.length > 1) {$thisStack.insertBefore($first);}
              }
          }
          if (o.event == 'click') {
              action($trigger); 
              if ($thislink.is('[href $= "#"]')) {
                  return false;
              } else {
                  if ($.isFunction(o.retFunc)) {
                    return o.retFunc($thislink) 
                  } else {
                    return true;
                  }
              }
          }
          if (o.event != 'click') {
              hTimeout = window.setTimeout(function() {
                  action($trigger);
              }, o.interval);        
              $thislink.click(function() {
                  $thislink.blur();
                  if ($thislink.attr('href')== '#') {
                      $thislink.blur();
                      return false;
                  }
              });
          }
      });
      if (o.event != 'click') {$(Obj).delegate('a.trigger', 'mouseleave', function() {window.clearTimeout(hTimeout); });}
      
      /* -----------------------------------------------
      // http://www.learningjquery.com/2007/10/improved-animated-scrolling-script-for-same-page-links:
      -------------------------------------------------- */
      function scrollableElement(els) {
        for (var i = 0, argLength = arguments.length; i < argLength; i++) {
          var el = arguments[i],
              $scrollElement = $(el);
          if ($scrollElement.scrollTop() > 0) {
            return el;
          } else {
            $scrollElement.scrollTop(1);
            var isScrollable = $scrollElement.scrollTop() > 0;
            $scrollElement.scrollTop(0);
            if (isScrollable) {
              return el;
            }
          }
        };
        return [];
      }; 
      /* ----------------------------------------------- */
});};
$.fn.nestedAccordion.defaults = {
  container : true, // {true} if the plugin is called on the closest named container, {false} if the pligin is called on the accordion element
  obj : 'ul', // the element which contains the accordion - 'ul', 'ol', 'div' 
  objClass : '.accordion', // the class name of the accordion - required if you call the accordion on the container
  objID : '', // the ID of the accordion (optional)
  wrapper :'li', // the common parent of 'a.trigger' and 'o.next' - 'li', 'div'
  el : 'li', // the parent of 'a.trigger' - 'li', '.h'
  head : '', // the headings that are parents of 'a.trigger' (if any)
  next : 'ul', // the collapsible element - 'ul', 'ol', 'div'
  initShow : '', // the initially expanded section (optional)
  expandSub : true, // {true} forces the sub-content under the 'current' item to be expanded on page load
  showMethod : 'slideDown', // 'slideDown', 'show', 'fadeIn', or custom
  hideMethod : 'slideUp', // 'slideUp', 'hide', 'fadeOut', or custom
  showSpeed : 400,
  hideSpeed : 800,
  scrollSpeed : 600, //speed of repositioning the newly opened section when it is pushed off screen.
  activeLink : true, //{true} if the accordion is used for site navigation
  event : 'click', //'click', 'hover'
  focus : true, // it is needed for  keyboard accessibility when we use {event:'hover'}
  interval : 400, // time-interval for delayed actions used to prevent the accidental activation of animations when we use {event:hover} (in milliseconds)
  collapsible : true, // {true} - makes the accordion fully collapsible, {false} - forces one section to be open at any time
  standardExpansible : false, //if {true}, the functonality will be standard Expand/Collapse without 'accordion' effect
  lastChild : true, //if {true}, the items without sub-elements will also trigger the 'accordion' animation
  shift: false, // false, 'clicked', 'all'. If 'clicked', the clicked item will be moved to the first position inside its level, 
                // If 'all', the clicked item and all following siblings will be moved to the top
  elToWrap: null, // null, or the element, besides the text node, to be wrapped by the trigger, e.g. 'span:first'
  uri : 'full', // 
  retFunc: null //
};
/* ---------------------------------------------
Feel free to remove the following code if you don't need these custom animations.
------------------------------------------------ */
//credit: http://jquery.malsup.com/fadetest.html
$.fn.slideFadeDown = function(speed, callback) { 
  return this.animate({opacity: 'show', height: 'show'}, speed, function() { 
    if (jQuery.browser.msie) { this.style.removeAttribute('filter'); }
    if (jQuery.isFunction(callback)) { callback(); }
  }); 
}; 
$.fn.slideFadeUp = function(speed, callback) { 
  return this.animate({opacity: 'hide', height: 'hide'}, speed, function() { 
    if (jQuery.browser.msie) { this.style.removeAttribute('filter'); }
    if (jQuery.isFunction(callback)) { callback(); }
  }); 
}; 
/* --- end of the optional code --- */
})(jQuery);
///////////////////////////
// The plugin can be called on the accordion element or on its closest named container.
// If the plugin is called on the closest named container, we can initialize all the accordions residing in a given section with just one call.
// EXAMPLES:
/* ---
$(function() {
// If the closest named container = #container1 or the accordion element is <ul id="subnavigation">:
/// Standard nested lists:
  $('#container1').accordion(); // we are calling the plugin on the closest named container
  $('#subnavigation').accordion({container:false}); // we are calling the plugin on the accordion element
  // this will expand the sub-list with "id=current", when the accordion is initialized:
  $('#subnavigation').accordion({container:false, initShow : "#current"});
  // this will expand/collapse the sub-list when the mouse hovers over the trigger element:
  $('#container1').accordion({event : "hover", initShow : "#current"});
 
// If the closest named container = #container2
/// Nested Lists + Headings + DIVs:
  $('#container2').accordion({el: '.h', head: 'h4, h5', next: 'div'});
  $('#container2').accordion({el: '.h', head: 'h4, h5', next: 'div', initShow : 'div.outer:eq(0)'});
  
/// Nested DIVs + Headings:
  $('#container2').accordion({obj: 'div', wrapper: 'div', el: '.h', head: 'h4, h5', next: 'div.outer'});
  $('#container2').accordion({objID: '#acc2', obj: 'div', wrapper: 'div', el: '.h', head: 'h4, h5', next: 'div.outer', initShow : '.shown', shift: 'all'});
});

/// We can globally replace the defaults, for example:
  $.fn.nestedAccordion.defaults.initShow = "#current";
--- */
/// Example options for Hover Accordion:
/* ---
$.fn.nestedAccordion.defaults.container=false;
$.fn.nestedAccordion.defaults.event="hover";
$.fn.nestedAccordion.defaults.focus=false; // Optional. If it is possible, use {focus: true}, since {focus: false} will break the keyboard accessibility
$.fn.nestedAccordion.defaults.initShow="#current";
$.fn.nestedAccordion.defaults.lastChild=false;
--- */