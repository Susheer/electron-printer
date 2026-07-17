# Binary Distribution Strategy

## Overview

`electron-printer` distributes prebuilt native binaries to eliminate
the need for local native compilation during installation.

Supported installations download the appropriate binary from GitHub
Releases using `prebuild-install`.

---

# Distribution Flow

```
Maintainer
      │
      ▼
Build native binaries
      │
      ▼
Publish GitHub Release
      │
      ▼
Publish npm package
      │
      ▼
npm install
      │
      ▼
prebuild-install
      │
      ▼
Download matching binary
      │
      ▼
Installation complete
```

---

# Objectives

- Simplify installation.
- Reduce platform-specific setup.
- Improve release consistency.
- Reduce maintenance effort.
- Support long-term N-API compatibility.

---

# Binary Hosting

Binary artifacts are published as GitHub Release assets.

The npm package contains only the runtime JavaScript files and metadata
required for installation.

---

# Supported Platforms

Platform support is documented separately and may expand over time.

Current release validation targets:

- Windows x64
- Windows ia32

Additional platforms will be added as the release process evolves.

---

# Future Work

- Automated GitHub Release publishing
- Multi-platform CI builds
- Build matrix support
- Additional architectures
- Full N-API binary distribution