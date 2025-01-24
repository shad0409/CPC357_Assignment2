# Smart Home IoT System

IoT system for monitoring temperature, humidity, and motion detection using ESP32, DHT11, and PIR sensors with Google Cloud Platform integration.

## Prerequisites

- Google Cloud Platform account
- Python 3.8+
- Arduino IDE
- MongoDB
- Google Cloud SDK

## Environment Setup

### 1. GCP Configuration

1. Install Google Cloud SDK
2. Initialize and create VM instance:
```bash
# Initialize GCP
gcloud init

# Configure firewall rules
gcloud compute firewall-rules create allow-mqtt --allow=tcp:1883
```

## Server Configuration
```bash
# Install required packages

# Update package list
sudo apt update

# Install MongoDB
sudo apt install -y mongodb

# Start MongoDB Service
sudo systemctl start mongodb
sudo systemctl enable mongodb

# Install Python dependencies
pip install paho-mqtt pymongo
```

## Hardware and Software Deployment

### 1. ESP32 Setup
- Open Arduino IDE
- Install ESP32 board support
- Install required libraries:
    - WiFi
    - PubSubClient
    - DHT sensor library
- Open arduino.ino
- Update WiFi credentials and MQTT broker address
- Upload to ESP32

### 2. Python Script Deployment
- Clone repository
- Upload python script to VM Instance
- Run python3 PythonScript.py
