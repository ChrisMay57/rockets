void movingHorizon(float ref1, float ref2)
{
   strokeWeight(3);
  // This is the function containing everything that has to
  // do with the moving parts of the artificial horizon
  
  float newX = map(ref1, 0, 300, -150, 150);
  //println("newX = " + newX);
  
  //sky
  fill(30,143,198);
  quad(25,25, 275,25, 275,ref2+newX, 25,ref2-newX);
  
  //ground
  fill(183,113,28);
  quad(25,ref2-newX, 275,ref2+newX, 275,275, 25,275);
  
  // white line
  stroke(250);
  line(25,ref2-newX, 275,ref2+newX); // tilting line
  
  // non tilting lines
  strokeWeight(2);
  // This is here so you can easily change the width of the gap
  // between the white lines
  int gap = 13;
  line(115, ref2+6*gap, 185, ref2+6*gap);
  line(140, ref2+5*gap, 160, ref2+5*gap);
  line(120, ref2+4*gap, 180, ref2+4*gap);
  line(140, ref2+3*gap, 160, ref2+3*gap);
  line(125, ref2+2*gap, 175, ref2+2*gap);
  line(140, ref2+1*gap, 160, ref2+1*gap);
  line(140, ref2-1*gap, 160, ref2-1*gap);
  line(125, ref2-2*gap, 175, ref2-2*gap);
  line(140, ref2-3*gap, 160, ref2-3*gap);
  line(120, ref2-4*gap, 180, ref2-4*gap);
  line(140, ref2-5*gap, 160, ref2-5*gap);
  line(115, ref2-6*gap, 185, ref2-6*gap);
}
