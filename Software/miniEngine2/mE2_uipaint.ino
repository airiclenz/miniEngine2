/*

    Author: Airic Lenz
    
    See www.airiclenz.com for more information

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/







// some variables to check if we do need a full repaint
uint8_t  menu_offset_old     = 255;
uint8_t  menu_screen_pos_old = 255; 
uint8_t  popup_menu_pos_old  = 255; 
//boolean  menu_editing_old    = false;


// display_width 

// display_height



// ======================================================================================
// paints the header base
// ======================================================================================
// result = true when the first line is on screen
void uipaint_headerBase(int height) {
  
  tft.setBackColor(color_schemes[uicore_col_scheme].background_header);
  tft.setColor(color_schemes[uicore_col_scheme].background_header);
  tft.fillRect(0, 0, display_width - 1, height - 6);
  
  tft.setBackColor(color_schemes[uicore_col_scheme].background);
  tft.setColor(color_schemes[uicore_col_scheme].background);
  tft.fillRect(0, height - 6, display_width - 1, height);
  
  tft.setColor(color_schemes[uicore_col_scheme].seperator);
  tft.fillRect(0, height - 4, display_width - 1, height - 3);
  
}


// ======================================================================================
// paints the full header
// ======================================================================================
void uipaint_header(boolean full_repaint) {
  
  if (full_repaint) {
    uipaint_headerBase(header_height);  
  
    tft.setBackColor(color_schemes[uicore_col_scheme].background_header);
    tft.setColor(color_schemes[uicore_col_scheme].font_header);
    tft.setFont(SMALL_FONT);
    
    if (isBit(core_mode, MODE_TIMELAPSE))     uicore_getShortString(60, 0);
    else if (isBit(core_mode, MODE_VIDEO))    uicore_getShortString(61, 0);
    else if (isBit(core_mode, MODE_PANORAMA)) uicore_getShortString(62, 0);
    
    if (!isBit(core_mode, MODE_PANORAMA)) {
      strcat(lines[0], STR_SPACE2);

      if      (core_setup_style == SETUP_STYLE_RUN)       strcat(lines[0], string_50_short);
      else if (core_setup_style == SETUP_STYLE_KEYFRAMES) strcat(lines[0], string_51_short);
    } 
        
    tft.print(lines[0], 7, 2); 
        
  }
  
}


// ======================================================================================
// paints the settings header
// ======================================================================================
void uipaint_headerSettings(boolean full_repaint) {
  
  // do we come from a regular screen?
  if (full_repaint) {
    uipaint_headerBase(header_height);  
    
    tft.setBackColor(color_schemes[uicore_col_scheme].background_header);
    tft.setColor(color_schemes[uicore_col_scheme].font_header);
    tft.setFont(SMALL_FONT);
    
    uicore_getShortString( 92, 0); // Settings
    
    // print the string
    tft.print(lines[0], 3, 1); 
  }
  
  
  // load the header strings
  uicore_getShortString( 93, 0); // GEN
  uicore_getShortString( 94, 1); // CAM
  uicore_getShortString( 95, 2); // MOT
  uicore_getShortString( 96, 3); // CHN
  uicore_getShortString( 97, 4); // TRG
  
    
  byte cnt;
  
  // loop all possible menu screens 
  for (int i=100; i<=104; i++) {
          
    cnt = i - 100;
    
    // is painting this item really needed?
    if (
        (full_repaint) ||
        
        ( (screen_code == i) &&
          (screen_code != screen_code_old) ) ||
            
        ( (screen_code_old == i) &&
          (screen_code != screen_code_old))) {
        
      // paint the long settings name
      tft.setBackColor(color_schemes[uicore_col_scheme].background_header);
      tft.setColor(color_schemes[uicore_col_scheme].font_header);
      tft.setFont(SMALL_FONT);
      
      
      uint16_t str_len;
          
      // paint the long version of the settings name
      switch (screen_code) {
        case 100:    str_len = strlen(string_93_long) * tft.getFontXsize();        // General
                     tft.print(string_93_long, display_width - str_len - 4, 1);    
                     break;
                     
        case 101:    str_len =strlen(string_94_long) * tft.getFontXsize();        // Camera
                     tft.print(string_94_long, display_width - str_len - 4, 1);    
                     break;
                     
        case 102:    str_len =strlen(string_95_long) * tft.getFontXsize();        // Motors
                     tft.print(string_95_long, display_width - str_len - 4, 1);    
                     break;
                     
        case 103:    str_len =strlen(string_96_long) * tft.getFontXsize();        // Daisy Chaining
                     tft.print(string_96_long, display_width - str_len - 4, 1);    
                     break;
                     
        case 104:    str_len =strlen(string_97_long) * tft.getFontXsize();        // Triggers
                     tft.print(string_97_long, display_width - str_len - 4, 1);    
                     break;
      }    
      
      
      // Set the main font      
      tft.setFont(uicore_fonts[uicore_font_index].font);  
                  
      // are we standing on the current settings screen?
      if (screen_code == i) {
        
        tft.setColor(color_schemes[uicore_col_scheme].font_bg_selected);
        tft.fillRoundRect(cnt*64, 25, (cnt+1)*64, header_height - 10);
                
        tft.setBackColor(color_schemes[uicore_col_scheme].font_bg_selected);
        tft.setColor(color_schemes[uicore_col_scheme].font_selected);
        
        // mE-Clear
        if (uicore_font_index == 0) tft.print(lines[cnt], (cnt*64)+9 , 30);
        // Sinclair
        if (uicore_font_index == 1) tft.print(lines[cnt], (cnt*64)+11 , 30);
        // Arial
        if (uicore_font_index == 2) tft.print(lines[cnt], (cnt*64)+10 , 30);
          
      } else {
         
        tft.setColor(color_schemes[uicore_col_scheme].background_header);
        tft.fillRect(cnt*64, 25, (cnt+1)*64, header_height - 10);
              
        tft.setBackColor(color_schemes[uicore_col_scheme].background_header);
        tft.setColor(color_schemes[uicore_col_scheme].font_header);
        
        // mE-Clear
        if (uicore_font_index == 0) tft.print(lines[cnt], (cnt*64)+9 , 30);
        // Sinclair
        if (uicore_font_index == 1) tft.print(lines[cnt], (cnt*64)+11 , 30);
        // Arial
        if (uicore_font_index == 2) tft.print(lines[cnt], (cnt*64)+10 , 30);
     
      }
      
    } // end: paint needed
    
  } // end_ for:
  
}

// ======================================================================================
// paints the edit-screen header
// ======================================================================================
void uipaint_headerEdit(boolean full_repaint) {
  
  if (full_repaint) {
    uipaint_headerBase(header_height);  
    
    tft.setBackColor(color_schemes[uicore_col_scheme].background_header);
    tft.setColor(color_schemes[uicore_col_scheme].font_header);
    tft.setFont(SMALL_FONT);
    
    uicore_getShortString( 80, 0); // Edit screen
    tft.print(lines[0], 7, 2); 
    
    
    tft.setColor(color_schemes[uicore_col_scheme].font);
    tft.setFont(uicore_fonts[uicore_font_index].font);
    
    uicore_getShortString(line_codes[menu_pos], 0);  // name of the current field 
    tft.print(lines[0], 7, 25); 
    
  }
    
}



// ======================================================================================
// paints the regular header
// ======================================================================================
void uipaint_battery(boolean paint) {
  
  if (paint && power_isShowBattery()) {
    
    uint8_t  batProcent = power_getBatteryLevel();
    itoa(batProcent, temp, 10);     
    
    
    if (core_isProgramRunningFlag()) {
        
      tft.setBackColor(color_schemes[uicore_col_scheme].background);
      tft.setColor(color_schemes[uicore_col_scheme].font_dashboard);
      
      // add the unit percent
      strcat(temp, STR_PERCENT);
      // add a space to clear the last position
      // this is needed if the total length of the percent
      // string goeas down by one: 10% -> 9%
      strcat(temp, STR_SPACE1);
      tft.print(temp, 7, 195);
      
    } else {
      
      uicore_getShortString( 98, 0); // Battery:
      
      tft.setBackColor(color_schemes[uicore_col_scheme].background_header);
      tft.setColor(color_schemes[uicore_col_scheme].font_header);
      tft.setFont(SMALL_FONT);
      
      if (batProcent == 100)   strcat(lines[0], STR_SPACE1);
      else if (batProcent > 9) strcat(lines[0], STR_SPACE2);
      else                     strcat(lines[0], STR_SPACE3);
      
      strcat(lines[0], temp);
      strcat(lines[0], STR_PERCENT);
      tft.print(lines[0], 209, 2);
      
    }
            
  }
   
}





// ======================================================================================
// paints the cycle time
// ======================================================================================
void uipaint_cycleLength() {
  
  if  (core_mode == MODE_TIMELAPSE) {
  
    tft.setBackColor(color_schemes[uicore_col_scheme].background_header);
    tft.setFont(uicore_fonts[uicore_font_index].font);
      
    sprintf(temp,"%.1f", ((float) setup_interval_length) / 1000.0);
    strcat(temp, string_6_short); // s
    
    
    if (system_cycle_too_long) {
      strcat(temp, STR_EXCLAMATION);
    }
    
    if (core_isProgramRunningFlag()) {
      
      tft.setBackColor(color_schemes[uicore_col_scheme].background);
      tft.setColor(color_schemes[uicore_col_scheme].font_dashboard);
      tft.print(temp, 7, 125); 
        
    } else {
      tft.setBackColor(color_schemes[uicore_col_scheme].background_header);
      tft.setColor(color_schemes[uicore_col_scheme].font_header);
      tft.print(temp, 7, 30);   
    }  
    
  }
  
  else if (isBit(core_mode, MODE_VIDEO)) {
    
    if (core_isProgramRunningFlag()) { 
      
      tft.setBackColor(color_schemes[uicore_col_scheme].background);
      tft.setColor(color_schemes[uicore_col_scheme].font_disabled);
      tft.print(STR_NONE, 7, 125); 
    }
    
  }
  
}


// ======================================================================================
// paints the shoot count
// ======================================================================================
void uipaint_shotCount() {
  
  // for the timelapse mode
  if  (core_mode == MODE_TIMELAPSE) {
  
    tft.setFont(uicore_fonts[uicore_font_index].font);
      
    strcpy(data_line, STR_RECBRACKOP);  // [
    
    byte charCount;
    
    // determinte the amount of digits we need to display
    if      ((setup_frame_count <  100) && (cam_shoot_count <  100)) { charCount = 2; }
    else if ((setup_frame_count < 1000) && (cam_shoot_count < 1000)) { charCount = 3; }
    else                                                             { charCount = 4; }
    
    // add spaces to the string if needed
    if (cam_shoot_count < 10) {
      
      for (byte i=1; i<charCount; i++) {
        strcat(data_line, STR_SPACE1);
      }
      
    } else if (cam_shoot_count < 100) {
      
      for (byte i=2; i<charCount; i++) {
        strcat(data_line, STR_SPACE1);
      }
      
    } else if ((cam_shoot_count < 1000) &&
               (charCount == 4)) {
      strcat(data_line, STR_SPACE1);  
      
    }
    
    itoa(cam_shoot_count, temp, 10);
    strcat(data_line, temp);
    strcat(data_line, STR_SLASH);
    
    if (setup_frame_count < 10) {
      strcat(data_line, STR_SPACE1);
    } 
    
    
    itoa(setup_frame_count, temp, 10);
    strcat(data_line, temp);
    strcat(data_line, STR_RECBRACKCL);
    
    if (core_isProgramRunningFlag()) {
      tft.setBackColor(color_schemes[uicore_col_scheme].background);
      tft.setColor(color_schemes[uicore_col_scheme].font_dashboard);
      tft.print(data_line, 130, 125);   // [0000/XXXX]
      
    } else {
      tft.setBackColor(color_schemes[uicore_col_scheme].background_header);
      tft.setColor(color_schemes[uicore_col_scheme].font_header);
      tft.print(data_line, 141 + ((4-charCount) << 5),  28);   // [0000/XXXX]
    }
    
  }
  
  else if (isBit(core_mode, MODE_VIDEO)) {
    
    if (core_isProgramRunningFlag()) {
      
      tft.setBackColor(color_schemes[uicore_col_scheme].background);
      tft.setColor(color_schemes[uicore_col_scheme].font_disabled);
      tft.print(STR_NONE, 130, 125);   
    }
    
  }
  
}



// ======================================================================================
// paints the dashboard
// ======================================================================================
void uipaint_dashboard(boolean fullRepaint) {
  
  if (fullRepaint) {
    
    // paint the dashboard header
    uipaint_headerBase(70);  
    
    // clear the rest of the screen
    tft.setBackColor(color_schemes[uicore_col_scheme].background);
    tft.setColor(color_schemes[uicore_col_scheme].background);
    tft.fillRect(0, 71, display_width - 1, display_height - 1);  
    
    // paint a "recording" sign (the one with the red dot)
    tft.setColor(color_schemes[uicore_col_scheme].background);
    tft.fillCircle(30,33,16);
    tft.setColor(color_schemes[uicore_col_scheme].font_dashboard);
    tft.drawCircle(30,33,14);
    tft.setColor(VGA_RED);
    tft.fillCircle(30,33,8);
      
    // write "Running..."
    tft.setBackColor(color_schemes[uicore_col_scheme].background_header);
    tft.setFont(uicore_fonts[uicore_font_index].font);
    tft.setColor(color_schemes[uicore_col_scheme].font_dashboard);
    tft.print(string_24_short, 80, 27);
    
    // paint the mode string
    tft.setBackColor(color_schemes[uicore_col_scheme].background);
    
    if (isBit(core_mode, MODE_TIMELAPSE)) {
      tft.print(string_60_short, 130, 195);  
    } else if (isBit(core_mode, MODE_VIDEO)) {
      tft.print(string_61_short, 130, 195);  
    } else if (isBit(core_mode, MODE_PANORAMA)) {
      tft.print(string_62_short, 130, 195);  
    }
    
    // Paint the section headers
    tft.setFont(SMALL_FONT);
    tft.setColor(color_schemes[uicore_col_scheme].font_soft);
    tft.print(string_25_short, 130, 100);  // Frames
    tft.print(string_26_short,   7, 100);  // Intervall
    tft.print(string_27_short, 130, 170);  // Mode
    tft.print(string_28_short,   7, 170);  // Battery
      
  }
  
  // shoot count
  uipaint_shotCount();
  
  // intervall
  uipaint_cycleLength();
  
  // battery
  uipaint_battery(uicore_isRepaintBatteryFlag() || fullRepaint);
  
  
  
   
  
}


// ======================================================================================
// paints a message
// ======================================================================================
void uipaint_message() {
  
  int ysize = 50 + (menu_length * 20);
  int ystart = (display_height - ysize) >> 1;
  uint16_t headerh = header_height >> 1;
  
  
  // clear the rest of the area
  tft.setBackColor(color_schemes[uicore_col_scheme].background);
  tft.setColor(color_schemes[uicore_col_scheme].background);
  tft.fillRect(0, ystart - 10, display_width, ystart);  
  tft.fillRect(0, ystart, 5, ystart + ysize + 10); 
  tft.fillRect(display_width - 5, ystart, display_width, ystart + ysize + 10);
  tft.fillRect(5, ystart + headerh, display_width - 5, ystart + ysize + 10);  
      
  // header background
  tft.setBackColor(color_schemes[uicore_col_scheme].font_bg_selected);
  tft.setColor(color_schemes[uicore_col_scheme].font_bg_selected);
  tft.fillRect(5, ystart, display_width - 7, ystart + headerh);
  
  // paint the hightlight frame
  tft.setColor(color_schemes[uicore_col_scheme].scrollbar);
  tft.drawRect(5, ystart, display_width - 6, ystart + ysize);
  
  // message window icon
  tft.setColor(color_schemes[uicore_col_scheme].font_selected);
  tft.fillCircle(21, ystart + (headerh >> 1), 12);
  tft.setColor(color_schemes[uicore_col_scheme].scrollbar);
  tft.drawCircle(21, ystart + (headerh >> 1), 12);
  
  tft.setColor(color_schemes[uicore_col_scheme].font);
  tft.setBackColor(color_schemes[uicore_col_scheme].font_selected);
  tft.setFont(SMALL_FONT);
  tft.print("i", 18, ystart + 8);
  
  // paint the title
  tft.setFont(uicore_fonts[uicore_font_index].font);
  tft.setColor(color_schemes[uicore_col_scheme].font_selected);
  tft.setBackColor(color_schemes[uicore_col_scheme].font_bg_selected);
  uicore_getLongString(line_codes[0]);
  tft.print(data_line, 50, ystart + 7);
  
  // paint the message                                                                                                                                                                                                                                                                                                                         text
  tft.setFont(SMALL_FONT);
  tft.setColor(color_schemes[uicore_col_scheme].font);
  tft.setBackColor(color_schemes[uicore_col_scheme].background);
  
  for (int i=1; i<=menu_length; i++) {
    uicore_getLongString(line_codes[i]);
    tft.print(data_line, 17, ystart + 22 + (20 * i));
  }
  
  
}




// ======================================================================================
// paints the menu entries 
// ======================================================================================
// result = true when the first line is on screen
void uipaint_menu(boolean full_repaint) {
  
  Serial.print("pre paint menu: ");
  Serial.println(setup_run_ramp_in[motor_selected]);
  
  
  // calculate scrolling pos   
  uint8_t menu_offset = menu_pos - menu_screen_pos;
  
  // bind the intitial odd-flag to the menu_offset
  boolean odd = isBit(menu_offset, BIT_0);
  
  // scrolled?
  boolean scrolled = menu_offset != menu_offset_old;
  
  // if we scrolled we need a full repaint even if it was set to false
  if (scrolled) full_repaint = true;
  

  // loop all menu lines  
  for (int i=0; i<menu_max_screen_lines; i++) {
    
    /*
    Serial.print("paint menu (pre ");
    Serial.print(line_codes[i + menu_offset]);
    Serial.print("): ");
    Serial.println(setup_run_ramp_in[motor_selected]);
    */
    
    // does this line need to be painted? 
    // (only paint what is really needed to be painted)
    if ( full_repaint ||
         (
           ((menu_screen_pos == i) || (menu_screen_pos_old == i)) 
         ) &&
         (
           !(menu_screen_pos == menu_screen_pos_old)
         )
       ) { 
         
      
      
      // if this item is not disabled
      if (lines_disabled[i + menu_offset] == false) {
        // generate the data string for the current line  
        uicore_generateDataString(line_codes[i + menu_offset]);
      } else {
        // empty the string
        strcpy(data_line, STR_NONE);  
      }
            
      byte data_len = strlen(data_line) - 1;
         
      // calculate the top-y-position for the current line
      int y = header_height + (i * menu_screen_line_height);   
              
      // selected line and cursor on the left side of the screen?
      if ((menu_screen_pos == i) &&
          (!menu_editing) && 
          (!core_isProgramRunningFlag())) {
      
        // set the colors for the bar   
        tft.setColor(color_schemes[uicore_col_scheme].font_bg_selected);
        tft.setBackColor(color_schemes[uicore_col_scheme].font_bg_selected);
                
        // paint the selection bar
        tft.fillRect(0, y, display_width - 8, y + menu_screen_line_height - 1);
               
                
        // paint the text line
        if (lines_disabled[i + menu_offset] == false) {
          tft.setColor(color_schemes[uicore_col_scheme].font_selected);
        } else {
          tft.setColor(color_schemes[uicore_col_scheme].font_disabled);
        }
        tft.setFont(uicore_fonts[uicore_font_index].font);
        tft.print(lines[i + menu_offset], 2, y + 7);   
        tft.print(data_line, display_width - (data_len * tft.getFontXsize()) - 25, y + 7);   
                       
      } 
        
      // regular menu line
      else if (i < menu_length) {
  
        if (odd) {
          tft.setColor(color_schemes[uicore_col_scheme].font_bg_odd);
          tft.setBackColor(color_schemes[uicore_col_scheme].font_bg_odd);
        } else {
          tft.setColor(color_schemes[uicore_col_scheme].font_bg_even);
          tft.setBackColor(color_schemes[uicore_col_scheme].font_bg_even);
        }
        // clear the selection bar
        tft.fillRect(0, y, display_width - 8, y + menu_screen_line_height - 1);
                
        // paint the text line
        if (lines_disabled[i + menu_offset] == false) {
          tft.setColor(color_schemes[uicore_col_scheme].font);
        } else {
          tft.setColor(color_schemes[uicore_col_scheme].font_disabled);
        }
        tft.setFont(uicore_fonts[uicore_font_index].font);
        tft.print(lines[i + menu_offset], 2, y + 7);   
        tft.print(data_line, display_width - (data_len * tft.getFontXsize()) - 25, y + 7);   
            
      } 
      
      // no menu lines anymore - clear anyway to delete possible former entries at that position
      else {
      
        if (odd) {
          tft.setColor(color_schemes[uicore_col_scheme].font_bg_odd);
        } else {
          tft.setColor(color_schemes[uicore_col_scheme].font_bg_even);
        }
          
        // clear the selection bar
        tft.fillRect(0, y, display_width - 8, y + menu_screen_line_height - 1);
        
      }   
    
    } // end: this line needs to be painted
     
    odd = !odd; 
        
  } // loop all lines
  
  
  // store the values needed for avoiding painting things 
  // that possibly don't need to be painted again the next time
  menu_screen_pos_old = menu_screen_pos;
  menu_offset_old     = menu_offset;
  
  
  // paint the scroll bar if more than 5 menu lines
  if (full_repaint) uipaint_scrollbar(menu_length > menu_max_screen_lines, false);
    
}





// ============================================================================
// paint the popup menu
// ============================================================================
void uipaint_popupMenu(boolean full_repaint) {
  
  // setup the odd flag
  boolean odd = false;
  tft.setFont(uicore_fonts[uicore_font_index].font);
      
  uint16_t ysize  = (menu_screen_line_height * popup_menu_length);
  uint16_t xstart = 35;
  uint16_t ystart = (display_height - ysize) >> 1;
  
  // if we have a full repaint, paint the menu frame
  if (full_repaint) {
    
    uint16_t frame = 11;
        
    // paint the black frame
    tft.setBackColor(color_schemes[uicore_col_scheme].background);
    tft.setColor(color_schemes[uicore_col_scheme].background);
    
    tft.fillRect(xstart - frame, ystart - frame, display_width - xstart + frame, ystart + frame);  
    tft.fillRect(xstart - frame, ystart - frame + ysize - 5, display_width - xstart + frame, ystart + ysize + frame);  
    tft.fillRect(xstart - frame, ystart, xstart, ystart + ysize);
    tft.fillRect(display_width - xstart, ystart, display_width - xstart + frame, ystart + ysize + frame);  
    
    // paint the hightlight frame
    tft.setColor(color_schemes[uicore_col_scheme].scrollbar);
    tft.drawRect(xstart -1, ystart - 1, display_width - xstart + 1, ystart + ysize);
    
    
  }
    
  // loop all popup-menu lines  
  for (int i=0; i<popup_menu_length; i++) {
    
    // calculate the top-y-position for the current line
    int y = ystart + (i * menu_screen_line_height);   
    
    // do we really need to pint this item?
    if (
         (full_repaint) ||
         ( 
           (
             (i == popup_menu_pos)     ||
             (i == popup_menu_pos_old)         
           ) &&
           (
             !(popup_menu_pos == popup_menu_pos_old)
           )
         )
       ) {  
      
      // is this the line we are standing on?    
      if (i == popup_menu_pos) {
        
        // set the colors for the selected item   
        tft.setColor(color_schemes[uicore_col_scheme].font_bg_selected);
        tft.setBackColor(color_schemes[uicore_col_scheme].font_bg_selected);
    
      // regular menu line coloring  
      } else {    
          
        // change the line color (odd / even with alternating colors)
        if (odd) {
          tft.setColor(color_schemes[uicore_col_scheme].font_bg_odd);
          tft.setBackColor(color_schemes[uicore_col_scheme].font_bg_odd);
        } else {
          tft.setColor(color_schemes[uicore_col_scheme].font_bg_even);
          tft.setBackColor(color_schemes[uicore_col_scheme].font_bg_even);
        }
      }
      
      //paint the menu line background
      tft.fillRect(xstart, y, display_width - xstart, y + menu_screen_line_height - 1);
    
      // set the color for the text
      if (i == popup_menu_pos) {
        tft.setColor(color_schemes[uicore_col_scheme].font_selected);
      } else {
        tft.setColor(color_schemes[uicore_col_scheme].font);
      }
      // paint the text line
      tft.print(lines[i], xstart + 2, y + 7);   
    
    }
        
    odd = !odd;
  }
  
  // store the menu position needed for avoiding painting things 
  // that possibly don't need to be painted again the next time
  popup_menu_pos_old = popup_menu_pos;
  
}





// ============================================================================
// paints the edit screen
// ============================================================================
void uipaint_editScreen(boolean full_repaint) {
  
  if (full_repaint) {
    
    tft.setBackColor(color_schemes[uicore_col_scheme].background);
    tft.setColor(color_schemes[uicore_col_scheme].background);
    tft.fillRect(0, header_height, display_width - 1, display_height - 1);
    
    // paint the help text with a small font
    tft.setFont(SMALL_FONT);
    tft.setColor(color_schemes[uicore_col_scheme].font_soft);
    
    // receive the long help text
    uicore_getLongString(line_codes[menu_pos]);
        
    // variables for realizing word-wrapping
    byte lineCount = 0;
    byte fullLen = strlen(data_line);
    byte subLen = 0;
    byte startPos = 0;

    // if we have a help-string
    if (fullLen > 0) {
      
      // loop all charakters of the string
      for (byte i=0; i<fullLen; i++) {
        
        // if we have found a lineFeed
        if (data_line[i] == STR_LINEFEED) {
        
          // if we are not just starting, jump over the delimiter charakter
          if (startPos != 0) startPos++;
          
          // copy the sub string to a temp buffer
          //memcpy(temp, data_line + startPos, i);
          strncpy(temp, &data_line[startPos], i - startPos);
                    
          // add the string-termination charakter          
          temp[i - startPos] = '\0';  
          
          // remember the new start pos for the next string
          startPos = i;
          
          // print the substring
          tft.print(temp, 7, header_height + 10 + (lineCount * 20)); 
          
          // increase the linecount
          lineCount++; 
          
        } 
        
      }
      
      // now paint the last line as this was not covered by the loop above.
      // if we had linefeeds jump over the delimiter charakter
      if (startPos != 0) startPos++;
      
      // copy the sub string to a temp buffer
      //memcpy(temp, data_line + startPos, fullLen);
      strncpy(temp, &data_line[startPos], fullLen - startPos);
      
      // add the string-termination charakter
      temp[fullLen - startPos] = '\0';

      
      // print the substring
      tft.print(temp, 7, header_height + 10 + (lineCount * 20)); 
          
    } // end: we have a help string
          
  } // end: full repaint
  
  // generate the data string
  uicore_generateDataString(line_codes[menu_pos]);
  
  // only paint now, if not 2nd repaint is defined - 
  // in this case we paint later anyway in the 2nd loop
  if (!uicore_is2ndRepaintFlag()) {
    
    // paint the data text
    tft.setColor(color_schemes[uicore_col_scheme].font);
    tft.setFont(uicore_fonts[uicore_font_index].font);
    
    // paint the string
    uint8_t len = (strlen(data_line) - 1) * tft.getFontXsize();
    tft.print(data_line, display_width - 30 - len , 190); 
    
    // clear the rest of the data line (in case ther is still old data)
    tft.setColor(color_schemes[uicore_col_scheme].background);
    tft.fillRect(0, 190, display_width - len - 31, 190 + tft.getFontYsize() - 1);
  }

}





// ============================================================================
// paints the scrollbar if required
// ============================================================================
void uipaint_scrollbar(boolean paint, boolean full_screen_scrolling) {
  
  uint16_t x, y, xend, yend;
  
  x = display_width - 7;
  y = header_height;
  xend = display_width - 1;
  yend = display_height - 1;
  
  
  // clear the scrollbar area
  tft.setColor(color_schemes[uicore_col_scheme].background);
  tft.fillRect( x, y, xend, yend);
  
  if (paint) {
  
    uint8_t first_visible_line; 
    
    // do we scroll with a cursor on the screen or just the full screen?
    if (full_screen_scrolling)   first_visible_line = menu_screen_pos;
    else                         first_visible_line = menu_pos - menu_screen_pos;

    uint16_t bar_max_len = display_height - header_height;
    uint16_t bar_length;
    uint16_t bar_start;
    
    // calc the scale factor
    float scaleFac = 1;
    if (menu_length > menu_max_screen_lines) {
      scaleFac = ((float)bar_max_len / ((float)menu_length * (float)menu_screen_line_height));
    }
    
    // calculate the length of the actual scroll bar
    bar_length = bar_max_len * scaleFac;     

    // calculate where the bar starts with the current position in the list
    bar_start = (first_visible_line * menu_screen_line_height) * scaleFac;
    
    // strange error
    uint16_t barEnd = y + bar_start + bar_length;
    if (barEnd > display_height - 1) barEnd = display_height - 1;
    
    // paint the scrollbar  
    tft.setColor(color_schemes[uicore_col_scheme].scrollbar);  
    tft.fillRect( x + 2, y + bar_start, 
                  xend - 1, barEnd);
   
  }
    
}



// ============================================================================
void uipaint_jogWindow(boolean full_repaint) {
  
  tft.setFont(uicore_fonts[uicore_font_index].font);
      
  uint16_t ysize   = 110;
  uint16_t xstart  = 11;
  uint16_t ystart  = (display_height - ysize) >> 1;
  uint16_t headerh = header_height >> 1;
  
  // if we have a full repaint, paint the menu frame
  if (full_repaint) {
    
    uint16_t frame = 11;
        
    // paint the black frame
    tft.setBackColor(color_schemes[uicore_col_scheme].background);
    tft.setColor(color_schemes[uicore_col_scheme].background);
    
    // black frame
    tft.fillRect(xstart - frame, ystart - frame, display_width - xstart + frame, ystart + frame);  
    tft.fillRect(xstart - frame, ystart - frame + ysize - 5, display_width - xstart + frame, ystart + ysize + frame);  
    tft.fillRect(xstart - frame, ystart, xstart, ystart + ysize);
    tft.fillRect(display_width - xstart, ystart, display_width - xstart + frame, ystart + ysize + frame);  
    
    // header background
    tft.setBackColor(color_schemes[uicore_col_scheme].font_bg_selected);
    tft.setColor(color_schemes[uicore_col_scheme].font_bg_selected);
    tft.fillRect(xstart, ystart, display_width - xstart, ystart + headerh);
    
    // header text
    tft.setBackColor(color_schemes[uicore_col_scheme].font_bg_selected);
    tft.setColor(color_schemes[uicore_col_scheme].font_selected);
    strcpy(data_line, string_81_short);
    strcat(data_line, STR_SPACE1);
    itoa(motor_selected + 1, temp, 10);
    strcat(data_line, temp);
    tft.print(data_line, xstart + 11, ystart + 7);  
    
    // window background
    tft.setBackColor(color_schemes[uicore_col_scheme].background);
    tft.setColor(color_schemes[uicore_col_scheme].background);
    tft.fillRect(xstart, ystart + headerh, display_width - xstart, ystart + ysize);
    
    // data  text
    tft.setBackColor(color_schemes[uicore_col_scheme].background);
    tft.setColor(color_schemes[uicore_col_scheme].font);
    tft.print(string_90_short, xstart + 11, ystart + headerh + 12);  
    tft.print(string_91_short, xstart + 11, ystart + headerh + 47);  
            
    // paint the hightlight frame
    tft.setColor(color_schemes[uicore_col_scheme].scrollbar);
    tft.drawRect(xstart -1, ystart - 1, display_width - xstart + 1, ystart + ysize);
    
  }
  
  
  tft.setBackColor(color_schemes[uicore_col_scheme].background);

  
  uint16_t str_len;
  float jspd = motors[motor_selected].getJogSpeed();
  float mpos = motors[motor_selected].getMotorPosition();  
  
  
  tft.setColor(color_schemes[uicore_col_scheme].font);
  
  sprintf(temp,"%.2f", jspd);
  strcpy(lines[0], STR_SPACE1);
  strcat(lines[0], temp);
  
  // motor type for correct unit 
  if (motors[motor_selected].getType() == TYPE_LINEAR) {
    strcat(lines[0], string_17_short);   // cm/s  
  } else {
    strcat(lines[0], string_16_short);   // °/s
  }
  
  str_len = strlen(lines[0]) * tft.getFontXsize();
  tft.print(lines[0], display_width - xstart - str_len - 7, ystart + headerh + 12);  
   
  sprintf(temp,"%.1f", mpos);
  strcpy(lines[1], STR_SPACE1);
  strcat(lines[1], temp);
  
  // motor type for correct unit 
  if (motors[motor_selected].getType() == TYPE_LINEAR) {  
    strcat(lines[1], string_5_short);    // cm
  } else {
    strcat(lines[1], string_22_short);    // °
  }
  
  str_len = (strlen(lines[1]) + 2) * tft.getFontXsize();
  tft.print(lines[1], display_width - xstart - str_len - 7, ystart + headerh + 47);  
  
  
}





// ============================================================================
void uipaint_splashScreen() {
  
  tft.setBackColor(0, 0, 0);
  tft.clrScr();
   
  int x = 80;
  int y = 70;
    
  tft.setFont(Sinclair_M);
  tft.setColor(255, 255, 255);
  
  int stp = 4;
  
  uicore_fadeInBacklight(3);
    
  uicore_fadeInString("m", x, y, stp, 210, 190, 170, 0);
  uicore_fadeInString("i", x + 14, y, stp, 210, 190, 170, 0);
  uicore_fadeInString("n", x + 32, y, stp, 210, 190, 170, 0);
  uicore_fadeInString("i", x + 45, y, stp, 210, 190, 170, 0);
  if (input_isKeyEvent()) return;
  
  uicore_fadeInString("E", x + 64, y, stp, 255, 255, 255, 0);
  uicore_fadeInString("n", x + 80, y, stp, 255, 255, 255, 0);
  uicore_fadeInString("g", x + 96, y, stp, 255, 255, 255, 0);
  uicore_fadeInString("i", x + 112, y, stp, 255, 255, 255, 0);
  uicore_fadeInString("n", x + 128, y, stp, 255, 255, 255, 0);
  uicore_fadeInString("e", x + 144, y, stp, 255, 255, 255, 0);
  if (input_isKeyEvent()) return;
  
  core_delay(250);
  
  //tft.setFont(DotMatrix_M);
  tft.setColor(255, 230, 150);

  // convert the version definitions to a usable string  
  char version[16];
  char subversion[8];
  itoa(VERSION, version, 10);
  strcat(version, STR_POINT);
  itoa(SUBVERSION, subversion, 10);
  strcat(version, subversion);
  
  // alpha
  strcat(version, STR_SPACE1);
  strcat(version, STR_VER);
  strcat(version, STR_SPACE1);
  
  itoa(SUBSUBVERSION, temp, 10);
  strcat(version, temp);
  
  // how width is the version string?
  uint16_t str_len = strlen(version) * tft.getFontXsize();
  
  if (input_isKeyEvent()) return;
    
  uicore_fadeInString(version, (display_width - str_len) / 2, y + 30, 1, 255, 50, 0, 2); 
  
  if (input_isKeyEvent()) return;
  
  core_delay(1700);
  
  if (input_isKeyEvent()) return;
  
  uicore_fadeOutBacklight(4);
  
}


// ============================================================================
void uidraw_verticalDotLine(int y) {

  for (int i=0; i<display_width; i+=2) {
    tft.drawPixel(i, y); 
  } 

}


