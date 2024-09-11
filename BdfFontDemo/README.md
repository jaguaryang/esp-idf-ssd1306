# BdfFontDemo for SSD1306

![BdfFontDemo-1](https://github.com/user-attachments/assets/6e4fb518-c835-44ff-b457-ae6c53158f27)
![BdfFontDemo-2](https://github.com/user-attachments/assets/224d12c4-8ecf-4116-868a-8f11ca480122)
![BdfFontDemo-3](https://github.com/user-attachments/assets/4dbb6f39-ae0f-4ca2-9e55-ba32476b018e)

u8g2 is a great Library for monochrome displays.   
This library contains a lot of BDF format fonts.   
This project is a demo that displays BDF format fonts.   

# How to use BDF Font
- Download the BDF font file.   
 Thank you olikraus for releasing useful software.   

 ```
 git clone https://github.com/olikraus/u8g2
 ls $HOME/u8g2-hal-esp-idf/examples/i2c/components/u8g2/tools/font/bdf
 ```

- Convert BDF font files to header files.   
 I used [this](https://github.com/pixelmatix/bdf2c) repository.   
 Thank you Pixelmatix for releasing useful software.   
```
cd esp-idf-ssd1306/BdfFontDemo

cc -o bdf2c bdf2c.c

./bdf2c -n ncenR12 -b < $HOME/u8g2-hal-esp-idf/examples/i2c/components/u8g2/tools/font/bdf/ncenR12.bdf > main/ncenR12.h

./bdf2c -n timR12 -b < $HOME/u8g2-hal-esp-idf/examples/i2c/components/u8g2/tools/font/bdf/timR12.bdf > main/timR12.h

./bdf2c -n battery -b < $HOME/u8g2-hal-esp-idf/examples/i2c/components/u8g2/tools/font/battery/battery19.bdf > main/battery.h

./bdf2c -n emoticons -b < $HOME/u8g2-hal-esp-idf/examples/i2c/components/u8g2/tools/font/emoticons/emoticons21.bdf > main/emoticons.h

./bdf2c -n Scroll_o_Sprites -b < $HOME/u8g2-hal-esp-idf/examples/i2c/components/u8g2/tools/font/pbm/Scroll-o-Sprites.bdf > main/Scroll-o-Sprites.h
```

- BDF font files can be viewed with the following command.
```
./bdf2c -B < $HOME/u8g2-hal-esp-idf/examples/i2c/components/u8g2/tools/font/bdf/ncenR12.bdf | more

./bdf2c -B < $HOME/u8g2-hal-esp-idf/examples/i2c/components/u8g2/tools/font/bdf/timR12.bdf | more

./bdf2c -B < $HOME/u8g2-hal-esp-idf/examples/i2c/components/u8g2/tools/font/battery/battery19.bdf | more

./bdf2c -B < $HOME/u8g2-hal-esp-idf/examples/i2c/components/u8g2/tools/font/emoticons/emoticons21.bdf | more

./bdf2c -B < $HOME/u8g2-hal-esp-idf/examples/i2c/components/u8g2/tools/font/pbm/Scroll-o-Sprites.bdf | more
```

- Add header files to main.c.
```
#include "ssd1306.h"
#include "ncenR12.h"
#include "timR12.h"
#include "battery.h"
#include "emoticons.h"
#include "Scroll-o-Sprites.h"
```


- Show BDF Font
```
ssd1306_clear_screen(&dev, false);
ssd1306_contrast(&dev, 0xff);
show_bdf_font_text(&dev, __ncenR12_bitmap__, "Hello World", 0, 0);
show_bdf_font_code(&dev, __battery_bitmap__, 48, 100, 0);
show_bdf_font_code(&dev, __battery_bitmap__, 49, 110, 0);
show_bdf_font_code(&dev, __battery_bitmap__, 50, 120, 0);
```


# Font display position
The font's display position starts from the upper left corner of this image.   
Therefore, there will be some white space above the font.   
```
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
______XX,X_______,________,
_____X__,_X______,________,
____X___,_XX_____,________,
____XX__,_XX_____,________,
____XX__,_XX_____,________,
________,XX______,________,
________,X_______,________,
_______X,________,________,
______X_,__X_____,________,
_____X__,__X_____,________,
____XXXX,XXX_____,________,
____XXXX,XXX_____,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
```

# Font bounding box and character bounding box
BDF font file has a font bounding box (FONTBOUNDINGBOX).   
This means there is a 3 pixel margin to the left of the font and a 6 pixel margin below.   
```
FONTBOUNDINGBOX 21 26 -3 -6
```

Each character has a character bounding box (BBX).   
This BBX value indicates that it should be moved one pixel to the right when displayed.   
By providing a character bounding box (BBX), this can omit the leftmost blank space.   
```
STARTCHAR d
ENCODING 100
SWIDTH 574 0
DWIDTH 10 0
BBX 8 12 1 0
BITMAP
0E
06
06
06
3E
66
C6
C6
C6
C6
66
3B
ENDCHAR
```

Fonts that take character bounding box (BBX) into consideration change as follows.   
```
STARTCHAR d
ENCODING 100
SWIDTH 574 0
DWIDTH 11 0
BBX 9 12 0 0
BITMAP
07
03
03
03
1F
33
63
63
63
63
33
1D8
ENDCHAR
```

Fonts that take character bounding box (BBX) and font bounding box (FONTBOUNDINGBOX) into consideration change as follows.   
```
STARTCHAR d
ENCODING 100
SWIDTH 574 0
DWIDTH 14 0
BBX 9 12 0 0
BITMAP
00
00
00
00
00
00
00
00
00E0
0060
0060
0060
03E0
0660
0C60
0C60
0C60
0C60
0660
030B
00
00
00
00
00
00
ENDCHAR
```

Both the font bounding box (FONTBOUNDINGBOX) and the character bounding box (BBX) determine the display position.   
This image takes into account the font bounding box (FONTBOUNDINGBOX) and character bounding box (BBX).   
```
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,XXX_____,________,
________,_XX_____,________,
________,_XX_____,________,
________,_XX_____,________,
______XX,XXX_____,________,
_____XX_,_XX_____,________,
____XX__,_XX_____,________,
____XX__,_XX_____,________,
____XX__,_XX_____,________,
____XX__,_XX_____,________,
_____XX_,_XX_____,________,
______XX,X_XX____,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
________,________,________,
```
