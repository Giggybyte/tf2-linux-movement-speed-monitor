echo running
cat /proc/$(pidof hl2_linux)/maps | grep /client.so | sed -n 1p | sed 's/-.*//' > start_address
