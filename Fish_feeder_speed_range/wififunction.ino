void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Connecting to");
  display.setCursor(0,10);
  display.println(ssid);
  display.display();

 

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  display.setCursor(0,40);
  display.println("IP address:");
  display.println(WiFi.localIP());
  display.display();

  delay(5000);
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  if (String(topic) == "fishfeederR/relay1") {
    Serial.print("Changing output to ");
   
    if(messageTemp == "A"){
      Serial.println("Kelajuan High");
      ledcWrite(3, 255);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
      ledcWrite(4, 0);
      client.publish("fishfeederR/relay1/status", "Food Feeder RUN HIGH");
    } 

   else if(messageTemp == "B"){
      Serial.println("Kelajuan High");
      ledcWrite(3, 200);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
      ledcWrite(4, 0);
      client.publish("fishfeederR/relay1/status", "Food Feeder RUN MEDIUM");
    }

    else if(messageTemp == "C"){
      Serial.println("Kelajuan lOW");
      ledcWrite(3,150);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
      ledcWrite(4, 0);
      client.publish("fishfeederR/relay1/status", "Food Feeder RUN LOW");
    }

    else if(messageTemp == "S"){
      Serial.println("Stop");
      ledcWrite(3, 0);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
      ledcWrite(4, 0);
      client.publish("fishfeederR/relay1/status", "Food Feeder STOP ");
      
    }

     else if(messageTemp == "T"){
      Serial.println("Motor Run 5 Second");
      client.publish("fishfeederR/relay1/status", "Food Feeder Run 5 Second");
      ledcWrite(3, 255);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
      ledcWrite(4, 0);
      delay(5000);

      ledcWrite(3, 0);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
      ledcWrite(4, 0);
       client.publish("fishfeederR/relay1/status", "Food Feeder STOP ");
    }

    else if(messageTemp == "R"){
      Serial.println("Motor Run 10 Second");
      client.publish("fishfeederR/relay1/status", "Food Feeder Run 10 Second");
      ledcWrite(3, 255);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
      ledcWrite(4, 0);
      delay(10000);

      ledcWrite(3, 0);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
      ledcWrite(4, 0);
       client.publish("fishfeederR/relay1/status", "Food Feeder STOP ");
    }


   else {
    
   }
  }

 if (String(topic) == "fishfeederR/relay2") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(Relay02,HIGH);
      client.publish("fishfeederR/relay2/status", "RELAY 2 ON");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(Relay02,LOW);
       client.publish("fishfeederR/relay2/status", "RELAY 2 OFF");
    }
  }

   if (String(topic) == "fishfeederR/buzzer") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(buzzer,HIGH);
      client.publish("fishfeederR/buzzer/status", "RELAY 2 ON");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(buzzer,LOW);
       client.publish("fishfeederR/buzzer/status", "RELAY 2 OFF");
    }
  }
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("iotfishfeederR001")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("fishfeederR/relay1");
      client.subscribe("fishfeederR/relay2");
      client.subscribe("fishfeederR/buzzer");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}