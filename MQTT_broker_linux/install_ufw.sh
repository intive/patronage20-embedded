sudo apt-get update
sudo apt-get install ufw

printf "# /etc/ufw/ufw.conf\nENABLED=no\nLOGLEVEL=low\nIPV6=yes" > /etc/ufw/ufw.conf

sudo ufw disable
sudo ufw reset

sudo ufw default deny incoming
sudo ufw default allow outgoing

sudo ufw allow from any to any port 21 proto tcp
sudo ufw allow from any to any port 80,443 proto tcp

#TODO: check if ufw configuration is complete
#sudo ufw enable