#include <IRremote.h>
#include <IRremoteInt.h>

const int RECV_PIN = 5;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  irrecv.blink13(true);
  Serial.println("IR ready to receive.");
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("decoding... ");
    Serial.println(results.decode_type);
    if (results.decode_type == NEC) {
      Serial.print("NEC: ");
    } else if (results.decode_type == SONY) {
      Serial.print("SONY: ");
    } else if (results.decode_type == RC5) {
      Serial.print("RC5: ");
    } else if (results.decode_type == RC6) {
      Serial.print("RC6: ");
    } else if (results.decode_type == PANASONIC) {
      Serial.print("PANASONIC: ");
    } else if (results.decode_type == JVC) {
      Serial.print("JVC: ");
    } else if (results.decode_type == SAMSUNG) {
      Serial.print("SAMSUNG: ");
    } else if (results.decode_type == WHYNTER) {
      Serial.print("WHYNTER: ");
    } else if (results.decode_type == AIWA_RC_T501) {
      Serial.print("AIWA_RC_T501: ");
    } else if (results.decode_type == LG) {
      Serial.print("LG: ");
    } else if (results.decode_type == SANYO) {
      Serial.print("SANYO: ");
    } else if (results.decode_type == MITSUBISHI) {
      Serial.print("MITSUBISHI: ");
    } else if (results.decode_type == DISH) {
      Serial.print("DISH: ");
    } else if (results.decode_type == SHARP) {
      Serial.print("SHARP: ");
    } else if (results.decode_type == DENON) {
      Serial.print("DENON: ");
    } else if (results.decode_type == PRONTO) {
      Serial.print("PRONTO: ");
    //} else if (results.decode_type == LEGO_PF) {
    //  Serial.print("LEGO_PF: ");
    } else if (results.decode_type == UNKNOWN) {
      Serial.print("UNKNOWN: ");
    }
    Serial.println(results.value, HEX);
    Serial.println("");
    irrecv.resume(); // Receive the next value
  }
}
