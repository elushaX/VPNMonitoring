sudo systemctl stop /etc/systemd/system/VPNMonitoring.service
sudo systemctl disable /etc/systemd/system/VPNMonitoring.service
sudo rm /etc/systemd/system/VPNMonitoring.service
sudo systemctl daemon-reload
