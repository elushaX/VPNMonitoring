
wd=$(pwd)

rm VPNMonitoring.service
cp ./VPNMonitoringTemplate.service VPNMonitoring.service

echo "ExecStart=/bin/bash $wd/Monitoring.sh" >> VPNMonitoring.service
echo "WorkingDirectory=$wd" >> VPNMonitoring.service

sudo cp ./VPNMonitoring.service /etc/systemd/system/VPNMonitoring.service
sudo systemctl daemon-restart
sudo systemctl enable /etc/systemd/system/VPNMonitoring.service
sudo systemctl start VPNMonitoring.service
