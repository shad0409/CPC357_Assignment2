import pymongo
import paho.mqtt.client as mqtt
from datetime import datetime, timezone
import json

# MongoDB configuration
mongo_client = pymongo.MongoClient("mongodb://localhost:27017/")
db = mongo_client["SmartHome"]
collection = db["sensor_data"]

# MQTT configuration
mqtt_broker_address = "34.42.195.199"
mqtt_topic = "SmartHome"

def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code == 0:
        print(f"Successfully connected")
        client.subscribe(mqtt_topic)

def on_message(client, userdata, message):
    try:
        payload = json.loads(message.payload.decode("utf-8"))
        print(f"Received message: {payload}")
        
        # Convert MQTT timestamp to datetime
        timestamp = datetime.now(timezone.utc)
        payload['server_timestamp'] = timestamp.strftime("%Y-%m-%dT%H:%M:%S.%fZ")

        # Insert into MongoDB
        collection.insert_one(payload)
        print("Data ingested into MongoDB")
    except json.JSONDecodeError:
        print("Error: Invalid JSON payload")
    except Exception as e:
        print(f"Error processing message: {e}")

# Create a MQTT client instance
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

# Attach the callbacks using explicit methods
client.on_connect = on_connect
client.on_message = on_message

# Connect to MQTT broker
client.connect(mqtt_broker_address, 1883, 60)

# Start the MQTT loop
client.loop_forever()