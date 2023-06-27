import os

os.system("sudo apt install python3-pip")
os.system("python3 -m pip install pyautogui")
os.system("pip3 install pyautogui")
os.system("sudo apt-get install scrot")
os.system("sudo apt-get install python3-tk")
os.system("sudo apt-get install python3-dev")

import pyautogui

im2 = pyautogui.screenshot('my_screenshot.png')