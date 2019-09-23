float x,y,z;
int i=0;

void setup() {
  size(200,200,P3D);
  x = 100;
  y = 100;
  z = 0;
  rectMode(CENTER);
  fill(51);
  stroke(255);
  rectMode(CENTER);
}

void draw() {
  background(100);
  
  pushMatrix();
  translate(x-10,y,z-100);
  rotateZ(PI/360*i);    // 
  rotateY(PI/360*i);
  rotateX(PI/360*i);
  box(100);
  popMatrix();
  

  
   i++;

  //z++; // The rectangle moves forward as z increments.
}




//translate(100, 100, 0);
//rotateZ(PI/8);
//rect(0, 0, 100, 100);
