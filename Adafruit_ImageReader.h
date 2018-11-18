/*!
 * @file Adafruit_ImageReader.h
 *
 * This is part of Adafruit's ImageReader library for Arduino, designed to
 * work with Adafruit_GFX plus a display device-specific library.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Phil "PaintYourDragon" Burgess for Adafruit Industries.
 *
 * BSD license, all text here must be included in any redistribution.
 */

#include <SD.h>
#include "Adafruit_SPITFT.h"

/** Status codes returned by drawBMP() and loadBMP() */
enum ImageReturnCode {
  IMAGE_SUCCESS,            // Successful load (or image clipped off screen)
  IMAGE_ERR_FILE_NOT_FOUND, // Could not open file
  IMAGE_ERR_FORMAT,         // Not a supported image format
  IMAGE_ERR_MALLOC          // Could not allocate image (loadBMP() only)
};

/** Image formats returned by loadBMP() */
enum ImageFormat {
  IMAGE_NONE,               // No image was loaded; IMAGE_ERR_* condition
  IMAGE_1,                  // GFXcanvas1 image (NOT YET SUPPORTED)
  IMAGE_8,                  // GFXcanvas8 image (NOT YET SUPPORTED)
  IMAGE_16                  // GFXcanvas16 image (SUPPORTED)
};

/*!
   @brief  Data bundle returned with an image loaded to RAM.
           Used by loadBMP(), not drawBMP().
*/
class Adafruit_Image {
  public:
    Adafruit_Image(void);
   ~Adafruit_Image(void);
    ImageFormat    format(void); // Return image format
    int16_t        width(void);  // Return image height in pixels
    int16_t        height(void); // Return image width in pixels
    void           draw(Adafruit_SPITFT &display, int16_t x, int16_t y);
  protected:
    ImageFormat    fmt;          // Canvas bundle type in use
    // MOST OF THESE ARE NOT SUPPORTED YET -- WIP
    union {                      // Single pointer, only one variant is used:
      GFXcanvas1  *canvas1;      // Canvas object if 1bpp format
      GFXcanvas8  *canvas8;      // Canvas object if 8bpp format
      GFXcanvas16 *canvas16;     // Canvas object if 16bpp
    } canvas;
    GFXcanvas1    *mask;         // 1bpp image mask (or NULL)
    uint16_t      *palette;      // Color palette for 8bpp image (or NULL)
  friend class     Adafruit_ImageReader;
};

/*!
   @brief  An optional adjunct to Adafruit_SPITFT that reads RGB BMP
           images (maybe others in the future) from an SD card. It's
           purposefully been made an entirely separate class (rather than
           part of SPITFT or GFX classes) so that Arduino code that uses
           GFX or SPITFT *without* image loading does not need to incur
           the significant RAM overhead of the SD library by its mere
           inclusion. The syntaxes can therefore be a bit bizarre, see
           examples for use.
*/
class Adafruit_ImageReader {
  public:
    Adafruit_ImageReader(void);
   ~Adafruit_ImageReader(void);
    ImageReturnCode drawBMP(char *filename, Adafruit_SPITFT &display,
                      int16_t x, int16_t y, boolean transact = true);
    ImageReturnCode loadBMP(char *filename, Adafruit_Image &img);
    ImageReturnCode bmpDimensions(char *filename, int32_t *w, int32_t *h);
    void            drawImage(Adafruit_SPITFT &display,
                      Adafruit_Image &img, boolean transact = true);
  private:
    File            file;
    ImageReturnCode coreBMP(char *filename, Adafruit_SPITFT *tft,
      uint16_t *dest, int16_t x, int16_t y, Adafruit_Image *img,
      boolean transact);
    uint16_t        readLE16(void);
    uint32_t        readLE32(void);
};
