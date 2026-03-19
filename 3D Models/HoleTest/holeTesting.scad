numHoles=10;
increment=0.05;//diameter in mm
minHoleSize=1.4; //value in mm
maxHoleSize=minHoleSize+increment*numHoles;
sheetHeight=2;
padding=1.5;
$fn=32;
difference(){
    
    cube([numHoles*(maxHoleSize+padding+0.2),maxHoleSize+2,sheetHeight]);
    {
        for(i=[1:numHoles]){
            translate([i*(maxHoleSize+padding),maxHoleSize/2+1,-0.1])
            cylinder(d=(minHoleSize+increment*i),h=10);
            }
            
        }



}
