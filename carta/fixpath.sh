#!/bin/sh
cd ../build/

mkdir -p cpp/desktop/desktop.app/Contents/Frameworks/
cp cpp/core/libcore.1.dylib cpp/desktop/desktop.app/Contents/Frameworks/

# need to rm for qt creator 4.2, otherwise when build+run together will result in core/libcore.1.dylib not able find out qwt
rm cpp/core/libcore.1.dylib
cp cpp/CartaLib/libCartaLib.1.dylib cpp/desktop/desktop.app/Contents/Frameworks/

install_name_tool -change qwt.framework/Versions/6/qwt ../ThirdParty/qwt-6.1.2/lib/qwt.framework/Versions/6/qwt cpp/desktop/desktop.app/Contents/MacOS/desktop
install_name_tool -change qwt.framework/Versions/6/qwt ../ThirdParty/qwt-6.1.2/lib/qwt.framework/Versions/6/qwt cpp/desktop/desktop.app/Contents/Frameworks/libcore.1.dylib

# not sure the effect of the below line, try comment
# install_name_tool -change libplugin.dylib cpp/plugins/CasaImageLoader/libplugin.dylib cpp/plugins/ImageStatistics/libplugin.dylib
install_name_tool -change libcore.1.dylib  cpp/desktop/desktop.app/Contents/Frameworks/libcore.1.dylib cpp/plugins/ImageStatistics/libplugin.dylib

install_name_tool -change libCartaLib.1.dylib  cpp/desktop/desktop.app/Contents/Frameworks/libCartaLib.1.dylib cpp/plugins/ImageStatistics/libplugin.dylib
install_name_tool -change libcore.1.dylib  cpp/desktop/desktop.app/Contents/Frameworks/libcore.1.dylib cpp/desktop/desktop.app/Contents/MacOS/desktop
install_name_tool -change libCartaLib.1.dylib  cpp/desktop/desktop.app/Contents/Frameworks/libCartaLib.1.dylib cpp/desktop/desktop.app/Contents/MacOS/desktop
install_name_tool -change libCartaLib.1.dylib  cpp/desktop/desktop.app/Contents/Frameworks/libCartaLib.1.dylib cpp/desktop/desktop.app/Contents/Frameworks/libcore.1.dylib

for f in `find ../ -name libplugin.dylib`; do install_name_tool -change libcore.1.dylib  cpp/desktop/desktop.app/Contents/Frameworks/libcore.1.dylib $f; done
for f in `find ../ -name libplugin.dylib`; do install_name_tool -change libCartaLib.1.dylib  cpp/desktop/desktop.app/Contents/Frameworks/libCartaLib.1.dylib $f; done
for f in `find ../ -name "*.dylib"`; do install_name_tool -change libwcs.5.15.dylib  ../ThirdParty/wcslib/lib/libwcs.5.15.dylib $f; echo $f; done
