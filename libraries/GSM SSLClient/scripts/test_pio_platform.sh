#!/bin/bash

# Function to list installed libraries
list_installed_libs() {
  echo "Listing installed libraries..."
  pio lib list
}

# Function to list installed platforms
list_installed_platforms() {
  echo "Listing installed platforms..."
  pio platform list
}

# Function to list installed frameworks for boards
list_installed_frameworks() {
  echo "Listing installed frameworks for boards..."
  pio boards --framework arduino
}

# Function to switch to a specific arduino-esp32 version
switch_esp32_version() {
  local version=$1
  echo "Switching to arduino-esp32 version $version..."
  pio platform install espressif32@$version
}

# Check if pio is installed
if ! command -v pio &> /dev/null; then
  echo "PlatformIO CLI (pio) could not be found, please install it first."
  exit 1
fi

# Main script
echo "PlatformIO CLI Utility Script"
echo "1. List installed libraries"
echo "2. List installed platforms"
echo "3. List installed frameworks for boards"
echo "4. Switch arduino-esp32 version"
echo "5. Exit"

while true; do
  read -p "Please select an option (1-5): " option
  case $option in
    1)
      list_installed_libs
      ;;
    2)
      list_installed_platforms
      ;;
    3)
      list_installed_frameworks
      ;;
    4)
      read -p "Enter the arduino-esp32 version to switch to (e.g., 2.0.16 or 3.0.0): " version
      switch_esp32_version $version
      ;;
    5)
      echo "Exiting..."
      exit 0
      ;;
    *)
      echo "Invalid option, please try again."
      ;;
  esac
done
