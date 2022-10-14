make_install:
	sudo apt-get install make

# The arduino shall appear on the terminal. 
detect_arduino:
	ls /dev/tty*

# /!\ Reboot after. 
adduser_serial: 
	sudo adduser SERIAL dialout

# Install python serial. 
install_pyserial: 
	python3 -m pip install pyserial
# If error : 
install_python3-pip: 
	sudo apt install python3-pip