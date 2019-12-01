#!/bin/sh

echo "cd . && scons"
(cd . && scons -Q target=gcw0)

rm -f quartermaster.opk

	mksquashfs ./dist/gcw0/default.gcw0.desktop ./quartermaster ./quartermaster.png ./assets quartermaster.opk -all-root -no-xattrs -noappend -no-exports