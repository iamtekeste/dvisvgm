// This file was automatically generated by opt2cpp.
// It is part of the dvisvgm package and published under the terms
// of the GNU General Public License version 3, or (at your option) any later version.
// See file COPYING for further details.
// (C) 2009-2013 Martin Gieseking <martin.gieseking@uos.de>

#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#if HAVE_CONFIG_H
#include "config.h"
#endif
#include "CmdLineParserBase.h"

class CommandLine : public CmdLineParserBase
{
   public:
      CommandLine () {init();}
      CommandLine (int argc, char **argv, bool printErrors) {parse(argc, argv, printErrors);}
      bool page_given () const {return _page_given;}
      const std::string& page_arg () const {return _page_arg;}
      bool fontmap_given () const {return _fontmap_given;}
      const std::string& fontmap_arg () const {return _fontmap_arg;}
      bool bbox_given () const {return _bbox_given;}
      const std::string& bbox_arg () const {return _bbox_arg;}
      bool output_given () const {return _output_given;}
      const std::string& output_arg () const {return _output_arg;}
      bool precision_given () const {return _precision_given;}
      int precision_arg () const {return _precision_arg;}
      bool stdout_given () const {return _stdout_given;}
      bool no_fonts_given () const {return _no_fonts_given;}
      int no_fonts_arg () const {return _no_fonts_arg;}
      bool no_styles_given () const {return _no_styles_given;}
      bool zip_given () const {return _zip_given;}
      int zip_arg () const {return _zip_arg;}
      bool rotate_given () const {return _rotate_given;}
      double rotate_arg () const {return _rotate_arg;}
      bool scale_given () const {return _scale_given;}
      const std::string& scale_arg () const {return _scale_arg;}
      bool translate_given () const {return _translate_given;}
      const std::string& translate_arg () const {return _translate_arg;}
      bool transform_given () const {return _transform_given;}
      const std::string& transform_arg () const {return _transform_arg;}
      bool cache_given () const {return _cache_given;}
      const std::string& cache_arg () const {return _cache_arg;}
      bool exact_given () const {return _exact_given;}
      bool keep_given () const {return _keep_given;}
#if !defined(HAVE_LIBGS) && !defined(DISABLE_GS)
      bool libgs_given () const {return _libgs_given;}
      const std::string& libgs_arg () const {return _libgs_arg;}
#endif
      bool mag_given () const {return _mag_given;}
      double mag_arg () const {return _mag_arg;}
      bool no_mktexmf_given () const {return _no_mktexmf_given;}
      bool no_specials_given () const {return _no_specials_given;}
      const std::string& no_specials_arg () const {return _no_specials_arg;}
      bool trace_all_given () const {return _trace_all_given;}
      bool trace_all_arg () const {return _trace_all_arg;}
      bool color_given () const {return _color_given;}
      bool help_given () const {return _help_given;}
      int help_arg () const {return _help_arg;}
      bool list_specials_given () const {return _list_specials_given;}
      bool progress_given () const {return _progress_given;}
      double progress_arg () const {return _progress_arg;}
      bool verbosity_given () const {return _verbosity_given;}
      unsigned verbosity_arg () const {return _verbosity_arg;}
      bool version_given () const {return _version_given;}
      bool version_arg () const {return _version_arg;}

   protected:
      void init ();
      const CmdLineParserBase::Option* options () const {return _options;}
      const char** helplines (size_t *numlines) const;
      void handle_page (InputReader &ir, const Option &opt, bool longopt);
      void handle_fontmap (InputReader &ir, const Option &opt, bool longopt);
      void handle_bbox (InputReader &ir, const Option &opt, bool longopt);
      void handle_output (InputReader &ir, const Option &opt, bool longopt);
      void handle_precision (InputReader &ir, const Option &opt, bool longopt);
      void handle_stdout (InputReader &ir, const Option &opt, bool longopt);
      void handle_no_fonts (InputReader &ir, const Option &opt, bool longopt);
      void handle_no_styles (InputReader &ir, const Option &opt, bool longopt);
      void handle_zip (InputReader &ir, const Option &opt, bool longopt);
      void handle_rotate (InputReader &ir, const Option &opt, bool longopt);
      void handle_scale (InputReader &ir, const Option &opt, bool longopt);
      void handle_translate (InputReader &ir, const Option &opt, bool longopt);
      void handle_transform (InputReader &ir, const Option &opt, bool longopt);
      void handle_cache (InputReader &ir, const Option &opt, bool longopt);
      void handle_exact (InputReader &ir, const Option &opt, bool longopt);
      void handle_keep (InputReader &ir, const Option &opt, bool longopt);
#if !defined(HAVE_LIBGS) && !defined(DISABLE_GS)
      void handle_libgs (InputReader &ir, const Option &opt, bool longopt);
#endif
      void handle_mag (InputReader &ir, const Option &opt, bool longopt);
      void handle_no_mktexmf (InputReader &ir, const Option &opt, bool longopt);
      void handle_no_specials (InputReader &ir, const Option &opt, bool longopt);
      void handle_trace_all (InputReader &ir, const Option &opt, bool longopt);
      void handle_color (InputReader &ir, const Option &opt, bool longopt);
      void handle_help (InputReader &ir, const Option &opt, bool longopt);
      void handle_list_specials (InputReader &ir, const Option &opt, bool longopt);
      void handle_progress (InputReader &ir, const Option &opt, bool longopt);
      void handle_verbosity (InputReader &ir, const Option &opt, bool longopt);
      void handle_version (InputReader &ir, const Option &opt, bool longopt);

   private:
      static const CmdLineParserBase::Option _options[];
      bool _page_given;
      std::string _page_arg;
      bool _fontmap_given;
      std::string _fontmap_arg;
      bool _bbox_given;
      std::string _bbox_arg;
      bool _output_given;
      std::string _output_arg;
      bool _precision_given;
      int _precision_arg;
      bool _stdout_given;
      bool _no_fonts_given;
      int _no_fonts_arg;
      bool _no_styles_given;
      bool _zip_given;
      int _zip_arg;
      bool _rotate_given;
      double _rotate_arg;
      bool _scale_given;
      std::string _scale_arg;
      bool _translate_given;
      std::string _translate_arg;
      bool _transform_given;
      std::string _transform_arg;
      bool _cache_given;
      std::string _cache_arg;
      bool _exact_given;
      bool _keep_given;
#if !defined(HAVE_LIBGS) && !defined(DISABLE_GS)
      bool _libgs_given;
      std::string _libgs_arg;
#endif
      bool _mag_given;
      double _mag_arg;
      bool _no_mktexmf_given;
      bool _no_specials_given;
      std::string _no_specials_arg;
      bool _trace_all_given;
      bool _trace_all_arg;
      bool _color_given;
      bool _help_given;
      int _help_arg;
      bool _list_specials_given;
      bool _progress_given;
      double _progress_arg;
      bool _verbosity_given;
      unsigned _verbosity_arg;
      bool _version_given;
      bool _version_arg;
};

#endif
