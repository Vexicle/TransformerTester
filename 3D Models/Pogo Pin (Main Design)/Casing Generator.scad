//


//----VARIABLES----

//--Transformer Dimensions--

//The length of the transformer body(long side, x axis)
tLength=7.87;

//The width of the transformer body(short side, y axis)
tWidth=5.23;
//Width assigned to depth for interchangeable use in code
tDepth=tWidth;

//The height of the transformer(top to bottom, z axis)
tHeight=5.59;

//The width of the metal legs used for interfacing with the transformer
tpinWidth=0.76;

//The distance from one pin to the consecutive one
pinSpacing=0.75;

//The distance that the pins extrude from the side of the body
pinExtrusion=0.66;


//--Casing Dimensions--

//The depth that the surface that the legs rest on is indented from the top of the main chassis
//-Partially outdated after removal of individual pinslots
pinDepth=3;

//The width of the walls enclosing the transformer
//-Partially outdated after pogo holes added
wallWidth=1;

//--PGP related dimensions--

//Diameter of the main body of the pogo pin(the bit that gets inserted into the hole)
pogoDiameter=1.49;

//Diameter of the hole that the pogo pin is inserted into
pogoHoleDiameter=1.75;

//Length of the main body of the pogo pin, from top of lip to spring
pogoLength=3.3;//length of main body of pin

//where the pogo hole is positioned, 0 is on the outside of the inset, 1 is on the inside of the inset
pogoPositionRatio=0.7;

//The extra radius of the lip compared to the main body
pogoLipRadius=0.1;

//The height of the lip (could be considered the "thickness")
pogoLipHeight=0.5;

//The padding between the edge of a pogo hole and the pin barrier
//-Outdated since slot barriers removed, but still used in code
holePadding=0.76;

//slot on the bottom for protecting the bottom of the pogo pins from damage. set to 0 to disable this, reccommended set to pogostemlength+0.5
//-Not used when PCB's are soldered to board or PCB
pogoProtection=0.00;


//--Funnel--
funnelHeight = 10;
// Using an angle (e.g., 30 degrees) makes the slope constant 
// regardless of height changes.
funnelSlopeAngle = 5; 

// Funnel Derived Logic:
// tan(angle) = opposite / adjacent -> expansion = height * tan(angle)
funnelExpansion = funnelHeight * tan(funnelSlopeAngle);





//--Miscellaneous--
//negligible variable used to avoid Z-fighting
$tol=0.01;

//Resolution of cylinders/circles. Eqivalent to x-sided circle/prism
$fn=32;

//Required clearance between different parts to allow proper movement
//-Calibrated for the printer we used
$clear=0.4;

//==============
//DERIVED VARIABLES
//==============

//STATIC

//Whether or not to include an indent to fit the pogo pin lip
lip=false;

//the number of legs on each side of the transformer
numPins=3;

//Calculated absolute pin positions:
//Middle of each pin can be found with 1.4+2.54*pinNumber starting at pinNumber=0 so:
basePinLoc=[1.4,3.94,6.48];//coordinates on the base trasformer
extraPinOffset=wallWidth+$clear;
pinLoc=[1.4+extraPinOffset,3.94+extraPinOffset,6.48+extraPinOffset];


//top slot width for ensuring proper transformer length fit
topSlotWidth=tWidth+2*$clear;

//offset of the top slot from the origin in the x axis
topSlotOffset=0.5;

//Derived casing dimensions
//inner casing cavity(This is the main cavity in the middle of the chassis)
innerCasingLength=tLength+(2*$clear);
innerCasingWidth=tWidth+(2*$clear)-2;
innerCasingHeight=pogoLength+pogoProtection+pinDepth;

//Outer casing dimensions. Everything else is cut out of this
outerCasingLength=innerCasingLength+2*wallWidth;//Length of outer casing
outerCasingWidth=innerCasingWidth+2*(2*wallWidth+pogoHoleDiameter);//Depth of outer casing
outerCasingHeight=innerCasingHeight-2*$tol;//Height of outer casing

// We calculate scale as: (Original + 2*Expansion) / Original
// This is moved here so the module stays clean
funnelScaleX = (outerCasingLength + 2 * funnelExpansion) / outerCasingLength;
funnelScaleY = (outerCasingWidth + 2 * funnelExpansion) / outerCasingWidth;

//Main code for assembling the chassis

//Subtracting everything from the main block
difference()
{//Block defined by outer casing vars to subtract everything from
    
    cube([outerCasingLength,//Length of outer casing 
        outerCasingWidth, //Depth of outer casing
        outerCasingHeight]);//Height of outer casing
    
        echo(outerCasingHeight)//debugging
    
//Everything below this point is grouped then subtracted from the main block

{
//inner cube acting as main central cavity
    
//moving cube to correct position
translate([
    (outerCasingLength)/2-innerCasingLength/2,
    outerCasingWidth/2-innerCasingWidth/2,
    -$tol])
   //Creating cuboid
    cube([
    innerCasingLength,
    innerCasingWidth, 
    innerCasingHeight]);
    
//creating topslot for proper transformer fit
    translate([wallWidth+topSlotOffset/2,
    outerCasingWidth/2-topSlotWidth/2,
    outerCasingHeight-pinDepth+$tol*3])
    //creating topslot cuboid
    cube([innerCasingLength-topSlotOffset,
    topSlotWidth,
    pinDepth+$tol]);
    
//Looping through each pin pair
for (i = [0:(numPins-1)]){
    pinInset(i); //Create a pin inset for this pair
    pogoHoles(i,lip);}//Create pogo holes for this pair
}

}


//Abstracted modules from the main difference function

//Originally used to create pinslots for each pogo pin
//-Having been widened, there are no longer slots but the logic still works so is still in place
module pinInset(pinNum){
//define values for dimensions of the bars hollowing out the pin inset pair
width=pogoDiameter+pogoLipRadius*2+holePadding+0.1;
depth=outerCasingWidth+2*$tol;
height=pinDepth;

//bottominset(Pogo protection, not used when soldering to board or pcb)
translate([pinLoc[pinNum],
    (depth/2)-$tol,
    (height/2)-$tol])
    
    cube([width,
    depth,
    pogoProtection],center=true);
//topinset
translate([pinLoc[pinNum],
    (depth/2)-$tol,
    pogoLength+pogoProtection+height+$tol-height/2])
    
    cube([width,
    depth,
    height],
    center=true);
    }
    
//Creating holes for the pogo pins
//Takes params:
    //pinNum(int) 0->2 for which pin the holes are being made for
    //includeLip(bool) whether or not to include an indent in the bottom to fit the lip
module pogoHoles(pinNum,includeLip){
    
    pinInsetWidth=pogoDiameter+pogoLipRadius*2+holePadding;
    radius=pogoHoleDiameter/2;
    height=5*outerCasingHeight;//arbitrary amount more than the outer transformer casing to make sure the hole goes all the way through
    
    
    translate([
    pinLoc[pinNum],
    (radius+wallWidth*2*pogoPositionRatio),
    height/2-$tol]){
        cylinder(h=height,r=radius,center=true);
    
    if(includeLip){
        translate([
        0,
        0,
        -(height/2)+pogoProtection])
        
            cylinder(h=pogoLipHeight,r=pogoLipRadius,center=true);
        }}
    
    translate([pinLoc[pinNum],outerCasingWidth-(radius+wallWidth*2*pogoPositionRatio),height/2-$tol]){
        cylinder(h=height,r=radius,center=true);
        if(includeLip){
        translate([0,0,-(height/2)+pogoProtection])
            cylinder(h=pogoLipHeight,r=pogoLipRadius+pogoDiameter/2,center=true);
        }}}

module funnel(){
    pinGapStart = pinLoc[0] - $clear;
    pinGapEnd = pinLoc[2] + $clear;
    funnelPinGapLength = pinGapEnd - pinGapStart;
        
    translate([outerCasingLength/2, outerCasingWidth/2, outerCasingHeight])
    // Uses the pre-calculated vector scale from the declarations
    linear_extrude(height = funnelHeight, scale = [funnelScaleX, funnelScaleY])
    {
        difference(){
            square([outerCasingLength, outerCasingWidth], center = true);
            
            union() {
                square([innerCasingLength - topSlotOffset, topSlotWidth], center = true);
                square([funnelPinGapLength, pinExtrusion + tWidth + 2 * pinExtrusion + 2 * $clear], center = true);
            }
        }    
    }
}
funnel();
