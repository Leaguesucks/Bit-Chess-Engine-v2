# Install necessary packages to run this project
# Run: chmod +x setup.sh to grant execution permission first

sudo apt update
sudo apt upgrade 
sudo apt install build-essential # Install C/C++ compilers
sudo apt install gdb # Install C/C++ debugger
sudo apt install python3-dev python3-venv python3-pip build-essential # Install tools for Python development
sudo apt install python3-tk tk-dev # Install tkinter

python3 -m venv venv
source venv/bin/activate
pip install --upgrade pip
pip install pillow