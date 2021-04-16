#include <ctype.h>
#include <ruby.h>
#include <ruby/encoding.h>

// Makes a camelized version of the given string.
//
// string.gsub!(/(?:_|(\/))([a-z\d]*)/i) { "#{$1}#{inflections.acronyms[$2] || $2.capitalize}" }
// string.gsub!("/", "::")
//
static VALUE camelize(VALUE string, VALUE kwargs) {
  static ID keywords[1];
  CONST_ID(keywords[0], "acronyms");

  VALUE options[1] = { Qundef };
  rb_get_kwargs(kwargs, keywords, 0, 1, options);

  // Configure the options based on the inputted keyword arguments
  VALUE acronyms = options[0] == Qundef ? rb_hash_new() : options[0];

  return string;
}

// FastCamelize::camelize
static VALUE fast_camelize(int argc, VALUE* argv, VALUE self) {
  VALUE string = Qnil;
  VALUE kwargs = Qnil;
  rb_scan_args(argc, argv, "10:", &string, &kwargs);

  return RB_TYPE_P(string, T_STRING) ? camelize(string, kwargs) : Qnil;
}

// String#camelize
static VALUE string_camelize(int argc, VALUE* argv, VALUE self) {
  VALUE kwargs = Qnil;
  rb_scan_args(argc, argv, "00:", &kwargs);

  return camelize(self, kwargs);
}

// Hook into Ruby and define the FastCamelize::camelize and
// String#camelize.
void Init_fast_camelize(void) {
  VALUE rb_cFastCamelize = rb_define_module("FastCamelize");
  rb_define_singleton_method(rb_cFastCamelize, "camelize", fast_camelize, -1);
  rb_define_method(rb_cString, "camelize", string_camelize, -1);
}
