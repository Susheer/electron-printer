# electron-printer Distribution Policy

This package now ships only the minimal runtime files and prebuilt binaries
required for production use. The full native C++ source code and build scripts
are intentionally excluded from the npm package.

## Purpose of This Change

Many production environments use automated security scanners and static analysis
tools on node_modules. Native C++ sources and build artifacts frequently trigger
false positives and add unnecessary overhead. To avoid this, the npm package
includes only:

- Runtime JavaScript files
- Prebuilt native binaries under /prebuilds
- package.json
- README.md
- Type definitions (if present)

## What This Means for Users

This change does *not* modify the runtime API or installation flow. Existing
applications continue working without any change.

However, developers who previously accessed or modified native code inside
node_modules will no longer find those files. Native development must be done
by cloning the GitHub repository directly.

## What This Means for Forks and Custom Builds

Fork maintainers or anyone needing custom native functionality must:

1. Clone the GitHub repository (not node_modules)
2. Build native binaries locally
3. Use those binaries through prebuilds or custom internal publish

The npm package is now strictly for production runtime usage.

## Summary

- Only runtime files + binaries are distributed
- Native sources and build scripts are excluded
- Reduces security scanner noise and compliance conflicts
- Native development must be done in the GitHub repo, not node_modules