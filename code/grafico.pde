import processing.serial.*;

  Serial myPort;        
  int xPos = 1;         
  float inByte = 0;

  void setup ()
  {
    size(800, 600);
    println(Serial.list());
    myPort = new Serial(this, Serial.list()[0], 9600);
    myPort.bufferUntil('\n');
    background(255);
  }

  void draw ()
  {  
    stroke(127, 34, 255);
    line(xPos, height, xPos, height - inByte);
   
    stroke(220,220,220);
    
    beginShape(LINES);
      vertex(0, 100);
      vertex(width, 100);
      vertex(0, 200);
      vertex(width, 200);
      vertex(0, 300);
      vertex(width, 300);
      vertex(0, 400);
      vertex(width, 400);
      vertex(0, 500);
      vertex(width, 500);
    endShape();
   
    if (xPos >= width) {
      xPos = 0;
      background(255);
    } else {
      xPos++;
    }
  }

  void serialEvent (Serial myPort) {
   String inString = myPort.readStringUntil('\n');

    if (inString != null) {
      inString = trim(inString);
      inByte = float(inString);
      println(inByte);
      inByte = map(inByte, 0, 1023, 0, height);
    }
  }
