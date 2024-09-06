
wd=$(pwd)

rm VPNMonitoring.service
cp ./VPNMonitoringTemplate.service VPNMonitoring.service

echo "ExecStart=/bin/bash $wd/Monitoring.sh" >> VPNMonitoring.service
echo "WorkingDirectory=$wd" >> VPNMonitoring.service

sudo cp ./VPNMonitoring.service /etc/systemd/system/

sudo systemctl daemon-reload
sleep 1
sudo systemctl enable VPNMonitoring.service
sleep 1
sudo systemctl start VPNMonitoring.service
sudo systemctl status VPNMonitoring.service
