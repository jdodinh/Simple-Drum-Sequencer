// constants won't change. They're used here to set pin numbers:
#include <LiquidCrystal.h>
const int on_off_toggle = 3;     // the the number of the toggle pin
const int chan_ctrl = 4;
const int beat_ind_ctrl = 5;
const int toggle_beat = 6;
const int reset_all = 7;
const int tempo_pin = 1;

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

int on_off_toggle_state;
int chan_ctrl_state;
int beat_ind_ctrl_state;
int toggle_beat_state;
int reset_all_state;
const int num_channels = 10;
const int num_beats = 16;
int channel = -1;
char channel_string[2];
int beat_index = -1;
int toggle = 0;
char channels[num_beats][num_channels];
int tempo_read = -1;
double tempo = 120;
int counter = 0;

LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

void setup() {
  

  for (int i = 0; i < num_channels; i++) {

    for (int j = 0; j < num_beats; j++) {
      channels[j][i] = '-';
    }
  }
//  channel = 2;
  lcd.begin(16, 2);
  lcd.clear();
  
  lcd.print("DRUM MACHINE");
//  itoa(channel, channel_string, 2);
//  lcd.write(channel_string);
//  lcd.print(channel);
  lcd.setCursor(0, 1);
  lcd.print("v1.1");
//  for (int i = 0; i<num_beats; i++) {
//    lcd.write(str[i]);
//  }
  
  Serial.begin( 115200 );

  
  

  pinMode(on_off_toggle, INPUT);
  pinMode(chan_ctrl, INPUT);
  pinMode(beat_ind_ctrl, INPUT);
  pinMode(toggle_beat, INPUT);
  pinMode(reset_all, INPUT);
  pinMode(tempo_pin, INPUT);
}

void loop() {
  counter = (counter+1)%1000;
  
  // read the state of the pushbutton value:
  on_off_toggle_state = digitalRead(on_off_toggle);
  chan_ctrl_state = digitalRead(chan_ctrl);
  beat_ind_ctrl_state = digitalRead(beat_ind_ctrl);
  toggle_beat_state = digitalRead(toggle_beat);
  reset_all_state = digitalRead(reset_all);
  tempo_read = analogRead(tempo_pin);
  tempo = ((double)tempo_read/1023)*140 + 60;
  
  if (counter==0) {
    lcd.setCursor(13, 0);
    lcd.print(tempo);
    lcd.home();
//    Serial.print("6 ");
  if (tempo < 100) {
    Serial.print("0");
  }
    Serial.print((int) tempo);
    Serial.print("\n");
  }

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:

  // CHECK IF TOGGLE IS PRESSED
  if (on_off_toggle_state == HIGH) {
    // turn LED on:
    while (on_off_toggle_state == HIGH) {
      on_off_toggle_state = digitalRead(on_off_toggle);
    }
    delay(15);
    toggle = (toggle+1)%2;
    Serial.print("1\n");
    delay(15);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SEQUENCER");
    lcd.setCursor(0,1);
    if (toggle == 1) {
      lcd.print("ON\0");
    }
    else {
      lcd.print("OFF\0");
    }
  }

  // CHECK IF CHANNEL CONTROL STATE IS PRESSED
  else if (chan_ctrl_state == HIGH) {
    while (chan_ctrl_state == HIGH) {
      chan_ctrl_state = digitalRead(chan_ctrl);
    }
    delay(15);
    Serial.print("2\n");
    channel = (channel + 1);
    if (channel > num_channels) {
      channel = 0;
    }
    beat_index = 0;
    lcd.clear();
    if (channel > -1) {
      lcd.print("Channel ");
      lcd.print(channel);
      lcd.setCursor(0, 1);
      if (channel>0) {
        for (int i = 0; i < num_beats; i++) {
          lcd.print(channels[i][channel-1]);
        }
      }
      else {
        lcd.setCursor(0, 1);
        lcd.print("(neutral)");
        }
    }
    

  }

  else if (beat_ind_ctrl_state == HIGH) {
    while (beat_ind_ctrl_state == HIGH) {
      beat_ind_ctrl_state = digitalRead(beat_ind_ctrl);
    }
    delay(15);
    Serial.print("3\n");
//    sendValue()
    beat_index = beat_index + 1;
    if (beat_index > num_beats) {
      beat_index = 0;
    }
    lcd.clear();
    lcd.print("Channel ");
    lcd.print(channel);
    lcd.setCursor(0, 1);
    if (channel>0) {
      for (int i = 0; i < num_beats; i++) {
        if (i == beat_index-1) {
          lcd.print(".");
        }
        else {
          lcd.print(channels[i][channel-1]);
        }
      }
    }

  }

  else if (toggle_beat_state == HIGH) {
    while (toggle_beat_state == HIGH) {
      toggle_beat_state = digitalRead(toggle_beat);
    }
    delay(15);
    Serial.print("4\n");
    if (channels[beat_index-1][channel-1] == '-') {
      channels[beat_index-1][channel-1] = '*';
    }
    else {
      channels[beat_index-1][channel-1] = '-';
    }
    lcd.clear();
    lcd.print("Channel ");
    lcd.print(channel);
    lcd.setCursor(0, 1);
    for (int i = 0; i < num_beats; i++) {
      lcd.print(channels[i][channel-1]);
    }
//    Serial.print("\n");
  }
  else if (reset_all_state == HIGH) {
    while (reset_all_state == HIGH) {
      reset_all_state = digitalRead(reset_all);
    }
    delay(15);
    Serial.print("5\n");
    channel = 0;
    beat_index = 0;
    lcd.clear();
    lcd.print("RESET");
    lcd.setCursor(0, 1);
    lcd.print("COMPLETED");
    for (int i = 0; i < num_channels; i++) {

    for (int j = 0; j < num_beats; j++) {
      channels[j][i] = '-';
    }
  }

//    
  }
}
void showChannel(int channel) {
  delay(100);
//  lcd.clear();
//  delay(100);
//  lcd.setCursor(0, 0);           
//  lcd.print("Channel");
//  delay(100);
//  lcd.setCursor(0, 1);
//  delay(100);       
//  lcd.print("Channel Data");
}

void sendValue (int x){              // function to send the pin value followed by a "space". 
 Serial.print(x);
 Serial.write(32); 
}
