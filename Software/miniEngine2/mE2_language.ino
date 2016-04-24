/*

    Authors: Airic Lenz, Sascha Henke, Marcello Brunetti
    Year of release: 2016
    
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



    ////////////////////////////////////////////////////////////////////

    H O W   T O   U S E   T H I S   F I L E :
    
    ////////////////////////////////////////////////////////////////////

    Please feel free to add your language. To do so please copy the 
    English part from begining and translate all the strings as you like.
    Please note that you only edit the text between the quotation marks.
    
    to make your language usable by the system, change the name of your 
    language from 
    
    #ifdef LANGUANGE_ENGLISH
    
    to the name of your language - for example:
    
    #ifdef LANGUANGE_GERMAN
    
    To enable your language, change the code of the file 
    'miniEngine2.ino' at the lines 62 and following.
    
    These are the available languages at the moment:
    
    LANGUANGE_ENGLISH       (Airic Lenz)
    LANGUANGE_GERMAN        (Sascha Henke, Airic Lenz)
    LANGUANGE_ITALIAN       (Marcello Brunetti) 
    
    ////////////////////////////////////////////////////////////////////
*/







////////////////////////////////////////////////////////////// 
//
//  ▓▓▓▓▓▓▓  ▓▓   ▓▓   ▓▓▓▓▓   ▓▓       ▓▓▓▓   ▓▓▓▓▓   ▓▓   ▓▓
//  ▓▓       ▓▓▓  ▓▓  ▓▓       ▓▓        ▓▓   ▓▓       ▓▓   ▓▓
//  ▓▓▓▓▓▓   ▓▓ ▓ ▓▓  ▓▓ ▓▓▓▓  ▓▓        ▓▓    ▓▓▓▓▓   ▓▓▓▓▓▓▓
//  ▓▓       ▓▓  ▓▓▓  ▓▓   ▓▓  ▓▓        ▓▓        ▓▓  ▓▓   ▓▓
//  ▓▓▓▓▓▓▓  ▓▓   ▓▓   ▓▓▓▓▓   ▓▓▓▓▓▓▓  ▓▓▓▓   ▓▓▓▓▓   ▓▓   ▓▓
//
//////////////////////////////////////////////////////////////     

#ifdef LANGUANGE_ENGLISH

////////////////////////////////////////////////////////
// MAIN STRINGS AND UNITS
    
const char* string_0_short   = "miniEngine";
const char* string_1_short   = "";            // unused 
const char* string_2_short   = "";            // unused 
const char* string_3_short   = "}";
const char* string_4_short   = "{";
const char* string_5_short   = "cm";
const char* string_6_short   = "s";
const char* string_7_short   = "CW";
const char* string_8_short   = "CCW";
const char* string_9_short   = "fps";
const char* string_10_short  = "st/cm";
const char* string_11_short  = "ms";
const char* string_12_short  = "s";
const char* string_13_short  = "min";
const char* string_14_short  = "linear";
const char* string_15_short  = "radial";
const char* string_16_short  = "`/s";     // `=°
const char* string_17_short  = "cm/s";
const char* string_18_short  = "fps";
const char* string_19_short  = "Canon";
const char* string_20_short  = "Nikon";
const char* string_21_short  = "st/`";    // `=°
const char* string_22_short  = "`";       // `=°
const char* string_23_short  = "empty";
const char* string_24_short  = "Running...";
const char* string_25_short  = "Frames";
const char* string_26_short  = "Interval";
const char* string_27_short  = "Mode";
const char* string_28_short  = "Battery";



////////////////////////////////////////////////////////
// SYSTEM MODES

const char* string_30_short = "SMS";
const char* string_31_short = "Cont.";

const char* string_30_long = "Shoot-Move-Shoot";
const char* string_31_long = "Continuous";

//40 for pano

const char* string_50_short = "Run";
const char* string_51_short = "Keyfrm.";

const char* string_50_long = "Run based setup";
const char* string_51_long = "Keyframes";


const char* string_60_short = "Timelapse";
const char* string_61_short = "Video";
const char* string_62_short = "Panorama";

const char* string_60_long = "Timelapse";
const char* string_61_long = "Video";
const char* string_62_long = "Panorama";


////////////////////////////////////////////////////////
// GENERAL EDIT-SCREEN & JOG STRINGS

const char* string_80_short = "Edit screen";
const char* string_81_short = "Jog motor";

const char* string_90_short = "Speed:";
const char* string_91_short = "Pos:";


////////////////////////////////////////////////////////
// GENERAL SETTING STRINGS
    
const char* string_92_short = "Settings";  
const char* string_93_short = "GEN";  
const char* string_94_short = "CAM";  
const char* string_95_short = "MOT";  
const char* string_96_short = "DCH";  
const char* string_97_short = "TRG";  
const char* string_98_short = "Battery:";  

const char* string_93_long = "       General";  
const char* string_94_long = "        Camera";  
const char* string_95_long = "        Motors";  
const char* string_96_long = "Daisy Chaining";  
const char* string_97_long = "      Triggers";  

////////////////////////////////////////////////////////
// SETTINGS GENERAL
const char* string_100_short = "B-Light Pw";
const char* string_101_short = "B-Light Tm";
const char* string_102_short = "Col Scheme";
const char* string_103_short = "Rot Flippd";
const char* string_104_short = "Autosave";
const char* string_105_short = "Show Info";
const char* string_106_short = "Deflt Setg";
const char* string_107_short = "Start Home";
const char* string_108_short = "Mode";
const char* string_109_short = "Setup Style";
const char* string_110_short = "Menu Font";
const char* string_111_short = "Move Style";
const char* string_112_short = "Reset all settings";
const char* string_113_short = "Loop";
const char* string_114_short = "Endless Shoot";


const char* string_100_long  = "Power of the backlight.\nValues above 100% are above 5V and\nabove the specifications of the\ndisplay (Overdrive)!";
const char* string_101_long  = "Time after which the backlight\nturns off.";
const char* string_102_long  = "The global color scheme. Use\nthis to improve dislay readability.";
const char* string_103_long  = "Invert the rotary-knob response.";
const char* string_107_long  = "Move all motors to their home\nposition before the program starts.\nThis takes effect on all\nnon-keyframe modes.";
const char* string_108_long  = "The way in which the motors move.";
const char* string_109_long  = "The way the moves are defined.\n'Keyframes' requires a connection to\na computer!";
const char* string_110_long  = "The used font on the display.";
const char* string_111_long  = "How should the motors move?";
const char* string_112_long  = "This will reset all settings\nto the default values.\nPress the Rotary button to Reset.\nPress [Start] to abort.";
const char* string_113_long  = "This feature will restart the move\nin the reverse direction after it is\ndone. This is only used in Video mode\nand will continue until you stop\nthe move.";
const char* string_114_long  = "This option lets the camera\ncontinue shooting after the program\nis done until stopped by the user.";

const char* string_115_long  = "Reset";
const char* string_116_long  = "The configuration was deleted.";
const char* string_117_long  = "Please turn the miniEngine OFF";
const char* string_118_long  = "and then ON again.";

////////////////////////////////////////////////////////
// SETTINGS CAMERA
const char* string_120_short = "Cam Type";
const char* string_121_short = "Test Shot";
const char* string_122_short = "Post Delay";
const char* string_123_short = "SHOOT!";
const char* string_124_short = "Focus Time";

const char* string_120_long  = "Camera trigger behaviour.";
const char* string_122_long  = "Delay after a camera shot before the\nmotor moves. Use this if you have\ndefined a long exposure-time in your\ncamera.";
const char* string_124_long  = "Time the camera focuses before it\nriggers. This can be used to wake\nup the camera.";


////////////////////////////////////////////////////////
// SETTINGS MOTOR
const char* string_140_short = "Motor Sel";
const char* string_141_short = "Go Home";  
const char* string_142_short = "Set Home";  
const char* string_143_short = "Post Delay";  
const char* string_144_short = "Motor Type";  
const char* string_145_short = "Max Speed";  
const char* string_146_short = "Ramp Time"; 
const char* string_147_short = "Limit SW"; 
const char* string_148_short = "Jog Motor 1"; 
const char* string_149_short = "Jog Motor 2"; 
const char* string_150_short = "Keep powered"; 
const char* string_151_short = "Calibratn"; 
const char* string_152_short = "Go Home (all)";  
const char* string_153_short = "Motor ";  
const char* string_154_short = "Motor Sleep";
const char* string_155_short = "Dir Flippd";
const char* string_156_short = "Set Start";
const char* string_157_short = "Set End";
const char* string_158_short = "GO!";
const char* string_159_short = "SET!";
const char* string_160_short = "Preview";
const char* string_161_short = "Check Speed";

const char* string_140_long  = "Select the motor you want to edit.";
const char* string_143_long  = "Delay after a motor move. Use this\ndelay for letting the motor\nsettle after a move.";
const char* string_144_long  = "The type of movements this motor\ndoes (linear or radial).";
const char* string_145_long  = "Max speed of the motor.\nPress [Menu] to change the granularity.";
const char* string_146_long  = "Ramping-duration for moves.\nShort times become faster but less\nsmooth moves.";
const char* string_150_long  = "Keep the motor powerd all the time.\nWhen enabled the motor will be\npowered immediately.";
const char* string_151_long  = "Motor and hardware calibration value\nin steps per cm or steps per `.\nPress [Menu] to change the granularity.";
const char* string_154_long  = "Turn the motor off when not used\n(Even during recording!).";
const char* string_155_long  = "Invert the motor direction.";
const char* string_161_long  = "Check the max speed for this\nmotor or ignore it. WARNING: Turning\nthis option off can damage\nyour system.";

////////////////////////////////////////////////////////
// SETTINGS CHAIN

const char* string_165_short = "Chain ID";
const char* string_166_short = "Master";
const char* string_167_short = "Device ";
const char* string_168_short = "Upd. Info";
const char* string_169_short = "UPDATE!";

const char* string_165_long  = "The id of the device in the\ndaisy-chain-network. Every\ndevice needs a unique ID!";
const char* string_168_long  = "Retreives the available devices\nin the daisy-chain-network.";


////////////////////////////////////////////////////////
// SETTINGS TRIGGER
const char* string_180_short = "Trigger Sel";
const char* string_181_short = "Enabled";
const char* string_182_short = "Action";
const char* string_183_short = "Type";
const char* string_184_short = "Trg. ";
const char* string_185_short = "Rising";
const char* string_186_short = "Falling";
const char* string_187_short = "Debounce";

const char* string_180_long  = "Select the trigger you want to edit.";
const char* string_181_long  = "Status of this trigger - On / Off.";
const char* string_182_long  = "This defines the action that should be\ndone when the trigger is triggered.\nStart = Start the program\nStop  = Stop the program\nShoot = Trigger the camera";
const char* string_183_long  = "The actual signal type to which this\ntrigger should listen.\nRAISING EDGE (0V --> 3.3V)\nFALLING EDGE (3.3V --> 0V)";
const char* string_187_long  = "Smoothens the trigger signal via\nsoftware when enabled.";

////////////////////////////////////////////////////////
// mode settings
    
const char* string_200_short = "Total Dist";  
const char* string_201_short = "Direction";  
const char* string_202_short = "Exposure";  
const char* string_203_short = "Rec Time";  
const char* string_204_short = "Playb Time";
const char* string_205_short = "Playb FPS";
const char* string_206_short = "Ramp In";
const char* string_207_short = "Ramp Out";
const char* string_208_short = "Move Delay";

const char* string_200_long = "The total move-distance of this motor\nduring the recording.\nPress [Menu] to change the granularity.";
const char* string_201_long = "The motor direction.";
const char* string_202_long = "The camera exposure time. This can\nbe 1/10 for just triggering the\ncamera. The real exposure might be\ndefined in the camera (see also\nCamera post delay)";
const char* string_203_long = "The total record-duration.";
const char* string_204_long = "Length of the final clip you want.";
const char* string_205_long = "Frames Per Second of your final clip.\nThis influences how many photos will\nbe made.";
const char* string_206_long = "Percent of move-time to ramp into\nthe move.";
const char* string_207_long = "Percent of move-time to ramp out\nof the move.";
const char* string_208_long = "Percent of record-time to wait\nbefore the move starts.";

////////////////////////////////////////////////////////
// panorama

const char* string_210_short = "# Pics Mot.1";
const char* string_211_short = "# Pics Mot.2";

const char* string_210_long = "The amount of pictures on the axis\nof motor 1.";
const char* string_211_long = "The amount of pictures on the axis\nof motor 2.";

////////////////////////////////////////////////////////
// messages

const char* string_220_long = "Trigger";
const char* string_221_long = "Waiting for the start-signal.";
const char* string_222_long = "Press [SELECT] to abort.";

const char* string_225_long = "Home";
const char* string_226_long = "Moving motors to the home-position.";

const char* string_227_long = "Speed warning";
const char* string_228_long = "Motor-speed limit for at least one";
const char* string_229_long = "motor exceeded for this move!";

const char* string_230_long = "Preview";
const char* string_231_long = "The timelpase preview is now";
const char* string_232_long = "done in real-speed...";

const char* string_233_long = "Daisy Chain";
const char* string_234_long = "Updating the daisy chain info.";
const char* string_235_long = "This might take some seconds...";
const char* string_236_long = "Registered in the chain...";
const char* string_237_long = "Waiting for master sync...";
const char* string_238_long = "Continue with any key.";

#endif 







////////////////////////////////////////////////////////
//
//   ▓▓▓▓▓   ▓▓▓▓▓▓▓  ▓▓▓▓▓▓   ▓▓   ▓▓   ▓▓▓▓▓   ▓▓   ▓▓
//  ▓▓       ▓▓       ▓▓   ▓▓  ▓▓▓ ▓▓▓  ▓▓   ▓▓  ▓▓▓  ▓▓
//  ▓▓ ▓▓▓▓  ▓▓▓▓▓    ▓▓▓▓▓▓   ▓▓ ▓ ▓▓  ▓▓▓▓▓▓▓  ▓▓ ▓ ▓▓
//  ▓▓   ▓▓  ▓▓       ▓▓▓▓     ▓▓   ▓▓  ▓▓   ▓▓  ▓▓  ▓▓▓
//   ▓▓▓▓▓   ▓▓▓▓▓▓▓  ▓▓  ▓▓▓  ▓▓   ▓▓  ▓▓   ▓▓  ▓▓   ▓▓
// 
////////////////////////////////////////////////////////

#ifdef LANGUANGE_GERMAN

////////////////////////////////////////////////////////
// MAIN STRINGS AND UNITS
    
const char* string_0_short   = "miniEngine";
const char* string_1_short   = "";            // unused 
const char* string_2_short   = "";            // unused 
const char* string_3_short   = "}";
const char* string_4_short   = "{";
const char* string_5_short   = "cm";
const char* string_6_short   = "s";
const char* string_7_short   = "rechts";
const char* string_8_short   = "links";
const char* string_9_short   = "fps";
const char* string_10_short  = "st/cm";
const char* string_11_short  = "ms";
const char* string_12_short  = "s";
const char* string_13_short  = "min";
const char* string_14_short  = "linear";
const char* string_15_short  = "radial";
const char* string_16_short  = "`/s";     // `=°
const char* string_17_short  = "cm/s";
const char* string_18_short  = "fps";
const char* string_19_short  = "Canon";
const char* string_20_short  = "Nikon";
const char* string_21_short  = "st/`";    // `=°
const char* string_22_short  = "`";       // `=°
const char* string_23_short  = "leer";
const char* string_24_short  = "Lauft...";
const char* string_25_short  = "Frames";
const char* string_26_short  = "Intervall";
const char* string_27_short  = "Modus";
const char* string_28_short  = "Batterie";



////////////////////////////////////////////////////////
// SYSTEM MODES

const char* string_30_short = "SMS";
const char* string_31_short = "Kont.";

const char* string_30_long = "Shoot-Move-Shoot";
const char* string_31_long = "Kontinuierlich";


const char* string_50_short = "Lauf";
const char* string_51_short = "Schl.Bilder";

const char* string_50_long = "Lauf Setup";
const char* string_51_long = "Schluesselbilder";


const char* string_60_short = "Zeitraffer";
const char* string_61_short = "Video";
const char* string_62_short = "Panorama";

const char* string_60_long = "Zeitraffer";
const char* string_61_long = "Video";
const char* string_62_long = "Panorama";


////////////////////////////////////////////////////////
// GENERAL EDIT-SCREEN & JOG STRINGS

const char* string_80_short = "Einstellungen";
const char* string_81_short = "Mot. beweg.";

const char* string_90_short = "Geschw:";
const char* string_91_short = "Pos:";


////////////////////////////////////////////////////////
// GENERAL SETTING STRINGS
    
const char* string_92_short = "Einstellungen";  
const char* string_93_short = "BAS";  
const char* string_94_short = "KAM";  
const char* string_95_short = "MOT";  
const char* string_96_short = "KOP";  
const char* string_97_short = "TRG";  
const char* string_98_short = "Batterie:";  

const char* string_93_long = "         Basic";  
const char* string_94_long = "        Kamera";  
const char* string_95_long = "       Motoren";  
const char* string_96_long = "      Kopplung";  
const char* string_97_long = "      Triggers";  

////////////////////////////////////////////////////////
// SETTINGS GENERAL
const char* string_100_short = "H-Licht Kraft";
const char* string_101_short = "H-Licht Zeit";
const char* string_102_short = "Farbschema";
const char* string_103_short = "Drehknopf invers";
const char* string_104_short = "Autospeichern";
const char* string_105_short = "Zeige Info";
const char* string_106_short = "Std. Einst.";
const char* string_107_short = "Beginn bei Start";
const char* string_108_short = "Modus";
const char* string_109_short = "Setup";
const char* string_110_short = "Schriftart";
const char* string_111_short = "Bewegart";
const char* string_112_short = "Zuruecksetzen";
const char* string_113_short = "Wiederholen";
const char* string_114_short = "Endlose Aufnahme";

const char* string_100_long  = "Helligkeit des Hintergrundlichts.\nEinstellungen ueber 100% sind ueber\n5V und ueber den Spezifikationen des\nDisplays!";
const char* string_101_long  = "Zeit nach der das Hintergrundlicht\naus geht.";
const char* string_102_long  = "Das Grundfarbschema. Benutze dies,\num die Lesbarkeit des Displays\nsicherzustellen.";
const char* string_103_long  = "Dreht die Richtung des\nDrehknopfs um.";
const char* string_107_long  = "Bewege alle Motoren zu Startposition\nbevor das Programm startet.\nDies hat nur Auswirkung auf Programme\nohne Schluesselbilder.";
const char* string_108_long  = "Die Auswahl, was du aufnehmen\nwillst.";
const char* string_109_long  = "Wie willst du deine Kamera-\nfahrt definieren?\nSchluesselbilder = setze Start & Ende\nLauf = definiere die Distanz";
const char* string_110_long  = "Die Schriftart, die benutzt werden\nsoll.";
const char* string_111_long  = "Wie sollen die Motoren sich bewegen?";
const char* string_112_long  = "Dies setzt alle Einstellungen zurueck\nauf die Standardeinstellungen.\nDruecke den Drehknopf zum bestaetigen.\nDruecke [Start] zum abbrechen.";
const char* string_113_long  = "Dies sorgt dafuer, dass die\nminiEngine den Lauf immer wieder\nwiederholt. Dies wird fortgesetzt,\nbis du abbrichst.";
const char* string_114_long  = "Diese Option laesst die Kamera\nnach Programmende weiter aufnehmen\nbis der Nutzer abbricht.";

const char* string_115_long  = "Reset";
const char* string_116_long  = "Die Einstellungen wurden geloescht.";
const char* string_117_long  = "Bitte schalte die miniEngine";
const char* string_118_long  = "aus, und dann wieder ein.";


////////////////////////////////////////////////////////
// SETTINGS CAMERA
const char* string_120_short = "Kamera Typ";
const char* string_121_short = "Kamera Test";
const char* string_122_short = "Pause danach";
const char* string_123_short = "TEST!";
const char* string_124_short = "Fokuszeit";

const char* string_120_long  = "Ausloeserverhalten der Kamera.";
const char* string_122_long  = "Zeit nach dem Ausloesen\nbevor der Motor sich bewegt.\nBenutze dies wenn du\nfur deine Kamera eine lange\nAusloesezeit definiert hast.";
const char* string_124_long  = "Die Zeit, die die Kamera\nvor dem ausloesen zum\nScharfstellen hat. Dies kann\nbenutzt werden um die Kamera aufzu-\nwecken.";

////////////////////////////////////////////////////////
// SETTINGS MOTOR
const char* string_140_short = "Motor Ausw";
const char* string_141_short = "Geh zu Start";  
const char* string_142_short = "Setze Start";  
const char* string_143_short = "Pause danach";  
const char* string_144_short = "Motor Typ";  
const char* string_145_short = "Max Geschw";  
const char* string_146_short = "Rampenzeit"; 
const char* string_147_short = "Endschalter"; 
const char* string_148_short = "Bewege Motor 1"; 
const char* string_149_short = "Bewege Motor 2"; 
const char* string_150_short = "Halte Strom"; 
const char* string_151_short = "Kalibr."; 
const char* string_152_short = "Alle zum Start";  
const char* string_153_short = "Motor ";  
const char* string_154_short = "Motorschlaf";
const char* string_155_short = "Richtng invers";
const char* string_156_short = "Setze Start";
const char* string_157_short = "Setze Ende";
const char* string_158_short = "START!";
const char* string_159_short = "SETZE!";
const char* string_160_short = "Vorschau";
const char* string_161_short = "Geschw-Kontr.";

const char* string_140_long  = "Waehle den zu bearbeitenden\nMotor aus.";

const char* string_143_long  = "Zeitverzoegerung nach der Motor-\nbewegung. Benutze diese Einstellung\num Vibrationen zu minimieren.";
const char* string_144_long  = "Die Art wie der Motor sich bewegt\n(linear oder radial).";
const char* string_145_long  = "Maximale Geschwindigkeit des Motors.\nDruecke [Menue]\num die Granularitaet zu aendern.";
const char* string_146_long  = "Rampendauer fuer Bewegungen.\nKuerzere Zeiten bedeuten schnellere\nBewegung aber dafuer weniger sanft.";
const char* string_150_long  = "Motor unter Strom halten.\nWenn aktiviert bekommt der Motor\nsofort Strom.";
const char* string_151_long  = "Motor- und Hardwarekalibrierung in\nSchritten / cm oder Schritten / Grad.\nDruecke [Menue]\num die Granularitaet zu aendern.";
const char* string_154_long  = "Schaltet den Motor aus\nwenn er nicht benutzt wird\n(Auch bei Aufnahme!).";
const char* string_155_long  = "Dreht die Motordrehrichtung um.";
const char* string_161_long  = "Prueft die max. Geschwindigkeit fuer\ndiesen Motor.\nAchtung: Das Abschalten\ndieser Funktion kann dein System\nbeschaedigen.";

////////////////////////////////////////////////////////
// SETTINGS CHAIN

const char* string_165_short = "GeraeteID";
const char* string_166_short = "Meister";
const char* string_167_short = "Geraet ";
const char* string_168_short = "Netz auslesen";
const char* string_169_short = "LIES!";

const char* string_165_long  = "Die ID dieses Geraetes im\nVerkettungsnetzwerk. Jedes\nGeraet benoetigt eine eineindeutige ID!";
const char* string_168_long  = "Liest die verfuegbaren Geraete,rec\ndie sich im Netzwerk befinden.";

////////////////////////////////////////////////////////
// SETTINGS TRIGGER
const char* string_180_short = "Trigger Ausw.";
const char* string_181_short = "aktiv";
const char* string_182_short = "Aktion";
const char* string_183_short = "Typ";
const char* string_184_short = "Trg. ";
const char* string_185_short = "Steigend";
const char* string_186_short = "Fallend";
const char* string_187_short = "Filtern";

const char* string_180_long  = "Waehle den Trigger, den du bearbeiten\nmoechtest.";
const char* string_181_long  = "Status des Triggers - An / Aus.";
const char* string_182_long  = "Dies definiert was passiert, wenn\nder Trigger ausgeloest wird.\nStart = Programmstart\nStop  = Stop des Programms\nShoot = Kamera ausloesen";
const char* string_183_long  = "Welches Signal soll den Trigger\nausloesen.\nSteigend (0V --> 3.3V)\nFallend (3.3V --> 0V)";
const char* string_187_long  = "Filtert das Eingangssignal so,\ndass Stoerungen in der Software\nherausgefiltert werden.";

////////////////////////////////////////////////////////
// mode settings
    
const char* string_200_short = "Distanz";  
const char* string_201_short = "Richtung";  
const char* string_202_short = "Belichtung";  
const char* string_203_short = "Aufnahme";  
const char* string_204_short = "Wiedergb";
const char* string_205_short = "Abspiel-FPS";
const char* string_206_short = "Beschleunigung";
const char* string_207_short = "Abbremsung";
const char* string_208_short = "Motor Delay";

const char* string_200_long = "Die Gesamtstrecke des Motors\nwaehrend der Aufnahme.\nDruecke [Menue]\num die Granularitaet zu aendern.";
const char* string_201_long = "Die Richtung der Motordrehung";
const char* string_202_long = "Die Belichtungszeit. Diese kann\nz.B. 1/10 sek zum Ausloesen der\nKamera betragen. Die wirkliche Ausloese-\nzeit wird in der Kamera definiert (siehe\nebenso Kamera Verzoegerung\nnach Ausloesung)";
const char* string_203_long = "Die Gesamtaufnahmezeit.";
const char* string_204_long = "Die Laenge des Clips der\nerstellt werden soll.";
const char* string_205_long = "Bilder pro Sekunde vom finalen Clip.\nDies beeinflusst wieviele Fotos\ninsgesamt gemacht werden.";
const char* string_206_long = "Prozent der Motorzeit\nfuer die Beschleunigungsphase.";
const char* string_207_long = "Prozent der Motorzeit\nfuer die Abbremsphase.";
const char* string_208_long = "Prozent of Aufnahmezeit\nals Verzoegerung befor der Motor\nstartet.";


////////////////////////////////////////////////////////
// panorama

const char* string_210_short = "# Pics Mot.1";
const char* string_211_short = "# Pics Mot.2";

const char* string_210_long = "The amount of pictures on the axis\nof motor 1.";
const char* string_211_long = "The amount of pictures on the axis\nof motor 2.";


////////////////////////////////////////////////////////
// messages

const char* string_220_long = "Trigger";
const char* string_221_long = "Warte auf das Start-Signal.";
const char* string_222_long = "Druecke [SELECT] um abzubrechen.";

const char* string_225_long = "Start";
const char* string_226_long = "Bewege Motoren zur Startposition.";

const char* string_227_long = "Geschw. Warnung";
const char* string_228_long = "Geschwindigkeitslimit fuer min. 1";
const char* string_229_long = "Motor ueberschritten!";

const char* string_230_long = "Vorschau";
const char* string_231_long = "Die Timelpase-Vorschau wird nun";
const char* string_232_long = "in Echtzeit gefahren...";

const char* string_233_long = "Verkettung";
const char* string_234_long = "Netzwerk wird analysiert.";
const char* string_235_long = "Dies kann etwas dauern...";
const char* string_236_long = "Im Netzwerk registriert...";
const char* string_237_long = "Warte auf Startsignal...";
const char* string_238_long = "Weiter mit jeder Taste...";

#endif





////////////////////////////////////////////////////////////// 
//                                                          //
//    ▓▓▓▓  ▓▓▓▓▓▓    ▓▓    ▓▓     ▓▓▓▓     ▓▓    ▓▓   ▓▓   //
//     ▓▓     ▓▓     ▓▓▓▓   ▓▓      ▓▓     ▓▓▓▓   ▓▓▓  ▓▓   //
//     ▓▓     ▓▓    ▓▓  ▓▓  ▓▓      ▓▓    ▓▓  ▓▓  ▓▓ ▓ ▓▓   //
//     ▓▓     ▓▓    ▓▓▓▓▓▓  ▓▓      ▓▓    ▓▓▓▓▓▓  ▓▓  ▓▓▓   //
//    ▓▓▓▓    ▓▓    ▓▓  ▓▓  ▓▓▓▓▓  ▓▓▓▓   ▓▓  ▓▓  ▓▓   ▓▓   //
//                                                          //
//////////////////////////////////////////////////////////////     

#ifdef LANGUANGE_ITALIAN

////////////////////////////////////////////////////////
// MAIN STRINGS AND UNITS
    
const char* string_0_short   = "miniEngine";
const char* string_1_short   = "";            // unused 
const char* string_2_short   = "";            // unused 
const char* string_3_short   = "}";
const char* string_4_short   = "{";
const char* string_5_short   = "cm";
const char* string_6_short   = "s";
const char* string_7_short   = "Orario";
const char* string_8_short   = "Antiorario";
const char* string_9_short   = "fps";
const char* string_10_short  = "passi/cm";
const char* string_11_short  = "ms";
const char* string_12_short  = "s";
const char* string_13_short  = "min";
const char* string_14_short  = "lineare";
const char* string_15_short  = "radiale";
const char* string_16_short  = "`/s";     // `=°
const char* string_17_short  = "cm/s";
const char* string_18_short  = "fps";
const char* string_19_short  = "Canon";
const char* string_20_short  = "Nikon";
const char* string_21_short  = "st/`";    // `=°
const char* string_22_short  = "`";       // `=°
const char* string_23_short  = "Vuoto";
const char* string_24_short  = "In Funzione...";
const char* string_25_short  = "Frames";
const char* string_26_short  = "Interval";
const char* string_27_short  = "Modo";
const char* string_28_short  = "Batteria";



////////////////////////////////////////////////////////
// SYSTEM MODES

const char* string_30_short = "SMS";
const char* string_31_short = "Cont.";

const char* string_30_long = "Scatt.Mov.Scatt";
const char* string_31_long = "Continuo";

//40 for pano

const char* string_50_short = "Mov";
const char* string_51_short = "Keyfrm.";

const char* string_50_long = "Movimento";
const char* string_51_long = "Keyframes";


const char* string_60_short = "Timelapse";
const char* string_61_short = "Video";
const char* string_62_short = "Panorama";

const char* string_60_long = "Timelapse";
const char* string_61_long = "Video";
const char* string_62_long = "Panorama";


////////////////////////////////////////////////////////
// GENERAL EDIT-SCREEN & JOG STRINGS

const char* string_80_short = "Modifica schermo";
const char* string_81_short = "Muovi motore";

const char* string_90_short = "Velocita':";
const char* string_91_short = "Posizione:";


////////////////////////////////////////////////////////
// GENERAL SETTING STRINGS
    
const char* string_92_short = "Impostazioni";  
const char* string_93_short = "GEN";  
const char* string_94_short = "CAM";  
const char* string_95_short = "MOT";  
const char* string_96_short = "RET";  
const char* string_97_short = "PUL";  
const char* string_98_short = "Batteria:";  

const char* string_93_long = "       Generali";  
const char* string_94_long = "        Camera";  
const char* string_95_long = "        Motori";  
const char* string_96_long = "          Rete";  
const char* string_97_long = "      Pulsanti";  

////////////////////////////////////////////////////////
// SETTINGS GENERAL
const char* string_100_short = "Pot. Luce LCD";
const char* string_101_short = "Temp Luce LCD";
const char* string_102_short = "Colori LCD";
const char* string_103_short = "Inv. Selettore";
const char* string_104_short = "Autosave";
const char* string_105_short = "Mostra Info";
const char* string_106_short = "Imp Dflt";
const char* string_107_short = "Start Home";
const char* string_108_short = "Modo";
const char* string_109_short = "Stile Imp.";
const char* string_110_short = "Font Menu";
const char* string_111_short = "Stile Mov.";
const char* string_112_short = "Reset Impostazioni";
const char* string_113_short = "Ciclo";
const char* string_114_short = "Endless shoot";

const char* string_100_long  = "Livello retroilluminazione.\nValori sopra al 100% sono sopra ai 5V\ne sopra alle specifiche costruttore\ndello schermo (Sovralimentazione)!";
const char* string_101_long  = "Tempo dopo il quale lo schermo si\ndisattiva.";
const char* string_102_long  = "Lo schema colori dello schermo. Usa\nquesto per migliorare la leggibilita'\ndello schermo.";
const char* string_103_long  = "Inverti la risposta di direzione\ndella manopola.";
const char* string_107_long  = "Muovi i motori al punto di partenza\nprima che il programma parta.\nQuesto interessa tutte\nle modalita' non-keyframe.";
const char* string_108_long  = "Il modo in cui i motori muovono.";
const char* string_109_long  = "Il modo in cui sono definiti i movimen\nti.'Keyframes' necessita di una\nconnessione ad un computer!";
const char* string_110_long  = "Il font in uso sul display.";
const char* string_111_long  = "Come si deve muovere?";
const char* string_112_long  = "Questo reimposta tutte le impostazio\nni sui valori di default.\nPremi il rotore per confermare.\nPremi il tasto [Start] per annullare.";
const char* string_113_long  = "Questa impostazione e' solo per la\nmodalita' video e serve a ripetere\nciclicamente il movimento in direzione\n contraria fino allo stop manuale";
const char* string_114_long  = "This option lets the camera\ncontinue shooting after the program\nis done until stopped by the user.";

const char* string_115_long  = "Reset";
const char* string_116_long  = "La configurazione e' stata eliminata.";
const char* string_117_long  = "Spegni il miniengine";
const char* string_118_long  = "e riaccendilo un altra volta.";

////////////////////////////////////////////////////////
// SETTINGS CAMERA
const char* string_120_short = "Tipo Camera";
const char* string_121_short = "Test Scatto";
const char* string_122_short = "Post. Rit.";
const char* string_123_short = "SHOOT!";
const char* string_124_short = "Tempo Fuoco";

const char* string_120_long  = "Ambiente di scatto camera.";
const char* string_122_long  = "Ritardo dopo che la macchina ha scat\ntato prima che i motori si muovano. Usa questo se hai\nimpostato delle lunghe esposizioni sulla\nfotocamera.";
const char* string_124_long  = "Tempo di messa a fuoco prima che la \nfotocamera scatti. This può essere usato per\nsvegliare la fotocamera dallo stand-by.";


////////////////////////////////////////////////////////
// SETTINGS MOTOR
const char* string_140_short = "Motore";
const char* string_141_short = "Vai allo zero";  
const char* string_142_short = "Imposta Zero";  
const char* string_143_short = "Rit.post Mov.";  
const char* string_144_short = "Tipo Mov.";  
const char* string_145_short = "Max Vel.";  
const char* string_146_short = "Intro Mossa"; 
const char* string_147_short = "Limitatore"; 
const char* string_148_short = "Muovi Motore 1"; 
const char* string_149_short = "Muovi Motore 2"; 
const char* string_150_short = "Mantieni Tensione"; 
const char* string_151_short = "Calibra"; 
const char* string_152_short = "Azzera tutti";  
const char* string_153_short = "Motore ";  
const char* string_154_short = "Stand-by Motore";
const char* string_155_short = "Inverti Direzione";
const char* string_156_short = "Imp. Zero";
const char* string_157_short = "Imp. Fine";
const char* string_158_short = "VAI!";
const char* string_159_short = "SET!";
const char* string_160_short = "Anteprima";
const char* string_161_short = "Controllo vel.";

const char* string_140_long  = "Seleziona il motore che vuoi \nmodificare.";
const char* string_143_long  = "Ritardo dopo il movimento. Usalo per\nlasciare stabilizzare la macchina\ndopo il movimento.";
const char* string_144_long  = "Il tipo di movimento che il motore\ndeve fare (lineare o radiale).";
const char* string_145_long  = "Velocita' massima del motore.\nPremi [Menu] per cambiare la\nrisoluzione.";
const char* string_146_long  = "Durata introduzione del movimento.\nTempi corti rendono piu' veloce ma\nmeno morbidi i movimenti.";
const char* string_150_long  = "Tieni il motore sempre alimentato.\nQuando abilitato il motore viene\nalimentato immediatamente.";
const char* string_151_long  = "Valore di calibrazione del motore\nin step per ogni centimetro (lineare)\no grado (radiale). Premi [Menu] per\n cambiare la nrisoluzione.";
const char* string_154_long  = "Spegni il motore quando non in uso\n(Anche durante le registrazioni!).";
const char* string_155_long  = "Inverti la direzione del motore.";
const char* string_161_long  = "Controlla la velocita' per questo\nmotore. ATTENZIONE: Spegnere questo\ncontrollo potrebbe danneggiare il tuo\n sistema.";

////////////////////////////////////////////////////////
// SETTINGS CHAIN

const char* string_165_short = "ID Rete";
const char* string_166_short = "Master";
const char* string_167_short = "Dispositivo ";
const char* string_168_short = "Agg. Info";
const char* string_169_short = "Aggiorna!";

const char* string_165_long  = "L'id del dispositivo nella rete. Ogni\ndispositivo deve avere un ID univoco!";
const char* string_168_long  = "Recupera i dispositivi presenti\n nella rete.";


////////////////////////////////////////////////////////
// SETTINGS TRIGGER
const char* string_180_short = "Seleziona";
const char* string_181_short = "Attivo";
const char* string_182_short = "Azione";
const char* string_183_short = "Tipo";
const char* string_184_short = "Pulsante";
const char* string_185_short = "Ascendente";
const char* string_186_short = "Discendente";
const char* string_187_short = "Rimbalzo";

const char* string_180_long  = "Seleziona il pulsante da editare.";
const char* string_181_long  = "Stato del pulsante - On / Off.";
const char* string_182_long  = "Definische l'azione che deve essere\nfatta quando il pulsante e' premuto.\nStart = Inizio del programma\nStop  = Stop del programma\nShoot = Scatto fotocamera";
const char* string_183_long  = "Il tipo di segnale che deve essere\ninterpretato.\nAscendente (0V --> 3.3V)\nDiscendente (3.3V --> 0V)";
const char* string_187_long  = "Stoppa il segnale attraverso il\nsoftware quando attivo.";

////////////////////////////////////////////////////////
// mode settings
    
const char* string_200_short = "Dist. Totale";  
const char* string_201_short = "Direzione";  
const char* string_202_short = "Esposizione";  
const char* string_203_short = "Temp. Reg.";  
const char* string_204_short = "Dur. Video";
const char* string_205_short = "Fps Video";
const char* string_206_short = "Introduzione";
const char* string_207_short = "Conclusione";
const char* string_208_short = "Motor Delay";

const char* string_200_long = "La distanza totale percorsa durante\nle riprese. Premi [Menu] per cambiare\nla risoluzione.";
const char* string_201_long = "La direzione del motore.";
const char* string_202_long = "Il tempo di esposizione dello scatto.\n Può essere 1/10 solo per far \nscattare la fotocamera. La reale \nesposizione deve essere impostata nella\nfotocamera (Controllare anche ritardo\npost scatto)";
const char* string_203_long = "La durata totale della registrazione.";
const char* string_204_long = "Lunghezza del video finale desiderato.";
const char* string_205_long = "Frame al secondo del video finale.\nQuesto influenza il numero delle foto\nche verranno scattate.";
const char* string_206_long = "Percentuale del tempo di registrazione\nper l'introduzione del movimento";
const char* string_207_long = "Percentuale del tempo di registrazione\nper la conclusione del movimento";
const char* string_208_long = "Percent of record-time to wait\nbefore the move starts.";


////////////////////////////////////////////////////////
// panorama

const char* string_210_short = "# Pics Mot.1";
const char* string_211_short = "# Pics Mot.2";

const char* string_210_long = "The amount of pictures on the axis\nof motor 1.";
const char* string_211_long = "The amount of pictures on the axis\nof motor 2.";


////////////////////////////////////////////////////////
// messages

const char* string_220_long = "Pulsante";
const char* string_221_long = "In attesa del segnale di partenza.";
const char* string_222_long = "Premi [SELECT] per terminare.";

const char* string_225_long = "Home";
const char* string_226_long = "Muovendo i motori alla posizione di partenza.";

const char* string_227_long = "Spia Velocità";
const char* string_228_long = "Limite per almeno un motore";
const char* string_229_long = "Limite superato per questo movimento!";

const char* string_230_long = "Anteprima";
const char* string_231_long = "L'anteprima del tme lapse e' in corso";
const char* string_232_long = "alla reale velocita' finale...";

const char* string_233_long = "Rete dispositivi";
const char* string_234_long = "Aggiorno le informazioni della rete.";
const char* string_235_long = "Attendi alcuni secondi...";
const char* string_236_long = "Registrato nella rete...";
const char* string_237_long = "Attendo il dispositivo Master...";
const char* string_238_long = "Premi un tasto per continuare.";

#endif 








