void frame(int theFill)
{
  // This whole void function contains parts that make up the 
  // background and some non moving parts of the artificial horizon
  
  // This is the stationary ground that surrounds the artificial horizon
  fill(183,113,28);
  strokeWeight(1);
  stroke(25);
  beginShape();
  vertex(25,150);
  vertex(49,150);
  bezierVertex(49,180, 70,250, 150,245);
  vertex(25,245);
  endShape();
  beginShape();
  vertex(275,150);
  vertex(250,150);
  bezierVertex(251,180, 230,250, 150,245);
  vertex(275,245);
  endShape();
  
  // This is the stationary sky that surrounds the artificial horizon
  fill(30,143,198);
  beginShape();
  vertex(25,150);
  vertex(49,150);
  bezierVertex(53,18, 246,18, 250,150);
  vertex(275,150);
  bezierVertex(265,-20, 35,-20, 25,150);
  endShape();
  fill(250);
  noStroke();
  triangle(150,50, 140,30, 160,30); // little white triangle
  
  // The little white lines separating the stationary ground and sky
  stroke(250);
  strokeWeight(4);
  line(25,150, 49,150);
  line(251,150, 275,150);
  
  // This is the black reverence marker for the wings
  strokeWeight(4);
  fill(25);
  stroke(25);
  line(150,150, 150,200);
  triangle(150,200, 125,250, 175,250);
  beginShape();
  vertex(60, 245);
  vertex(0, 300);
  vertex(300, 300);
  vertex(240, 245);
  endShape(CLOSE);
 
  beginShape(LINES);
  vertex(90, 150);
  vertex(125, 150);
  vertex(125, 150);
  vertex(150, 170);
  vertex(150, 170);
  vertex(175, 150);
  vertex(175, 150);
  vertex(210, 150);
  endShape();
  
  // This is a frame surrounding the whole artificial horizon to cover 
  // the background
  fill(theFill);
  noStroke();
  beginShape();
  vertex(25,275);
  vertex(300,275);
  vertex(300,300);
  vertex(25,300);
  endShape();
  beginShape();
  vertex(25,300);
  vertex(25,0);
  vertex(0,0);
  vertex(0,300);
  endShape();
  beginShape();
  vertex(0,0);
  vertex(275,0);
  vertex(275,25);
  vertex(0,25);
  endShape();
  beginShape();
  vertex(275,0);
  vertex(300,0);
  vertex(300,275);
  vertex(275,275);
  endShape();
  
  // The black ring separating the moving and stationary parts 
  // of the horizon
  ellipseMode(CENTER);
  noFill();
  stroke(25);
  strokeWeight(3);
  ellipse(150, 150, 200, 200);
  
  // This is more frame surrounding the artificial horizon
  fill(theFill);
  stroke(theFill);
  beginShape();
  vertex(150, 25);
  bezierVertex(-16,25, -16,275, 150, 275);
  vertex(150,275);
  vertex(25,275);
  vertex(25,25);
  endShape();
  fill(theFill);
  beginShape();
  vertex(150, 25);
  bezierVertex(316,25, 316,275, 150, 275);
  vertex(150,275);
  vertex(275,275);
  vertex(275,25);
  endShape();
  
  // Small black ring separating the stationary ground 
  // from the background
  ellipseMode(CENTER);
  noFill();
  stroke(25);
  strokeWeight(2);
  ellipse(150, 150, 250, 250);
  
  // this repositions the origin to, essentially, divide the window into
  // two regions, the horizon and the compass.
//  translate(0,300);
//  // This is a frame that goes around the compass to cover the background
//  fill(theFill);
//  noStroke();
//  beginShape();
//  vertex(25,275);
//  vertex(300,275);
//  vertex(300,300);
//  vertex(25,300);
//  endShape();
//  beginShape();
//  vertex(25,300);
//  vertex(25,0);
//  vertex(0,0);
//  vertex(0,300);
//  endShape();
//  beginShape();
//  vertex(0,0);
//  vertex(275,0);
//  vertex(275,25);
//  vertex(0,25);
//  endShape();
//  beginShape();
//  vertex(275,0);
//  vertex(300,0);
//  vertex(300,275);
//  vertex(275,275);
//  endShape();
//  
//  stroke(theFill);
//  beginShape();
//  vertex(150, 25);
//  bezierVertex(-16,25, -16,275, 150, 275);
//  vertex(150,275);
//  vertex(25,275);
//  vertex(25,25);
//  endShape();
//  
//  beginShape();
//  vertex(150, 25);
//  bezierVertex(316,25, 316,275, 150, 275);
//  vertex(150,275);
//  vertex(275,275);
//  vertex(275,25);
//  endShape();
}
