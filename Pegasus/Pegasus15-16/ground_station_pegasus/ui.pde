public void addInstruments() {
  fill(153);
  stroke(3);
  strokeWeight(1);
  pushMatrix();  
  //translate(5, 100); // so it's not in the corner
  //int mX = (mouseX > 350) ? 350 : mouseX; 
  movingHorizon(mouseX, mouseY);
  frame(background); 
  compass(180*sin(2*PI*millis()/30000));
  popMatrix();
  fill(background); 
  noStroke();
  // rect(0, 0, 500, 100); // unnecessary with following rect   
  //rect(10, 0, 280, 100);
  rect(10, 300, 280, height);
  noFill(); 
  stroke(0); 
  strokeWeight(1); 
  rect(5, 5, instrumentSize-10, height-15, borderRadius, borderRadius, borderRadius, borderRadius);
}

// Draw rectangles. 
public void rects() {
  fill(153);
  stroke(3);
  strokeWeight(1);
  // what does this do
  // rect(5, 5, width-10, mapLocation+40, borderRadius, borderRadius, borderRadius, borderRadius); 

  rect(5, mapLocation+mapSizeY+rectBuffer-10, width-10, height-mapLocation-mapSizeY-5-rectBuffer, borderRadius, borderRadius, borderRadius, borderRadius);
  rectMode(CORNERS); 
  rect(5+mapSizeX+mapLocation, mapLocation+50, width-5, mapLocation+50+mapSizeY, borderRadius, borderRadius, borderRadius, borderRadius);
  rectMode(CORNER); 
  noFill(); 
  rect(5, mapLocation+50, mapSizeX+50, mapSizeY, borderRadius, borderRadius, borderRadius, borderRadius);
}

public void setElementsVisible(boolean vis) {
  myKnobA.setVisible(vis);
  myKnobB.setVisible(vis);
  cp5.getController("refresh").setVisible(vis);
  cp5.getController("button2").setVisible(vis);
}

