
python3 ../graphana/trackBasicSystemLoad.py &
python3 ../graphana/trackAllTimeUsers.py &
python3 ../graphana/trackActiveUsers.py &

sudo ../shadowsocks/build/libSocks eth0 &

wait
