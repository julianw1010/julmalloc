#alloc
gcc -shared -Wl,-soname,liballoctest.so.1 -o liballoc.so.1.0 build/alloc/CMakeFiles/*/sources/*.o
mkdir --parents build/lib/
mv liballoc.so.1.0 build/lib
ln -sf liballoc.so.1.0 build/lib/liballoc.so.1
ln -sf liballoc.so.1.0 build/lib/liballoc.so