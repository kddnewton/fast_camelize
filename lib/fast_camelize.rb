# frozen_string_literal: true

require "fast_camelize/version"
require "fast_camelize/fast_camelize"

# By default, FastCamelize already defines String#camelize and
# FastCamelize::camelize. In the case that we're using ActiveSupport,
# however, there is the additional functionality of defined acronyms that we
# need to account for. In this case, we're going to first allow ActiveSupport to
# do that work, then we're going to do the actual camelization.
module FastCamelize
  # Override ActiveSupport::Inflector::camelize to use FastCamelize::camelize.
  module ActiveSupportInflectorPatch
    # Disabling rubocop here so that we can directly match the implementation in
    # the Rails source, so it's easier when Rails changes it.
    # rubocop:disable all
    def camelize(string, uppercase_first_letter = true)
      # This assignment is largely just here for type checking. This module is
      # always going to be included in ActiveSupport::Inflector, but rbs doesn't
      # have requires_ancestor-ish support yet.
      inflections = ActiveSupport::Inflector.inflections

      string = string.to_s
      if uppercase_first_letter
        string =
          string.sub(/^[a-z\d]*/) do |match|
            inflections.acronyms[match] || match.capitalize
          end
      else
        string =
          string.sub(inflections.acronyms_camelize_regex) do |match|
            match.downcase
          end
      end

      FastCamelize.camelize(string, acronyms: inflections.acronyms)
    end
    # rubocop:disable all
  end

  # Override String#camelize if we need to get back to the original behavior
  # that fast_camelize overroad.
  module ActiveSupportStringPatch
    def camelize(first_letter = :upper)
      case first_letter
      when :upper
        ActiveSupport::Inflector.camelize(self, true)
      when :lower
        ActiveSupport::Inflector.camelize(self, false)
      else
        # Skipping here because Steep doesn't understand that this is going to
        # be included into a class that has Kernel included.
        __skip__ =
          begin
            raise ArgumentError, "Invalid option, use either :upper or :lower."
          end
      end
    end
  end

  # Override ActiveSupport::Inflector::method_added so that if and when the
  # camelize method gets defined, we can immediately redefine it.
  module ActiveSupportDelayedPatch
    def method_added(method)
      FastCamelize.active_support if method == :camelize
      super
    end
  end

  # Hook into ActiveSupport::Inflector to take advantage of FastCamelize.
  def self.active_support
    ActiveSupport::Inflector.alias_method(:as_camelize, :camelize)
    ActiveSupport::Inflector.prepend(ActiveSupportInflectorPatch)
    String.prepend(ActiveSupportStringPatch)
  end
end

if defined?(ActiveSupport)
  FastCamelize.active_support
else
  module ActiveSupport
    module Inflector
      prepend(FastCamelize::ActiveSupportDelayedPatch)
    end
  end
end
