// Bitmaps typedef structure
typedef struct
{ 
   uint16_t,                  // Picture Width - 1 
   uint16_t,                  // Picture Height - 1 
   uint8_t,                   // RLE Copressed flag, if yes value = 1 
   uint32_t,                  // Bitmap array size 
   const uint *BitmapArray,   // Bitmap point array 
}; BITMAP_INFO_16
