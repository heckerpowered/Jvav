# Build on macOS
xmake f -p macosx -a arm64 -m release
xmake

xmake f -p windows -a x64 -m release
xmake

# Restore
xmake f -p macosx -a arm64 -m debug