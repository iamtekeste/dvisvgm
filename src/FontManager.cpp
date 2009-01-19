/***********************************************************************
** FontManager.cpp                                                    **
**                                                                    **
** This file is part of dvisvgm -- the DVI to SVG converter           **
** Copyright (C) 2005-2007 Martin Gieseking <martin.gieseking@uos.de> **
**                                                                    **
** This program is free software; you can redistribute it and/or      **
** modify it under the terms of the GNU General Public License        **
** as published by the Free Software Foundation; either version 2     **
** of the License, or (at your option) any later version.             **
**                                                                    **
** This program is distributed in the hope that it will be useful,    **
** but WITHOUT ANY WARRANTY; without even the implied warranty of     **
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      **
** GNU General Public License for more details.                       **
**                                                                    **
** You should have received a copy of the GNU General Public License  **
** along with this program; if not, write to the Free Software        **
** Foundation, Inc., 51 Franklin Street, Fifth Floor,                 **
** Boston, MA 02110-1301, USA.                                        **
***********************************************************************/
// $Id$

#include <cstdlib>
#include <fstream>
#include "Font.h"
#include "FontEncoding.h"
#include "FontManager.h"
#include "KPSFileFinder.h"
#include "Message.h"
#include "macros.h"

using namespace std;


FontManager::FontManager () 
{
	FontEncoding enc("base-MinionPro-ab"); // @@
	enc.read(); // @@
}


FontManager::~FontManager () {
	FORALL(_fonts, vector<Font*>::iterator, i)
		delete *i;
	typedef map<string,FontEncoding*>::iterator Iterator;
	FORALL(_encMap, Iterator, i)
		delete i->second;
}


/** Returns a unique ID that identifies the font. 
 *  @param n local font number, as used in DVI and VF files 
 *  @return non-negative font ID if font was found, -1 otherwise */
int FontManager::fontID (int n) const {
	if (_vfStack.empty()) {
		Num2IdMap::const_iterator it = _num2id.find(n);
		return (it == _num2id.end()) ? -1 : it->second;
	}
	VfNum2IdMap::const_iterator vit = _vfnum2id.find(_vfStack.top());
	if (vit == _vfnum2id.end())
		return -1;
	const Num2IdMap &num2id = vit->second;
	Num2IdMap::const_iterator it = num2id.find(n);
	return (it == num2id.end()) ? -1 : it->second;
}


int FontManager::fontID (const Font *font) const {
	for (unsigned i=0; i < _fonts.size(); i++)
		if (_fonts[i] == font)
			return i;
	return -1;
}


int FontManager::fontID (string name) const {
	map<string,int>::const_iterator it = _name2id.find(name);
	if (it == _name2id.end())
		return -1;
	return it->second;
}


int FontManager::fontnum (int id) const {
	if (id < 0 || size_t(id) > _fonts.size())
		return -1;
	if (_vfStack.empty()) {
		FORALL(_num2id, Num2IdMap::const_iterator, i)
			if (i->second == id)
				return i->first;
	}
	else {
		VfNum2IdMap::const_iterator it = _vfnum2id.find(_vfStack.top());
		if (it == _vfnum2id.end())
			return -1;
		const Num2IdMap &num2id = it->second;
		FORALL(num2id, Num2IdMap::const_iterator, i)
			if (i->second == id)
				return i->first;
	}
	return -1;
}


int FontManager::vfFirstFontNum (VirtualFont *vf) const {
	VfFirstFontMap::const_iterator it = _vfFirstFontMap.find(vf);
	return (it == _vfFirstFontMap.end()) ? -1 : it->second;
}


/** Returns a previously registered font.
 *  @param n local font number, as used in DVI and VF files 
 *  @return pointer to font if font was found, 0 otherwise */
Font* FontManager::getFont (int n) const {
	int id = fontID(n);
	return (id < 0) ? 0 : _fonts[id];
}


Font* FontManager::getFont (string name) const {
	int id = fontID(name);
	if (id < 0)
		return 0;
	return _fonts[id];
}


Font* FontManager::getFontById (int id) const {
	if (id < 0 || size_t(id) >= _fonts.size())
		return 0;
	return _fonts[id];
}


/** Returns the current active virtual font. */
VirtualFont* FontManager::getVF () const {
	return _vfStack.empty() ? 0 : _vfStack.top();
}


/** Registers a new font to be managed by the FontManager. If there is
 *  already a registered font assigned to number n, nothing happens.
 *  @param fontnum local font number, as used in DVI and VF files 
 *  @param name fontname, e.g. cmr10 
 *  @param checksum checksum to be compared with TFM checksum
 *  @param dsize design size in TeX point units
 *  @param ssize scaled size in TeX point units 
 *  @return id of registered font */
int FontManager::registerFont (UInt32 fontnum, string name, UInt32 checksum, double dsize, double ssize) {
	int id = fontID(fontnum);
	if (id >= 0)
		return id;

	Font *newfont = 0;
	int newid = _fonts.size();      // the new font gets this ID
	Name2IdMap::iterator it = _name2id.find(name);
	if (it != _name2id.end()) {  // font with same name already registered?
		Font *font = _fonts[it->second];
		newfont = font->clone(dsize, ssize);
	}
	else {
		if (KPSFileFinder::lookup(name+".pfb"))
			newfont = PhysicalFont::create(name, checksum, dsize, ssize, PhysicalFont::PFB);
		else if (KPSFileFinder::lookup(name+".ttf"))
			newfont = PhysicalFont::create(name, checksum, dsize, ssize, PhysicalFont::TTF);
		else if (KPSFileFinder::lookup(name+".vf"))
			newfont = VirtualFont::create(name, checksum, dsize, ssize);
		else if (KPSFileFinder::lookup(name+".mf"))
			newfont = PhysicalFont::create(name, checksum, dsize, ssize, PhysicalFont::MF);
		else {
			newfont = new EmptyFont(name);
			Message::wstream(true) << "font '" << name << "' not found\n";
		}
		_name2id[name] = newid;


		if (const char *encname = KPSFileFinder::lookupEncName(name)) {
			_encMap[name] = new FontEncoding(encname);
		}
	}
	_fonts.push_back(newfont);
	if (_vfStack.empty())  // register font referenced in dvi file?
		_num2id[fontnum] = newid;
	else {  // register font referenced in vf file
		VirtualFont *vf = const_cast<VirtualFont*>(_vfStack.top());
		_vfnum2id[vf][fontnum] = newid;
		if (_vfFirstFontMap.find(vf) == _vfFirstFontMap.end()) // first fontdef of VF?
			_vfFirstFontMap[vf] = fontnum;
	}
	return newid;
}


/** Enters a new virtual font frame. 
 *  This method must be called before processing a VF character.
 *  @param vf virtual font */
void FontManager::enterVF (VirtualFont *vf) {
	if (vf) 
		_vfStack.push(vf);
}


/** Leaves a previously entered virtual font frame. 
 *  @throw FontException if there is no VF frame to leave */
void FontManager::leaveVF () {
	if (!_vfStack.empty())
		_vfStack.pop();
}


void FontManager::assignVfChar (int c, vector<UInt8> *dvi) {
	if (!_vfStack.empty() && dvi)
		_vfStack.top()->assignChar(c, dvi);
}


int FontManager::decodeChar (int c) {
	return 0;
}

ostream& FontManager::write (ostream &os, Font *font, int level) {
#if 0
	if (font) {
		int id = -1;
		for (int i=0; i < fonts.size() && id < 0; i++)
			if (fonts[i] == font)
				id = i;

		VirtualFont *vf = dynamic_cast<VirtualFont*>(font);
		for (int j=0; j < level+1; j++)
			os << "  ";
		os << "id " << id 
			<< " fontnum " << fontnum(id) << " "
			<< (vf ? "VF" : "PF") << " " 
			<< font->name() 
			<< endl;
		
		if (vf) {
			enterVF(vf);
			const Num2IdMap &num2id = vfnum2id.find(vf)->second;
			FORALL(num2id, Num2IdMap::const_iterator, i) {
				Font *font = fonts[i->second];
				write(os, font, level+1);
			}
			leaveVF();
		}
	}
	else {
		for (int i=0; i < fonts.size(); i++)
			write(os, fonts[i], level);
		os << endl;
	}
#endif
	return os;
}
