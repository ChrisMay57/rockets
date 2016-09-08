import processing.serial.*;

// Mapping
import de.fhpotsdam.unfolding.*;
import de.fhpotsdam.unfolding.geo.*;
import de.fhpotsdam.unfolding.utils.*;
import de.fhpotsdam.unfolding.providers.OpenStreetMap.*;

// Main UI variables 
int instrumentSize = 300; // X position of instruments
PFont titleFont;

// Graphing
import org.gwoptics.graphics.graph2D.Graph2D;
import org.gwoptics.graphics.graph2D.traces.ILine2DEquation;
import org.gwoptics.graphics.graph2D.traces.RollingLine2DTrace;

// Control
import controlP5.*;

int theHeight = 800; 

// Map variables
UnfoldingMap map;
Location locationBerlin = new Location(52.5f, 13.4f);
Location locationLondon = new Location(51.5f, 0f);
int mapLocation = 60; 
int mapSizeX = 450; 
int mapSizeY = 500;
int titleBound = 40; 

// SSI Image
PImage ssiImg;

// Graph variables
RollingLine2DTrace r; 
Graph2D g;
Graph2D g2;
int graphBounds = 80; 

// Getter for first equation graphed. 
class eq implements ILine2DEquation {
  public double computePoint(double x, int pos) {
    return sin(millis()/1000);
  }
}

// Control 
ControlP5 cp5;
int myColorBackground = color(0, 0, 0);
int knobValue = 100;
Knob myKnobA;
Knob myKnobB;
int knobX = 100; 

/*
 * Setup 
 */
public void setup() {
  size(1400, theHeight, P2D);
  titleFont = loadFont("NewsGothicMT-Bold-40.vlw"); 
  noStroke();
  // ssiImg = loadImage("ssi.png");
  // pegasus = loadImage("peg.png"); 

  // Map
  map = new UnfoldingMap(this, 5, mapLocation+50, mapSizeX+50, mapSizeY, new OpenStreetMapProvider());
  map.setTweening(true);
  map.zoomToLevel(3);
  map.panTo(new Location(40f, 8f));
  MapUtils.createDefaultEventDispatcher(this, map);

  // Graph
  r  = new RollingLine2DTrace(new eq(), 100, 0.1f);
  r.setTraceColour(255, 0, 0);
  r.setLineWidth(3);

  g = new Graph2D(this, 400, 200, false);
  g.setYAxisMax(2);
  g.setYAxisMin(-2);
  g.addTrace(r);
  g.position.y = mapLocation+titleBound+40;
  g.position.x = mapLocation+mapSizeX+graphBounds;
  g.setYAxisTickSpacing(100);
  g.setXAxisMax(25f);
  g.setXAxisTickSpacing(5);

  g2 = new Graph2D(this, 400, 200, false);
  g2.setYAxisMax(600);
  g2.position.y = mapLocation+titleBound*2+220;
  g2.position.x = mapLocation+mapSizeX+graphBounds;
  g2.setYAxisTickSpacing(100);
  g2.setXAxisMax(25f);
  g2.setXAxisTickSpacing(5);

  // Control
  cp5 = new ControlP5(this);
  myKnobA = cp5.addKnob("motorSpeed")
    .setRange(0, 255)
      .setValue(50)
        .setPosition(knobX+instrumentSize, mapLocation+mapSizeY+100)
          .setRadius(50)
            .setDragDirection(Knob.VERTICAL)
              ;               
  myKnobB = cp5.addKnob("targetDirection")
    .setRange(0, 255)
      .setValue(50)
        .setPosition(2*knobX+50+instrumentSize, mapLocation+mapSizeY+100)
          .setRadius(50)
            .setDragDirection(Knob.VERTICAL)
              ;        
  cp5.addButton("refresh")
    .setValue(0)
      .setPosition(4*knobX+50+instrumentSize, mapLocation+mapSizeY+100)
        .setSize(100, 50)
          ;
  cp5.addButton("button2")
    .setValue(100)
      .setPosition(4*knobX+50+instrumentSize, mapLocation+mapSizeY+170)
        .setSize(100, 50)
          ;

  cp5.addButton("serialConnection")
    .setPosition(15, height-50)
      .setSize(100, 20)
        ;

  cp5.addButton("controller")
    .setPosition(15, height-100)
      .setSize(100, 20)
        ;

  cp5.addButton("emergencyDeploy")
    .setPosition(width-150, height-80)
      .setSize(100, 50)
        .setColorBackground(color(255, 0, 0)) 
          ;

  textSize(25);
  frameRate(10);
}

int background = 107; // color 

/*
 * Draw 
 */

int rectBuffer = 80; // distance from bottom of map to rectangle 
int borderRadius = 10; // consistent rectangle radius 

String theText = "Pegasus Ground Station"; 
//float theWidth = textWidth(theText);

// Control flow 
boolean showSerial = false; 
boolean controller = false; 
int textBase = 200;
int chosenSerial = -1; 

public void draw() { 
  smooth();
  background(background);
  // add instruments
  
  if (showSerial) {
    setElementsVisible(false); // hide knobs
    noFill(); 
    rect(mapLocation, mapLocation+50, width-2*mapLocation, height-2*mapLocation, borderRadius, borderRadius, borderRadius, borderRadius);
    for (int ii = 0; ii < Serial.list ().length; ii ++) {
      text((ii+1) + ". " + Serial.list()[ii], textBase-50, textBase+50*ii);
    }
  } else if (controller) {
    setElementsVisible(false); 
    addInstruments(); 
  } else {
    setElementsVisible(true); 
    map.draw();
    rects(); 
    g.draw();
    g2.draw();
    fill(0); 

    textFont(titleFont, 40);
    text(theText, width/2-textWidth(theText)/2, 70);
  }

  // Draws locations on screen positions according to their geo-locations.
  // Fixed-size markers (add later)
}

public void serialConnection(int theValue) {
  showSerial = !showSerial;
}

public void controller(int theValue) {
  controller = !controller;
}

