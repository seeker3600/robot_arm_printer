import processing.serial.*;

final int OFFSET = 200;
Serial myPort;  // Create object from Serial class

void setup()
{
  String portName = Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port
  println(portName); //print it out in the console
  myPort = new Serial(this, portName, 9600);
  
  size(500, 500);
}

class Point
{
  final int x;
  final int y;
  public Point(int x, int y) {this.x = x;this.y = y;}
}

ArrayList<Point> hist = new ArrayList<Point>();

void draw()
{
  if ( myPort.available() == 0) return;

  String val = myPort.readStringUntil('\n');         // read it and store it in val
  if (val == null) return;
  println(val); //print it out in the console
  
  String[] list = split(val, '\t');
  
  background(204);
  
  int x0 = 0;
  int y0 = 0;
  
  stroke( 0, 0, 0 );
  for(int n = 0; n < list.length; n += 2)
  {
    int x = int(float(list[n]));
    int y = int(float(list[n + 1]));
    
    line(OFFSET + x0, height - OFFSET - y0, OFFSET + x, height - OFFSET - y);
    
    x0 = x;
    y0 = y;
  }
  
  hist.add(new Point(x0, y0));
  
  stroke( 255, 0, 0 );
  Point p0 = hist.get(0);
  for(Point p : hist)
  {
    line(OFFSET + p0.x, height - OFFSET - p0.y, OFFSET + p.x, height - OFFSET - p.y);
  }
}
