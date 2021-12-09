# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/) and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.1.4] - 2021-12-09

### Changed

- Change `strncpy` instances to `memcpy` when the string isn't null terminated to avoid warnings.

## [0.1.3] - 2021-11-17

### Changed

- Require MFA for releasing.

## [0.1.2] - 2021-04-16

### Changed

- Ensure we patch String again so that it uses the ActiveSupport version.

## [0.1.1] - 2021-04-16

### Changed

- Better documentation and ensure we only capitalize non-start segments.

## [0.1.0] - 2021-04-16

### Added

- 🎉 Initial release! 🎉

[unreleased]: https://github.com/kddnewton/fast_camelize/compare/v0.1.4...HEAD
[0.1.4]: https://github.com/kddnewton/fast_camelize/compare/v0.1.3...v0.1.4
[0.1.3]: https://github.com/kddnewton/fast_camelize/compare/v0.1.2...v0.1.3
[0.1.2]: https://github.com/kddnewton/fast_camelize/compare/v0.1.1...v0.1.2
[0.1.1]: https://github.com/kddnewton/fast_camelize/compare/v0.1.0...v0.1.1
[0.1.0]: https://github.com/kddnewton/fast_camelize/compare/384d75...v0.1.0
