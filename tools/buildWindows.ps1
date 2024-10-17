# Build Win32 64bit
# node_modules\.bin\node-pre-gyp.cmd configure --target=$env:nodejs_version
# node_modules\.bin\node-pre-gyp.cmd build package --target=$env:nodejs_version --target_arch=x64

# Build Win32 32bit
# node_modules\.bin\node-pre-gyp.cmd configure --target=$env:nodejs_version
# node_modules\.bin\node-pre-gyp.cmd build package --target=$env:nodejs_version --target_arch=ia32

if ($env:build_electron -ne "true") {
  echo "Skipping Electron Build as flag not set"
  return
}

# Build Electron Versions
./tools/buildElectronWindows.ps1 32.1.2
./tools/buildElectronWindows.ps1 33.0.0
