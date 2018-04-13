// This file was automatically generated by opt2cpp.
// It is part of the dvisvgm package and published under the terms
// of the GNU General Public License version 3, or (at your option) any later version.
// See file COPYING for further details.
// Copyright (C) 2016-2018 Martin Gieseking <martin.gieseking@uos.de>

#ifndef COMMANDLINE_HPP
#define COMMANDLINE_HPP

#include <config.h>
#include <array>
#include <vector>
#include "CLCommandLine.hpp"

using CL::Option;
using CL::TypedOption;

class CommandLine : public CL::CommandLine {
	public:
		CommandLine () : CL::CommandLine(
			"This program converts DVI files, as created by TeX/LaTeX, to\nthe XML-based scalable vector graphics format SVG.",
			"[options] dvifile\n-E [options] epsfile",
			"Copyright (C) 2005-2018 Martin Gieseking <martin.gieseking@uos.de>"
		) {}

		CommandLine (int argc, char **argv) : CommandLine() {
			parse(argc, argv);
		}

		// option variables
		TypedOption<std::string, Option::ArgMode::REQUIRED> bboxOpt {"bbox", 'b', "size", "min", "set size of bounding box"};
		TypedOption<std::string, Option::ArgMode::OPTIONAL> cacheOpt {"cache", 'C', "dir", "set/print path of cache directory"};
		Option clipjoinOpt {"clipjoin", 'j', "compute intersection of clipping paths"};
		Option colorOpt {"color", '\0', "colorize messages"};
		Option colornamesOpt {"colornames", '\0', "prefer color names to RGB values if possible"};
		Option commentsOpt {"comments", '\0', "add comments with additional information"};
		Option epsOpt {"eps", 'E', "convert an EPS file to SVG"};
		Option exactOpt {"exact", 'e', "compute exact glyph boxes"};
		TypedOption<std::string, Option::ArgMode::REQUIRED> fontFormatOpt {"font-format", 'f', "format", "svg", "select file format of embedded fonts"};
		TypedOption<std::string, Option::ArgMode::REQUIRED> fontmapOpt {"fontmap", 'm', "filenames", "evaluate (additional) font map files"};
		Option gradOverlapOpt {"grad-overlap", '\0', "create overlapping color gradient segments"};
		TypedOption<int, Option::ArgMode::REQUIRED> gradSegmentsOpt {"grad-segments", '\0', "number", 20, "number of color gradient segments per row"};
		TypedOption<double, Option::ArgMode::REQUIRED> gradSimplifyOpt {"grad-simplify", '\0', "delta", 0.05, "reduce level of detail for small segments"};
		TypedOption<int, Option::ArgMode::OPTIONAL> helpOpt {"help", 'h', "mode", 0, "print this summary of options and exit"};
		Option keepOpt {"keep", '\0', "keep temporary files"};
		TypedOption<std::string, Option::ArgMode::REQUIRED> libgsOpt {"libgs", '\0', "filename", "set name of Ghostscript shared library"};
		TypedOption<std::string, Option::ArgMode::REQUIRED> linkmarkOpt {"linkmark", 'L', "style", "box", "select how to mark hyperlinked areas"};
		Option listSpecialsOpt {"list-specials", 'l', "print supported special sets and exit"};
		TypedOption<double, Option::ArgMode::REQUIRED> magOpt {"mag", 'M', "factor", 4, "magnification of Metafont output"};
		TypedOption<int, Option::ArgMode::OPTIONAL> noFontsOpt {"no-fonts", 'n', "variant", 0, "draw glyphs by using path elements"};
		Option noMergeOpt {"no-merge", '\0', "don't merge adjacent text elements"};
		Option noMktexmfOpt {"no-mktexmf", '\0', "don't try to create missing fonts"};
		TypedOption<std::string, Option::ArgMode::OPTIONAL> noSpecialsOpt {"no-specials", 'S', "prefixes", "don't process [selected] specials"};
		Option noStylesOpt {"no-styles", '\0', "don't use CSS styles to reference fonts"};
		TypedOption<std::string, Option::ArgMode::REQUIRED> outputOpt {"output", 'o', "pattern", "set name pattern of output files"};
		TypedOption<std::string, Option::ArgMode::REQUIRED> pageOpt {"page", 'p', "ranges", "1", "choose page(s) to convert"};
		TypedOption<int, Option::ArgMode::REQUIRED> precisionOpt {"precision", 'd', "number", 0, "set number of decimal points (0-6)"};
		TypedOption<double, Option::ArgMode::OPTIONAL> progressOpt {"progress", 'P', "delay", 0.5, "enable progress indicator"};
		Option relativeOpt {"relative", 'R', "create relative path commands"};
		TypedOption<double, Option::ArgMode::REQUIRED> rotateOpt {"rotate", 'r', "angle", "rotate page content clockwise"};
		TypedOption<std::string, Option::ArgMode::REQUIRED> scaleOpt {"scale", 'c', "sx[,sy]", "scale page content"};
		Option stdinOpt {"stdin", '\0', "read DVI/EPS input from stdin"};
		Option stdoutOpt {"stdout", 's', "write SVG output to stdout"};
		TypedOption<std::string, Option::ArgMode::OPTIONAL> tmpdirOpt {"tmpdir", '\0', "path", "set/print the directory for temporary files"};
		TypedOption<bool, Option::ArgMode::OPTIONAL> traceAllOpt {"trace-all", 'a', "retrace", false, "trace all glyphs of bitmap fonts"};
		TypedOption<std::string, Option::ArgMode::REQUIRED> transformOpt {"transform", 'T', "commands", "transform page content"};
		TypedOption<std::string, Option::ArgMode::REQUIRED> translateOpt {"translate", 't', "tx[,ty]", "shift page content"};
		TypedOption<unsigned, Option::ArgMode::REQUIRED> verbosityOpt {"verbosity", 'v', "level", 7, "set verbosity level (0-7)"};
		TypedOption<bool, Option::ArgMode::OPTIONAL> versionOpt {"version", 'V', "extended", false, "print version and exit"};
		TypedOption<int, Option::ArgMode::OPTIONAL> zipOpt {"zip", 'z', "level", 9, "create compressed .svgz file"};
		TypedOption<double, Option::ArgMode::REQUIRED> zoomOpt {"zoom", 'Z', "factor", 1.0, "zoom page content"};

	protected:
		std::vector<OptSectPair>& options () const override {return _options;}
		const char* section (size_t n) const override {return n < _sections.size() ? _sections[n] : nullptr;}

	private:
		std::array<const char*, 5> _sections {{
			"Input options",
			"SVG output options",
			"SVG transformations",
			"Processing options",
			"Message options",
		}};

		mutable std::vector<OptSectPair> _options = {
			{&pageOpt, 0},
			{&fontmapOpt, 0},
#if !defined(DISABLE_GS)
			{&epsOpt, 0},
#endif
			{&stdinOpt, 0},
			{&bboxOpt, 1},
#if !defined(DISABLE_GS)
			{&clipjoinOpt, 1},
#endif
			{&colornamesOpt, 1},
			{&commentsOpt, 1},
#if !defined(DISABLE_WOFF)
			{&fontFormatOpt, 1},
#endif
#if !defined(DISABLE_GS)
			{&gradOverlapOpt, 1},
#endif
#if !defined(DISABLE_GS)
			{&gradSegmentsOpt, 1},
#endif
#if !defined(DISABLE_GS)
			{&gradSimplifyOpt, 1},
#endif
			{&linkmarkOpt, 1},
			{&outputOpt, 1},
			{&precisionOpt, 1},
			{&relativeOpt, 1},
			{&stdoutOpt, 1},
			{&tmpdirOpt, 1},
			{&noFontsOpt, 1},
			{&noMergeOpt, 1},
			{&noStylesOpt, 1},
			{&zipOpt, 1},
			{&rotateOpt, 2},
			{&scaleOpt, 2},
			{&translateOpt, 2},
			{&transformOpt, 2},
			{&zoomOpt, 2},
			{&cacheOpt, 3},
			{&exactOpt, 3},
			{&keepOpt, 3},
#if !defined(HAVE_LIBGS) && !defined(DISABLE_GS)
			{&libgsOpt, 3},
#endif
			{&magOpt, 3},
			{&noMktexmfOpt, 3},
			{&noSpecialsOpt, 3},
			{&traceAllOpt, 3},
			{&colorOpt, 4},
			{&helpOpt, 4},
			{&listSpecialsOpt, 4},
			{&progressOpt, 4},
			{&verbosityOpt, 4},
			{&versionOpt, 4},
		};
};

#endif

