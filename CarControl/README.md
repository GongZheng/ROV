etc/network/interfaces setting:
source-directory /etc/network/interfaces.d

auto lo
iface lo inet loopback

iface eth0 inet manual

allow-hotplug wlan0
iface wlan0 inet manual
    #wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf

allow-hotplug wlan1
iface wlan1 inet manual
    wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf



/etc/rc.local setting:
_IP=$(hostname -I) || true
if [ "$_IP" ]; then
  printf "My IP address is %s\n" "$_IP"
fi
/home/pi/create_ap/create_ap wlan0 eth0 pi 123456789 &
python3 /home/pi/car_server_pwm.py &
python2 /home/pi/ROV/file/manage.py runserver 0.0.0.0:8080 &
exit 0

note:
you can exchange your file path!
