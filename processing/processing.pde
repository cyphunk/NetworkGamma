
int port = 4740;
String ip = "127.0.0.1";
byte maxInVal = 127;
byte outVal;
float outStep;
float valInc = TWO_PI/127;

// import UDP library
import hypermedia.net.*;
UDP udp;  // the UDP object
byte[] data = new byte[3];

void setup() {
  udp = new UDP(this);
  udp.listen(false);
  data[0]=maxInVal;
  data[1]=maxInVal;
  data[2]=maxInVal;
}

void draw() {
  //                    base           value          cieling+base?
  outVal = (byte)(int(  (maxInVal/2) + sin(outStep) * (maxInVal/2)  ));
  outStep += valInc;

  data[0] = outVal;
  data[1] = outVal;
  data[2] = outVal;
  udp.send(data, ip, port);
  println( data[0] + " " + data[1] + " " + data[2]);
}

