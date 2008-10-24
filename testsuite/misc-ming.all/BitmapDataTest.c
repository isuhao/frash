/* 
 *   Copyright (C) 2007 Free Software Foundation, Inc.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <ming.h>

#include "ming_utils.h"

#define OUTPUT_VERSION 8
#define OUTPUT_FILENAME "BitmapDataTest.swf"

const char* mediadir=".";

void
addRedSquareExport(SWFMovie mo)
{
	SWFShape sh;
	SWFMovieClip mc;

	sh = make_fill_square (0, 0, 60, 60, 255, 0, 0, 255, 0, 0);
	mc = newSWFMovieClip();

	SWFMovieClip_add(mc, (SWFBlock)sh);
	/* This is here just to turn the clip into an active one */
	add_clip_actions(mc, "onRollOver = function() {};");
	add_clip_actions(mc, "onMouseDown = function() { _root.mouseDown++; _root.note(_name+' mouseDown '+_root.mouseDown); };");
	add_clip_actions(mc, "onMouseUp = function() { _root.mouseUp++; _root.note(_name+' mouseUp '+_root.mouseUp); };");
	SWFMovieClip_nextFrame(mc);

	SWFMovie_addExport(mo, (SWFBlock)mc, "redsquare");

	SWFMovie_writeExports(mo);
}


int
main(int argc, char** argv)
{
  SWFMovie mo;
  SWFMovieClip mc;
  SWFMovieClip dejagnuclip, staticSquare;
  SWFShape shape;
  SWFDisplayItem it;

  if ( argc>1 ) mediadir=argv[1];
  else
  {
    fprintf(stderr, "Usage: %s <mediadir>\n", argv[0]);
    return 1;
  }
	
  Ming_init();
  Ming_useSWFVersion (OUTPUT_VERSION);
	
  mo = newSWFMovie();
  SWFMovie_setDimension(mo, 640, 480);

  if (mo == NULL) return -1;

  addRedSquareExport(mo);

  add_actions(mo, "_root.onKeyDown = function() {"
	                "play(); }; "
                    "Key.addListener(_root);");

  SWFMovie_setRate(mo, 2);
    dejagnuclip = get_dejagnu_clip(
            (SWFBlock)get_default_font(mediadir), 10, 10, 200, 800, 600);
    SWFMovie_add(mo, (SWFBlock)dejagnuclip);
 
  SWFMovie_nextFrame(mo);

  add_actions(mo, "BitmapData = flash.display.BitmapData;"
		  "Rectangle = flash.geom.Rectangle;"
		  "bmp = new BitmapData(150, 150, false);"
		  "rect = new Rectangle(10, 10, 100, 100);"
		  "bmp.fillRect(rect, 0x223322);"
		  "mc = _root.createEmptyMovieClip('mc', getNextHighestDepth());"
          "d = mc.getNextHighestDepth();"
          "mc.attachBitmap(bmp, d);"
          "bmp2 = new BitmapData(20, 20, true);"
          "rect2 = new Rectangle (10, 10, 20, 20);"
          "bmp2.fillRect(rect2, 0xffffff22);"
          "d2 = mc.getNextHighestDepth();"
          "mc.attachBitmap(bmp2, d2);"
          "note('1. You should see a small yellow square in the top left\n"
                "corner of a larger green square. Press a key.');"
          "stop();"
          );

    SWFMovie_nextFrame(mo);

    add_actions(mo, "rect = new Rectangle (90, 90, 120, 120);"
		    "bmp.fillRect(rect, 0xffee66);"
            "note('2. You should see a new pale yellow square covering the \n"
                "bottom right corner of the large green square. Press a key.');"
            "stop();"
            );

    SWFMovie_nextFrame(mo);

    add_actions(mo, "mc.createEmptyMovieClip('d', d);"
            "note('3. You should see just the small yellow square in the top\n"
                "left corner. Press a key.');");

    add_actions(mo, "stop();");

    SWFMovie_nextFrame(mo);
    
    // Place a dynamic character at depth 4
    add_actions(mo, "mc.removeMovieClip();"
            "_root.attachMovie('redsquare', 'rs', 4);");

    // Place a static character at depth 3
    staticSquare = newSWFMovieClip();
    shape = make_fill_square (300, 0, 60, 60, 0, 255, 0, 255, 0, 255);
    SWFMovieClip_add(staticSquare, (SWFBlock)shape);  
    SWFMovieClip_nextFrame(staticSquare);
    
    it = SWFMovie_add(mo, (SWFBlock)staticSquare);  
    SWFDisplayItem_setDepth(it, 3); 
    SWFDisplayItem_setName(it, "staticSquare");

    add_actions(mo, 
            "note('4. You should see a red square in the top right and a\n"
            "purple square in the top centre. Press a key.');");
    add_actions(mo, "stop();");

    SWFMovie_nextFrame(mo);

    add_actions(mo, "staticSquare.swapDepths(20);"
            "note('5. There should have been no change. Press a key.');"
            "stop();"
            );

    SWFMovie_nextFrame(mo);

    add_actions(mo, "_root.attachBitmap(bmp, 2);"
            "note('6. You should see the green square and pale yellow square\n"
            "under the red square. The purple square should still be there.\n"
            "Press a key');"
            "stop();"
            );

    SWFMovie_nextFrame(mo);

    add_actions(mo, "_root.attachBitmap(bmp, 3);"
            "note('7. There should have been no change. Press a key');"
            "stop();"
            );

    SWFMovie_nextFrame(mo);
    
    add_actions(mo, "_root.attachBitmap(bmp2, 20);"
            "note('8. The purple square should have gone. The small yellow\n"
            "square should have replaced the top left corner of the red\n"
            "square. Press a key.');"
            "stop();"
            );

  //Output movie
  puts("Saving " OUTPUT_FILENAME );
  SWFMovie_save(mo, OUTPUT_FILENAME);

  return 0;
}
