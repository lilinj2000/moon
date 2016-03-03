
home_3rd=~/3rd

if test -d /llj/3rd ; then
   home_3rd=/llj/3rd
fi

home_boost=$home_3rd/boost
home_ctp=$home_3rd/v6.3.6_20141230traderapi_linux64

export LD_LIBRARY_PATH=$home_boost/lib:$home_ctp

