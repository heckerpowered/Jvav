# Build on macOS
xmake f -p macosx -a arm64 -m release
xmake -y

xmake f -p windows -a x64 -m release
xmake -y

# Restore
xmake f -p macosx -a arm64 -m debug