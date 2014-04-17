/*************************************************************************
** HtmlSpecialHandler.cpp                                               **
**                                                                      **
** This file is part of dvisvgm -- the DVI to SVG converter             **
** Copyright (C) 2005-2014 Martin Gieseking <martin.gieseking@uos.de>   **
**                                                                      **
** This program is free software; you can redistribute it and/or        **
** modify it under the terms of the GNU General Public License as       **
** published by the Free Software Foundation; either version 3 of       **
** the License, or (at your option) any later version.                  **
**                                                                      **
** This program is distributed in the hope that it will be useful, but  **
** WITHOUT ANY WARRANTY; without even the implied warranty of           **
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         **
** GNU General Public License for more details.                         **
**                                                                      **
** You should have received a copy of the GNU General Public License    **
** along with this program; if not, see <http://www.gnu.org/licenses/>. **
*************************************************************************/

#include <config.h>
#include <sstream>
#include "HtmlSpecialHandler.h"
#include "InputReader.h"
#include "Message.h"
#include "SVGTree.h"
#include "XMLNode.h"

using namespace std;

// variable to select the link marker variant (none, underlined, boxed, or colored background)
HtmlSpecialHandler::LinkMarker HtmlSpecialHandler::LINK_MARKER = HtmlSpecialHandler::LM_LINE;
Color HtmlSpecialHandler::LINK_BGCOLOR;


bool HtmlSpecialHandler::process (const char *prefix, istream &is, SpecialActions *actions) {
	if (!actions)
		return true;
	_actions = actions;
	StreamInputReader ir(is);
	ir.skipSpace();
	map<string,string> attribs;
	map<string,string>::iterator it;
	if (ir.check("<a ") && ir.parseAttributes(attribs, '"') > 0) {
		if ((it = attribs.find("href")) != attribs.end())   // <a href="URI">
			hrefAnchor(it->second);
		else if ((it = attribs.find("name")) != attribs.end())  // <a name="ID">
			nameAnchor(it->second);
		else
			return false;  // none or only invalid attributes
	}
	else if (ir.check("</a>"))
		closeAnchor();
	else if (ir.check("<img src=")) {
	}
	else if (ir.check("<base ") && ir.parseAttributes(attribs, '"') > 0 && (it = attribs.find("href")) != attribs.end())
		_base = it->second;
	return true;
}


/** Handles anchors with href attribute: <a href="URI">...</a>
 *  @param uri value of href attribute */
void HtmlSpecialHandler::hrefAnchor (string uri) {
	closeAnchor();
	int id=0;
	if (uri[0] == '#') {  // reference to named anchor?
		string name = uri.substr(1);
		NamedAnchors::iterator it = _namedAnchors.find(name);
		if (it == _namedAnchors.end()) {
			id = _namedAnchors.size()+1;
			_namedAnchors[name] = NamedAnchor(-id, 0);
		}
	}
	if (!_base.empty() && uri.find("://") != string::npos) {
		if (*_base.rbegin() != '/' && uri[0] != '/' && uri[0] != '#')
			uri = "/" + uri;
		uri = _base + uri;
	}
	XMLElementNode *anchor = new XMLElementNode("a");
	anchor->addAttribute("xlink:href", id ? "#loc"+XMLString(id) : uri);
	anchor->addAttribute("xlink:title", uri);
	_actions->pushContextElement(anchor);
	_actions->bbox("{anchor}", true);  // start computing the bounding box of the linked area
	_depthThreshold = _actions->getDVIStackDepth();
	_anchorType = AT_HREF;
}


/** Handles anchors with name attribute: <a name="NAME">...</a>
 *  @param name value of name attribute */
void HtmlSpecialHandler::nameAnchor (const string &name) {
	closeAnchor();
	_anchorName = name;
	NamedAnchors::iterator it = _namedAnchors.find(name);
	if (it == _namedAnchors.end()) { // first occurrence of this anchor
		int id = _namedAnchors.size()+1;
		_namedAnchors[name] = NamedAnchor(id, _actions->getY());
	}
	else if (it->second.id < 0) {   // anchor referenced but not defined yet
		it->second.id *= -1;
		it->second.pos = _actions->getY();
	}
	else
		Message::wstream(true) << "named hyperref anchor '" << name << "' redefined\n";
	_anchorType = AT_NAME;
}


/** Handles the closing tag (</a> of the current anchor element. */
void HtmlSpecialHandler::closeAnchor () {
	if (_anchorType == AT_HREF) {
		markLinkedBox();
		_actions->popContextElement();
		_depthThreshold = 0;
	}
	else if (_anchorType == AT_NAME)
		_anchorName.clear();
	_anchorType = AT_NONE;
}


/** Marks a single rectangular area of the linked part of the document with a line or
 *  a box so that it's noticeable by the user. Additionally, an invisible rectangle is
 *  placed over this area in order to avoid flickering of the mouse cursor when moving
 *  it over the hyperlinked area. */
void HtmlSpecialHandler::markLinkedBox () {
	const BoundingBox &bbox = _actions->bbox("{anchor}");
	if (bbox.width() > 0 && bbox.height() > 0) {  // does the bounding box extend in both dimensions?
		if (LINK_MARKER != LM_NONE) {
			const double linewidth = min(0.5, bbox.height()/15);
			XMLElementNode *rect = new XMLElementNode("rect");
			double x = bbox.minX();
			double y = bbox.maxY()+linewidth;
			double w = bbox.width();
			double h = linewidth;
			if (LINK_MARKER == LM_LINE)
				rect->addAttribute("fill", _actions->getColor().rgbString());
			else {
				x -= linewidth;
				y = bbox.minY()-linewidth;
				w += 2*linewidth;
				h += bbox.height()+linewidth;
				if (LINK_MARKER == LM_BGCOLOR)
					rect->addAttribute("fill", LINK_BGCOLOR.rgbString());
				else {  // LM_BOX
					rect->addAttribute("fill", "none");
					rect->addAttribute("stroke", _actions->getColor().rgbString());
					rect->addAttribute("stroke-width", linewidth);
				}
			}
			rect->addAttribute("x", x);
			rect->addAttribute("y", y);
			rect->addAttribute("width", w);
			rect->addAttribute("height", h);
			_actions->prependToPage(rect);
			if (LINK_MARKER == LM_BOX) {
				x -= linewidth;
				y -= linewidth;
				w += 2*linewidth;
				h += 2*linewidth;
			}
			_actions->embed(BoundingBox(x, y, x+w, y+h));
		}
		// Create an invisible rectangle around the linked area so that it's easier to access.
		// This is only necessary when using paths rather than real text elements together with fonts.
		if (!SVGTree::USE_FONTS) {
			XMLElementNode *rect = new XMLElementNode("rect");
			rect->addAttribute("x", bbox.minX());
			rect->addAttribute("y", bbox.minY());
			rect->addAttribute("width", bbox.width());
			rect->addAttribute("height", bbox.height());
			rect->addAttribute("fill", "white");
			rect->addAttribute("fill-opacity", 0);
			_actions->appendToPage(rect);
		}
	}
}


/** This method is called every time the DVI position changes. */
void HtmlSpecialHandler::dviMovedTo (double x, double y) {
	if (_actions && _anchorType != AT_NONE) {
		// Start a new box if the current depth of the DVI stack underruns
		// the initial threshold which indicates a line break.
		if (_actions->getDVIStackDepth() < _depthThreshold) {
			markLinkedBox();
			_depthThreshold = _actions->getDVIStackDepth();
			_actions->bbox("{anchor}", true);  // start a new box on the new line
		}
	}
}


void HtmlSpecialHandler::dviEndPage (unsigned pageno) {
	if (_actions) {
		// create views for all collected named anchors defined on the recent page
		const BoundingBox &pagebox = _actions->bbox();
		for (NamedAnchors::iterator it=_namedAnchors.begin(); it != _namedAnchors.end(); ++it) {
			if (it->second.id > 0) {  // current anchor referenced?
				ostringstream oss;
				oss << pagebox.minX() << ' ' << it->second.pos << ' '
					 << pagebox.width() << ' ' << pagebox.height();
				XMLElementNode *view = new XMLElementNode("view");
				view->addAttribute("id", "loc"+XMLString(it->second.id));
				view->addAttribute("viewBox", oss.str());
				_actions->appendToDefs(view);
			}
		}
		closeAnchor();
		_namedAnchors.clear();
		_actions = 0;
	}
}


bool HtmlSpecialHandler::setLinkMarker (const string &type) {
	if (type.empty() || type == "none")
		LINK_MARKER = LM_NONE;
	else if (type == "line")
		LINK_MARKER = LM_LINE;
	else if (type == "box")
		LINK_MARKER = LM_BOX;
	else {
		if (!LINK_BGCOLOR.set(type, false))
			return false;
		LINK_MARKER = LM_BGCOLOR;
	}
	return true;
}


const char** HtmlSpecialHandler::prefixes () const {
	static const char *pfx[] = {"html:", 0};
	return pfx;
}
