# N-API Migration Plan

## Status

**Proposed**

## Overview

Migrate `electron-printer` to N-API-based binary distribution to build a single native addon that works across supported Node.js runtimes implementing the same N-API version. This reduces the number of binaries that must be built, tested, and maintained while simplifying installation.

## Target Architecture

```text
              Build Once
                  │
                  ▼
      electron-printer.node
           (N-API v8)
                  │
    ┌─────────────┼─────────────┐
    ▼             ▼             ▼
 Node.js      Electron       NW.js
18,20,22...   28,35,42...   (N-API compatible)
```

## Current Installation

```text
npm install
   │
   ├── prebuild-install
   └── node-gyp rebuild (fallback)
```

## Target Installation

```text
npm install
   │
   └── Download N-API prebuilt binary
```

Supported users should not need:
- C/C++ compiler
- Python
- Visual Studio Build Tools
- `node-gyp`

## Migration Plan

1. Document the migration and binary distribution strategy.
2. Generate N-API prebuilt binaries for supported platforms.
3. Validate installation across supported runtimes.
4. Automate binary publishing and releases.
5. Update project documentation.

## Compatibility

This migration changes only the build and distribution process. No breaking JavaScript API changes are planned.

## Success Criteria

- One binary per platform/architecture and N-API version.
- Installation succeeds without native compilation.
- Automated binary publishing with each release.
- Updated documentation and release process.