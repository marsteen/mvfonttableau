#!/bin/sh
find . -name "*.cpp" -exec svn add {} \;
find . -name "*.c" -exec svn add {} \;
find . -name "*.h" -exec svn add {} \;
find . -name "*.a" -exec svn add {} \;
find . -name "*.hpp" -exec svn add {} \;
find . -name "*.sh" -exec svn add {} \;

