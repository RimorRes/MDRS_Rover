int pull(int Timeout = 1000){ // Receive char text from serial port.
    Serial.setTimeout(Timeout);
    return Serial.readString();
}

void push(const char* txt){ // Send char text via serial port.
    Serial.println(txt);
}