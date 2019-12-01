#!/bin/sh

echo "cd . && scons -c"
(cd . && scons -c)

echo "rm -f ./config.log"
rm -f ./config.log

echo "rm -rf ./.scon*"
rm -rf ./.scon*

find . -type f -iname \*.o -delete

find . -type f -iname quartermaster -delete
