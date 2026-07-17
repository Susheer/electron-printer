# Changelog

All notable changes to this project will be documented in this file.

The format is based on Keep a Changelog,
and this project adheres to Semantic Versioning.

---

# Changelog

## [4.0.1] - 2026-07-17

### Added
- Initial N-API v8 binary distribution.
- Windows x64 prebuilt binaries.
- Windows ia32 prebuilt binaries.

### Changed
- Switched to versioned N-API binary layout.
- Simplified npm package contents.
- Refactored build system to use a configurable build matrix.

### Breaking
- Native source files are no longer distributed through npm.
- Package now expects prebuilt binaries instead of compiling during installation.

## 3.1.0
- remove node-gyp fallback from the install script
- limit published package contents using the files field
- add grunt release helper script
- prepare package metadata for prebuilt binary distribution
- bump package version to 3.1.0
