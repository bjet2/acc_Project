import processing.serial.*;

Serial port;
String dataIn = "-1,-1,-1,-1";            // initalize in case data is not ready
char lf = 10;                         // Assci code for line feed
String ssx="NULL";                    // sub strings with data for x,y and z
String ssy ="NULL";
String ssz = "NULL";
String sstime = "NULL";
int i,j,k;                            // indexes for parsing string. Need one for each comma

float ax,ay,az;                       // acceleration values etc. ... For calculations this will be considered the final value (Vf)
float ax_i=0,ay_i=0,az_i=0;           // previous or intital value of acceleration (Vi) etc
float time_a = 0,time_i = 0; 

float vx=0,vy=0,vz=0;
float vx_i=0,vy_i=0,vz_i=0;

boolean newData = false; 


void setup(){
  // set up canvas
  size(800,440);
  background(255);  
  stroke(0);
  fill(0);
  textSize(32);
  
  time_i = 0; 
  newData = false;
  
  // Start Serial port
  String arduinoPort = Serial.list()[0];
  port = new Serial(this, arduinoPort,9600);
  // set condition for handler
  port.bufferUntil(lf);              
}

void draw(){

  
   // get the index for commas
  for(i=0; dataIn.charAt(i)!= ',' | i>dataIn.length();i++){ }
  for(j=i+1; dataIn.charAt(j)!= ',' | i>dataIn.length();j++){}
  for(k=j+1; dataIn.charAt(k)!= ',' | k>dataIn.length();k++){}
  // Use these indexes to subdivide the string then convert sub strings into floats
  // also use /255*9.8 to convert to N/kg
  
  ssx =dataIn.substring(0,i);                      // rough calibration done (scale and offset) //<>//
  ax= (float(ssx));
  ssy = dataIn.substring(i+1,j);
  ay= (float(ssy));
  ssz = dataIn.substring(j+1,k);
  az = (float(ssz));            // not yet calibrated
  sstime = dataIn.substring(k+1,dataIn.length());
  time_a = float(sstime); //<>//
  
  if(newData== false & ax==0.0 & ay==0.0 & az==0.0 & time_a==0.0){
    newData = true;
    println("start found");
  }
  // if data valid use to create scaled numbers for plotting
  // leave time in milliseconds
  if(newData== true & (time_a != time_i)){
    
     ax = ax/255*9.8;
     ay = ay/255*9.8;
     az = az/255*9.8;
     
     println(" a = ",ax,",",ay,",",az," , ",time_a/1000);
     
    vx = vx_i+(ax+ax_i)/2*(time_a-time_i)*10;
    vy = vy_i+(ay+ay_i)/2*(time_a-time_i)*10;
    vz = vz_i+(az+az_i)/2*(time_a-time_i)*10;
    
    println(" v = ",vx,",",vy,",",vz," , ",time_a/1000);
    println(" ");
    
    // visually display data ... red is a, green is v, blue is displacement 
    background(255);
    line(0,220,800,220);
    line(400,0,400,400);
    fill(0,2550,0);
    ellipse((400+(vx)),(220-(vy)),10,10);
    fill(0,255,0);
    
    fill(255,0,0);
    ellipse((400+(ax*10)),(220-(ay*10)),10,10);
    fill(0,255,0);
    
    ax_i= ax;
    ay_i= ay;
    az_i= az;
    
    vx_i = vx;
    vy_i = vy;
    vz_i = vz;
    
    time_i=time_a;  
  }
}



void serialEvent(Serial p) {             // handler ... get data when linefeed comes in
  dataIn = p.readString(); 
} 
