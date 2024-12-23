sudo apt update
sudo apt -y upgrade
sudo apt -y install ruby
sudo gem install shell
python -m venv ../tools/python
../tools/python/bin/pip install pyusb
sudo apt -y install minicom
echo "SUBSYSTEM==\"tty\", ATTRS{idVendor}==\"0483\",ATTRS{idProduct}==\"5740\", SYMLINK+=\"USB_SPIKE\",MODE=\"0666\"" > /tmp/serial_temp_rule
sudo mv /tmp/serial_temp_rule /etc/udev/rules.d/99-serial.rules
sudo udevadm control --reload
