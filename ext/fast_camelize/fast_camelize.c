#include <ctype.h>
#include <stdbool.h>
#include <ruby.h>
#include <ruby/encoding.h>

/**
 * This function works by maintaining a state machine that tracks which
 * characters are allowed at any given state, and reacting appropriately. The
 * state machine looks like:
 *
 *         ┌ * ┐                    ┌ _/ ┐
 *         │   v                    │    v
 *    ┌─────────────┐           ┌─────────────┐
 *    │             │─── _/ ───>│             │
 * ──>│   DEFAULT   │           │   SEGSTART  │
 *    │             │<─── * ────│             │
 *    └─────────────┘           └─────────────┘
 *           ^                      │    ^
 *           *                      │    │
 *           │                      │    │
 *    ┌─────────────┐               │    |
 *    │             │<─ a-zA-Z0-9 ──┘    |
 *    │   SEGBODY   │                    |
 *    │             │──── _/ ────────────┘
 *    └─────────────┘
 *     │           ^
 *     └ a-zA-Z0-9 ┘
 */

// This is the current state of the state machine being used to compile the
// camelized string.
enum state {
  STATE_DEFAULT,
  STATE_SEGSTART,
  STATE_SEGBODY
};

// Determines if this is an ASCII alphanumeric character.
static inline int isasciialnum(int character) {
  return (
    (character >= 'a' && character <= 'z') ||
    (character >= 'A' && character <= 'Z') ||
    (character >= '0' && character <= '9')
  );
}

// Copies a segment of characters over onto the output buffer. If the segment
// is a key in the acronyms hash, then it's going to copy over the value from
// the hash instead.
static void copy_segment(char *result, size_t *result_size, char *segment, size_t *segment_size, VALUE acronyms, bool capitalize) {
  segment[*segment_size] = '\0';
  VALUE acronym = rb_hash_aref(acronyms, rb_str_new_cstr(segment));

  if (acronym == Qnil) {
    strncpy(result + *result_size, segment, *segment_size);
    if (capitalize) result[*result_size] = toupper(result[*result_size]);
    *result_size += *segment_size;
  } else {
    size_t acronym_size = RSTRING_LEN(acronym);
    strncpy(result + *result_size, StringValueCStr(acronym), acronym_size);
    *result_size += acronym_size;
  }

  *segment_size = 0;
}

// Makes a camelized version of the given string. Effectively implementing the
// following Ruby code:
//
// gsub!(/(?:_|(\/))([a-z\d]*)/i) { "#{$1}#{acronyms[$2] || $2.capitalize}" }
// gsub!("/", "::")
//
static VALUE camelize(VALUE string, VALUE kwargs) {
  static ID keywords[1];
  CONST_ID(keywords[0], "acronyms");

  VALUE options[1] = { Qundef };
  rb_get_kwargs(kwargs, keywords, 0, 1, options);

  // Configure the options based on the inputted keyword arguments
  VALUE acronyms = options[0] == Qundef ? rb_hash_new() : options[0];

  // Set the initial state and build a buffer for the resulting string
  enum state state = STATE_DEFAULT;

  char result[RSTRING_LEN(string) * sizeof(unsigned int) * 2];
  char segment[RSTRING_LEN(string) * sizeof(unsigned int) * 2];

  size_t result_size = 0;
  size_t segment_size = 0;

  // Set up a couple of variables necessary for iterating through the string
  rb_encoding *encoding = rb_enc_from_index(ENCODING_GET(string));
  int codepoint_size;
  unsigned int codepoint;

  // Determine the bounds of the string for our loop
  char *pointer = RSTRING_PTR(string);
  char *end = RSTRING_END(string);

  while (pointer < end) {
    codepoint = rb_enc_codepoint_len(pointer, end, &codepoint_size, encoding);
    pointer += codepoint_size;

    switch (state) {
      case STATE_DEFAULT:
        if (codepoint != '_' && codepoint != '/') {
          rb_enc_mbcput(codepoint, &segment[segment_size], encoding);
          segment_size += codepoint_size;
        } else {
          if (segment_size > 0) {
            copy_segment(result, &result_size, segment, &segment_size, acronyms, false);
          }
          if (codepoint == '/') {
            memcpy(result + result_size, "::", 2);
            result_size += 2;
          }
          state = STATE_SEGSTART;
        }

        break;
      case STATE_SEGSTART:
        if (codepoint == '_') {
          // do nothing
        } else if (codepoint == '/') {
          memcpy(result + result_size, "::", 2);
          result_size += 2;
        } else if (isasciialnum(codepoint)) {
          segment[segment_size++] = codepoint;
          state = STATE_SEGBODY;
        } else {
          rb_enc_mbcput(codepoint, &result[result_size], encoding);
          result_size += codepoint_size;
          state = STATE_DEFAULT;
        }
        break;
      case STATE_SEGBODY:
        if (isasciialnum(codepoint)) {
          segment[segment_size++] = codepoint;
        } else {
          copy_segment(result, &result_size, segment, &segment_size, acronyms, true);

          if (codepoint == '_') {
            state = STATE_SEGSTART;
          } else if (codepoint == '/') {
            memcpy(result + result_size, "::", 2);
            result_size += 2;
            state = STATE_SEGSTART;
          } else {
            rb_enc_mbcput(codepoint, &result[result_size], encoding);
            result_size += codepoint_size;
            state = STATE_DEFAULT;
          }
        }
        break;
    }
  }

  // If we're in the DEFAULT or SEGBODY state, then we need to flush out the
  // remaining segment.
  if ((state == STATE_SEGBODY || state == STATE_DEFAULT) && segment_size > 0) {
    copy_segment(result, &result_size, segment, &segment_size, acronyms, state == STATE_SEGBODY);
  }

  return rb_enc_str_new(result, result_size, encoding);
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
  VALUE rb_mFastCamelize = rb_define_module("FastCamelize");
  rb_define_singleton_method(rb_mFastCamelize, "camelize", fast_camelize, -1);
  rb_define_method(rb_cString, "camelize", string_camelize, -1);
}
