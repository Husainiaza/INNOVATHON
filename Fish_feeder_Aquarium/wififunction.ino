void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

 

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

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
  if (String(topic) == "fishfeeder/relay1") {
    Serial.print("Changing output to ");
   
    if(messageTemp == "A"){
      Serial.println("Kelajuan High");
      ledcWrite(3, 255);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
      ledcWrite(4, 0);
      client.publish("fishfeeder/relay1/status", "Food Feeder RUN HIGH");
    } 

   else if(messageTemp == "B"){
      Serial.println("Kelajuan High");
      ledcWrite(3, 200);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
      ledcWrite(4, 0);
      client.publish("fishfeeder/relay1/status", "Food Feeder RUN MEDIUM");
    }

    else if(messageTemp == "C"){
      Serial.println("Kelajuan lOW");
      ledcWrite(3,150);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
      ledcWrite(4, 0);
      client.publish("fishfeeder/relay1/status", "Food Feeder RUN LOW");
    }

    else if(messageTemp == "S"){
      Serial.println("Stop");
      ledcWrite(3, 0);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
      ledcWrite(4, 0);
      client.publish("fishfeeder/relay1/status", "Food Feeder STOP ");
      
    }

     else if(messageTemp == "T"){
      Serial.println("Motor Run 5 Second");
      client.publish("fishfeeder/relay1/status", "Food Feeder Run 5 Second");
      ledcWrite(3, 255);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
      ledcWrite(4, 0);
      delay(5000);

      ledcWrite(3, 0);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
      ledcWrite(4, 0);
       client.publish("fishfeeder/relay1/status", "Food Feeder STOP ");
    }

    else if(messageTemp == "R"){
      Serial.println("Motor Run 10 Second");
      client.publish("fishfeeder/relay1/status", "Food Feeder Run 10 Second");
      ledcWrite(3, 255);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
      ledcWrite(4, 0);
      delay(10000);

      ledcWrite(3, 0);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
      ledcWrite(4, 0);
       client.publish("fishfeeder/relay1/status", "Food Feeder STOP ");
    }


   else {
    
   }
  }

 if (String(topic) == "fishfeeder/relay2") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(Relay02,HIGH);
      client.publish("fishfeeder/relay2/status", "RELAY 2 ON");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(Relay02,LOW);
       client.publish("fishfeeder/relay2/status", "RELAY 2 OFF");
    }
  }

   if (String(topic) == "fishfeeder/buzzer") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(buzzer,HIGH);
      client.publish("fishfeeder/buzzer/status", "RELAY 2 ON");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(buzzer,LOW);
       client.publish("fishfeeder/buzzer/status", "RELAY 2 OFF");
    }
  }
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("iotfishfeeder001")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("fishfeeder/relay1");
      client.subscribe("fishfeeder/relay2");
      client.subscribe("fishfeeder/buzzer");
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