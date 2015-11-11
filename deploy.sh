#! /bin/sh

home_app=~/app

if test -d /llj/app ; then
   home_app=/llj/app
fi

home_moon=${home_app}/moon

./configure --prefix=${home_moon}

if test -d ${home_moon}; then
    rm -rf ${home_moon}
fi

make install

make distclean
