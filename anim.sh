destGif=animatedCPNtp20StationaryFrame.gif
convert -delay 50 -loop 0 dir-pics2/ntp20_fixed_cpframe_*png ${destGif}

if [ -e ${destGif} ] ; then
  eog ${destGif} &
fi

