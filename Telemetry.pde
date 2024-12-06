import processing.serial.*;

Serial myPort;

// Variables to keep track of orientation
int Roll = 0;
int Pitch = 0;
int Yaw = 0;
int servoYaw = 0;
int servoPitch = 0;
int distance = 0;

// 3D Model objects
PShape imu;
PShape laser;

PFont font;

void setup() {
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 115200);
  myPort.clear();
  
  size(1200, 800, P3D); 
  
  // Load IMU mount 3D model
  imu = loadShape("ImageToStl.com_imu+mount.obj"); 
  imu.scale(2);
  
  // Load Laser Diode 3D model which will represent gimbal movement
  laser = loadShape("ImageToStl.com_laser diode module.obj"); 
  laser.scale(2);
  
  // Load custom font
  font = createFont("Courier", 18);
  textFont(font);
  textSize(18);
}

void draw() {
  // Reading in data from serial port
  while (myPort.available() > 0) {
    String inString = myPort.readStringUntil('\n');
    if (inString != null) {
      inString = trim(inString);
      String[] systData = splitTokens(inString, "\t"); // Splitting data based on tabs
      if (systData.length == 6) { 
        Yaw = int(systData[0]);
        Pitch = int(systData[1]);
        Roll = int(systData[2]);
        servoYaw = int(systData[3]);
        servoPitch = int(systData[4]);
        distance = int(systData[5]);
      }
    }
  }

  background(0); // black background
  lights();

  // Split window into 4 squares
  stroke(255); 
  strokeWeight(2);
  line(width / 2, 0, width / 2, height); // Vertical dividing line
  line(0, height / 2, width, height / 2); // Horizontal dividing line

  // IMU Orientation in top left
  pushMatrix();
  translate(width / 3, height / 3, 0); 
  rotateX(radians(Pitch));
  rotateY(radians(Yaw));
  rotateZ(radians(Roll + 180));
  shape(imu);
  drawAxis(50); // Attach axes to IMU
  popMatrix();

  // Blue rectangle outline around IMU readings
  stroke(0, 0, 255); 
  noFill();
  rect(10, 10, 230, 110);

  // IMU Roll, Pitch, Yaw readings
  fill(255); // 
  textAlign(LEFT, CENTER);
  text("IMU Roll: " + Roll, 20, 25);
  text("IMU Pitch: " + Pitch, 20, 55);
  text("IMU Yaw: " + Yaw, 20, 85);

  // Laser Orientation in the top right
  pushMatrix();
  translate(3 * width / 4, height / 4, 0); // Center of top-right quadrant
  rotateX(radians(servoPitch + 180));
  rotateY(-radians(servoYaw));
  shape(laser);
  drawAxis(50); // Attach axes to Laser
  popMatrix();

  // Blue rectangle outline around Laser readings
  stroke(0, 0, 255); // Blue outline
  noFill();
  rect(width / 2 + 10, 10, 240, 80);

  // Laser Pitch and Yaw readings
  fill(255); 
  textAlign(LEFT, CENTER);
  //text("Laser Roll: " + Roll, width / 2 + 20, 25);
  text("Laser Pitch: " + servoPitch, width / 2 + 20, 25);
  text("Laser Yaw: " + servoYaw, width / 2 + 20, 55);

  // Crosshair with Distance Reading in bottom left
  stroke(255); 
  line(width / 4 - 50, 3 * height / 4, width / 4 + 50, 3 * height / 4); // Horizontal line
  line(width / 4, 3 * height / 4 - 50, width / 4, 3 * height / 4 + 50); // Vertical line
  fill(255);
  textAlign(CENTER, CENTER);
  textSize(24);
  text("Distance: " + distance + " cm", width / 4, 3 * height / 4 + 120);

  // Laser Status
  fill(255);
  textAlign(CENTER, CENTER);
  textSize(24);
  
  if (distance < 20) {
    text("Target Accquired. Firing", 3 * width / 4, 3 * height / 4);
  } else {
      text("System Idle. No Target in Range.", 3 * width / 4, 3 * height / 4);
  }
}
// Function to draw the axes attached to the 3D objects
void drawAxis(float size) {
  strokeWeight(2);
  
  // X Axis (Red) -  Pitch
  stroke(255, 0, 0);
  line(0, 0, 0, size, 0, 0);
  pushMatrix();
  translate(size, 0, 0); 
  fill(255, 0, 0); 
  textAlign(CENTER, CENTER);
  textSize(22);
  text("X", 0, 0);
  popMatrix();
  
  // Y Axis (Green) - Yaw
  stroke(0, 255, 0);
  line(0, 0, 0, 0, size, 0);
  pushMatrix();
  translate(0, size, 0); 
  rotateX(radians(180));
  fill(0, 255, 0); 
  textAlign(CENTER, CENTER);
  textSize(22);
  text("Y", 0,-12);
  popMatrix();
  
  // Z Axis (Blue) - Roll
  stroke(0, 0, 255);
  line(0, 0, 0, 0, 0, size);
  pushMatrix();
  translate(0, 0, size);
  fill(0, 0, 255);
  textAlign(CENTER, CENTER);
  textSize(22);
  text("Z", 0, 0);
  popMatrix();
}
