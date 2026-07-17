# N-API Validation

## Objective

Validate that the distributed N-API binary loads successfully without local compilation.

## Validation Matrix

| Runtime | Version | Status |
|----------|---------|--------|
| Node.js | 20.x | ✅ |
| Electron | 42.x | ☐ |
| Electron | Latest Stable | ☐ |

## Validation Steps

### Node.js

```bash
npm install
npm test
```

Expected:

- Installation completes without rebuilding.
- Native module loads successfully.
- All exported APIs are available.

### Electron

```bash
npm install electron@42.7.0
```

Run an Electron application that imports:

```javascript
const printer = require("@thesusheer/electron-printer");
```

Expected:

- Module loads successfully.
- No ABI mismatch errors.
- No native compilation.